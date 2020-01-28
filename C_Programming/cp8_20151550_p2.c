#include<stdio.h>
#include<stdlib.h>

void push_back(int **a, int *size, int value);
void pop_back(int **a, int *size);
double average(int *a, int size);
double variance(int *a, int size);
int main()
{
	int n;
	int *array;
	array = (int *)malloc(sizeof(int));
	scanf("%d",&n);
	int count,count2;
	int status;
	int size=0;
	int value;
	for(count = 0; count < n; count ++)
	{
		scanf("%d",&status);
		if(status == 1)
		{
			scanf("%d", &value);
			push_back(&array,&size,value);
			for(count2 = 0; count2 < size; count2++)
			{
				printf("%d ",array[count2]);
			}
			printf("\n");
			printf("%lf %lf\n",average(array,size),variance(array,size));
		}
		if(status == 2)
		{
			pop_back(&array,&size);
			for(count2 = 0; count2 < size; count2++)
			{
				printf("%d ",array[count2]);
			}
			printf("\n");
			printf("%lf %lf\n",average(array,size),variance(array,size));
		}
	}
	free(array);
}
void push_back(int **a, int *size, int value)
{	
	*size += 1;
	*a=(int *)realloc(*a,(*size)*sizeof(int));
	*(*a+*size-1) = value;
}
void pop_back(int **a, int *size)
{
	*size -= 1;
	*a=(int *)realloc(*a,(*size)*sizeof(int));
}
double average(int *a, int size)
{
	int count;
	double result=0;
	for(count = 0; count < size ; count++)
	{
		result += a[count];
	}
	result /= size;
	return result;
}
double variance(int *a, int size)
{
	double av = average(a,size);
	int count;
	double result;
	for(count = 0; count < size; count++)
	{
		result += (a[count]-av)*(a[count]-av);
	}
	result /= size;
	return result;
}
