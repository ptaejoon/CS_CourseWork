#include<stdio.h>
#include<string.h>
int strCmpPk(char* s1,char* s2);
void strPk(char* s1, char* s2);
int main(){
	char s1[100],s2[100];
	printf("first string : ");
	gets(s1);
	printf("second string : ");
	gets(s2);
	if(strCmpPk(s1,s2) < 0)
	{
		printf("string1 < string2\n");
	}
	if(strCmpPk(s1,s2) == 0)
	{
		printf("string1 == string2\n");
	}
	if(strCmpPk(s1,s2) > 0)
	{
		printf("string1 > string2\n");
	}
	return 0;
		
}
int strCmpPk(char* s1, char* s2)
{
	int result;
	strPk(s1,s2);
	result =strcmp(s1,s2);
	return result;

}
void strPk(char* s1, char* s2)
{
	int n,m;
	int count,count2;
	n = strlen(s1);
	m = strlen(s2);
	for(count = 0; count < n; count++)
	{
		if(s1[count] >= 'a' && s1[count] <= 'z')
		{
		}
		else if(s1[count] >= 'A' && s1[count] <= 'Z')
		{
		}
		else
		{
			for(count2 = count; count2 < n-1; count2++)
			{
				s1[count2] = s1[count2+1];
			}
			count -=1;
			n -= 1;
		
		}
	}
	s1[n] = '\0';
	for(count = 0; count < m; count++)
	{
		if(s2[count] >= 'a' && s2[count] <= 'z')
		{
		}
		else if(s2[count] >= 'A' && s2[count] <= 'Z')
		{
		}
		else
		{
			for(count2 = count ; count2 < m-1; count2++)
			{
				s2[count2] = s2[count2+1];
			}
			count -= 1;
			m -= 1;
		}
	}
	s2[m] = '\0';
}
