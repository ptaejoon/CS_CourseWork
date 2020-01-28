#include<stdio.h>
#include<stdlib.h>
int *get_new_array(int size);
void get_new_array_call_by_value(int *a,int size);
void get_new_array_call_by_reference(int **a, int size);
void print_state(int *a);
int main()
{
	int *a = NULL;
	print_state(a);

	get_new_array_call_by_value(a,10);
	print_state(a);
	free(a);

	get_new_array_call_by_reference(&a,10);
	print_state(a);
	free(a);

	a = get_new_array(10);
	print_state(a);
	free(a);
	return 0;
}
int *get_new_array(int size)
{
	int *array;
	array = (int *)malloc(sizeof(int)*size);
	return array;
}
void get_new_array_call_by_value(int *a,int size)
{
	a = (int *)malloc(sizeof(int)*size); // call by reference와 달리 a 가 동적할당을 받음.
}
void get_new_array_call_by_reference(int **a, int size)
{
		*a = (int *)malloc(sizeof(int)*size); // call by reference 는 포인터 변수 *a가 동적 할당을 받음.
							// malloc은 int를 위한 기억 장소를 할당해 그 시작 주소값을 되돌려주는 함수이다. 그런데 call by value는 변수의 값을 넘겨주는 방식이고, call by reference는 변수의 주소를 넘겨주는 방식이므로 malloc함수를 통해 메모리를 할당받으려면 call by reference를 이용해야 한다.
}
void print_state( int *a)
{
	if(a == NULL)
	{
		printf("not allocated\n");
	}
	else
	{
		printf("allocated\n");
	}
}
