#include<stdio.h>
#include<stdlib.h>
void double_double(int **a,int *size);
void swap(int *a, int *b);
int main()
{
	int n;
	int *array;
	int count;
	scanf("%d",&n);
	array = (int*)malloc(sizeof(int)*n);
	for(count = 0; count < n; count++)
	{
		scanf("%d",&array[count]);
	}
	double_double(&array,&n);
	for(count = 0; count < n; count++)
	{
		printf("%d ",array[count]);
	}
	printf("\n");
	double_double(&array,&n);
	for(count = 0; count < n; count++)
	{
		printf("%d ",array[count]);
	}
	printf("\n");
	
	double_double(&array,&n);
	for(count = 0; count < n; count++)
	{
		printf("%d ",array[count]);
	}
	free(array);
	printf("\n");
}

void double_double(int **a,int *size)
{
	int num,count,count2;
	num = *size *2;
	*a = (int*)realloc(*a, num *sizeof(int));
	for(count = 0; count < *size; count++)
	{
		*(*(a)+count+*size) =  *(*(a)+count) * 2;
	}
	for(count = 0; count < num-1; count++)
	{
		for(count2 = 0; count2 < num-1-count ; count2++)
		{
			swap((*a+count2),(*a+count2+1));
		}
	}
	*size *=2;
}
void swap(int *a, int *b)
{
	int temp=*a;
	if(*a > *b)
	{
		*a = *b;
		*b = temp;
	}
}
