#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
// I wrote every comment in English to prevent an error by encoding problem

int** Tarr; // global 2D array for save input

/*
nSixSol is function that solves Maximum Sum Subrectangle with O(n^6) way.
nSixSol selects two points as upper-left vertex and lower-right vertex of a subrectangle.
then sum all elements inside the subrectangle and save it to temporalSum, and check it whether is maximum value.
all parameters in for loop are all written like n1, n2, ... to show the complexity of algorithm
MaximumSum : result of the algorithm.
FirstFlag is only used once to initialisze MaximumSum
*/
int nSixSol(int row, int col)
{
	int MaximumSum;
	int n1, n2, n3, n4, n5, n6;
	int temporalSum;
	int FirstFlag = 0;
	for (n1 = 0; n1 < row; n1++)
	{
		for (n2 = 0; n2 < col; n2++)
		{
			for (n3 = n1; n3 < row; n3++)
			{
				for (n4 = n2; n4 < col; n4++)
				{
					temporalSum = 0;
					for (n5 = n1; n5 <= n3; n5++)
					{
						for (n6 = n2; n6 <= n4; n6++)
						{
							temporalSum += Tarr[n5][n6];
						}
					}
					if (FirstFlag == 0)
					{
						FirstFlag = 1;
						MaximumSum = temporalSum;
					}
					if (temporalSum > MaximumSum)MaximumSum = temporalSum;
				}
			}
		}
	}
	return MaximumSum;
}
/*
nFourSol is function that solves Maximum Sum Subrectangle with O(n^4) way
at each vertex, nFourSol find Maximum value of partial Sum.
temporalRow saves partial sum of previous row
by find all partial sum in each vertex, we can find Maximum Sum of Subrectangle.
all parameters, MaximumSum and FirstFlag are used same as nSixSol.
*/

int nFourSol(int row, int col)
{
	int MaximumSum;
	int FirstFlag = 0;
	int temporalSum;
	int n1, n2, n3, n4;
	int* temporalRow;
	int rowSum;
	temporalRow = (int*)calloc(sizeof(int),col);
	for (n1 = 0; n1 < row; n1++)
	{
		for (n2 = 0; n2 < col; n2++)
		{
			memset(temporalRow, 0, sizeof(int) * col);
			for (n3 = n1; n3 < row; n3++)
			{
				rowSum = 0;
				for (n4 = n2; n4 < col; n4++)
				{
					rowSum += Tarr[n3][n4];
					temporalSum = rowSum + temporalRow[n4];
					if (FirstFlag == 0)
					{
						FirstFlag = 1;
						MaximumSum = temporalSum;
					}
					if (temporalSum > MaximumSum)MaximumSum = temporalSum;
					temporalRow[n4] = temporalSum;
				}
			}
		}
	}
	free(temporalRow);
	return MaximumSum;
}
/*
nThreeSol is function that solves Maximum Sum Subrectangle with O(n^3) way
From start, set Tarr to be array of partial sum from 0 to itself in same row.
by doing this, Tarr[i][j] becomes Tarr[i][0] + Tarr[i][1] + ..... + Tarr[i][j];
and then, n1 and n2 selects the range of width of a subrectangle.
n3, then find best range of height by using DP. from 0 to end, n3 start to add Tarr[n3][n2]-Tarr[n3][n1-1].
That is it adds the sum of values ranges from n1 to n2 in a row. it saved to colSum.
if colSum < 0 then it set to zero since it means adding the previous row lower the maximum value.
all parameter, MaximumSum and FirstFlag are used same as nThreeSol.
*/
int nThreeSol(int row, int col)
{

	int MaximumSum;
	int FirstFlag = 0;
	int n1, n2, n3;
	int colSum;
	for (n1 = 0; n1 < row; n1++)
	{
		for (n2 = 0; n2 < col; n2++)
		{
			if (n2 != 0)Tarr[n1][n2] += Tarr[n1][n2 - 1];
		}
	}
	for (n1 = 0; n1 < col; n1++)
	{
		for (n2 = n1; n2 < col; n2++)
		{
			colSum = 0;
			for (n3 = 0; n3 < row; n3++)
			{
				
				if (n1 != 0)colSum += Tarr[n3][n2] - Tarr[n3][n1 - 1];
				else colSum += Tarr[n3][n2];
				if (FirstFlag == 0)
				{
					FirstFlag = 1;
					MaximumSum = colSum;
				}
				if (colSum > MaximumSum)MaximumSum = colSum;
				if (colSum < 0)colSum = 0;
			}
		}
	}
	return MaximumSum;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Execution Error\n");
		printf("./mp1_20180001 input00000.txt index(1-3) is required\n");
		return 1;
	}
	//make Exception about command.
	FILE *input;
	FILE *output;
	input = fopen(argv[1],"r");
	char outputFile[200] = "result_";
	strcat(outputFile,argv[1]);
	output = fopen(outputFile,"w");
	//open input file
	//open output file as "result_[input file]"

	if (input == NULL)
	{
		printf("Input File Error!\n");
		return 2;
	}
	//make Exception about Input File

	int row, col;
	fscanf(input, "%d %d", &row, &col);
	//get the value of row and column

	int idx = (int)argv[2][0] - '0';

	if (strlen(argv[2]) != 1 || idx < 1 || idx > 3)
	{
		printf("Index Error!\n Index should be within 1 - 3 \n");
		return 3;
	}
	//make Exception about Index

	int t_row, t_col;
	Tarr = (int **)malloc(sizeof(int *) * row);
	for (t_row = 0; t_row < row; t_row++)
	{
		Tarr[t_row] = (int *)malloc(sizeof(int) * col);
	}
	//Allocate Tarr array
	fprintf(output,"%s\n", argv[1]);
	fprintf(output,"%d\n", idx);
	fprintf(output,"%d\n", row);
	fprintf(output,"%d\n", col);

	clock_t Algo_start, Algo_end;

	for (t_row = 0; t_row < row; t_row++)
	{
		for (t_col = 0; t_col < col; t_col++)
		{
			fscanf(input, "%d", &Tarr[t_row][t_col]);
		}
	}
	//Allocate Tarr value

	int result = 0;
	Algo_start = clock();
	//for each index, start different algorithm solving Max Sum of Subrect
	switch (idx){
		case 1:
			result = nSixSol(row, col);
			break;
		case 2:
			result = nFourSol(row, col);
			break;
		case 3:
			result = nThreeSol(row, col);
			break;
	}
	Algo_end = clock();

	fprintf(output,"%d\n", result);
	fprintf(output,"%lf\n", (double)(Algo_end -Algo_start)/CLOCKS_PER_SEC*1000);
	//Since CLOCKS PER SEC can be different in each computer, Divide Total Clocks with CLOCKS_PER_SEC and then multiply 1000 to show millisecond of execution time.
	fclose(input);
	fclose(output);
	return 0;
}
