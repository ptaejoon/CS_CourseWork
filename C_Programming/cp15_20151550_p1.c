#include<stdio.h>

typedef struct{
	int numerator; //분자
	int denominator; //분모
	}FRACTION;

void multFr(FRACTION* pFr1,FRACTION* pFr2, FRACTION* pRes)
{
	pRes->numerator = pFr1->numerator * pFr2->numerator;
	pRes->denominator = pFr1->denominator * pFr2->denominator;
}
int main()
{
	FRACTION pFr1;
	FRACTION pFr2;
	FRACTION pRes;
	printf(" x/y : ");
	scanf("%d",&pFr1.numerator);
	getchar();
	scanf("%d",&pFr1.denominator);

	printf(" x/y : ");
	scanf("%d",&pFr2.numerator);
	getchar();
	scanf("%d",&pFr2.denominator);
	multFr(&pFr1,&pFr2,&pRes);
	printf("%d/%d * %d/%d = %d/%d\n",pFr1.numerator,pFr1.denominator,pFr2.numerator,pFr2.denominator,pRes.numerator,pRes.denominator);
}
