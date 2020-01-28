#include "SIFT.h"
#define DIST_RATIO_THR 0.49

//실습에서 작성한 코드 사용
int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &matchingIdx) {
	///////////////////////////////////////////////////////////////
	// 이미지1의 디스크립터들과 이미지2의 디스크립터들 중 매칭되는 페어의 개수를 계산하는 함수
	// 이미지1의 1개의 디스크립터와 이미지2의 모든 디스크립터 사이의 L2 거리를 계산 
	// 가장 가까운 거리 (d1) 와 두번째로 가까운 거리(d2) 의 비율 (d1/d2) 을 구해 설정된 임계값 (DIST_RATIO_THR) 보다 작다면 매칭으로 판단
	// machingIdx 에는 매칭이 된 인덱스를 저장 (machingIdx[idx1] = idx2, idx1은 이미지1의 디스크립터의 인덱스이고, idx2는 매칭된 이미지2의 디스크립터의 인덱스)
	// 함수의 리턴값으로는 매칭된 페어의 개수를 출력
	//////////////////////////////////////////////////////////////
	int numMatches = 0;
	matchingIdx.resize(descriptor1.rows);

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
				float temp = (descriptor2.at<float>(kp2x, kp2y) - descriptor1.at <float>(kp1x, kp2y));//(descriptor2.at<uchar>(kp2y, kp2x) - descriptor1.at<uchar>(kp2y, kp1x));
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
		if (sqrt(min1) / sqrt(min2) < DIST_RATIO_THR && min2 != 0)
		{
			matchingIdx[kp1x] = idx2;
			numMatches += 1;
		}
		else
		{
			matchingIdx[kp1x] = -1;
		}

	}

	return numMatches;
}


Mat stack_imgs(const Mat &im1, const Mat &im2) {
	//두개의 이미지를 하나의 이미지로 합치는 함수
	//두개의 이미지 크기가 다를 수 있기 때문에 opencv의 hconcat, hstack을 사용하지 않고, copyTo 함수를 사용한다.
	//두개의 이미지가 들어갈수있는 큰 하나의 Mat stacked을 만들고 (Mat type은 이미지1, 2와 같게 한다),
	//copyTo 함수를 사용해 이미지1과 이미지2를 stacked에 복사한다.

	
	int row = im1.cols + im2.cols;
	int height = im1.rows > im2.rows ? im1.rows : im2.rows;
	Mat stacked = Mat::ones(height,row, CV_8UC3);
	Mat first = stacked(Rect(0, 0, im1.cols, im1.rows));
	im1.copyTo(first);
	
	Mat next = stacked(Rect(im1.cols, 0,im2.cols, im2.rows));
	im2.copyTo(next);

	return stacked;
}


Mat DrawMatches(const Mat &im1, const Mat &im2, vector<KeyPoint> &keypoints1, vector<KeyPoint> &keypoints2, vector<int> &matchingIdx) {
	//매칭페어의 정보를 받아서 매칭된 키포인트의 위치와 매칭 페어 사이의 연결 선을 그리는 함수
	//stack_imgs() 함수를 사용해 2개의 이미지를 연결해 하나의 이미지로 만듦key
	//키포인트의 위치는 opencv의 circle() 함수 사용, 연결선은 opencv의 line() 함수 사용
	
	Mat result_im;
	result_im = stack_imgs(im1, im2);//
	for (int i = 0; i < keypoints1.size(); i++)
	{
		circle(result_im, keypoints1[i].pt,1,Scalar(0,255,0));
	}
	for (int i = 0; i < keypoints2.size(); i++)
	{
		circle(result_im, Point(keypoints2[i].pt.x + im1.cols, keypoints2[i].pt.y), 1, Scalar(0, 255, 0));
	}
	for (int i = 0; i < matchingIdx.size(); i++)
	{
		if (matchingIdx[i] != -1)
		{
			line(result_im, keypoints1[i].pt, Point(keypoints2[matchingIdx[i]].pt.x + im1.cols, keypoints2[matchingIdx[i]].pt.y), Scalar(0, 255, 0));
		}
	}
	//line(result_im,ke//,keypoints2)
	//Draw keypoint location and line between matching pair
	//내용 구현

	return result_im;
}