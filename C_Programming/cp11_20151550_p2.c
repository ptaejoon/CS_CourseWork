#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char *my_strncpy(char *a,char *b, int n);
void initialization(char *a,int n);

int main()
{
	char *str1;
	char str[20];
	int n,i;
	printf("Input str : ");
	gets(str);
	n = strlen(str);
	str1 = (char *)malloc((n+1)*sizeof(char));
	strcpy(str1,str);
	for( i =0 ; i < strlen(str1)+1 ; i++)
	{
		initialization(str,strlen(str));
		my_strncpy(str,str1,i);

		printf("%s\n",str);
	}
	free(str1);
}

char *my_strncpy(char *a, char *b, int n)
{
	int count;
	for(count =0; count < n; count++)
	{
		*(a+count) = *(b+count);
	}
}
void initialization(char *a, int n)
{
	int count;
	for (count = 0; count < n; count++)
	{

		
		a[count] = '*';
		
	}
}
