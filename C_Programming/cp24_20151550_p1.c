#include<stdio.h>
#include<stdlib.h>
typedef struct node
{
	int data;
	struct node* next;
}QUEUE_NODE;
typedef struct Qh* Qhead; 
typedef struct Qh
{
	QUEUE_NODE* front;
	int count;
	QUEUE_NODE* rear;
}QUEUE_HEAD;

void Exit(QUEUE_HEAD* n)
{
	return; 
}
void Enqueue(QUEUE_HEAD* n)
{
	QUEUE_NODE* q;
	int num;
	q = (QUEUE_NODE *)malloc(sizeof(QUEUE_NODE));
	if(n->count == 0)
	{
		n->front = q;
		n->rear = q;
	}
	else
	{
		n->rear->next = q;
		n->rear = q;
	}
	printf("Input Data > ");
	scanf("%d",&num);
	q->data = num;
	q->next = NULL;
	n->count++;
}
void Dequeue(QUEUE_HEAD * n)
{
	QUEUE_NODE* q = n->front;
	if(n->count == 0)
	{
		printf("큐가 비어있음\n");
		return;
	}
	int num;
	num = n->front->data;
	printf("Dequeue %d\n",num);
	n->front = q->next;
	free(q);
	n->count -= 1;
}
void PrintAll(QUEUE_HEAD n)
{
	QUEUE_NODE* q=n.front;
	while(q != NULL)
	{
		printf("%d ",q->data);
		q = q->next;
	}
	printf("\n");
}
int main()
{
	Qhead head=NULL;
	head=(Qhead)malloc(sizeof(QUEUE_HEAD));
	head->count = 0;
	head->front = NULL;
	head->rear = NULL;
	int num;
	printf("****** Menu ******\n");
	printf("* 0 : Exit       *\n");
	printf("* 1 : Enqueue    *\n");
	printf("* 2 : Dequeue    *\n");
	printf("* 3 : Print All  *\n");
	printf("******************\n");
	while(1)
	{
		printf("Select > ");
		scanf("%d",&num);
		switch(num)
		{
			case 0:
				return;
			case 1:
				Enqueue(head);
				break;
			case 2:
				Dequeue(head);
				break;
			case 3:
				PrintAll(*head);
				break;
			default:
				break;
		}
	}
}
