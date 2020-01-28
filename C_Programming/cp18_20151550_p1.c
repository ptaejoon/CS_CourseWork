#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef union
{
	int i;
	float f;
	double d;
}utemp;
typedef struct 
{
	int type;
	char name[20];
	utemp u;
}stemp;

void power(stemp *s)
{
	if(s->type == 1)
	{
		s->u.i = s->u.i * s->u.i;
	}
	if(s->type == 2)
	{
		s->u.f = s->u.f * s->u.f;
	}
	if(s->type == 3)
	{
		s->u.d = s->u.d * s->u.d;
	}
}
int main()
{
	int n;
	int count;
	int check;
	printf("Number of Iterations : ");
	scanf("%d",&n);
	stemp *s;
	s = (stemp *)malloc((n+1)*sizeof(stemp));
	for(count = 0; count < n; count++)
	{
		printf("Type [1 for int, 2 for float, 3 for double] : ");
		scanf("%d",&s[count].type);
		if(s[count].type == 1)
		{
			printf("Enter an integer value : ");
			scanf("%d",&s[count].u.i);
			strcpy(s[count].name,"integer");
		}
		if(s[count].type == 2)
		{
			printf("Enter a float value : ");
			scanf("%f",&s[count].u.f);
			strcpy(s[count].name,"float");
		}
		if(s[count].type == 3)
		{
			printf("Enter a double value : ");
			scanf("%lf",&s[count].u.d);
			strcpy(s[count].name,"double");
		}
		power(&s[count]);
	}
	
	printf("----------Result----------\n");
	for(count = 0; count < n; count++)
	{
		if(s[count].type == 1)
		{
			printf("%-10s: %d\n",s[count].name,s[count].u.i);
		}
		if(s[count].type == 2)
		{
			printf("%-10s: %.4f\n",s[count].name,s[count].u.f);
		}
		if(s[count].type == 3)
		{
			printf("%-10s: %.4lf\n",s[count].name,s[count].u.d);
		}
	}

	free(s);
}

	
