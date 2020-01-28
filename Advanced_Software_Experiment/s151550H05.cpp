#include "functionL05.h"

#define OUTPUT_STEPS   1	// display detailed solution finding steps(stdout)
							// must use redirection to save and see these outputs

#define BISECTION      1	// use bisection methof
#define NEWTON_RAPHSON 2	// use Newton-Raphson method
#define SECANT         3	// use Secant method(HW)
#define IMPROVED       4	// use improved method over the Newton-Raphson(HW)

void   pdfGeneration(void);					// nomalize samples to generate pdf
void   cdfGeneration(void);					// cdf generation
double interpolationF(double x, double U);	// given x, find the value of F_X(x)-U
double interpolationFD(double x);			// given x, fine the value of p_X(x)

double genRandN_Bisection(double U, int si);		// function using the bisection method
double genRandN_NewtonRaphson(double U, int si);	// function using the Newton-Raphson method	
double genRandN_Secant(double U, int si);			// function using the secant method(HW)
double genRandN_Improved(double U, int si);			// function using the improved method(HW)

int main(void) {
	int TN;				// # of tests

	srand((unsigned int)time(NULL));	// init rand()
	//srand(0);			// use this line when debugging
	scanf("%d", &TN);	// read # of tests		
	int bal=0;
	for (int t = 0; t < TN; ++t) {
		start_time = clock();				// start time measure
		bal = 0;
		// preparation
		getParameters_openFiles();			// get parameters and open files
		fscanf(sfp, "%d %lf", &SN, &dx);	// read # of samples and x-distance between two adjacent samples
		allocateMemory();
		readSamples();
		pdfGeneration();
		cdfGeneration();
		save_pdf_cdf();

		// start generation
		fprintf(Xfp, "%d\n", XN);			// write # of random numbers generated
		for (int d = 0; d < XN; d++) {
			double U = (double)rand() / RAND_MAX;	// generate U : [0.0,1.0]
			double X;			// random number generated

			// generate a random number
			switch (method) {
				case (BISECTION):
					X = genRandN_Bisection(U, d);	// d is needed to display steps 
					fflush(stdout);		// can be removed(just for debugging)
					break;
				case (NEWTON_RAPHSON):	
					X = genRandN_NewtonRaphson(U, d);
					fflush(stdout);		// "
					break;
				case (SECANT):	
					X = genRandN_Secant(U, d);
					fflush(stdout);		// "
					break;
				case (IMPROVED):
					X = genRandN_Improved(U, d);
					fflush(stdout);		// "
					break;
				default:
					printf("Unknown method");
					exit(-1);
			}
			fprintf(Xfp, "%.12f\n", X);		// save X
			if (X < 0)bal++;//histoGram[0] += 1;
			else if (X >= 1)bal++;//histoGram[SN-1] += 1;
			else histoGram[(int)(X*SN)] += 1;
			//else histoGram[idx] += 1;
			
			//histoGram[d] = histoGram[d] + 1;
			// find the proper histogram bin for X and increase the bin by one
			
			// *** histoGram[ ]을 설정하는 이 부분을 작성하세요
			//     구한 X 값으로부터 histoGram[] 배열의 index i를 구한후
			//     histoGram[i]=histoGram[i]+1해주면 됩니다.

		}
		//printf("발산수 : %d", bal);
		end_time = clock();					// stop time measure
		//printf("Time elapsed : %f\n",((float)end_time - (float)start_time)/CLOCKS_PER_SEC);
		saveHistogram();					// write histogram to the file
		closeFile_deallocateMemory();		// deallocate memory
	}
}

double genRandN_Bisection(double U, int si) {
	// initial interval is [0.0, 1.0] (can always find a root)
	double L = 0.0, R = 1.0;
	double x1 = (L + R) / 2;	// you may change the name of variable x1 if you want
	double x0;
	int iter;
	double temp;
	if (showSteps == OUTPUT_STEPS) {
			printf(" n(         %d)     xn1           |f(xn1)|  U = %lf\n",si,U);
	}
	for (iter = 0; iter <= maxIter; iter++)
	{
		x0 = x1;
		x1 = (L + R) / 2;
		if (showSteps == OUTPUT_STEPS) {
			// 중간 단계를 표시하는 코드
			printf("%2d  %.18e  %.2e\n", iter, x1, fabs(interpolationF(x1, U)));
		}
		temp = x1;
		if (interpolationF(x1, U) * interpolationF(L, U) < 0)
		{
			R = temp;
		}
		else L = temp;
		x1 = temp;

		if (iter != 0 && fabs(x1 - x0) < epsilon)
			break;
		if (fabs(interpolationF(x1, U)) < delta)
			break;

	}
	// *** 이 함수를 작성하세요
	//     이 함수는 근이 반드시 존재합니다

	return x1;		// you can change the return variable if you want
}

double genRandN_NewtonRaphson(double U, int si) {
	double x1 = U+0.01;	// just for initial buid without coding
	double x0 = 0.0;
	if (U > 0.9) {
		x1 = U - 0.01;
	}

	int iter;
	if (showSteps == OUTPUT_STEPS) {
		printf(" n(         %d)     xn1           |f(xn1)|  U = %lf\n", si, U);
	}
	for (iter = 0; iter <= maxIter; iter++) {
		
		if (showSteps == OUTPUT_STEPS) {
			// 중간 단계를 표시하는 코드
			printf("%2d  %.18e  %.2e\n", iter, x1, fabs(interpolationF(x1, U)));
		}
		if (fabs(interpolationF(x1,U)) < delta)
			break;
		if (iter != 0 && fabs(x1 - x0) < epsilon)
			break;

		// 다음 두 줄이 Newton-Raphsom method core 입니다
		x0 = x1;
		x1 = x1 - (interpolationF(x1, U) / interpolationFD(x1));
		//if (x1 < 0 || x1 >1)break;

	}
	return x1;	// you can change the return variable if you want
}

double genRandN_Secant(double U, int si) {
	double x0=0.5, x1=x0+0.01;		// these variables are for initial build without coding
	//double x0 = U, x1 = U + 0.01;
	//if (x1 > 1)x1 = x0 - 0.01;
	double temp;
	double L = 0, R = 1;
	for (int iter = 0; iter < 5; iter++)
	{
		x0 = x1;
		x1 = (L + R) / 2;
		temp = x1;
		if (interpolationF(x1, U) * interpolationF(L, U) < 0)
		{
			R = temp;
		}
		else L = temp;
		x1 = temp;

	}
	int iter;
	if (showSteps == OUTPUT_STEPS) {
		printf(" n(         %d)     xn1           |f(xn1)|  U = %lf\n", si, U);
	}
	for (iter = 0; iter <= maxIter; iter++) {

		if (showSteps == OUTPUT_STEPS) {
			// 중간 단계를 표시하는 코드
			printf("%2d  %.18e  %.2e\n", iter, x1, fabs(interpolationF(x1, U)));
		}
		if (fabs(interpolationF(x1, U)) < delta)
			break;
		if (iter != 0 && fabs(x1 - x0) < epsilon)
			break;

		// 다음 두 줄이 Newton-Raphsom method core 입니다
		temp = x0;
		x0 = x1;
		x1 = x1 - interpolationF(x1, U)*(x1-temp)/ (interpolationF(x1, U)- interpolationF(temp, U));
		//if (x1 < 0 || x1 > 1)break;
	}

	return x1;	// you can change the return variable if you want
}

double genRandN_Improved(double U, int si) {
	double temp;			// these variables are for initial build without coding
							// you may change the names or delete them if needed

	// *** 이 함수를 작성하세요
	//     숙제입니다.
	//     Bisection과 Newton-Raphson을 적절히 조합하여 어떤 경우에도 발산하지 
	//     않는 함수를 만들어 봅시다.
	//     필요하다면, 이 함수에서 호출하는 다른 함수를 작성하여 호출해도 좋습니다.
	
	double x1 = U + 0.01;	// just for initial buid without coding
	double x0 = 0.0;
	if (U > 0.9) {
		x1 = U - 0.01;
	}
	int needBS = 0;
	int iter;
	if (showSteps == OUTPUT_STEPS) {
		printf(" n(         %d)     xn1           |f(xn1)|  U = %lf\n", si, U);
	}
	for (iter = 0; iter <= maxIter; iter++) {

		if (showSteps == OUTPUT_STEPS) {
			// 중간 단계를 표시하는 코드
			printf("%2d  %.18e  %.2e\n", iter, x1, fabs(interpolationF(x1, U)));
		}
		if (fabs(interpolationF(x1, U)) < delta)
			break;
		if (iter != 0 && fabs(x1 - x0) < epsilon)
			break;

		// 다음 두 줄이 Newton-Raphsom method core 입니다
		temp = x0;
		x0 = x1;
		x1 = x1 - (interpolationF(x1, U) / interpolationFD(x1));
		if (x1 < 0 || x1 > 1) {
			needBS = 1;
			iter++;
			break;
		}
	}
	double R, L;
	if (needBS == 1)
	{
		if (x1 < 0) {
			R = x0;
			L = 0;
		}
		else {
			R = 1;
			L = x0;
		}
		for (; iter <= maxIter; iter++)
		{
			x1 = (L + R) / 2;
			if (showSteps == OUTPUT_STEPS) {
				// 중간 단계를 표시하는 코드
				printf("%2d  %.18e  %.2e\n", iter, x1, fabs(interpolationF(x1, U)));
			}
			//temp = x1;
			if (interpolationF(x1, U) * interpolationF(L, U) < 0)
			{
				R = x1;
			}
			else L = x1;

			if (iter != 0 && fabs(x1 - x0) < epsilon)
				break;
			if (fabs(interpolationF(x1, U)) < delta)
				break;
			x0 = x1;


		}
	}
	return x1;
}

void pdfGeneration(void) {
	// input pdfX[], pdfY[], SN
	// output pdfX[] (scale so that the range is 0 ~ 1)
	//        pdfY[] normalization
	double inte = 0;
	for (int counter = 1; counter < SN-1; counter++)
	{
		inte += pdfY[counter];
	}
	inte += (pdfY[0] + pdfY[SN - 1]) / 2;
	inte /= (SN-1);
	for (int counter = 0; counter < SN-1; counter++)
	{
		pdfX[counter] = (double)counter / (SN-1);
		pdfY[counter] = pdfY[counter] / inte ;
	}
	pdfX[SN - 1] = 1;
	pdfY[SN - 1] /= inte;
	// *** 이 함수를 작성하세요


}

void cdfGeneration(void) {
	// input:  normalized pdfX[], pdfY[], SN
	// output: cdfY[]
	cdfY[0] = 0;
	double dx = (double)1/(SN-1);
	for (int counter = 1; counter < (SN-1); counter++)
	{
		cdfY[counter] = pdfY[counter-1]*dx + cdfY[counter - 1];
	}
	cdfY[SN - 1] = 1;
	// *** 이 함수를 작성하세요

}

double interpolationF(double x, double U) {
	// return the value of F_X(x) - U by interpolation
	double F = 0;		// just for building before coding
	
	int counter = 1;
	while (x > pdfX[counter] && counter < SN-1 )counter++;

	F = cdfY[counter - 1] + (cdfY[counter] - cdfY[counter - 1])*(x - pdfX[counter - 1]) / (pdfX[counter] - pdfX[counter - 1]);
    // *** 이 함수를 작성하세요
	F = F - U;
	return F;	// you may modify the variable name
}

double interpolationFD(double x) {
	// return the value of p_X(x) by interpolation
	double FD = 0;		// just for building before coding
	int counter = 1;
	while (x > pdfX[counter] && counter < SN - 1)counter++;
	FD = pdfY[counter - 1] + (pdfY[counter] - pdfY[counter - 1])*(x - pdfX[counter - 1]) / (pdfX[counter] - pdfX[counter - 1]);
	return FD;	// you may modify the variable name
}
