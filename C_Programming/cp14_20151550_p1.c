#include<stdio.h>
#include<stdlib.h>
#include<math.h>
typedef float* Vector_Comp;
typedef float Comp;
typedef double VectorSize;
typedef double Distance;
typedef float Scalar;
typedef struct{
	Vector_Comp comps;
	int vec_size;
}Vector;

VectorSize vSize(Vector v)
{
	VectorSize a;
	int count;
	for(count = 0; count < v.vec_size ; count++)
	{
		a  += v.comps[count]*v.comps[count];
	}
	return sqrt(a);
}
Distance DistVector(Vector v1, Vector v2)
{
	int count;
	Distance result;
	float value;
	for(count = 0; count < v1.vec_size; count++)
	{
		value = v1.comps[count] - v2.comps[count];
		result += value*value;
	}
	return sqrt(result);
}
Scalar innerProduct(Vector v1, Vector v2)
{
	int count;
	float value;
	Scalar result;
	for(count = 0; count < v1.vec_size; count++)
	{
		value = v1.comps[count] * v2.comps[count];
		result += value;
	}
	return result;
}

int main()
{
	int count;
	Vector v1;
	printf("size of vector v1 : ");
	scanf("%d",&v1.vec_size);
	v1.comps = (float *)malloc((v1.vec_size+1)*sizeof(float));
	printf("vector 1 : ");
	for(count = 0; count < v1.vec_size; count++)
	{
		scanf("%f",v1.comps+count);
	}


	Vector v2;
	printf("size of vector v2 : ");
	scanf("%d",&v2.vec_size);
	v2.comps = (float *)malloc((v2.vec_size+1)*sizeof(float));
	printf("vector 1 : ");
	for(count = 0; count < v2.vec_size; count++)
	{
		scanf("%f",v2.comps+count);
	}
	printf("\n\n");
	printf("--------------------------\n\n");
	printf("Size of v1 = %.2f\n",vSize(v1));
	printf("Size of v2 = %.2f\n",vSize(v2));
	if(v1.vec_size == v2.vec_size)
	{
		printf("Distance v1,v2 = %.3f\n",DistVector(v1,v2));
		printf("Inner Product v1 * v2 = %f\n",innerProduct(v1,v2));
	}
	free(v1.comps);
	free(v2.comps);
}
