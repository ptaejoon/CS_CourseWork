#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <asm/ioctl.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <asm/uaccess.h>
#include <linux/delay.h>

#define DEVICE_DRIVER_NAME "dev_driver"
#define DEVICE_MAJOR_NUM 242
#define FND_ADDRESS 0x08000004
#define DOT_ADDRESS 0x08000210
#define TEXTLCD_ADDRESS 0x08000090
#define LED_ADDRESS 0x08000016
#define IOCTL_INIT _IOR(DEVICE_MAJOR_NUM, 0, unsigned char *)
#define IOCTL_START_TIMER _IO(DEVICE_MAJOR_NUM, 1)
/*
    physical address for FPGA devices
*/
unsigned char fpga_number[10][10] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // 0
	{0x0c,0x1c,0x1c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x1e}, // 1
	{0x7e,0x7f,0x03,0x03,0x3f,0x7e,0x60,0x60,0x7f,0x7f}, // 2
	{0xfe,0x7f,0x03,0x03,0x7f,0x7f,0x03,0x03,0x7f,0x7e}, // 3
	{0x66,0x66,0x66,0x66,0x66,0x66,0x7f,0x7f,0x06,0x06}, // 4
	{0x7f,0x7f,0x60,0x60,0x7e,0x7f,0x03,0x03,0x7f,0x7e}, // 5
	{0x60,0x60,0x60,0x60,0x7e,0x7f,0x63,0x63,0x7f,0x3e}, // 6
	{0x7f,0x7f,0x63,0x63,0x03,0x03,0x03,0x03,0x03,0x03}, // 7
	{0x3e,0x7f,0x63,0x63,0x7f,0x7f,0x63,0x63,0x7f,0x3e}, // 8
	{0x3e,0x7f,0x63,0x63,0x7f,0x3f,0x03,0x03,0x03,0x03} // 9
};
unsigned char led_number[9] = {0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
/*
 predefined shapes for LED and Dot Matrix
*/

static unsigned char *fnd;
static unsigned char *dot;
static unsigned char *textlcd;
static unsigned char *led;
/*
    mapped address of FPGA devices.
*/
int major_number;
int dev_driver_open(struct inode *, struct file *);
int dev_driver_release(struct inode *, struct file *);
long dev_driver_ioctl( struct file *, unsigned int, unsigned long);

static struct vars {
	struct timer_list timer;
    int interval;
    int cnt;
	int curPos;
    int curCount;
    unsigned char display[4];
    unsigned char text[33];
    unsigned char curChar;
    unsigned char upperLR;
    unsigned char lowerLR;
};
/*
    user defined data structure to deal with devices following counter.
*/
struct vars tm;

static struct file_operations device_driver_fops =
{ 
  .unlocked_ioctl = dev_driver_ioctl,
  .open = dev_driver_open,
  .release = dev_driver_release,
};
/*
    link user defined functions into driver's command
    since ioctl had changed to unlocked_ioctl, I linked dev_driver_ioctl into unlocked_ioctl
*/

int dev_driver_open(struct inode *minode, struct file *mfile) {
    printk("driver_open\n");
    printk("driver_open fnd addr :%u\n",(unsigned int)fnd);
    printk("driver_open dot addr :%u\n",(unsigned int)dot);
    printk("driver_open led addr :%u\n",(unsigned int)led);
    printk("driver_open textlcd addr :%u\n",(unsigned int)textlcd);
	return 0;
}
/*
    a function called when device driver's open function is called.
*/
int dev_driver_release(struct inode *minode, struct file *mfile) {
	printk("driver_release\n");
	return 0;
}
/*
    a function called when device driver's close function is called.
*/


void writeFND(void)
{
    unsigned short int fnd_value;

    fnd_value = tm.display[0] << 12 | tm.display[1] <<8|tm.display[2] <<4 | tm.display[3];
    outw(fnd_value,(unsigned int)fnd);  
}
/*
    write program's value into FND
*/
void writeDOT(void)
{
    int i;
    unsigned short int val;
    for(i=0; i < 10; i++)
    {
        val = fpga_number[tm.curChar][i];
        outw(val,(unsigned int)dot+i*2);
    }
}
/*
    write program's value into Dot matrix
*/
void writeLED(void)
{
    unsigned short val;
    val = (unsigned short)led_number[tm.curChar];
    outw(val,(unsigned int)led);
}
/*
    write program's value into LED 
*/
void writeLCDTEXT(void)
{
    unsigned short int val;
    int i;
    for(i=0; i<32; i=i+2)
    {
        val = (tm.text[i] & 0xff) << 8 | (tm.text[i+1] & 0xff);
        outw(val,(unsigned int)textlcd+i);
    }
    
}
/*
    write program's value into LCDtext 
*/

static void activate_per_time(unsigned long timeout){
    int i;
    tm.cnt--;
    if(tm.cnt <= 0)
    {
        for(i = 0; i < 4; i++)tm.display[i] = 0;
        tm.curChar = 0;
        memset(tm.text,' ',32);
        writeDOT();
        writeFND();
        writeLCDTEXT();
        writeLED();
        printk("Counter Finish\n");
        return;
    }
    tm.curCount += 1;
    tm.curChar += 1;
    if(tm.curChar == 9)
    {
        tm.curChar = 1;
    }
    if(tm.curCount == 8)
    {
        tm.curPos += 1;
        if(tm.curPos == 4)
        {
            tm.curPos = 0;
        }
        tm.curCount = 0;
    }
    if(tm.upperLR == 1)
    {
        for(i = 15; i > 0;i--)tm.text[i] = tm.text[i-1];
        tm.text[0] = ' ';
        if(tm.text[15] != ' ')tm.upperLR = 2;
    }
    else
    {
        for(i = 0; i < 15;i++)tm.text[i] = tm.text[i+1];
        tm.text[15] = ' ';
        if(tm.text[0] != ' ')tm.upperLR = 1;
    } 
    if(tm.lowerLR == 1)
    {
        for(i = 31; i > 16;i--)tm.text[i] = tm.text[i-1];
        tm.text[16] = ' ';
        if(tm.text[31] != ' ')tm.lowerLR = 2;
    }
    else{
        for(i = 16; i < 31; i++)tm.text[i] = tm.text[i+1];
        tm.text[31] = ' ';
        if(tm.text[16] != ' ')tm.lowerLR = 1;
    }
    for(i = 0; i < 4; i++)tm.display[i] = 0;
    tm.display[tm.curPos] = tm.curChar;
    writeDOT();
    writeFND();
    writeLCDTEXT();
    writeLED();
    
    printk("%d\n",tm.cnt);
    tm.timer.expires = get_jiffies_64() + (tm.interval * HZ)/10;
    tm.timer.function = activate_per_time;
    tm.timer.data = (unsigned long)&tm;
    add_timer(&tm.timer);

}
/*
    If count is over (lower than 1), erase all value on device and finish timer function.
    For each count, change value of tm's variables, which is data for writing device.
    Then write device to show new value on FPGA.
*/
long dev_driver_ioctl( struct file *mfile, unsigned int ioctl_num, unsigned long param)
{
    unsigned char *pm;
    unsigned char value[6];
    int count;

    switch(ioctl_num)
    {
        case IOCTL_INIT:
            printk("Setting Information...\n");
            pm = (unsigned char*)param;
            if(copy_from_user(&value,pm,6))
                return -EFAULT;
            tm.interval = value[0];
            tm.cnt = value[1];
            tm.curCount =0;
            memcpy(tm.display,value+2,sizeof(char*)*4);
	        for(count = 0; count < 4; count++){
                tm.display[count] -= 0x30;
                if(tm.display[count]!=0)
                {
                    tm.curChar=tm.display[count];
                    tm.curPos = count;
                }
            }
            memcpy(tm.text,"20151550        ",16);
            memcpy(tm.text+16,"TaeJoon Park    ",16);
            tm.upperLR = 1;
            tm.lowerLR = 1;
	        writeFND();
	        writeDOT();
            writeLED();
            writeLCDTEXT();
	        break;
        case IOCTL_START_TIMER:
            printk("Timer Start\n");
            tm.timer.expires = jiffies + (tm.interval * HZ)/10;
            tm.timer.function = activate_per_time;
            tm.timer.data = (unsigned long)&tm;
            add_timer(&tm.timer);
            break;
    }
    return 1;
}
/*
    Two cases for ioctl:
        case 1 :
            Initialization Setting.
            Set kernel variables to user's input. 
        case 2 :
            Set timer and activate 'activate_per_time function at designated interval 

*/

int __init device_driver_init(void)
{

    printk("device_driver_init\n");
    major_number = register_chrdev(DEVICE_MAJOR_NUM, DEVICE_DRIVER_NAME, &device_driver_fops);
    if(major_number <0) {
	printk( "error %d\n",major_number);
	return major_number;
    }
    fnd = ioremap(FND_ADDRESS, 0x4);
    dot = ioremap(DOT_ADDRESS, 0x10);
    textlcd = ioremap(TEXTLCD_ADDRESS,0x32);
    led = ioremap(LED_ADDRESS,0x1);
    init_timer(&tm.timer);
    printk("init fnd addr :%u\n",(unsigned int)fnd);
    printk("init dot addr :%u\n",(unsigned int)dot);
    printk("init led addr :%u\n",(unsigned int)led);
    printk("init textlcd addr :%u\n",(unsigned int)textlcd);
    printk("dev_file: /dev/%s , major: %d\n", DEVICE_DRIVER_NAME, DEVICE_MAJOR_NUM);
    printk("init module\n");
    return 0;
}
/*
register device driver and allocate memory for controling devices
enroll timer
print memories of allocated devices to see if there is possibility for kernel panic.
*/

void __exit device_driver_exit(void)
{
    printk("device_driver_exit\n");
    printk("fnd addr :%u\n",(unsigned int)fnd);
    printk("dot addr :%u\n",(unsigned int)dot);
    printk("led addr :%u\n",(unsigned int)led);
    printk("textlcd arr :%u\n",(unsigned int)textlcd);
    iounmap(fnd);
    printk("successfully unmap fnd\n");
    iounmap(dot);
    printk("successfully unmap dot\n");
    iounmap(textlcd);
    printk("successfully unmap textlcd\n");
    iounmap(led);
    printk("successfully unmap led\n");
    unregister_chrdev(DEVICE_MAJOR_NUM, DEVICE_DRIVER_NAME);
    printk("successfully unregister device driver\n");
    del_timer_sync(&tm.timer);
    printk("successfully delete timer \n");
    return;
}
/*
unregister device driver and deallocate memories mapped to control devices
unroll timer

*/

module_init(device_driver_init);
module_exit(device_driver_exit);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Taejoon Park");
