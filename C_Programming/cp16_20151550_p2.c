#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	char code[11];
	char subject[30];
	char professor[11];
	int students;
	int Eng;
}classify;
void case1(int amount, classify* a)
{
	int count;
	for(count = 0; count <= amount; count++)
	{
		printf("%s %s %s %d\n",a[count].code,a[count].subject,a[count].professor,a[count].students);
	}
}
void case2(int amount, classify* a)
{
	int count;
	printf("English lectures : \n");
	for(count = 0; count <= amount; count++)
	{
		if(a[count].Eng == 1)
		{
		printf("%s %s %s %d\n",a[count].code,a[count].subject,a[count].professor,a[count].students);
		}
	}
}
void case3(int amount, classify* a)
{
	char compare[11];
	int count;
	printf("Enter code : ");
	scanf("%s",compare);
	for(count = 0; count <= amount; count++)
	{
		if(strcmp(a[count].code , compare ) == 0)
		{
			printf("Change the number of students : ");
			scanf("%d",&a[count].students);
			printf("Change complete! \n");
			break;
		}
		if(count == amount)
		{
			printf("Code finding failed\n");
		}
	}
}
void case4(int amount, classify* a)
{
	int count, count2;
	int temp1,temp2;
	char temp3[11],temp4[30],temp5[11];
	for(count = 0; count <= amount; count++)
	{
		for(count2 = 0; count2 <= amount - count -1; count2++)
		{
			if(a[count2].students < a[count2+1].students)
			{
				strcpy(temp3,a[count2].code);
				strcpy(temp4,a[count2].subject);
				strcpy(temp5,a[count2].professor);
				temp1 = a[count2].students;
				temp2 = a[count2].Eng;
				strcpy(a[count2].code,a[count2+1].code);
				strcpy(a[count2].subject,a[count2+1].subject);
				strcpy(a[count2].professor,a[count2+1].professor);
				a[count2].students = a[count2+1].students;
				a[count2].Eng = a[count2+1].Eng;
				strcpy(a[count2+1].code,temp3);
				strcpy(a[count2+1].subject,temp4);
				strcpy(a[count2+1].professor,temp5);
				a[count2+1].students = temp1;
				a[count2+1].Eng = temp2;
			}
		}
	}
	printf("Sort Complete!\n");
}
void case5(int amount, classify* a)
{
	int count = 0;
	FILE *outputdata;
	outputdata = fopen("output.txt","w");
	if(outputdata == NULL)
	{
		printf("output file open error! \n");
		return ;
	}
	while(1)
	{
		if(count == amount)
		{
			break;
		}
		fprintf(outputdata,"%s %s %s %d %d",a[count].code,a[count].subject,a[count].professor,a[count].students,a[count].Eng);
		fprintf(outputdata,"\n");
		count++;
	}
	fclose(outputdata);
	printf("Export complete!\n");
	

}
void Exit(classify *a)
{
	free(a);
}

int main(){
	classify *a;
	FILE *inputdata;
	int fpointer;
	int amount = 0;
	int amount2=0;
	char entercheck;
	inputdata = fopen("input.txt","r");
	if(inputdata == NULL)
	{
		printf("File open error!\n");
		return 0;
	}
	int input;
	while(1)
	{
		entercheck = fgetc(inputdata);
		if (entercheck == '\n')
		{
			amount2 += 1;
		}
		if (entercheck == EOF)
		{
			printf("%d",amount2);
			break;
		}
	}
	fclose(inputdata);

	a = (classify *)malloc((amount2+1)*sizeof(classify));
	inputdata = fopen("input.txt","r");
	while(1)
	{
		
		fpointer = fscanf(inputdata,"%s %s %s %d %d",a[amount].code,a[amount].subject,a[amount].professor,&a[amount].students,&a[amount].Eng);
		fgetc(inputdata);
		if(fpointer == EOF)
		{
			break;
		}
		amount++;
	}
	fclose(inputdata);


	printf("----------Menu----------\n");
	printf("1) Print All\n");
	printf("2) Find English Lecture\n");
	printf("3) Modify Attendance\n");
	printf("4) Sort\n");
	printf("5) Export Data\n");
	printf("6) Quit\n");
	printf("------------------------\n\n");

	while(1){
		printf("Select > ");
		scanf("%d", &input);
		switch(input){
		case 1 :
			case1(amount2, a);
			break;
		case 2 :
			case2(amount2, a);
			break;
		case 3 : 
			case3(amount2, a);
			break; 
		case 4 : 
			case4(amount2, a);
			break;
		case 5 :
			case5(amount2, a);
			break;
		case 6 : 
			Exit( a);
			return 0;
		default : printf("Wrong Input!!\n");
		}
		
	}
	
	
	return 0;
}
