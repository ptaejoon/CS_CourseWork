#include<stdio.h>

int main()
{
	int H,W,N;
	int n;
	scanf("%d %d %d",&H,&W,&N);
	n = N;
	int a=0;
	int b=0;
	int room[9][99];
	
	while(n != 1)
	{
		
		*(*(room + a) + b) = n;
		a++;
		
		if(a == H)
		{
			a = 0;
			b +=1;
		}
		n -= 1;
	}
	printf("%d%.2d\n",a+1,b+1);
	return 0;
}

