#include<stdio.h>
#include<string.h>
#include<stdlib.h>
// union은 struct와 사용방식은 같지만 메모리를 아끼기 위해 사용된다.
// 만약 밑의 short,medium,long을 struct로 선언해주었을 경우 main문의 u의 크기는 10+30+50의 90byte였을 것이다.
// 반면 union은 union내부에서 선언된 변수들 중 가장 많은 메모리를 차지하는 변수의 크기만큼만 할당한다.
// 이 세 개의 변수들이 50byte의 메모리공간을 공유하여 사용한다.
typedef union{
	char Short[10];
	char Medium[30];
	char Long[50]; // 이 union내부에서 가장 큰 변수는 Long이고 크기는 50byte이다.
	// union의 크기는 따라서 Long의 크기가 된다.
}Name;

int main()
{
	int n;
	char string[50];
	Name u;
	
	while(1){
		printf("Type the string : ");
		gets(string);
		n = strlen(string);
		if(n < 10)
		{
			strcpy(u.Short,string);
			printf("Saved in Short Size.\n");
			printf("Size of Data : %d Bytes\n",(int)sizeof(u));
		}
		else if(n < 30)
		{
			strcpy(u.Medium,string);
			printf("Saved in Medium Size.\n");
			printf("Size of Data : %d Bytes\n",(int)sizeof(u));
		}
		else if(n < 50)
		{
			strcpy(u.Long,string);
			printf("Saved in Long Size. \n");
			printf("Size of Data : %d Bytes\n", (int)sizeof(u));
		}
	}
}
