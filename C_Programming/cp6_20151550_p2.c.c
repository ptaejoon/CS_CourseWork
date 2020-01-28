#include<stdio.h>

void swap(int *a,int*b);
void sort(int *a,int m);
void all_sort(int a[][100],int n, int m);
int binary_search(int *a,int *endPtr, int key, int **findPtr);
int all_binary_search(int a[][100],int n,int m,int key,int **findPtr);

int main()
{
	int count,count2;
	int n,m,k;
	int *findPtr;
	int array[100][100];
	scanf("%d %d %d", &n,&m,&k);
	for(count = 0; count < n; count++)
	{
		for(count2 = 0; count2 < m; count2++)
		{
			scanf("%d",&array[count][count2]);
		}
	}

	all_sort(array,n,m);

	if(all_binary_search(array,n,m,k,&findPtr) == 1)
	{
		printf("%d\n",all_binary_search(array,n,m,k,&findPtr));
		printf("%p\n",findPtr);
	}
	else
	{
		printf("%d\n",all_binary_search(array,n,m,k,&findPtr));
	}
	return 0;
}
void swap(int *a, int *b)
{
	int temp;
	if(*a > *b)
	{
		temp = *a;
		*a = *b;
		*b = temp;
	}
}
void sort(int *a, int m)
{
	int count,count2;
	for(count =0; count < m-1; count++)
	{
		for(count2 = 0; count2 < m-count-1; count2++)
		{
			swap(&a[count2],&a[count2+1]);
		}
	}
}
void all_sort(int a[][100],int n, int m)
{
	int count;
	for(count = 0; count < n; count++)
	{
		sort(a[count],m);
	}
}
int binary_search(int *a,int *endPtr,int key,int **findPtr)
{
	int *temp;
	int *start=a;
	do
	{
		
		temp = start + (endPtr - start)/2;
		if(key == *temp){
			*findPtr = temp;
			return 1;
		}
		if(key > *temp)
		{
			start = temp+1;
		}
		if(key < *temp)
		{
			endPtr = temp-1;
		}
	
	}while(start <= endPtr);
	return 0;;
}



int all_binary_search(int a[][100],int n,int m,int key, int **findPtr)
{
	int count;
	for(count = 0; count < n; count++)
	{
		if(binary_search(a[count],a[count]+m-1,key,findPtr))
		{
			
			return 1;
		}
	}
	return 0;
}
