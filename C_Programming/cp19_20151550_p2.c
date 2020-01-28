#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct _Node* nptr;
typedef struct _Node{
	int st_id;
	char st_name[31];
	nptr link;
}Node;
nptr head = NULL;
void create_Node(int id, char *name){
	Node *new;
	nptr find;
	find = head;
	new =(nptr)malloc(sizeof(Node));
	
	new->st_id = id;
	new->link = NULL;
	strcpy(new->st_name,name);
	if(head == NULL)
	{
		head = new;
	}
	else
	{
		while (1)
		{
			if (head->link == NULL)
			{
				break;
			}
			head = head->link;
		}
		head->link = new;
		head = find;
	}
}
int main()
{

	char name[31];
	int id;
	FILE *file;
	file = fopen("input.txt","r");
	int num;
	int count;
	nptr pnew = head;
	
	fscanf(file,"%d\n",&num);
	if(file == NULL)
	{
		printf("File open error!\n");
		return 0;
	}
	for(count = 0; count < num; count++)
	{
		fscanf(file,"%d / ",&id);
		fgets(name,30,file);
		create_Node(id,name);
		
	}
	fclose(file);
	printf("------Student list------\n");
	for(count = 0; count < num; count++)
	{
		printf("%d / %s",head->st_id,head->st_name);
		head = head->link;
	}
	for (pnew; pnew!=NULL; pnew=pnew->link)
	{
		free(pnew);
	}
	return 0;
}
