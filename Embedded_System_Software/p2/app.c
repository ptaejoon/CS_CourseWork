#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <asm/ioctl.h>
#include <string.h>

#define IOCTL_INIT _IOR(242,0,unsigned char *)
#define IOCTL_START_TIMER _IO(242,1)
int main(int argc, char** argv)
{
    int interval;
    int cnt;
    unsigned char init[6];
    if(argc != 4)
    {
        printf("Ex) ./app TIMER_INTERVAL[1-100] TIMER_CNT[1-100] TIMER_INIT[0001-8000]  \n");
        return 0;
    }
    int df = open("/dev/dev_driver",O_WRONLY);
    /* opend device driver */
    interval = atoi(argv[1]);
    cnt = atoi(argv[2]);
    if(cnt < 1 ||cnt > 100)
    {
        printf("TIMER_CNT is in between [1 - 100]\n");
        return 1;
    }
    if(interval < 1 || interval > 100)
    {
        printf("TIMER_INTERVAL is in between [1 - 100]\n");
        return 2;
    }
    int count;
    int zeroNum = 0;
    for(count =0; count < 4; count++)
    {
        if(argv[3][count] == '0')zeroNum += 1;
        if(argv[3][count] < '0' || argv[3][count] > '8')
        {
            printf("TIMER_INIT is in between [0001-8000]\n");
            return 3;
        }
    }
    /* check whether timer_init have 3 zero and one non-zero num */
    if(zeroNum != 3)
    {
        printf("TIMER_INIT should have only one non-zero number");
    }
    init[0] = interval;
    init[1] = cnt;
    memcpy(init+2,argv[3],4);
    ioctl(df,IOCTL_INIT ,init);
    /* call ioctl for initialization */
    ioctl(df,IOCTL_START_TIMER);
    /* call ioctl for timer */

    close(df);
    return 4;
}
