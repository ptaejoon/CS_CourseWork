#include<stdio.h>
#include<stdlib.h>

int *get_next_process(int *prev_pointer,int size){
	int next_size;
	int *next_pointer;
	int count;
	if(size % 2 == 0)
	{
		next_size = size/2;
	}
	else
	{
		next_size = size/2 +1;
	}
	next_pointer = (int *)malloc(next_size*sizeof(int));
	for(count = 0; count < next_size; count++)
	{
		next_pointer[count] = prev_pointer[2*count]+prev_pointer[2*count+1];
		printf("%d ",next_pointer[count]);
	}
	printf("\n");
	if(next_size != 1)
	{
		get_next_process(next_pointer,next_size);
	}
	free(next_pointer);
	return next_pointer;
}
int swap(int *a,int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int main()
{
	int n,count,count2;
	scanf("%d",&n);
	int *array;
	array = (int *)malloc(sizeof(int)*n);
	for(count = 0; count < n; count++)
	{
		scanf("%d",array+count);
	}
	for(count = 0; count < n-1 ; count++)
	{
		for(count2 = 0; count2 < n-count-1; count2++)
		{
			swap(&array[count2],&array[count2+1]);
		}
	}
	get_next_process(array,n);
	free(array);
	return 0;
}	
