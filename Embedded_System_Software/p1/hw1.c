#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<dirent.h>
#include<linux/input.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/time.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<termios.h>
#include<signal.h>
#include<time.h>
#define FPGA_BASE_ADDRESS 0x08000000
#define LED_ADDR 0x16

#define shmkeyInput 100
#define shmkeyOutput 101
#define Memsize 200
//definition for shared memory
#define waitVal 0
#define sendVal 1
#define doingVal 2
//definition for communication between process
#define BUFF_SIZE 64

#define KEY_RELEASE 0
#define KEY_PRESS 1
#define KEY_INPUT_DONE 2
//defintion for button push

#define MAX_BUTTON 9
//definition for switch button

#define swChange 12
#define modeChange 2 
//definition for input.

void executeReadkey();
int  checkSwitch();
void modeInit();
void swInit();
void writeFND();
void writeLED();
void writeDotMat();
void writeTextLCD();

unsigned char dotOne[10] = {0x0c,0x1c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x3f,0x3f};
unsigned char dotA[10] = {0x1c,0x36,0x63,0x63,0x63,0x7f,0x7f,0x63,0x63,0x63};
//mode 3's dot matrix's image 1 and A.

void writeFND(int device, unsigned char* data){
    unsigned char dataChar[4];
    int i;
    for(i = 3; i >= 0; i--)dataChar[i] = data[i];
    i = write(device,&dataChar,4);
}
//get 4 characters and write it to FND. 

void writeLED(int device, unsigned char* data){
    unsigned long *fpgaAddr;
    unsigned char val = data[0];
    fpgaAddr = (unsigned long *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, device, FPGA_BASE_ADDRESS);
    unsigned char *ledAddr;
    ledAddr = (unsigned char*)((void*)fpgaAddr+LED_ADDR);
    *ledAddr = val;
}
//get a character and write it to LED

void writeDotMat(int device,unsigned char* data){
    int str_size = sizeof(unsigned char)*10;
    write(device,data,str_size);
}
//get 10 characters and write it to Dot Matrix
void writeTextLCD(int device,unsigned char* data){
    unsigned char string[32];
    memset(string,0,sizeof(string));
    int i;
    for(i = 0; i<32; i++)string[i]=data[i];
    write(device,string,32);
}
//get 32 characters and write it to Text LCD

void initModeOne(unsigned char* outmem)
{
    time_t base_time = time(NULL);
    struct tm *local;
    local = localtime(&base_time);
    outmem[1] = ((local->tm_hour)/10);
    outmem[2] = ((local->tm_hour)%10);
    outmem[3] = ((local->tm_min)/10);
    outmem[4] = ((local->tm_min)%10);
    outmem[51] = 0;
    outmem[55] = 0;
    outmem[56] = 0;
    memset(outmem+7,' ',sizeof(unsigned char)*32);
    memset(outmem+41,0,sizeof(unsigned char)*10);
}
//when mode is changed to 1, initialize shared memory for output process

void initModeTwo(unsigned char* outmem)
{
    outmem[1] = 0;
    outmem[2] = 0;
    outmem[3] = 0;
    outmem[4] = 0;
    outmem[5] = 0x40;
    outmem[51] = 0;
    outmem[55] = 0;
    outmem[56] = 1;
    memset(outmem+7,' ',sizeof(unsigned char)*32);
    memset(outmem+41,0,sizeof(unsigned char)*10);
}
//when mode is changed to 2, initialize shared memory for output process

void initModeThree(unsigned char* outmem)
{
    outmem[1] = 0;
    outmem[2] = 0;
    outmem[3] = 0;
    outmem[4] = 0;
    outmem[5] = 0x00;
    memset(outmem+7,' ',sizeof(unsigned char)*32);
    outmem[39] = 0;
    outmem[40] = 0;
    strcpy(outmem+41,dotA);
    outmem[51] = 0;
    outmem[55] = 0;
    outmem[56] = 2;
}
//when mode is changed to 3, initialize shared memory for output process

void initModeFour(unsigned char* outmem)
{
    outmem[1] = 0;
    outmem[2] = 0;
    outmem[3] = 0;
    outmem[4] = 0;
    outmem[5] = 0x00;
    memset(outmem+7,' ',sizeof(unsigned char)*32);
    memset(outmem+41,0,sizeof(unsigned char)*10);
    outmem[51] = 1;
    outmem[52] = 6;
    outmem[53] = 0;
    outmem[54] = 0;
    outmem[55] = 0;
    outmem[56] = 3;
}
//when mode is changed to 4, initialize shared memory for output process

void modeInit(unsigned char* inmem,unsigned char*outmem)
{
    switch(inmem[1])
    {
        case 0:
            initModeOne(outmem);
            outmem[5] = 0x80;
            break;
        case 1:
            initModeTwo(outmem);
            break;
        case 2:
            initModeThree(outmem);
            break;
        case 3:
            initModeFour(outmem);
            break;
        default:
            printf("There Must Be Error on Mode Initialization!\n");
    }
    inmem[modeChange] = 0;
}
//Change mode considering mode number. If mode is changed, it needs to be initialized according to the mode nubmer.

void modeOneBlink(unsigned char *outmem)
{
    if(outmem[6] == 0)
    {
        /*if(outmem[55] == 0)
        {
            initModeOne(outmem);
        }*/
    }
    else
    {
        if(outmem[5] == 0x30)
        {
            outmem[5] = 0x20;
        }
        else if(outmem[5] == 0x20)outmem[5] = 0x10;
        else outmem[5] = 0x20;
    }
}
//Mode 1 needs blinking LED when revising clock's time
//it's function for blinking
void minuteUp(unsigned char *outmem)
{
    if(outmem[55] == 1)
    {
        outmem[4] += 1;
         if(outmem[4] == 10)
        {
            outmem[4] = 0;
            outmem[3] += 1;
        }
        if(outmem[3] == 6)
        {
            outmem[3] = 0;
            outmem[2] += 1;
        }
        if(outmem[2] >= 10)
        {
            outmem[2] -= 10;
            outmem[1] += 1;
        }
        if(outmem[1] == 2 && outmem[2] == 4)
        {
            outmem[1] = 0;
            outmem[2] = 0;
        }

    }
    else
    {
        initModeOne(outmem);
    }
}
//Mode 1 is a clock. if 60 seconds passed, then a minute should up.
// it's a function to make it possible.
void modeFourBlink(unsigned char *outmem)
{
    if(outmem[51] == 1)
    {
       outmem[41+outmem[53]] = (outmem[41+outmem[53]]) ^ (1<<outmem[52]);
    }
}
//Mode 4 needs blinking for dot matrix.
//it's a function to make a dot blink.

void swModeOne(unsigned char* sw, unsigned char *outmem)
{
    if(sw[0] == 1){
        if(outmem[6] == 0)
        {
            outmem[6] =1;
            outmem[5] = 0x30;
        }
        else{
            outmem[6] = 0;
            outmem[5] = 0x80;
        }
    }
    if(sw[1] == 1){
        if(outmem[6] ==1)initModeOne(outmem);
    }
    if(sw[3] ==1){
        if(outmem[6] == 1)
        {
            outmem[4] += 1;
            outmem[55] = 1; 
        }
    }
    if(sw[2] == 1){
        if(outmem[6] == 1)
        {
            outmem[2] += 1;
            outmem[55] = 1;
        }
    }
    if(outmem[4] == 10)
    {
        outmem[4] = 0;
        outmem[3] += 1;
    }
    if(outmem[3] == 6)
    {
        outmem[3] = 0;
        outmem[2] += 1;
    }
    if(outmem[2] >= 10)
    {
        outmem[2] -= 10;
        outmem[1] += 1;
    }
    if(outmem[1] == 2 && outmem[2] == 4)
    {
        outmem[1] = 0;
        outmem[2] = 0;
    }
}
//A function called when a switch has pushed and the mode is 1.
//if sw[0] (switch 1) is pushed, it changes it's mode from unchangeable to changeable, changeable to unchangeable.
//if sw[1] (switch 2) is pushed(only when it's in changeable state), it reset it's time to current time.
//if sw[2] (switch 3) is pushed(only when it's in changeable state), it plus one hour to clock.
//if sw[3] (switch 4) is pushed(only when it's in changeable state), it plus one minute to clock.

void swModeTwo(unsigned char* sw, unsigned char *outmem,int *num)
{
    if(sw[0] == 1)
    {
        if(outmem[5] == 0x10)outmem[5] = 0x80;
        else
        {
            outmem[5] = outmem[5] >> 1;
        }
    }
    int digit;
    if(outmem[5] == 0x80)digit=2;
    else if(outmem[5] == 0x40)digit=10;
    else if(outmem[5] == 0x20)digit=8;
    else if(outmem[5] == 0x10)digit=4;
    
    if(sw[3] == 1){ //1
        *num += 1;
    }
    if(sw[2] == 1) // 10
    {
        *num += 1*digit;
    }
    if(sw[1] == 1) //100
    {
       *num += 1*digit*digit;
    }
    int temp = *num;
    outmem[4] = temp % digit;
    temp /= digit;
    outmem[3] = temp % digit;
    temp /= digit;
    outmem[2] = temp % digit;
    temp /= digit;
    outmem[1] = temp % digit;
    *num = (*num) %(digit* digit*digit*digit);
    if(outmem[5] == 0x40)outmem[1] = 0;
}
//A function called when a switch has pushed and the mode is 2.
//if sw[0] (switch 1) is pushed, it changes digit in the order of 10-8-4-2-10 
//if sw[1] (switch 2) is pushed, it plus 100 to FND. (* if digit is 8, then it plus 1*8*8)
//if sw[2] (switch 3) is pushed, it plus 10 to FND.
//if sw[3] (switch 4) is pushed, it plus 1 to FND.
//***************** Notice ************************************************
//* if the number becomes too high to express since digit changes,        *
//* it only express smallest part it can express and save the small value *
//* ex) 17(10) -> 0001(2) . and 1 becomes the number                      *
//*************************************************************************

void swModeThree(unsigned char* sw, unsigned char *outmem)
{
    unsigned char newC=0;
    unsigned char temp;
    unsigned char* string = outmem+7;
    //string is a part of shared memory
    //it's start address of Text LCD
    if(sw[1] == 1 && sw[2] == 1)
    {
        memset(string,' ',sizeof(unsigned char)*32);
        outmem[39] = 0;
            
    }//delete operation
    else if(sw[4] == 1 && sw[5] == 1){
        if(outmem[40]==0)
        {
            outmem[40]=1;
            strcpy(outmem+41,dotOne);
        }
        else
        {
            outmem[40]=0;
            strcpy(outmem+41,dotA);
        }
    }//eng to num, num to eng
    else if(sw[7] == 1 && sw[8] == 1){
        temp = ' ';
        newC = 1;
    }//insert blink
    else{
        if(outmem[39] != 0)temp = string[outmem[39]-1];
        else temp = string[0];
        if(sw[0] == 1)
        {
            if(outmem[40] == 1){newC=1;temp='1';}
            else if(temp == '.')string[outmem[39]-1] ='Q';
            else if(temp == 'Q')string[outmem[39]-1]='Z';
            else if(temp == 'Z')string[outmem[39]-1]='.';
            else{newC = 1;temp = '.';}
        }
        else if(sw[1] == 1)
        {
            if(outmem[40] == 1){newC=1;temp='2';}
            else if(temp == 'A')string[outmem[39]-1] ='B';
            else if(temp == 'B')string[outmem[39]-1]='C';
            else if(temp == 'C')string[outmem[39]-1]='A';
            else{newC = 1;temp = 'A';}

        }
        else if(sw[2] == 1)
        {
            if(outmem[40] == 1){newC=1;temp='3';}
            else if(temp == 'D')string[outmem[39]-1] ='E';
            else if(temp == 'E')string[outmem[39]-1]='F';
            else if(temp == 'F')string[outmem[39]-1]='D';
            else{newC = 1;temp = 'D';}
        }
        else if(sw[3] == 1)
        {
            if(outmem[40] == 1){newC=1;temp='4';}
            else if(temp == 'G')string[outmem[39]-1] ='H';
            else if(temp == 'H')string[outmem[39]-1]='I';
            else if(temp == 'I')string[outmem[39]-1]='G';
            else{newC = 1;temp = 'G';}
        }
        else if(sw[4] == 1)
        {
            if(outmem[40] == 1){newC=1;temp='5';}
            else if(temp == 'J')string[outmem[39]-1] ='K';
            else if(temp == 'K')string[outmem[39]-1]='L';
            else if(temp == 'L')string[outmem[39]-1]='J';
            else{newC = 1;temp = 'J';}
        }
        else if(sw[5] == 1)
        {
            if(outmem[40] == 1){newC=1;temp='6';}
            else if(temp == 'M')string[outmem[39]-1] ='N';
            else if(temp == 'N')string[outmem[39]-1]='O';
            else if(temp == 'O')string[outmem[39]-1]='M';
            else{newC = 1;temp = 'M';}

        }
        else if(sw[6] == 1)
        {
            if(outmem[40] == 1){newC=1;temp='7';}
            else if(temp == 'P')string[outmem[39]-1] ='R';
            else if(temp == 'R')string[outmem[39]-1]='S';
            else if(temp == 'S')string[outmem[39]-1]='P';
            else{newC = 1;temp = 'P';}
        }
        else if(sw[7] == 1)
        {
            if(outmem[40] == 1){newC=1;temp='8';}
            else if(temp == 'T')string[outmem[39]-1] ='U';
            else if(temp == 'U')string[outmem[39]-1]='V';
            else if(temp == 'V')string[outmem[39]-1]='T';
            else{newC = 1;temp = 'T';}

        }
        else if(sw[8] == 1)
        {
            if(outmem[40] == 1){newC=1;temp='9';}
            else if(temp == 'W')string[outmem[39]-1] ='X';
            else if(temp == 'X')string[outmem[39]-1]='Y';
            else if(temp == 'Y')string[outmem[39]-1]='W';
            else{newC = 1;temp = 'W';}

        }
    }//each switch part insertion
    unsigned char cpEnNum = outmem[40];
    if(newC == 1)
    { 
        string[outmem[39]] = temp;
        outmem[39] += 1;
    }//dealing when new character should inserted.
    if(outmem[39] >= 32)
    {
        if(newC == 1){
            int count;
            for(count = 0; count < 31; count++)
            {
                string[count] = string[count+1];
            }
            string[31] = temp;
            outmem[40] = cpEnNum;
        
            outmem[39] = 32;
        }
    }
    outmem[4] += 1;
    if(outmem[4] >= 10){outmem[3] +=1;outmem[4] -= 10;}
    if(outmem[3] >= 10){outmem[2] +=1;outmem[3] -= 10;}
    if(outmem[2] >= 10){outmem[1] +=1;outmem[2] -= 10;}
    if(outmem[1] == 10)outmem[1] = 0;
    //480 ~ 484 lines are parts to deal with FND numbers.
}
//A function called when a switch has pushed and the mode is 3.
//if sw[0-8] (switch 1~9) is pushed, it inserts a character to TEXT LCD.
//However, if same number is provided in sequence, it changes value like a cellphone's text editor works
//if sw[1] and sw[2] are pushed concurrently, it clears LCD.
//if sw[4] and sw[5] are pushed concurrently, it change alphabet to number, number to alphabet.
//if sw[7] and sw[8] are pushed concurrently, it makes a blank.

void swModeFour(unsigned char* sw, unsigned char* outmem)
{
    if(sw[0] == 1)
    {
        memset(outmem+41,0,sizeof(unsigned char)*10);
        outmem[51] = 1;
        outmem[52] = 6;
        outmem[53] = 0;
        outmem[54] = 0;
    } //reset button
    outmem[41+outmem[53]] = outmem[54];
    //get previous position. (for blinking)
    if(sw[1] == 1){outmem[53] -= 1;if(outmem[53] < 0)outmem[53] = 0;}
    if(sw[3] == 1){outmem[52] += 1;if(outmem[52] > 6)outmem[52] = 6;}
    if(sw[7] == 1){outmem[53] += 1;if(outmem[53] > 9)outmem[53] = 9;}
    if(sw[5] == 1){outmem[52] -= 1;if(outmem[52] < 0)outmem[52] = 0;}
    // dealing current point's movement
    outmem[54] = outmem[41+outmem[53]];
    // save changed position. (for blinking)
    if(sw[2] == 1){if(outmem[51] == 1)outmem[51] =0;else outmem[51] = 1;}
    // set cursor blink or not
    if(sw[6] == 1){ memset(outmem+41,0,sizeof(unsigned char) * 10);outmem[54] = 0;}
    // clear all matrix but left cursor state and current position.
    if(sw[8] == 1)
    {
        int count;
        for(count = 9; count >= 0; count--)outmem[41+count] = outmem[41+count] ^ 0xff;
        outmem[54] = outmem[54] ^ 0xff;
    }
    //reverse all dot matrix
    if(sw[4] == 1)
    {
        if(outmem[41+outmem[53]] & (1<<outmem[52]) == 0){
            outmem[41+outmem[53]] = (outmem[41+outmem[53]]) | (1<<outmem[52]);
            outmem[54] = outmem[41+outmem[53]];
        }
        else{
            outmem[41+outmem[53]] = (outmem[41+outmem[53]]) ^ (1<<outmem[52]);
            outmem[54] = outmem[41+outmem[53]];
        }
    }
    //if a point is selected, then set it light if it was not, set it 0 if it was light before.
    outmem[4] += 1;
    if(outmem[4] >= 10){outmem[3] +=1;outmem[4] -= 10;}
    if(outmem[3] >= 10){outmem[2] +=1;outmem[3] -= 10;}
    if(outmem[2] >= 10){outmem[1] +=1;outmem[2] -= 10;}
    if(outmem[1] == 10)outmem[1] = 0;
    //480 ~ 484 lines are parts to deal with FND numbers.
    if(sw[0] == 1)
    {
        outmem[1] = 0;
        outmem[2] = 0;
        outmem[3] = 0;
        outmem[4] = 0;
    }
    //In case of reset button, set FND value to Zero.
}
//A function called when a switch has pushed and the mode is 4.
//if sw[0] (switch 1) is pushed, it changes digit in the order of 10-8-4-2-10 
//if sw[1] (switch 2) is pushed, it plus 100 to FND. (* if digit is 8, then it plus 1*8*8)
//if sw[2] (switch 3) is pushed, it plus 10 to FND.
//if sw[3] (switch 4) is pushed, it plus 1 to FND.

void swInit(unsigned char* inmem,unsigned char* outmem,int* md2)
{
    switch(inmem[1])
    {
        case 0:
            swModeOne(inmem+3, outmem);
            break;
        case 1:
            swModeTwo(inmem+3, outmem,md2);
            break;
        case 2:
            swModeThree(inmem+3, outmem);
            break;
        case 3:
            swModeFour(inmem+3,outmem);
            break;
        default:
            printf("There Must Be Error on Mode Initializaition!\n");
    }
    inmem[swChange] = 0;
    //set swChange'th value to 0 means a switch operation is done
}
//This function is called when a switch has pushed.
//It means, if a switch returns 1, swInit function is executed.
//following the mode state, it executes function for switch operation.
int checkSwitch(unsigned char* shmem, unsigned char but[])
{
    int i;
    for(i = MAX_BUTTON-1; i >= 0; i--)
    {
        shmem[i+3] = but[i];
        if(but[i] == 1)shmem[12] = 1;
    }
    if(shmem[12] == 1)return sendVal;
    return waitVal;

}
//if a switch has 1, it means input device sends signal for processing.
//therefore send the signal that it has a operation to do.
void executeReadkey(unsigned char* shmem, int code)
{
    //vol - : 114
    //vol + : 115
    //prog : 116
    //back : 158
    switch(code)
    {
        case 114:
            shmem[1] -= 1;
            if(shmem[1] == 255)shmem[1] = 3;
            shmem[2] = 1;
            break;
        case 115:
            shmem[1] += 1;
            if(shmem[1] == 4)shmem[1] = 0;
            shmem[2] = 1;
            break;
        case 116:
            break;
        case 158:
            shmem[0] = 1;
           
            break;
        default:
            break;
     }
}
//function to deal with read key.
void deviceOff(unsigned char* outmem)
{
    memset(outmem,0,sizeof(unsigned char)*54);
}
//to turn device off, change all output device's state to basic state.
int main(void)
{
	pid_t sender,receiver;
	int inTmain = 0;
	int mainTout = 0;
	unsigned char *inBuffer;
	unsigned char *inString; 
    //instring[0] : program off /0:program running  1: program shutdown
    //instring[1] : mode / 0 : Clock 1 : Counter 2 : Text editor 3 : Draw Board
    //instring[2] : mode change check / 0 : no mode changed 1 : mode changed
    //instring[3 ~ 11] : switch button
    //instring[12] : switch change check
	unsigned char *outBuffer;
	unsigned char *outString;
    //outstring[0] : program off /0:program running 1: program shutdown
    //outstring[1~4] : FND value
    //outstring[5] : LED state
    //outstring[6] : LED blinking / 0: no 1: yes
    //outstring[7~38] : TextLCD 
    //outstring[39] : TextLCDpointer
    //outstring[40] : eng or num / 0: eng 1: num
    //outstring[41~50] : dotmat
    //outstring[51] : Draw board blinking
    //outstring[52,53] : x,y of cursor
    //outstring[54] : current value of point at cursor
    //outstring[55] : is manipulated time ? / 0 : no 1 :yes
    //outstring[56] : mode / 0 : Clock 1 : counter 2 : Text editor 3 : Draw Board
	sender = fork();
    //sender : input process
    //parent : main process
    //receiver : output process
	if(sender == 0)
	{
        //input process's code
			sleep(1);
			inTmain = shmget((key_t)shmkeyInput,0,0);
			if(inTmain == -1)
			{
					perror("shmget failed");
					exit(0);
			}
			inBuffer = (unsigned char *)shmat(inTmain,NULL,0);
			inString = inBuffer+1;
			char* deviceReadkey = "/dev/input/event0";
			char* deviceSwitch = "/dev/fpga_push_switch";
            struct input_event ev[BUFF_SIZE];
            unsigned char swBuff[MAX_BUTTON];
			int readkey,rd,rkvalue,size = sizeof(struct input_event);
			int sw;
           
            if((readkey = open(deviceReadkey, O_RDONLY|O_NONBLOCK)) == -1){
				printf("Device Open Error: %s\n",deviceReadkey);
			}
            if((sw = open(deviceSwitch,O_RDWR|O_NONBLOCK)) == -1){
                printf("Device Open Error: %S\n",deviceSwitch);
            }
            //initial part. get device driver of switch and readkey.
			while(1)
			{
                //waiting state.
                //at each second, check if switch or readkey send signal.
				if ((rd = read (readkey, ev, size * BUFF_SIZE)) != -1)
				{    
                    rkvalue = ev[0].value;
				}
               
				if( rkvalue == KEY_PRESS ) {
                    executeReadkey(inString,ev[0].code);
                    if(ev[0].code == 158)
                    {
                        
                        inBuffer[0] = sendVal;
                        shmdt(inBuffer);
                        exit(0);
                    }
                    rkvalue = KEY_INPUT_DONE;
                    inBuffer[0] = sendVal;
                    sleep(1);
				}
                read(sw,&swBuff,sizeof(swBuff));
                if(checkSwitch(inString,swBuff) == sendVal)
                {
                    inBuffer[0] = sendVal;
                    sleep(1);
                }
			    
				if(inBuffer[0] == doingVal)
				{
			
					inBuffer[0] = waitVal;
				}

			}
	}
	else{
		receiver = fork();

		if(receiver == 0)
		{
            //output process's code
				sleep(1);
				mainTout = shmget((key_t)shmkeyOutput,0,0);
				if(mainTout == -1)
				{
						perror("shmat failed:");
						exit(0);
                        return;
				}
				outBuffer = (unsigned char *)shmat(mainTout,NULL,0);
				outString = outBuffer + 1;
				outBuffer[0] = waitVal;
                int devFND;
                int devTextLCD;
                int devDotMat;
                int devLED;
                int counter = 0;
                devFND = open("/dev/fpga_fnd", O_RDWR);
                devLED = open("/dev/mem",O_RDWR|O_SYNC);
                devTextLCD = open("/dev/fpga_text_lcd",O_WRONLY);
                devDotMat = open("/dev/fpga_dot",O_WRONLY);
                //get device driver of output devices
                if(devFND <0 || devLED < 0 || devTextLCD < 0 || devDotMat < 0){
                    printf("Device open error\n");
                    exit(1);
                }
				while(1)
				{	
                    //change output devices at every second				
							if(outString[0] == 1)
                            {
                                //turning off devices.
                                deviceOff(outString);
                                writeFND(devFND,outString+1);
                                writeLED(devLED,outString+5);
                                writeDotMat(devDotMat,outString+41);
                                writeTextLCD(devTextLCD,outString+7);
                                shmdt(outBuffer);
                                exit(0);
                            }
                            
                            writeFND(devFND,outString+1);
                            writeLED(devLED,outString+5);
                            modeOneBlink(outString);
                            writeDotMat(devDotMat,outString+41);
                            writeTextLCD(devTextLCD,outString+7);
                            modeFourBlink(outString);
                            outBuffer[0] =doingVal;
                            sleep(1);
                            counter++;
                            if(counter == 60 && outString[56] == 0)
                            {
                                minuteUp(outString);
                                counter = 0;
                            }
				}
		}
		else
		{
            //main process's code
				inTmain = shmget((key_t)shmkeyInput,(size_t)Memsize,0666|IPC_CREAT);
				mainTout = shmget((key_t)shmkeyOutput,(size_t)Memsize,0666|IPC_CREAT);
            //create shared memory at parent part.
            //for safety, set sleep 1 seconds to childs to create part declared before.
				inBuffer = (char *)shmat(inTmain,NULL,0);
				outBuffer = (char *)shmat(mainTout,NULL,0);
                memset(inBuffer,0,sizeof(inBuffer));
                memset(outBuffer,0,sizeof(inBuffer));
                int child;
                int modeTwoNum=0;
				inString = inBuffer+1;
				outString = outBuffer + 1;
				outBuffer[0] = 0;
                inBuffer[0] = waitVal;
                modeInit(inString,outString);
                outBuffer[0] = sendVal;
				while(1)
				{
						if(inBuffer[0] ==  sendVal)
						{
                            //sendVal : input devices got values to deal with.
							//doingVal : a process has done some operation.
                                if(inString[0] == 1)
                                {
                                    //turning off device.
                                    //let child processes exit first
                                    outString[0] = 1;
                                    outBuffer[0] = sendVal;
                                    waitpid(receiver,&child ,0);
                                    waitpid(sender,&child,0);                                   
                                    shmdt(inBuffer);
                                    shmdt(outBuffer);
                                    shmctl(inTmain,IPC_RMID,NULL);
                                    shmctl(mainTout,IPC_RMID,NULL);
                                    return;
                                }
                                if(inString[modeChange] == 1)
                                {
                                    modeInit(inString,outString);
                                    outBuffer[0] = sendVal;
                    
                                    
                                }
                                if(inString[swChange] == 1)
                                {
                                    swInit(inString,outString,&modeTwoNum);
                                    outBuffer[0] = sendVal;
                                }
                                inBuffer[0] = doingVal;
						}
                        if(outBuffer[0] == doingVal)outBuffer[0] = waitVal;
				}
				
		}
	}
}
	
