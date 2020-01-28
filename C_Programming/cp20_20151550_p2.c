#include<stdio.h>
#include<stdlib.h>
typedef struct node* nptr;
typedef struct node
{
	char data;
	nptr link;
}NODE;

nptr head = NULL;

void InsertNodeAtFront(char a)
{
	nptr n;
	n = (nptr)malloc(sizeof(NODE));
	if(head == NULL)
	{
		n->data = a;
		n->link = NULL;
		head = n;
		return;
	}
	n->data = head->data;
	head->data = a;
	n->link = head->link;
	head->link = n;
	
	
}
void InsertNodeAtBack(char b)
{
	nptr n;
	nptr check;
	n = (nptr)malloc(sizeof(NODE));
	if(head == NULL)
	{
		n->data = b;
		n->link = NULL;
		head = n;
		return;
	}
	check = head;
	while(check->link != NULL)
	{
		check=check->link;
	}
	check->link = n;
	n->data = b;
	n->link = NULL;
}
void PrintList()
{
	for(head;head->link !=NULL; head = head->link)
	{
		printf("%c",head->data);
		free(head);
	}
	printf("%c",head->data);
	printf("\n");
	free(head);

}
int main()
{
	nptr n;
	
	InsertNodeAtFront('p');
	InsertNodeAtFront('p');
	InsertNodeAtFront('a');
	InsertNodeAtBack('l');
	InsertNodeAtBack('e');
	PrintList();
	
	return 0;
}
