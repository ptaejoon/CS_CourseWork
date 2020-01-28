#include<stdio.h>
void mat_mul(int a[100][100],int b[100][100],int c[100][100],int N);
int main()
{
	int count,count2;
	int n;
	int a[100][100];
	int b[100][100];
	int c[100][100];
	scanf("%d",&n);
	for(count = 0; count < n; count++)
	{	
		for(count2 = 0; count2< n; count2++)
		{
			scanf("%d",(*(a+count)+count2));
		}
	}
	for(count = 0; count < n; count++)
	{	
		for(count2= 0; count2<n;count2++)
		{	
			scanf("%d",(*(b+count)+count2));
		}
	}
	mat_mul(a,b,c,n);
	for(count = 0; count < n; count++)
	{
		for(count2= 0;count2<n;count2++)
		{
			printf("%d ",*(*(c+count)+count2));
		}
		printf("\n");
	}
	return 0;
}
void mat_mul(int a[100][100],int b[100][100],int c[100][100],int N)
{
	int count,count2,count3;

	for(count = 0; count < N; count++)
	{
		for(count2 = 0; count2 < N; count2++)
		{
			*(*(c+count)+count2) = 0;
			for(count3 = 0; count3 < N; count3++)
			{

				
				*(*(c+count)+count2) += *(*(a+count)+count3) * *(*(b+count3)+count2);

			}
		}
	}
}
	
