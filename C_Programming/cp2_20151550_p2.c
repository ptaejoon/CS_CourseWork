#include<stdio.h>

void swap(int *a,int *b);
int *f(int *a, int *b, int *c);

int main()
{
	int a,b,c;
	scanf("%d %d %d", &a,&b,&c);
	
	printf("%d\n",*f(&a,&b,&c));
	printf("%d %d %d\n",a ,b,c);

	return 0;

}

void swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
int *f(int *a, int *b, int *c)
{
	int temp;
	if(*a > *b)
	{
		swap(a,b);
	}
	if(*c <  *b)
	{
		swap(b,c);
	}
	if(*a > *b)
	{
		swap(a,b);
	}
	return b;
}
