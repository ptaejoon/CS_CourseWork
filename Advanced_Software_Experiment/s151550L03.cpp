#include "SIFT.h"
#define DIST_RATIO_THR 0.49
#include <iostream>
#include <iterator>
#include <random>
using namespace std;
float calGaus(int x, int y, double sig)
{
	float Ans = (float)CV_PI * 2 * sig*sig;
	Ans = 1 / Ans * exp(float(-(x*x + y * y)) / sig / sig / 2);
	return Ans;
}	
void customGaussianBlur(const Mat &src, Mat &dst, double sigma){
	////////////////////////////////////////////////////////////
	// ����þ� Ŀ���� ������ �̹����� �����ϴ� �Լ�
	// Ŀ���� ũ��� 2*(4*sigma)+1 (�Ҽ����� �ø� �Ǵ� �ݿø����� ó���Ѵ�)
	// ����þ� Ŀ���� float���� ���� ����
	// ����þ� Ŀ���� Ŀ�� �� ��� �� ��ü�� �������� ������ 0~1������ ���� �������� ����ȭ
	// ����þ� Ŀ���� �̹����� �����Ҷ��� filter2D() �Լ��� ���
	////////////////////////////////////////////////////////////

	// Declare variable. (Kernel, Kernel size)
	int kernel_size = (int)2*ceil(sigma*4)+1;
	Mat gau_kernel = Mat(kernel_size,kernel_size,CV_32FC1);
	//���� ����


	// Compute gaussian kernel value
	//���� ����
	float sum = 0;
	float cal;
	for (int ky = 0; ky < kernel_size; ky++)
	{
		for (int kx = 0; kx < kernel_size; kx++)
		{
			cal = calGaus(kernel_size / 2 - kx, kernel_size / 2 - ky, sigma);
			sum += cal;
			gau_kernel.at<float>(ky, kx) = cal;
		}
	}
	// Normalize the kernel
	//���� ����
	for (int ky = 0; ky < kernel_size; ky++)
	{
		for (int kx = 0; kx < kernel_size; kx++)
		{
			gau_kernel.at<float>(ky, kx) = gau_kernel.at<float>(ky, kx) / sum;
		}
	}

	// Apply Gaussian kernel
	//���� ����
	filter2D(src,dst,-1,gau_kernel);
}
//////////////////////////////////////////////


int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &machingIdx) {
	///////////////////////////////////////////////////////////////
	// �̹���1�� ��ũ���͵�� �̹���2�� ��ũ���͵� �� ��Ī�Ǵ� ����� ������ ����ϴ� �Լ�
	// �̹���1�� 1���� ��ũ���Ϳ� �̹���2�� ��� ��ũ���� ������ L2 �Ÿ��� ��� 
	// ���� ����� �Ÿ� (d1) �� �ι�°�� ����� �Ÿ�(d2) �� ���� (d1/d2) �� ���� ������ �Ӱ谪 (DIST_RATIO_THR) ���� �۴ٸ� ��Ī���� �Ǵ�
	// machingIdx ���� ��Ī�� �� �ε����� ���� (machingIdx[idx1] = idx2, idx1�� �̹���1�� ��ũ������ �ε����̰�, idx2�� ��Ī�� �̹���2�� ��ũ������ �ε���)
	// �Լ��� ���ϰ����δ� ��Ī�� ����� ������ ���
	//////////////////////////////////////////////////////////////
	int numMatches = 0;
	machingIdx.resize(descriptor1.rows);

	for (int kp1x = 0; kp1x < descriptor1.rows; kp1x++)
	{
		float min1 = 0;
		float min2 = 0;
		int idx2;
		for (int kp2x = 0; kp2x < descriptor2.rows; kp2x++)
		{
			float d = 0;
			for (int kp2y = 0; kp2y < descriptor2.cols; kp2y++)
			{
				float temp = (descriptor2.at<float>(kp2x, kp2y) - descriptor1.at <float> (kp1x, kp2y));//(descriptor2.at<uchar>(kp2y, kp2x) - descriptor1.at<uchar>(kp2y, kp1x));
				temp = temp * temp;
				d += temp;
			}
			if (kp2x == 0)min1 = d;
			if (d < min1)
			{
				min2 = min1;
				min1 = d;
				idx2 = kp2x;
			}
		}
		if (sqrt(min1) / sqrt(min2) < DIST_RATIO_THR && min2 != 0 )
		{
			machingIdx[kp1x] = idx2;
			numMatches += 1;
		}
		else
		{
			machingIdx[kp1x] = -1;
		}

	}


	return numMatches;
}