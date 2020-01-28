#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
// I wrote every comment in English to prevent an error by encoding problem
int* Arr;
// Arr is array for unsorted list in input file.

/*
insertion_s function sorts Arr in a way of insertion sorting
parameter : num
num : the size of Arr
*/

void insertion_s(int num) {
	int i, j;
	int temp_val;
	for (i = 1; i < num; i++)
	{
		j = i;
		temp_val = Arr[j];
		while (j > 0 && temp_val < Arr[j - 1]) {
			Arr[j] = Arr[j - 1];
			j--;
		}
		Arr[j] = temp_val;
	}
}
/*
partition function sets rightmost value to pivot, and sorts array based on the pivot.
Therefore, the list become [(numbers smaller than pivot), (pivot), (numbers bigger than pivot)]
and it returns pivot to call smaller size of quicksort
parameter: left, right
left, right : leftmost and rightmost index of sorting range
*/

int partition(int left, int right)
{
	int pivot = left;
	
	int idx;
	int temp;
	for (idx = left; idx < right; idx++)
	{
		if (Arr[right] > Arr[idx])
		{
			temp = Arr[pivot];
			Arr[pivot] = Arr[idx];
			Arr[idx] = temp;
			pivot++;
		}
	}
	temp = Arr[pivot];
	Arr[pivot] = Arr[idx];
	Arr[idx] = temp;
	return pivot;

}

/*
quick_s function sorts Arr in a way of Quick Sorting
until right is smaller or same with left, quick_s keep call two smaller quick_s divided by pivot
parameter : left, right
*/
void quick_s(int left, int right) {
	if (right > left) {
		int pivot = partition(left, right);
		quick_s(left, pivot - 1);
		quick_s(pivot + 1, right);
	}
}
/*
merge function combine two range in Arr while sort the value in non-decreasing order.
first range is in between left and mid, and the other one is in between mid and right
merge function need additional left-right+1 size array to temporarily save the value of list
parameter : left, right, mid
mid : middle point of left and right
*/

void merge(int left, int right, int mid)
{
	int* temporalArr;
	temporalArr = (int*)malloc(sizeof(int) * (right+1));

	int leftIdx, rightIdx,idx;
	for (idx = 0; idx < right - left + 1; idx++)temporalArr[left+idx] = Arr[left + idx];
	leftIdx = left;
	rightIdx = mid + 1;
	idx = left;
	while (leftIdx <= mid && rightIdx <= right)
	{
		if (temporalArr[leftIdx] >= temporalArr[rightIdx])
		{
			Arr[idx++] = temporalArr[rightIdx++];
		}
		else {
			Arr[idx++] = temporalArr[leftIdx++];
		}
	}
	while (leftIdx <= mid)
		Arr[idx++] = temporalArr[leftIdx++];
	while (rightIdx <= right)
		Arr[idx++] = temporalArr[rightIdx++];
	free(temporalArr);
}
/*
merge_s function sorts Arr in a way of Merge Sorting 
merge_s divide the range of array in half, and call two another merge_s.
after it finish dividing, now it combine the range again while sorting
parameter : left, right
*/
void merge_s(int left,int right)
{
	int mid = (left + right) / 2;
	if (right > left)
	{
		merge_s(left, mid);
		merge_s(mid+1, right);
		merge(left, right, mid);
	}
}
/*combination_s function is for algorithm 4, which is fastest sorting
it's different in choosing pivot with quick_s.
it choose it's pivot with rand function. After that, it changes pivot's value with rightmost index's value. Then it calls partition.
by doing this, worst case of quick sort can be prevented.
parameter : left, right
*/
void combination_s(int left, int right)
{
	int num;
	double temp;
	if (right > left) {
		num = (rand()%(right-left+1)) + left;
		temp = Arr[right];
		Arr[right] = Arr[num];
		Arr[num] = temp;
		int pivot = partition(left, right);
		combination_s(left, pivot - 1);
		combination_s(pivot + 1, right);
	}

}
/*radix_s function sorts list in a way of Radix sort do.
it only run when Arr only has positive elements
before radix_s is called, onlyPos is called to check whether Arr has negative elements
parameter : n
n : size of Arr
*/
void radix_s(int n)
{
	int i, max_val = 0, min_val = 0, sig_digit=1, bin_count[10];
	int m_count=0;
	for(i = 0; i < n; i++){
		if(Arr[i] > max_val)max_val = Arr[i];
	}
	int *temporalArr = (int *)malloc(sizeof(int)*n);
	int temp;
	while(max_val / sig_digit > 0){
		for(i = 0; i < 10; i++)bin_count[i] = 0;

		for(i = 0; i < n; i++){
			temp = (Arr[i]/sig_digit)%10;
			bin_count[temp]++;
		}
		for(i = 1; i < 10; i++)
			bin_count[i] += bin_count[i-1];
		for(i=n-1; i>= 0; i--){
			temp = (Arr[i]/sig_digit)%10;
			temporalArr[--bin_count[temp]] = Arr[i];
		}
		for(i=0; i<n; i++) Arr[i] = temporalArr[i];

		sig_digit *= 10;
	}
	free(temporalArr);	
}
/*onlyPos checks if Arr has negative element
if Arr has negative element, it returns 0
else it returns 1
parameter : n
*/
int onlyPos(int n){
	int i;
	for(i = 0; i < n; i++)
	{
		if(Arr[i] < 0)return 0;
	}
	return 1;
}
int main(int argc, char *argv[])
{
	time_t t;
	srand(time(&t));
	if (argc != 3)
	{
		printf("Execution Error\n");
		printf("./mp2_20180001 input00000.txt index(1-4) is required\n");
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

	int num;
	fscanf(input, "%d", &num);
	//get and write the value of row and column

	int idx = (int)argv[2][0] - '0';

	if (strlen(argv[2]) != 1 || idx < 1 || idx > 4)
	{
		printf("Index Error!\n Index should be within 1 - 4 \n");
		return 3;
	}
	//make Exception about Index

	Arr = (int *)malloc(sizeof(int ) * num);
	//Allocate Arr array
	fprintf(output,"%s\n", argv[1]);
	fprintf(output,"%d\n", idx);
	fprintf(output,"%d\n", num);

	clock_t Algo_start, Algo_end;
	int n;
	for ( n = 0; n < num; n++)
	{
		fscanf(input, "%d", &Arr[n]);
	}
	
	//Allocate Arr value

	Algo_start = clock();
	//srat time
	//for each index, start different sorting algorithm
	switch (idx){
		case 1:
			insertion_s(num);
			break;
		case 2:
			quick_s(0,num-1);
			break;
		case 3:
			merge_s(0,num-1);
			break;
		case 4:
			// if input size is too small, function call overhead has big impact on runtime
			// therefore execute insertion sort rather than quick sort
			if (num < 16)insertion_s(num);
			else {
			//if Arr only has positive number, execute radix sort
			//else execute combination_s which is quick_sort with randomized pivot selection
				if(onlyPos(num))radix_s(num);
				else combination_s(0, num - 1);
			}
			break;
	}
	Algo_end = clock();

	fprintf(output,"%lf\n", (double)(Algo_end -Algo_start)/CLOCKS_PER_SEC*1000);
	for (n = 0; n < num; n++)
	{
		fprintf(output, "%d ", Arr[n]);
	}
	//Since CLOCKS PER SEC can be different in each computer, Divide Total Clocks with CLOCKS_PER_SEC and then multiply 1000 to show millisecond of execution time.
	fclose(input);
	fclose(output);
	free(Arr);
	return 0;
}
