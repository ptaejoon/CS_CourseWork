#include "SIFT.h"
#define DIST_RATIO_THR 0.49

//�ǽ����� �ۼ��� �ڵ� ���
int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &matchingIdx) {
	///////////////////////////////////////////////////////////////
	// �̹���1�� ��ũ���͵�� �̹���2�� ��ũ���͵� �� ��Ī�Ǵ� ����� ������ ����ϴ� �Լ�
	// �̹���1�� 1���� ��ũ���Ϳ� �̹���2�� ��� ��ũ���� ������ L2 �Ÿ��� ��� 
	// ���� ����� �Ÿ� (d1) �� �ι�°�� ����� �Ÿ�(d2) �� ���� (d1/d2) �� ���� ������ �Ӱ谪 (DIST_RATIO_THR) ���� �۴ٸ� ��Ī���� �Ǵ�
	// machingIdx ���� ��Ī�� �� �ε����� ���� (machingIdx[idx1] = idx2, idx1�� �̹���1�� ��ũ������ �ε����̰�, idx2�� ��Ī�� �̹���2�� ��ũ������ �ε���)
	// �Լ��� ���ϰ����δ� ��Ī�� ����� ������ ���
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
	//�ΰ��� �̹����� �ϳ��� �̹����� ��ġ�� �Լ�
	//�ΰ��� �̹��� ũ�Ⱑ �ٸ� �� �ֱ� ������ opencv�� hconcat, hstack�� ������� �ʰ�, copyTo �Լ��� ����Ѵ�.
	//�ΰ��� �̹����� �����ִ� ū �ϳ��� Mat stacked�� ����� (Mat type�� �̹���1, 2�� ���� �Ѵ�),
	//copyTo �Լ��� ����� �̹���1�� �̹���2�� stacked�� �����Ѵ�.

	
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
	//��Ī����� ������ �޾Ƽ� ��Ī�� Ű����Ʈ�� ��ġ�� ��Ī ��� ������ ���� ���� �׸��� �Լ�
	//stack_imgs() �Լ��� ����� 2���� �̹����� ������ �ϳ��� �̹����� ����key
	//Ű����Ʈ�� ��ġ�� opencv�� circle() �Լ� ���, ���ἱ�� opencv�� line() �Լ� ���
	
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
	//���� ����

	return result_im;
}