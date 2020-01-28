#include<stdio.h>
void* f(void *pa,void *pb, int option);
int main()
{
	int option;
	int a,b;
	float fa,fb;
	double da,db;
	void *pa,*pb,*ans;
	
	scanf("%d",&option);
	if(option == 0)
	{
		scanf("%d %d",&a,&b);
		pa=&a;
		pb=&b;
		ans =f(pa,pb,option);
		printf("%d\n",*(int*)ans);
	}
	else if(option == 1)
	{
		scanf("%f %f",&fa, &fb);
		pa=&fa;
		pb=&fb;
		ans = f(pa,pb,option);
		printf("%f\n",*(float*)ans);
	}
	else if(option == 2)
	{
		scanf("%lf %lf",&da, &db);
		pa=&da;
		pb=&db;
		ans = f(pa,pb,option);
		printf("%lf\n",*(double*)ans);
	}
	

}
void* f(void *pa,void *pb,int option)
{
	if(option == 0)
	{
		if(*(int*)pa <*(int*)pb)
		{
			return pa;
		}
		else
		{
			return pb;
		}
	}
	if(option == 1)
	{	
		if(*(float*)pa < *(float*)pb)
		{
			return pa;
		}
		else
		{
			return pb;
		}
	}
	if(option == 2)
	{
		if(*(double*)pa < *(double*)pb)
		{
			return pa;
		}
		else
		{
			return pb;
		}
	}
}

