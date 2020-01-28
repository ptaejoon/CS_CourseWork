#include<stdio.h>
void f(int *H,int *M,int *S,int T);
int main()
{
	int h,m,s,t;
	scanf("%d %d %d %d",&h,&m,&s,&t);
	f(&h,&m,&s,t);
	printf("%02d:%02d:%02d\n",h,m,s);
	return 0;
}

void f(int *H,int *M,int *S,int T)
{
	int a,b,c;
	a = T/3600;
	b = (T-a*3600)/60;
	c = (T-a*3600-b*60);
	*S += c;
	if(*S >= 60)
	{
		*S -=60;
		b +=1;

	}
	*M += b;
	if(*M >= 60)
	{
		*M -= 60;
		a +=1;
	}
	*H += a;
	if(*H >= 24)
	{
		*H -= 24;
	}
}

