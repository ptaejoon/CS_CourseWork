#include<stdio.h>

int main(){
	int a;
	int *pa;
	int **ppa;
	scanf("%d",&a);
	pa = &a;
	ppa = &pa;



	printf("%d\n",is_prime(&ppa));
	return 0;
}
int is_prime(int ***a)
{
	int num;

	
	if(***a <= 1)
	{
	
		return 0;
	}

	else
	{
		
		for(num=2; num <=***a/2; num++)
		{
			
			if(***a % num == 0)
			{
		
				return 0;
			}
		}
		return 1;
	}
}
