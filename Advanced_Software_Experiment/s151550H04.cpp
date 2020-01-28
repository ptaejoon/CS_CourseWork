#include "function.h"

void showMethod_Parameters(const char *method, int parameterN, double parameters[]);

void NewtonRaphson (
	double x1, double p[], int maxIter,
	double delta, double epsilon, int showSteps,
	int chkConvergence, double root,
	double *ans, double *abs_error, int *iteration
);
void Secant (
	double x0, double x1, double p[], int maxIter,
	double delta, double epsilon, int showSteps,
	int chkConvergence, double root,
	double *ans, double *abs_error, int *iteration
);
void Bisection (
	double L, double R, double p[], int maxIter,
	double delta, double epsilon, int showSteps,
	int chkConvergence, double root,
	double *ans, double *abs_error, int *iteration
);
// ���ο� Ǯ�� ����� ���� �� ���⿡ �ۼ��� �Լ��� �����ϼ���

void functionSelection(int problemID) {
	switch (problemID) {
	case PROBLEM1:
		_F = _LabF1;
		_FP = _LabFP1;
		printf("Root finding for %s ", LabF1str);
		break;
	case PROBLEM2:
		_F = _LabF2;
		_FP = _LabFP2;
		printf("Root finding for %s ", LabF2str);
		break;
	case PROBLEM3:
		_F = _LabF3;
		_FP = _LabFP3;
		printf("Root finding for %s ", LabF3str);
		break;
	case PROBLEM4:
		_F = _LabF4;
		_FP = _LabFP4;
		printf("Root finding for %s ", LabF4str);
		break;
	case PROBLEM5:
		_F = _LabF5;
		_FP = _LabFP5;
		printf("Root finding for %s ", LabF5str);
		break;
	// ����(����)�� �߰��� ������ �̰��� case ���� �߰��Ͽ�
	// ���� �Լ��� �����ϰ� ������ ����ؾ� �մϴ�.

	default:
		printf("Problem id = %d is not defined\n", problemID);
		exit(-1);
	}
}

void finalOutput(int problemID, double parameters[], double ans, 
	double absfval, int iteration) {

	switch (problemID) {
	case PROBLEM1:
		printf("ans: x= %.18e |f(x)|= %.10e iteration= %2d\n", ans, absfval, iteration);
		break;
	case PROBLEM2:
		printf("ans: x= %.18e |f(x)|= %.10e iteration= %2d\n", ans, absfval, iteration);
		break;
	case PROBLEM3:
		printf("ans: x= %.18e, y = %.18e |f(x)|= %.10e iteration= %2d\n", ans, parameters[0]*ans + parameters[1], absfval, iteration);
		break;
	case PROBLEM4:
		printf("ans: x= %.18e |f(x)|= %.10e iteration= %2d\n", ans, absfval, iteration);
		break;
	case PROBLEM5:
		printf("ans: x= %.18e |f(x)|= %.10e iteration= %2d\n", ans, absfval, iteration);
		break;
	// ����(����)�� �߰��� ������ �̰��� case ���� �߰��Ͽ�
	// ���� root �� ��Ÿ ������ ����մϴ�.
	// �ʿ�� ������ ������ �߰��ص� �����մϴ�.

	default:
		printf("Problem id = %d is not defined\n", problemID);
		exit(-1);
	}
}

int main(void) {
	int TN;		// test cases
	// 1st line of a test
	int problemID, solMethod, parameterN;
		double parameters[PARAMETER_N] = { 0 };
	// 2nd line of a test
	double x0, x1;
		int checkConvergence;
		double root;
	// 3rd line of a test
	int maxIter;
		double delta, epsilon;
		int    showSteps;
	// method output
	double ans, absfval;
		int iteration;

	scanf("%d", &TN);	// get # of tests
	for (int t = 0; t < TN; t++) {
		if (t != 0) {
			printf("\n");
		}
		// read the 1st line of a test
		scanf("%d %d %d", &problemID, &solMethod, &parameterN);
		for (int i = 0; i < parameterN; i++) {
			scanf("%lf", &parameters[i]); // read function parameters if needed(0~3)
		}

		functionSelection(problemID);	// ������ �����մϴ�

		// ��û�� Ǯ�� ����� ���� �ش� Ǯ�� �Լ��� ȣ���մϴ�
		switch (solMethod) {
			case NEWTON_R:
				// read the 2nd line of a test
				scanf("%lf %d %lf", &x0, &checkConvergence, &root);
				// read the 3rd line of a test
				scanf("%d %lf %lf %d", &maxIter, &delta, &epsilon, &showSteps);
				
				showMethod_Parameters(NRstr, parameterN, parameters);
				
				NewtonRaphson(x0, parameters, maxIter, delta, epsilon, showSteps,
					checkConvergence, root, &ans, &absfval, &iteration);
				break;
			case SECANT:
				// ������ �ۼ��ϼ���
				scanf("%lf %lf %d %lf", &x0, &x1, &checkConvergence, &root);
				// read the 3rd line of a test
				scanf("%d %lf %lf %d", &maxIter, &delta, &epsilon, &showSteps);

				showMethod_Parameters(SCstr, parameterN, parameters);

				Secant(x0,x1, parameters, maxIter, delta, epsilon, showSteps,
					checkConvergence, root, &ans, &absfval, &iteration);

				break;
			case BISECTION:
				// ������ �ۼ��ϼ���
				scanf("%lf %lf %d %lf", &x0, &x1, &checkConvergence, &root);
				// read the 3rd line of a test
				scanf("%d %lf %lf %d", &maxIter, &delta, &epsilon, &showSteps);

				showMethod_Parameters(BSstr, parameterN, parameters);

				Bisection(x0, x1, parameters, maxIter, delta, epsilon, showSteps,
					checkConvergence, root, &ans, &absfval, &iteration);

				break;

			// ���ο� Ǯ�� ����� �߰��� ��� ���⿡ case ���� �߰��Ͽ�
			// �ʿ��� �Է��� �ް�, �ش� Ǯ�� �Լ��� ȣ���ϼ���

			default:
				printf("Unknown solving method code %d.\n", solMethod);
		}
		finalOutput(problemID, parameters, ans, absfval, iteration);
	}
}

void NewtonRaphson(
	double x1, double p[], int maxIter,
	double delta, double epsilon, int showSteps,
	int chkConvergence, double root, 
	double *ans, double *abs_error, int *iteration
) {

	double x0, e0, e1, c;
	
	int iter;

	if (showSteps == OUTPUT_STEPS) {
		if (chkConvergence == CONVERGENCE_CHECK)
			printf(" n              xn1           |f(xn1)|     e1       c=e1/(e0*e0)\n");
		else
			printf(" n              xn1           |f(xn1)|\n");
	}
	for (iter = 0; iter <= maxIter; iter++) {
		if (showSteps == OUTPUT_STEPS) {
			// �߰� �ܰ踦 ǥ���ϴ� �ڵ�
			if (iter == 0 || chkConvergence != CONVERGENCE_CHECK) {
				printf("%2d  %.18e  %.2e\n", iter, x1, fabs(_F(x1, p[0], p[1], p[2])));
			}
			else if (chkConvergence == CONVERGENCE_CHECK) {
				// ���ŵ��� �����ϱ� ���Ͽ� ����ϴ� �ڵ�
				e0 = fabs(x0 - root);
				e1 = fabs(x1 - root);
				c = e1 / (e0*e0);  // �� ���� ��� ���� ��� ������ ��������
				printf("%2d  %.18e  %.2e %.6e %.6e\n", iter, x1, fabs(_F(x1, p[0], p[1], p[2])), e1, c);
			}
		}
		if (fabs(_F(x1, p[0], p[1], p[2])) < delta)
			break;
		if (iter != 0 && fabs(x1 - x0) < epsilon)
			break;

		// ���� �� ���� Newton-Raphsom method core �Դϴ�
		x0 = x1;
		x1 = x1 - (_F(x1, p[0], p[1], p[2]) / _FP(x1, p[0], p[1], p[2]));

	}
	// ���� ����� �����մϴ�
	*iteration = iter;	// �� �ݺ� ȸ��
	*ans = x1;			// ���� root �ٻ簪(��Ȯ�� ���� ����)
	*abs_error = fabs(_F(x1, p[0], p[1], p[2]));	// f(ans) ��(0�� �����ؾ���)

	return;
}

void Secant (
	double x0, double x1, double p[], int maxIter,
	double delta, double epsilon, int showSteps,
	int chkConvergence, double root,
	double *ans, double *abs_error, int *iteration
) {

	// �Լ��� �ۼ��ϼ���
	double e0, e1, c;
	double temp;
	int iter;

	if (showSteps == OUTPUT_STEPS) {
		if (chkConvergence == CONVERGENCE_CHECK)
			printf(" n              xn1           |f(xn1)|     e1       c=e1/(e0*e0)\n");
		else
			printf(" n              xn1           |f(xn1)|\n");
	}
	for (iter = 0; iter <= maxIter; iter++) {
		if (showSteps == OUTPUT_STEPS) {
			// �߰� �ܰ踦 ǥ���ϴ� �ڵ�
			if (iter == 0 || chkConvergence != CONVERGENCE_CHECK) {
				printf("%2d  %.18e  %.2e\n", iter, x1, fabs(_F(x1, p[0], p[1], p[2])));
			}
			else if (chkConvergence == CONVERGENCE_CHECK) {
				// ���ŵ��� �����ϱ� ���Ͽ� ����ϴ� �ڵ�
				e0 = fabs(x0 - root);
				e1 = fabs(x1 - root);
				c = e1 / (e0*e0);  // �� ���� ��� ���� ��� ������ ��������
				printf("%2d  %.18e  %.2e %.6e %.6e\n", iter, x1, fabs(_F(x1, p[0], p[1], p[2])), e1, c);
			}
		}
		if (fabs(_F(x1, p[0], p[1], p[2])) < delta)
			break;
		if (iter != 0 && fabs(x1 - x0) < epsilon)
			break;
		// ���� �� ���� Newton-Raphsom method core �Դϴ�
		temp = x0;
		x0 = x1;
		x1 = x1 - _F(x1, p[0], p[1], p[2])*(x1 - temp) / (_F(x1, p[0], p[1], p[2]) - _F(temp, p[0], p[1], p[2]));
	
	}
	// ���� ����� �����մϴ�
	*iteration = iter;	// �� �ݺ� ȸ��
	*ans = x1;			// ���� root �ٻ簪(��Ȯ�� ���� ����)
	*abs_error = fabs(_F(x1, p[0], p[1], p[2]));	// f(ans) ��(0�� �����ؾ���)

	return;
}

void Bisection(
	double L, double R, double p[], int maxIter,
	double delta, double epsilon, int showSteps,
	int chkConvergence, double root,
	double *ans, double *abs_error, int *iteration
) {
	double x0,x1, e0, e1, c;
	int iter;
	double temp;
	x1 = (L + R) / 2;
	x0 = x1;
	if (showSteps == OUTPUT_STEPS) {
		if (chkConvergence == CONVERGENCE_CHECK)
			printf(" n              xn1           |f(xn1)|     e1       c=e1/(e0*e0)\n");
		else
			printf(" n              xn1           |f(xn1)|\n");
	}
	for (iter = 0; iter <= maxIter; iter++) {
		x0 = x1;
		x1 = (L + R) / 2;
		if (showSteps == OUTPUT_STEPS) {
			// �߰� �ܰ踦 ǥ���ϴ� �ڵ�
			if (iter == 0 || chkConvergence != CONVERGENCE_CHECK) {
				printf("%2d  %.18e  %.2e\n", iter, x1, fabs(_F(x1, p[0], p[1], p[2])));
			}
			else if (chkConvergence == CONVERGENCE_CHECK) {
				// ���ŵ��� �����ϱ� ���Ͽ� ����ϴ� �ڵ�
				e0 = fabs(x0 - root);
				e1 = fabs(x1 - root);
				c = e1 / (e0*e0);  // �� ���� ��� ���� ��� ������ ��������
				printf("%2d  %.18e  %.2e %.6e %.6e\n", iter, x1, fabs(_F(x1, p[0], p[1], p[2])), e1, c);
			}
		}
		

		// ���� �� ���� Newton-Raphsom method core �Դϴ�

		
		if (_F(x1, p[0], p[1], p[2]) == 0)break;
		temp = x1;
		if (_F(x1, p[0], p[1], p[2])*_F(L, p[0], p[1], p[2]) < 0)
		{
			R = temp;
		}
		else L = temp;
		x1 = temp;
		
		if (fabs(_F(x1, p[0], p[1], p[2])) < delta)
			break;
		if (iter != 0 && fabs(x1 - x0) < epsilon)
			break;

	}
	// ���� ����� �����մϴ�
	*iteration = iter;	// �� �ݺ� ȸ��
	*ans = x1;			// ���� root �ٻ簪(��Ȯ�� ���� ����)
	*abs_error = fabs(_F(x1, p[0], p[1], p[2]));	// f(ans) ��(0�� �����ؾ���)

	return;

}

void showMethod_Parameters(const char *method, int parameterN, double parameters[]) {
	// output solution method, and paramters if used
	printf("by the %s\n", method);
	if (parameterN != 0) {
		for (int i = 0; i < parameterN; i++) {
			printf("%c = %.6e ", pidx[i], parameters[i]);
		}
		printf("\n");
	}
}