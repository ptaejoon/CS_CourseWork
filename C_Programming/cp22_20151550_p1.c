#include<stdio.h>
#include<stdlib.h>

typedef struct node* nptr;
typedef struct node
{
	int data;
	struct node* link;
}STACK_NODE;

typedef struct 
{
	int count;
	struct node* link;
}STACK;

void Exit(STACK *s)
{
	nptr pnew=s->link;
	nptr prenew = pnew;
	while(pnew != NULL)
	{
		pnew=pnew->link;
		free(prenew);
		prenew = pnew;
	}
}
void Push(STACK *s)
{
	int num;
	nptr pnew;
	pnew = (nptr)malloc(sizeof(STACK_NODE));
	printf("Input Data > ");
	scanf("%d",&num);
	pnew->data = num;
	pnew->link = NULL;
	if(s->count == 0)
	{
		s->link = pnew;
	}
	else
	{
		pnew->link = s->link;
		s->link = pnew;
	}
	s->count++;
}
void Pop(STACK *s)
{
	nptr pnew;
	pnew = s->link;
	s->link = s->link->link;
	printf("Pop %d\n",pnew->data);
	free(pnew);
	s->count--;
	
}
void PrintAll(STACK s)
{
	nptr pnew;
	pnew = s.link;
	while(pnew != NULL)
	{
		printf("%d ",pnew->data);
		pnew = pnew->link;
	}
	printf("\n");
	printf("Stack Level : %d\n",s.count);

}
void main()
{
	int choice;
	STACK* head;
	
	head = (STACK *)malloc(sizeof(STACK));
	head->count = 0;
	head->link = NULL;
	
	printf("****** Menu ******\n");
	printf("* 0 : Exit       *\n");
	printf("* 1 : Push       *\n");
	printf("* 2 : Pop        *\n");
	printf("* 3 : Print All  *\n");
	printf("******************\n");
	while(1)
	{
		printf("Selet > ");
		scanf("%d",&choice);
		switch(choice)
		{
			case 0:	
				return;
			case 1:
				Push(head);
				break;
			case 2:
				Pop(head);
				break;
			case 3:
				PrintAll(*head);
				break;
			default:
				printf("Wrong Select!\n");
		}
	}
	free(head);
}

