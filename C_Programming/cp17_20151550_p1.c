#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct {
	char dept_name[25];
	int dept_no;
}dept;

enum Gender { Male = 0, Female };

typedef struct{
	int employee_id;
	char name[25];
	enum Gender gender;
	dept department;
	double salary;
}employee_data;

employee_data update(employee_data e, int n)
{
	printf("------Update Information------\n");
	printf("Input the department Number : ");
	scanf("%d",&e.department.dept_no);

	if(e.department.dept_no == 1)
	{
		strcpy(e.department.dept_name,"FINANCE");
	}
	else if(e.department.dept_no == 2)
	{
		strcpy(e.department.dept_name,"SALES");
	}
	else if(e.department.dept_no == 3)
	{
		strcpy(e.department.dept_name,"RESEARCH");
	}
	else if(e.department.dept_no == 4)
	{
		strcpy(e.department.dept_name,"PLANNING");
	}
	printf("------Updated employee data------\n");
	printf("%d / %s / ",e.employee_id,e.name);
	if(e.gender == Male)
	{
		printf("Male");
	}
	else
	{
		printf("Female");
	}
	printf(" / %s / %d / %.2lf\n",e.department.dept_name,e.department.dept_no,e.salary);
	getc(stdin);
}
int search(employee_data *e, int num, int keyword)
{
	int count;
	int value;
	for(count = 0; count < num; count++)
	{
		if(e[count].employee_id == keyword)
		{
			value = count+1;
			break;
		}
		if(count == num-1)
		{
			return 0;
		}
	}
	printf("------Employee Data------\n");
	printf("%d / %s / ",e[value-1].employee_id,e[value-1].name);
	if(e[value-1].gender == Male)
	{
		printf("Male / ");
	}
	else
	{
		printf("Female / ");
	}
	printf("%s / %d / %.2lf\n",e[value-1].department.dept_name,e[value-1].department.dept_no,e[value-1].salary);
	return value;
}



int main()
{
	int a;
	char check;
	char name[10];
	char getstring[20];
	char name2[10];
	FILE *inputdata;
	employee_data *data;
	char con;
	inputdata = fopen("17_input.txt","r");
	char EOFcheck;
	int n=0;
	int count ;
	int comparenum;
	if(inputdata == NULL)
	{
		printf("Input file open error!\n");
		return 0;
	}
	fscanf(inputdata,"%d",&n);
	fgetc(inputdata);
	data = (employee_data*)malloc((n+1)*sizeof(employee_data));
	for(count = 0; count<n;count++)
	{
		fscanf(inputdata,"%d / %s %s / %s / %s / %d / %lf\n",&data[count].employee_id,name,name2,getstring,data[count].department.dept_name,&data[count].department.dept_no,&data[count].salary);
		strcat(data[count].name,name);
		strcat(data[count].name," ");
		strcat(data[count].name,name2);
		if(strcmp(getstring,"Male")==0)
		{
			data[count].gender = 0;
		}
		else if(strcmp(getstring,"Female")==0)
		{
			data[count].gender = 1;
		}
	}
	while(1)
	{
		printf("Employee ID : ");
		scanf("%d",&comparenum);
		scanf("%c",&check);
		a =search(data, n, comparenum);
	
		if (a != 0)
		{
			update(data[a-1],n);
		}
		else
		{
			printf("No employee\n");
		}
		printf("Continue? <y/n> : ");
		scanf("%c",&con);
		if(con == 'y')
		{
			continue;
		}
		else if(con == 'n')
		{
			break;
		}
	}
	fclose(inputdata);
}




	
