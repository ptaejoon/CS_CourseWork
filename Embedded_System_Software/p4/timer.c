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
#include <asm/ioctl.h>
#define TEXTLCD_ADDRESS 0x08000090
#define FND_ADDRESS 0x08000004
#define DEVICE_MAJOR_NUM 242
#define DEVICE_DRIVER_NAME "timer"

#define IOCTL_GAME_FINISH _IO(DEVICE_MAJOR_NUM, 0)
#define IOCTL_START_GAME _IO(DEVICE_MAJOR_NUM, 1)
#define IOCTL_SCORE_GET _IO(DEVICE_MAJOR_NUM, 2)
//set IOCTL seperator
static unsigned char *fnd;
static unsigned char *textlcd;
//character device
static int major_number=0;
static int best_score = 0;
//Save best_score every time game is finished

static int timer_open(struct inode *, struct file *);
static int timer_release(struct inode *, struct file *);
long timer_ioctl( struct file *mfile, unsigned int ioctl_num, unsigned long param);


irqreturn_t home_handler(int irq, void* dev_id, struct pt_regs* reg);
//set handler
static int score = 0;
unsigned char text[33];

static struct vars {
	struct timer_list timer;
	unsigned long left;
	unsigned char display[4];
};
// data structure for timer.

struct vars tm;
// one for  timer, one for check exit check

void writeFND(void)
{
	unsigned short int fnd_value;
	fnd_value = tm.display[0] << 12 | tm.display[1] <<8| tm.display[2] << 4 | tm.display[3];
	outw(fnd_value,(unsigned int)fnd);
}
//function for write FND
void writeLCDTEXT(void)
{
    unsigned short int val;
    int i;
    for(i=0; i<32; i=i+2)
    {
        val = (text[i] & 0xff) << 8 | (text[i+1] & 0xff);
        outw(val,(unsigned int)textlcd+i);
    }
    
}
//function for write LCDText
void display(void)
{
	tm.display[0] = (unsigned int)(score/1000);
	tm.display[1] = (unsigned int)(score/100)%10;
	tm.display[2] = (unsigned int)((score%100)/10);
	tm.display[3] = (unsigned int)(score%10);
}
//a function to change FND's value
static struct file_operations timer_fops =
{
	.open = timer_open,
	.unlocked_ioctl = timer_ioctl,
	.release = timer_release,
};

static void timer_func(unsigned long time)
{
	//FND work
	if(score != 0)score -= 1;
	display();
	writeFND();
	tm.timer.expires = get_jiffies_64() + 1*(HZ);
	tm.timer.function = timer_func;
	tm.timer.data = (unsigned long)&tm;
	add_timer(&tm.timer);
}
//a function for timer to activate timer.
//at each second, score is decreased by 1

void call_timer(void)
{
	tm.timer.expires = get_jiffies_64() + 1*(HZ);
	tm.timer.function = timer_func;
	tm.timer.data = (unsigned long)&tm;
	add_timer(&tm.timer);
}
//special function to save milisecond for pause. 
//The difference between call_timer and timer_func is call_timer called only once and call timer function calls timer_func.

irqreturn_t home_handler(int irq, void* dev_id, struct pt_regs* reg) {
	
	memcpy(text,"Best Score: ",12);
	text[12] = (unsigned int)(best_score/1000) + '0';
	text[13] = (unsigned int)(best_score/100)%10 + '0';
	text[14] = (unsigned int)((best_score%100)/10) + '0';
	text[15] = (unsigned int)(best_score%10) + '0';
    memcpy(text+16,"                ",16);
	writeLCDTEXT();
	return IRQ_HANDLED;
}
// when home key is pressed, print best score at lcdtext

static int timer_open(struct inode *minode, struct file *mfile){
	int ret;
	int irq;
	printk(KERN_ALERT "Open timer\n");
	memcpy(text,"                                ",32);
	memset(tm.display,0,4);
	writeFND();
	writeLCDTEXT();
	gpio_direction_input(IMX_GPIO_NR(1,11));
	irq = gpio_to_irq(IMX_GPIO_NR(1,11));
	ret=request_irq(irq, home_handler, IRQF_TRIGGER_RISING, "home", 0);
	return 0;
}
// when device driver is opened, request interrupt number and connect it with interrupt handling functions
// initialize value of FND and LCDText
static int timer_release(struct inode *minode, struct file *mfile){
	free_irq(gpio_to_irq(IMX_GPIO_NR(1, 11)), NULL);
	del_timer_sync(&tm.timer);
	memset(tm.display,0,4);
	writeFND();
	printk(KERN_ALERT "Release timer\n");
	return 0;
}
// function that is activated when device driver close.
long timer_ioctl( struct file *mfile, unsigned int ioctl_num, unsigned long param)
{
    switch(ioctl_num)
    {
        case IOCTL_START_GAME:
			memset(tm.display,0,4);
			memcpy(text,"                                ",32);
			del_timer(&tm.timer);
			score = 0;
			writeFND();
			writeLCDTEXT();
			call_timer();
			break;
        
		case IOCTL_GAME_FINISH:
            del_timer(&tm.timer);
			if(best_score < score)
			{
				best_score = score;
				return 1;
			}
			else return 0;
		case IOCTL_SCORE_GET:
			score += 30;
			display();
			writeFND();
    }
    return 1;
}
// ioctl called through JNI and it controls devices to have changed value
// timer functions are activated from this function.
// when game finished, timer is deleted. If score is higher than best_score, save it to best_score
// in SCORE_GET, if pair of cards are found, 30 score is incremented 
static int __init timer_init(void) {
	major_number = register_chrdev(DEVICE_MAJOR_NUM,DEVICE_DRIVER_NAME,&timer_fops);
	if(major_number < 0)
	{
	    printk("Error %d\n",major_number);
	    return major_number;
	}
	printk("timer driver registered\n");
	fnd = ioremap(FND_ADDRESS, 0x4);
	textlcd = ioremap(TEXTLCD_ADDRESS, 0x32);
	init_timer(&(tm.timer));
	return 0;

}
// for device driver registeration.
static void __exit timer_exit(void) {
	iounmap(fnd);
	iounmap(textlcd);
	unregister_chrdev(DEVICE_MAJOR_NUM,DEVICE_DRIVER_NAME);
	del_timer_sync(&tm.timer);
	printk("Unregister timer driver\n");
}
// for device driver unregisteration.
module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
