#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
typedef struct {
	int hour;
	int minute;
	int second;
	}Time;
	


	
int main()
{
	time_t timer;
	struct tm *t;
	char check[10];
	timer = time(NULL);
	int count=0;
	t = localtime(&timer);
	Time* prevTimePtr;
	prevTimePtr = (Time*)malloc(sizeof(Time));
	prevTimePtr->hour = t->tm_hour;
	prevTimePtr->minute = t->tm_min;
	prevTimePtr->second = t->tm_sec;
	Time currTime;
	int a,b,c;

	
	while(1)
	{
		timer = time(NULL);
		t = localtime(&timer);
		currTime.hour = t->tm_hour;
		currTime.minute = t->tm_min;
		currTime.second = t->tm_sec;
		printf("system> ");
		gets(check);
		if(strcmp(check, "check")==0)
		{
			count++;
			printf("Check Point #%.2d \n",count);
			printf("Current Time = %02d : %02d : %02d \n", currTime.hour,currTime.minute,currTime.second);
			printf("Previous Time = %02d : %02d : %02d \n",prevTimePtr->hour,prevTimePtr->minute,prevTimePtr->second);
			a = currTime.hour-prevTimePtr->hour;
			b = currTime.minute-prevTimePtr->minute;
			c = currTime.second-prevTimePtr->second;
			if(c < 0)
			{
				c += 60;
				b -= 1;
			}
			if(b < 0)
			{
				b += 60;
				a -=1;
			}	

			
				
			
			printf("Elasped Time = %02d : %02d : %02d \n", a,b,c);
		
			
		}
		if(strcmp(check , "quit")==0)
		{
			break;

		}
		prevTimePtr->hour = currTime.hour;
		prevTimePtr->minute = currTime.minute;
		prevTimePtr->second = currTime.second;
	}

}
