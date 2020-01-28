#include<stdio.h>

void upper_to_lower(char str[]);
void lower_to_upper(char str[]);
int main()
{
	char a[] = "There's no Free Lunch";
	char b[] = "123AbCdEfGh";	
	upper_to_lower(a);
	lower_to_upper(a);
	upper_to_lower(b);
	lower_to_upper(b);

	
	return 0;
}

void upper_to_lower(char str[])
{
	int count=0;
	while(str[count] != '\0')
	{
		if(str[count] >= 65 && str[count] <= 90)
		{
			str[count] = str[count] +32;
		}
		printf("%c",str[count]);
		count++;
	}
	printf("\n");
}
void lower_to_upper(char str[])
{
	int count=0;
	while(str[count] != '\0')
	{
		if(str[count] >= 97 && str[count] <= 122)
		{
			str[count] = str[count] - 32;
			
		}
		printf("%c",str[count]);
		count++;
	}
	printf("\n");

	
}

