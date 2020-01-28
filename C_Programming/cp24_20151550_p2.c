#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node{
	char command[10];
	struct node* next;
}QUEUE_NODE;

typedef struct{
	QUEUE_NODE * front;
	int count;
	QUEUE_NODE * rear;
}QUEUE;

typedef struct QUEUE* Qhead;

int CheckCommand(char* command)
{
	if(strcmp(command, "h")==0 || strcmp(command,"history") == 0 || strcmp(command,"q") == 0 || strcmp(command,"quit") == 0 || strcmp(command,"mkdir") == 0 || strcmp(command,"dir") == 0  || strcmp(command,"cd") == 0 ||strcmp(command,"help") == 0)
	{
		return 1;
	}
	return 0;
}
int DequeuePrint(QUEUE* pQueue, char* dataOut)
{
	QUEUE_NODE *n = pQueue->front;
	if(pQueue->front == NULL)
	{
		return 0;
	}
	if(pQueue->front == pQueue->rear )
	{
		printf("%s\n",n->command);
		free(n);
		return 0;
	}
	printf("%s\n",n->command);
	pQueue->front = n->next;
	free(n);

	return 1;
}
void EnqueueCommand(QUEUE* pQueue,char * dataIn)
{
	QUEUE_NODE *n;
	QUEUE_NODE *p;
	QUEUE_NODE *q;
	if(CheckCommand(dataIn))
	{
		
		
		if(strcmp(dataIn,"h") == 0 || strcmp(dataIn,"history")==0)
		{
			while(DequeuePrint(pQueue, dataIn));	
	
		}
		else if(strcmp(dataIn,"q") == 0 || strcmp(dataIn,"quit")==0)
		{
			
			return;
		}
		else
		{
			n=(QUEUE_NODE *)malloc(sizeof(QUEUE_NODE));
			pQueue->count+=1;
			strcpy(n->command,dataIn);
			n->next =NULL;
			if(pQueue->front == NULL)
			{
				pQueue->front = n;
				pQueue->rear = n;
			}
			else
			{
				pQueue->rear->next = n;
				pQueue->rear = n;
			}
			printf("[Valid] ");
			puts(dataIn);
		}
	}
	else
	{
		printf("[Invalid]\n");
	}
}
int main()
{
	char command[10];
	QUEUE* pQueue;
	pQueue = (QUEUE *)malloc(sizeof(QUEUE));
	
	pQueue->front = NULL;
	pQueue->count = 0;
	pQueue->rear = NULL;

	while (1)
	{
		printf(">>");
		gets(command);
		EnqueueCommand(pQueue,command);
		if(strcmp(command,"q") == 0 || strcmp(command,"quit") == 0)
		{
			break;
		}
	}

	return 0;
}
