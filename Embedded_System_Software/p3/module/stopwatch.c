#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <mach/gpio.h>
#include <linux/platform_device.h>
#include <asm/gpio.h>
#include <linux/wait.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <asm/param.h>

#define FND_ADDRESS 0x08000004
#define DEVICE_MAJOR_NUM 242
#define DEVICE_DRIVER_NAME "stopwatch"

static unsigned char *fnd;

static int major_number=0;

static unsigned char fin=0;
static unsigned char fin_counter = 0;
static unsigned char on = 0;
static int stopwatch_open(struct inode *, struct file *);
static int stopwatch_release(struct inode *, struct file *);
static int stopwatch_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

irqreturn_t home_handler(int irq, void* dev_id, struct pt_regs* reg);
irqreturn_t back_handler(int irq, void* dev_id, struct pt_regs* reg);
irqreturn_t volup_handler(int irq, void* dev_id, struct pt_regs* reg);
irqreturn_t voldown_handler(int irq, void* dev_id, struct pt_regs* reg);

static struct vars {
	struct timer_list timer;
	unsigned long left;
	unsigned char display[4];
};
// data structure for timer.

struct vars tm;
struct vars fin_tm;
// one for stopwatch, one for check exit check

wait_queue_head_t wq_write;
DECLARE_WAIT_QUEUE_HEAD(wq_write);

void writeFND(void)
{
	unsigned short int fnd_value;
	fnd_value = tm.display[0] << 12 | tm.display[1] <<8| tm.display[2] << 4 | tm.display[3];
	outw(fnd_value,(unsigned int)fnd);
}
void display_set(void)
{
	tm.display[3] += 1;
	if(tm.display[3] == 10)
	{
		tm.display[3] = 0;
		tm.display[2] += 1;
	}
	if(tm.display[2] == 6)
	{
		tm.display[2] = 0;
		tm.display[1] += 1;
	}
	if(tm.display[1] == 10)
	{
		tm.display[1] = 0;
		tm.display[0] += 1;	
	}
	if(tm.display[0] == 6)
	{
		tm.display[0] = 0;
	}
}
//a function to change FND's value
static struct file_operations stopwatch_fops =
{
	.open = stopwatch_open,
	.write = stopwatch_write,
	.release = stopwatch_release,
};

static void timer_func(unsigned long time)
{
	//FND work
	display_set();
	writeFND();
	tm.timer.expires = get_jiffies_64() + 1*(HZ);
	tm.timer.function = timer_func;
	tm.timer.data = (unsigned long)&tm;
	add_timer(&tm.timer);
}
//a function for timer to activate stopwatch

void call_timer(void)
{
	on = 1;
	tm.timer.expires = get_jiffies_64() +tm.left;
	tm.timer.function = timer_func;
	tm.timer.data = (unsigned long)&tm;
	add_timer(&tm.timer);
}
//special function to save milisecond for pause. 
//The difference between call_timer and timer_func is call_timer called only once and set timer to left time in case of pause.

irqreturn_t home_handler(int irq, void* dev_id, struct pt_regs* reg) {
	if(on == 0)call_timer();
	return IRQ_HANDLED;
}
// when home key is called, call call_timer function
// if timer is called multiple times in sequence, there will be error on memory. Thus by using 'on' variable, check it is already work as stopwatch.

irqreturn_t back_handler(int irq, void* dev_id, struct pt_regs* reg) {
        tm.left = tm.timer.expires-get_jiffies_64();
		del_timer(&tm.timer);
		//del_timer_sync(&tm.timer);
		on = 0;
		return IRQ_HANDLED;
}
// when back key is called, save left milisecond for next start and delete timer from time schedule.
irqreturn_t volup_handler(int irq, void* dev_id,struct pt_regs* reg) {
	tm.left = HZ;
	memset(tm.display,0,4);
	writeFND();
	del_timer(&tm.timer);

	//del_timer_sync(&tm.timer);
	on = 0;
	return IRQ_HANDLED;
}
// when vol+ key is called, reset every variable and FND to 0000

static void fin_timer_func(unsigned long time)
{
	printk("gpio : %d\n",gpio_get_value(IMX_GPIO_NR(5, 14)));
	if(fin == 1)
	{
		fin_counter = 0;
		return;
	}
	fin_counter += 1;
	printk("keeping vol - button down will close program in %d\n",3-fin_counter);
	if(fin_counter == 3)
	{
		memset(tm.display,0,4);
		writeFND();
		__wake_up(&wq_write,1,1,NULL);
		return ;
	}
	fin_tm.timer.expires = get_jiffies_64() + HZ;
	fin_tm.timer.function = fin_timer_func;
	fin_tm.timer.data = (unsigned long)&fin_tm;
	add_timer(&fin_tm.timer);
}
// timer function to check if user press vol - button for 3 seconds.
irqreturn_t voldown_handler(int irq, void* dev_id, struct pt_regs* reg) {
		printk("gpio : %d",gpio_get_value(IMX_GPIO_NR(5, 14)));
		fin = gpio_get_value(IMX_GPIO_NR(5, 14));
		
		if(fin == 0)
		{
			printk("keep pressing vol - button will close program in %d\n",3-fin_counter);
			fin_tm.timer.expires = get_jiffies_64() + HZ;
			fin_tm.timer.function = fin_timer_func;
			fin_tm.timer.data = (unsigned long)&fin_tm;
			add_timer(&fin_tm.timer);
		}
		else
		{
			printk("unpressed before exit\n");
			del_timer(&fin_tm.timer);
			fin_counter = 0;
		}
		
        return IRQ_HANDLED;
}
// interrupt handling function for vol- button.

static int stopwatch_open(struct inode *minode, struct file *mfile){
	int ret;
	int irq;
	printk(KERN_ALERT "Open Stopwatch\n");
//	home - start
	gpio_direction_input(IMX_GPIO_NR(1,11));
	irq = gpio_to_irq(IMX_GPIO_NR(1,11));
	ret=request_irq(irq, home_handler, IRQF_TRIGGER_RISING, "home", 0);
	// back - pause
	gpio_direction_input(IMX_GPIO_NR(1,12));
	irq = gpio_to_irq(IMX_GPIO_NR(1,12));
	ret=request_irq(irq, back_handler, IRQF_TRIGGER_RISING, "back", 0);
	// VOL+ - reset
	gpio_direction_input(IMX_GPIO_NR(2,15));
	irq = gpio_to_irq(IMX_GPIO_NR(2,15));
	ret=request_irq(irq, volup_handler, IRQF_TRIGGER_RISING, "volup", 0);
	// VOL- - exit
	gpio_direction_input(IMX_GPIO_NR(5,14));
	irq = gpio_to_irq(IMX_GPIO_NR(5,14));
	ret=request_irq(irq, voldown_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "voldown", 0);
	return 0;
}
// when device driver is opened, request interrupt number and connect it with interrupt handling functions

static int stopwatch_release(struct inode *minode, struct file *mfile){
	free_irq(gpio_to_irq(IMX_GPIO_NR(1, 11)), NULL);
	free_irq(gpio_to_irq(IMX_GPIO_NR(1, 12)), NULL);
	free_irq(gpio_to_irq(IMX_GPIO_NR(2, 15)), NULL);
	free_irq(gpio_to_irq(IMX_GPIO_NR(5, 14)), NULL);
	del_timer_sync(&tm.timer);
	del_timer_sync(&fin_tm.timer);
	memset(tm.display,0,4);
	fin = 0;
	fin_counter = 0;
	on = 0;
	writeFND();
	printk(KERN_ALERT "Release Stopwatch\n");
	return 0;
}
// function that is activated when device driver close.
static int stopwatch_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos ){
	memset(tm.display,0,4);
	tm.left = HZ;
	fin = 0;
	fin_counter = 0;
	on = 0;
	writeFND();
    printk("User Program waits for timer\n");
    interruptible_sleep_on(&wq_write);
	return 0;
}
// write function just called once in user file and this write function will block user program.
// stopwatch functions are activated from this function.

static int __init stopwatch_init(void) {
	major_number = register_chrdev(DEVICE_MAJOR_NUM,DEVICE_DRIVER_NAME,&stopwatch_fops);
	if(major_number < 0)
	{
	    printk("Error %d\n",major_number);
	    return major_number;
	}
	printk("Stopwatch driver registered\n");
	fnd = ioremap(FND_ADDRESS, 0x4);
	init_timer(&(tm.timer));
	init_timer(&(fin_tm.timer));
	return 0;

}
// for device driver registeration.
static void __exit stopwatch_exit(void) {
	iounmap(fnd);
	unregister_chrdev(DEVICE_MAJOR_NUM,DEVICE_DRIVER_NAME);
	del_timer_sync(&tm.timer);
	del_timer_sync(&fin_tm.timer);
	printk("Unregister Stopwatch driver\n");
}
// for device driver unregisteration.
module_init(stopwatch_init);
module_exit(stopwatch_exit);
MODULE_LICENSE("GPL");
