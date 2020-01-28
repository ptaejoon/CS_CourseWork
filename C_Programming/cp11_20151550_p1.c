#include<stdio.h>
#include<string.h>

void split_string(char *str, char *string_1,char *string_2);
int main()
{
	char str[50];
	char string_1[50];
	char string_2[50];
	printf("Input str : ");
	gets(str);
	split_string(str,string_1,string_2);
}
void split_string(char *str, char *string_1, char *string_2)
{
	int a;
	char temp[50];
	char temp2[50];
	int count;
	a=strlen(str);
	
	for(count = 0; count < a/2 ; count++)
	{
		temp[count] = *(str+count);
		temp2[count] = *(str+count+a/2);
	}
	temp[a/2] = '\0';
	temp2[a/2] = '\0';
	strcpy(string_1,temp);
	strcpy(string_2,temp2);
	printf("[%s] -> [%s] [%s] \n",str,string_1,string_2);
}
