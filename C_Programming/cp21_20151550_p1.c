#include<stdio.h>
#include<stdlib.h>
typedef struct node* nptr;
typedef struct node
{
	int data;
	nptr link;
}NODE;

void Insert(nptr p,int num)
{
	nptr pnew;
	nptr a=p;
	if (p == NULL)
	{
		p->data=num;
		p->link = NULL;
	}
	else
	{
		while (a->link != NULL)
		{
			a = a->link;
		}
		pnew = (nptr)malloc(sizeof(NODE));
		pnew->data = num;
		pnew->link = NULL;
		a->link = pnew;
			
	}
	

}

void Delete(nptr* p)
{
	nptr pnew = *p;
	*p = (*p)->link;
	free(pnew);
}
void PrintAll(nptr p)
{
	nptr pnew;
	if(p == NULL)
	{
		printf("Empty\n");
		return 0;
	}
	
	while(p->link != NULL)
	{
		printf("%d ",p->data);
		p = p->link;
	}
	printf("%d ", p->data);
	printf("\n");
		
}
int main()
{
	int A[10] = { 3, 9, 8, 2, 5, 10, 7, 1, 4, 6};
	int count;
	nptr head = NULL;
	head = (nptr)malloc(sizeof(NODE));
	head->link = (nptr)malloc(sizeof(NODE));
	head->link = NULL;
	for(count = 0; count < 10; count++)
	{
		Insert(head->link,A[count]);
	}
	
	for(count = 0; count < 10; count++)
	{
		PrintAll(head);
		Delete(&head);	
	}
	PrintAll(head);
}
