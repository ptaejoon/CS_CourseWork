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
void DeleteNodeAtFront()
{
	nptr n;
	n = head;
	head = head->link;
	free(n);

}
void DeleteNodeAtBack()
{
	nptr n = head;
	while(n->link->link != NULL)
	{
		n = n->link;
	}
	n->link = NULL;
	free(n->link);

}
void PrintList()
{
	nptr n = head;
	for(n;n->link !=NULL; n = n->link)
	{
		printf("%c",n->data);
	}
	printf("%c",n->data);
	printf("\n");

}
int main()
{
	nptr n;
	
	InsertNodeAtFront('n');
	InsertNodeAtFront('e');
	InsertNodeAtFront('p');
	InsertNodeAtBack('l');
	InsertNodeAtFront('o');
	InsertNodeAtBack('a');
	InsertNodeAtBack('b');
	printf("----insertion Complete----\n");
	PrintList();
	DeleteNodeAtFront();
	DeleteNodeAtBack();
	DeleteNodeAtBack();
	DeleteNodeAtBack();
	printf("-----Delete Complete-----\n");
	PrintList();
	
	return 0;
}
