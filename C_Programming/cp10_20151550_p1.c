#include<stdio.h>
#include<stdlib.h>

int main()
{
	char *string;
	int count = 0;
	int count2 = 0;
	int result = 1;
	int count3 = 0;
	string = (char *)malloc(sizeof(char));
	do
	{
		string = (char *)realloc(string,(count + 1)*sizeof(char));
		scanf("%c",(string + count));
		count++;
	} while (*(string+count-1) != '\n');

	
	while (count2 < count-1)
	{

		if (*(string + count2) == ' ')
		{
			if (count2 != 0 && count2 != count-2 && *(string + count2) != *(string + count2 + 1))
			{
				result += 1;
			}
		}
		count2++;
	}
	if (*(string) == '\n')
	{
		result = 0;
	}
	if (*(string) == ' ')
	{
		while (count3 < count-1)
		{
			if (count3 == count - 2)
			{
				result = 0;
			}
			count3++;
		}
	}
	printf("%d\n", result);
	free(string);
	
}