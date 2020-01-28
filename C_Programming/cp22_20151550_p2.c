#include<stdio.h>
#include<stdlib.h>
typedef struct node* nptr;
typedef struct 
{
	int count;
	struct node* link;
}STACK;

typedef struct node
{
	char data;
	struct node* link;
}STACK_NODE;

void function(STACK *s,char a)
{
	nptr pnew;
	pnew = (nptr)malloc(sizeof(STACK_NODE));
	pnew->data = a;
	pnew->link = NULL;

	if(s->count == 0)
	{
		s->link =  pnew;
	}
	else
	{
		pnew->link = s->link;
		s->link = pnew;
	}
	s->count++;
}
void check(STACK *s)
{
	nptr pnew = s->link;
	nptr pre_pnew = s;
	while(pnew->link != NULL)
	{
		while(pre_pnew->link != pnew)
		{	
			pre_pnew = pre_pnew->link;
		}
		
		if(pnew->data == ']' && pnew->link->data == '[')
		{
			pre_pnew->link = pnew->link->link;
			free(pnew->link);
			free(pnew);
			return;
		}
		else if(pnew->data == ')' && pnew->link->data == '(')
		{
			pre_pnew->link = pnew->link->link;
			free(pnew->link);
			free(pnew);
			return;
		}
		else if(pnew->data == '}' && pnew->link->data == '{')
		{
			pre_pnew->link = pnew->link->link;
			free(pnew->link);
			free(pnew);
			return;
		}
		pnew= pnew->link;
	}
}		
int main()
{
	STACK *head;
	head->count = 0;
	head->link=NULL;
	char a;
	int num = 0;
	while(a != '\n')
	{
		scanf("%c",&a);
		if(a == '(' || a == ')' || a== '{' || a=='}' || a=='[' || a == ']')
		{
			
			function(head,a);
		}
	}
	for(num = 0; num < head->count/2; num++)
	{
		check(head);
	}
	if(head->link == NULL)
	{
		printf("Matched Parentheses!\n");
	}
	else
	{
		printf("Unmatched Parentheses!\n");
	}
}
