
#include<stdio.h>
#include<stdlib.h>

typedef struct node{
	int data;
	struct node *link;
}NODE;
void Insert(NODE* n,int* a,int check)
{
	int num = *a;
	n->link = (NODE *)malloc(sizeof(NODE));
	if(num != check)
	{
		n->data = num;
	}
	else
	{
		n->data = num;
		return ;
	}
	if(&n->data != NULL)
	{	
		Insert(n->link,a+1,check);
	}
}
void PrintAll(NODE *n,int check)
{
	

	
		printf("%d ",n->data);
		if(check != n->data)
		{
			PrintAll(n->link,check);
			free(n);
		}
		else{
			printf("\n");
		}
	
}

int main()
{
	int A[10] = {3,9,8,2,5,10,7,1,4,6};
	int a = A[9];
	NODE *n;
	NODE *p;
	int count;
	n = (NODE *)malloc(sizeof(NODE));
	p = n;
	Insert(n,A,a);
	PrintAll(p,a);

	

}
