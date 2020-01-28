#include<stdio.h>

void swap(int *a,int *b);
void sort(int *a,int n);
void print_array(int *a,int n);

int main()
{
	int count;
	int n;
	int a[1001];
	scanf("%d", &n);
	for(count = 0; count < n; count++)
	{
		scanf("%d",&a[count]);
	}
	sort(a , n);
	print_array(a,n);
}

void swap(int *a, int *b)
{
	int temp;
	if(*a > *b)
	{
		temp = *b;
		*b = *a;
		*a = temp;
	}
}
void sort(int *a, int n)
{
	int count1,count2;
	for(count1 = 0; count1 < n-1; count1++)
	{
		for(count2 = 0; count2 < n-count1-1; count2++)
		{
			swap((a+count2),(a+count2+1));
			
			
		}
	}
}
void print_array(int *a, int n)
{
	int count;
	for(count = 0; count < n; count++)
	{
		printf("%d ",*(a+count));
	}
	printf("\n");
}

	
