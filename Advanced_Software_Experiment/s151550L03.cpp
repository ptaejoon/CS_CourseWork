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
	// 가우시안 커널을 생성해 이미지에 적용하는 함수
	// 커널의 크기는 2*(4*sigma)+1 (소수점은 올림 또는 반올림으로 처리한다)
	// 가우시안 커널은 float형의 값을 가짐
	// 가우시안 커널은 커널 값 계산 후 전체의 총합으로 나눠서 0~1사이의 값을 가지도록 정규화
	// 가우시안 커널을 이미지에 적용할때는 filter2D() 함수를 사용
	////////////////////////////////////////////////////////////

	// Declare variable. (Kernel, Kernel size)
	int kernel_size = (int)2*ceil(sigma*4)+1;
	Mat gau_kernel = Mat(kernel_size,kernel_size,CV_32FC1);
	//내용 구현


	// Compute gaussian kernel value
	//내용 구현
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
	//내용 구현
	for (int ky = 0; ky < kernel_size; ky++)
	{
		for (int kx = 0; kx < kernel_size; kx++)
		{
			gau_kernel.at<float>(ky, kx) = gau_kernel.at<float>(ky, kx) / sum;
		}
	}

	// Apply Gaussian kernel
	//내용 구현
	filter2D(src,dst,-1,gau_kernel);
}
//////////////////////////////////////////////


int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &machingIdx) {
	///////////////////////////////////////////////////////////////
	// 이미지1의 디스크립터들과 이미지2의 디스크립터들 중 매칭되는 페어의 개수를 계산하는 함수
	// 이미지1의 1개의 디스크립터와 이미지2의 모든 디스크립터 사이의 L2 거리를 계산 
	// 가장 가까운 거리 (d1) 와 두번째로 가까운 거리(d2) 의 비율 (d1/d2) 을 구해 설정된 임계값 (DIST_RATIO_THR) 보다 작다면 매칭으로 판단
	// machingIdx 에는 매칭이 된 인덱스를 저장 (machingIdx[idx1] = idx2, idx1은 이미지1의 디스크립터의 인덱스이고, idx2는 매칭된 이미지2의 디스크립터의 인덱스)
	// 함수의 리턴값으로는 매칭된 페어의 개수를 출력
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