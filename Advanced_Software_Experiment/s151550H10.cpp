#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <random>
#include <time.h>

#include <math.h>
#include <time.h>
#include <Windows.h>

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;
float compute_time1, compute_time2;


#define MATDIM 1024
#define HW1_N 100000
#define HW3_N 200000
//double* hw1_x;
//double hw1_E, hw1_var1, hw1_var2;
float* hw1_x, hw1_E, hw1_var1, hw1_var2;
float hw2_a, hw2_b, hw2_c, hw2_naive_ans[2], hw2_pre_ans[2];
float hw3_tmp;
float hw3_x, hw3_y, hw3_z;
float hw3_res[1000000];
float hw3_res2[1000000];
float ttt;

/* hw1 */
void hw1();
void init_hw1(int fixed);
void hw1_calc_e();
void hw1_calc_var1();
void hw1_calc_var2();
/* hw2 */
void hw2();
void hw2_naive();
void hw2_safe();
float hw2_verify(float x);
/* hw3 */
void hw3();
float hw3_sum(float* input);
double hw3_double_sum(float* input);
float hw3_kahan_sum(float* input);
double hw3_kahan_sum2(float* input);


void main(void)
{
	srand((unsigned)time(NULL));

	hw1();
	hw2();
	hw3();

	return;
}

/* hw1 */
void hw1() {
	/* hw1 */
	puts("====== hw1 ======");
	init_hw1(1);
	CHECK_TIME_START;
	hw1_calc_e();
	hw1_calc_var1();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw1_calc_var1 = %f ms\n", compute_time);
	printf("hw1_calc_var1 value = %.15f\n", hw1_var1);


	CHECK_TIME_START;
	hw1_calc_var2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw1_calc_var2 = %f ms\n", compute_time);
	printf("hw1_calc_var2 value = %.15f\n", hw1_var2);
	puts("");
}
void init_hw1(int fixed)
{
	float *ptr;
	hw1_x = (float *)malloc(sizeof(float)*HW1_N);

	if (fixed)
	{
		double tmp = HW1_N;
		for (int i = 0; i < HW1_N; i++)
		{
			if (i & 1) tmp -= 0.0001;
			hw1_x[i] = tmp;
		}
	}
	else
	{
		srand((unsigned)time(NULL));
		ptr = hw1_x;
		for (int i = 0; i < HW1_N; i++)
			*ptr++ = ((float)rand() / (float)RAND_MAX) * 2;
	}
}
void hw1_calc_e()
{
	hw1_E = 0;
	for(int i = 0; i < HW1_N; i++)
		hw1_E += hw1_x[i];
	hw1_E /= HW1_N;
}
void hw1_calc_var1()
{
	hw1_var1 = 0;
	for (int i = 0; i < HW1_N; i++)
		hw1_var1 += pow((hw1_x[i] - hw1_E), 2);

	hw1_var1 /= (HW1_N - 1);
}
void hw1_calc_var2()
{
	hw1_var2 = 0;

	float sum = 0;
	for (int i = 0; i < HW1_N; i++)
		sum += hw1_x[i];
	sum = pow(sum, 2);
	float sqSum = 0;
	for (int i = 0; i < HW1_N; i++)
	{
		sqSum += hw1_x[i] * hw1_x[i];
	}
	sqSum *= HW1_N;
	hw1_var2 = (sqSum - sum) / HW1_N / (HW1_N - 1);
}

/* hw2 */
void hw2() {
	/* hw2 */
	puts("====== hw2 ======");
	printf("a, b, c : ");
	scanf("%f %f %f", &hw2_a, &hw2_b, &hw2_c);
	hw2_naive();
	hw2_safe();
	printf("naive result    : %.15f, %.15f\n", hw2_naive_ans[0], hw2_naive_ans[1]);
	printf("advanced result : %.15f, %.15f\n", hw2_pre_ans[0], hw2_pre_ans[1]);
	puts("");
	printf("Verifying naive ans    : %.15f, %.15f\n", hw2_verify(hw2_naive_ans[0]), hw2_verify(hw2_naive_ans[1]));
	printf("Verifying advanced ans : %.15f, %.15f\n", hw2_verify(hw2_pre_ans[0]), hw2_verify(hw2_pre_ans[1]));
	puts("");
}
void hw2_naive()
{
	hw2_naive_ans[0] = (-hw2_b + sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c))/(2*hw2_a);
	hw2_naive_ans[1] = (-hw2_b - sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c))/(2*hw2_a);
}
void hw2_safe()
{
	double rootEq = hw2_b * hw2_b - 4 * hw2_a * hw2_c;
	hw2_pre_ans[0] = (-hw2_b + sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
	hw2_pre_ans[1] = (-hw2_b - sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
	if (hw2_b > 0)
	{
		if (hw2_b * hw2_b > abs(hw2_a * hw2_c)) {
			hw2_pre_ans[0] = (-2 * hw2_c) / (hw2_b + sqrt(rootEq));
		}
	}
	else if (hw2_b < 0)
	{
		if (hw2_b * hw2_b > abs(hw2_a * hw2_c))
			hw2_pre_ans[1] = (-2 * hw2_c) / (hw2_b + sqrt(rootEq));
	}
}
float hw2_verify(float x)
{
	return hw2_a * x * x + hw2_b * x + hw2_c;
}


/* hw3 */

void hw3() {
	/* hw3 */
	puts("====== hw3 ======");
	srand(0);
	float input[HW3_N];

	for (int i = 0; i < HW3_N; i++) {
		input[i] = rand() / (float)RAND_MAX * (1.0f);
	}

	printf("sum %d random float number\n", HW3_N);
	printf("normal sum of input : %.6f\n", hw3_sum(input));
	printf("double sum of input : %.6lf\n", hw3_double_sum(input));
	printf(" kahan sum of input : %.6f\n", hw3_kahan_sum(input));
	printf(" kahan sum of input : %.6lf\n", hw3_kahan_sum2(input));

}


float hw3_sum(float* input) {
	float sum = 0;
	for (int i = 0; i < HW3_N; i++)sum += input[i];
	return sum;
}
double hw3_double_sum(float* input) {
	double sum = 0;
	for (int i = 0; i < HW3_N; i++)sum += (double)input[i];
	return sum;
}
float hw3_kahan_sum(float* input) {
	float sum = 0.0;
	float compen = 0.0;
	float y, t;
	for (int i = 0; i < HW3_N; i++)
	{
		y = input[i] - compen;
		t = sum + y;
		compen = (t - sum) - y;
		sum = t;
	}
	return sum;
}
double hw3_kahan_sum2(float* input) {
	double sum = 0.0;
	double compen = 0.0;
	double y, t;
	for (int i = 0; i < HW3_N; i++)
	{
		y = (double)input[i] - compen;
		t = sum + y;
		compen = (t - sum) - y;
		sum = t;
	}
	return sum;
}