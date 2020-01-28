#include<stdio.h>
#include<stdlib.h>

int main()
{
	char *string;
	int count = 0;
	int count2 = 0;
	string = (char *)malloc(17 * sizeof(char));
	do
	{
		scanf("%c", (string + count));
		if (*(string + count) == 'K' || *(string + count) == 'M' || *(string + count) == 'G' || *(string + count) == 'P' || *(string + count) == 'T')
		{
			for (count2 = 0; count2 <= count; count2++)
			{
				printf("%c", *(string + count2));
			}
			printf("\n");
			return 0;
		}
		count++;

	} while (*(string + count - 1) != '\n');
	count -= 1;
	if (count / 3 == 0)
	{
		if (count % 3 == 1)
		{
			printf("%c\n", *string);
		}
		if (count % 3 == 2)
		{
			for (count2 = 0; count2 < 2; count2++)
			{
				printf("%c", *(string + count2));
			}
			printf("\n");
		}
	}
	if (count / 3 == 1)
	{
		if (count % 3 == 0)
		{
			for (count2 = 0; count2 < 3; count2++)
			{
				printf("%c", *(string + count2));
			}
			printf("\n");
		}
		if (count % 3 == 1)
		{
			for (count2 = 0; count2 < 1; count2++)
			{
				printf("%c", *(string + count2));

			}
			printf("K\n");
		}
		if (count % 3 == 2)
		{
			for (count2 = 0; count2 < 2; count2++)
			{
				printf("%c", *(string + count2));
	
			}
			printf("K\n");
		}
	}
	if (count / 3 == 2)
	{
		if (count % 3 == 0)
		{
			for (count2 = 0; count2 < 3; count2++)
			{
				printf("%c", *(string + count2));

			}
			printf("K\n");
		}
		if (count % 3 == 1)
		{
			for (count2 = 0; count2 < 1; count2++)
			{
				printf("%c", *(string + count2));
			
			}
			printf("M\n");
		}
		if (count % 3 == 2)
		{
			for (count2 = 0; count2 < 2; count2++)
			{
				printf("%c", *(string + count2));
	
			}
			printf("M\n");
		}

	}
	if (count / 3 == 3)
	{
		if (count % 3 == 0)
		{
			for (count2 = 0; count2 < 3; count2++)
			{
				printf("%c", *(string + count2));
		
			}
			printf("M\n");
		}
		if (count % 3 == 1)
		{
			for (count2 = 0; count2 < 1; count2++)
			{
				printf("%c", *(string + count2));
			
			}
			printf("G\n");
		}
		if (count % 3 == 2)
		{
			for (count2 = 0; count2 < 2; count2++)
			{
				printf("%c", *(string + count2));
	
			}
			printf("G\n");
		}
	}
	if (count / 3 == 4)
	{
		if (count % 3 == 0)
		{
			for (count2 = 0; count2 < 3; count2++)
			{
				printf("%c", *(string + count2));

			}
			printf("G\n");
		}
		if (count % 3 == 1)
		{
			for (count2 = 0; count2 < 1; count2++)
			{
				printf("%c", *(string + count2));
	
			}
			printf("T\n");
		}
		if (count % 3 == 2)
		{
			for (count2 = 0; count2 < 2; count2++)
			{
				printf("%c", *(string + count2));
	
			}
			printf("T\n");
		}
	}
	if (count / 3 == 5)
	{
		if (count % 3 == 0)
		{
			for (count2 = 0; count2 < 3; count2++)
			{
				printf("%c", *(string + count2));

			}
			printf("T\n");
		}
		if (count % 3 == 1)
		{
			for (count2 = 0; count2 < 1; count2++)
			{
				printf("%c", *(string + count2));
	
			}
			printf("P\n");
		}
		if (count % 3 == 2)
		{
			for (count2 = 0; count2 < 2; count2++)
			{
				printf("%c", *(string + count2));

			}
			printf("P\n");
		}
	}

	free(string);
}
	