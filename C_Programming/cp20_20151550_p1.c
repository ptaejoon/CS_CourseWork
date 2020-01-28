#include<stdio.h>
#include<stdlib.h>
typedef struct node* nptr;
typedef struct node{
	int data;
	nptr link;
}NODE;

void Insert(nptr *n,int number)
{
	nptr pn;
	pn = (nptr)malloc(sizeof(NODE));
	if((*n)==NULL)
	{
		*n = pn;
		pn->data = number;
		pn->link = NULL;
		return;
	}
	
	pn->data = (*n)->data;
	(*n)->data = number;
	pn->link = (*n)->link;
	(*n)->link = pn;
}
void PrintAll(nptr n)
{
	while(n->link != NULL)
	{
		printf("%d ",n->data);
		n = n->link;
	}
	printf("%d\n",n->data);
}

int main(){
	nptr n;
	n = (nptr)malloc(sizeof(NODE));
	n = NULL;
	int A[10] = {3,9,8,2,5,10,7,1,4,6};
	int count;
	for(count = 0; count < 10; count++)
	{
		Insert(&n,A[count]);
	}

	PrintAll(n);
	for(n ; n->link != NULL; n=n->link)
	{
		free(n);
	}
}

