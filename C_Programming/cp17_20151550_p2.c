#include<stdio.h>
typedef struct {
	double x;
	double y;
	double z;
	double r;
}SPHERE;

double Distance(SPHERE one, SPHERE two)
{
	double a;
	a = (one.x-two.x)*(one.x-two.x)+(one.y-two.y)*(one.y-two.y)+(one.z-two.z)*(one.z-two.z);
	return a;
}
double CrossSectionArea(SPHERE one, SPHERE two)
{
	double a;
	a = one.r*one.r*3.14+two.r*two.r*3.14;
	return a;
}
double SurfaceArea(SPHERE one, SPHERE two)
{
	double a;
	a = 4*3.14*(one.r*one.r + two.r*two.r);
	return a;
}
double Volume(SPHERE one, SPHERE two)
{
	double a;
	a = 4 * 3.14 * (one.r*one.r*one.r + two.r*two.r*two.r) / 3;
	return a;
}

int main()
{
	SPHERE one,two;
	printf("Type First Sphere Coordinate : ");
	scanf("%lf %lf %lf",&one.x,&one.y,&one.z);
	printf("Type First Sphere Radius : ");
	scanf("%lf",&one.r);
	printf("Type Second Sphere Coordinate : ");
	scanf("%lf %lf %lf",&two.x,&two.y,&two.z);
	printf("Type Second Sphere Radius : ");
	scanf("%lf",&two.r);
	if((one.r+two.r)*(one.r+two.r) < Distance(one,two))
	{
		printf("Sum of Cross Section Area : %.2lf\n",CrossSectionArea(one,two));
	}
	if((one.r+two.r)*(one.r+two.r) == Distance(one,two))
	{
		printf("Sum of Surface Area : %.2lf\n",SurfaceArea(one, two));
	}
	if((one.r+two.r)*(one.r+two.r) > Distance(one,two))
	{
		printf("Sum of Volume : %.2lf\n",Volume(one,two));
	}
	return 0;
}
