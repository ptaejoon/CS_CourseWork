#include<stdio.h>

void shift_left(int *a);
void shift_n_left(int *a,int n);
void print_array(int *a);

int main()
{
	
	int a[10] = {1,2,3,4,5,6,7,8,9,10};
	int n;
	scanf("%d",&n);
	
	shift_n_left(a,n);
	print_array(a);
}

void shift_left(int *a)
{
	int n;
	int count;

	n = *a;

	for(count = 0; count <9; count++)
	{

	*(a+count) = *(a + count + 1);
	}

	*(a+9) = n;
	
}
void shift_n_left(int *a, int n)
{
	int count;
	for(count=0; count < n%10 ; count++)
	{
		
		shift_left(a);
	}
}
void print_array(int *a)
{	
	int n;
	for(n=0; n<10 ; n++)
	{

		printf("%d ",*(a+n));
	}
	printf("\n");
}

