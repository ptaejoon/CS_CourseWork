#include<stdio.h>

void swap(int *a,int *b);
void sort(int *a,int m);
void print_array(int *a, int m);
void all_sort(int a[][100],int n,int m);
void print_all_array(int a[][100],int n,int m);

int main(){
	int n,m;
	int array[100][100];
	scanf("%d %d",&n,&m);
	int count,count2;
	for(count = 0; count<n; count++)
	{
		for(count2 = 0; count2<m; count2++)
		{
			scanf("%d",&array[count][count2]);
		}
	}
	all_sort(array,n,m);
	print_all_array(array,n,m);
	return 0;

}
void swap(int *a, int *b)
{
	int temp;
	temp = *b;
	*b = *a;
	*a = temp;
	
}
void sort(int *a,int m)
{
	int count,count2;
	for(count = 0; count < m-1; count++)
	{
		for(count2 =0; count2<m-count-1;count2++)
		{
			if(a[count2]>a[count2+1])
			swap(&a[count2],&a[count2+1]);
		}
	}
}
void point_array(int *a, int m)
{
	int count;
	for(count =0; count<m; count++){
		printf("%d ", a[count]);
	}
	printf("\n");
}
void all_sort(int a[][100],int n, int m)
{
	int count;
	for(count = 0; count < n; count++)
	{
		sort(a[count],m);
	}
}
void print_all_array(int a[][100],int n, int m)
{
	int count,count2;
	for(count =0; count < n ; count++){
		
		point_array(a[count],m);
	}
}
