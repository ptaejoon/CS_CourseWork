

#include <iostream>
#include <time.h>// 시간 측정 위해서
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <queue>
using namespace cv;

Mat input_im, dst;
clock_t start_time, end_time;
using namespace std;

void UserDefinedAvg(long knum, string name)
{
	start_time = clock();

	if (knum > 1)
	{
		if (knum % 2 == 0)
		{
			knum -= 1;
		}
	}
	else
	{
		knum = 1;
	}
	int ksize = knum;
	if (ksize == 1)
	{
		int row = input_im.rows;
		int height = input_im.cols;
		dst.create(row, height, CV_8UC1);
		uchar *dstData = dst.data;
		uchar *imData = input_im.data;
		for (int r = 0; r < height*row; r++)
		{
			*dstData++ = *imData++;
		}
		end_time = clock();
		cout << "Kernel Size : " << ksize << ", Exec Time : " << (double)(end_time - start_time) << " (msec)" << endl;
		imshow(name, dst);

	}
	else {
		int row = input_im.rows;
		int height = input_im.cols;
		int val;
		dst.create(row, height, CV_8UC1);
		int *Avg = new int[row + 1];
		int *HAvg = new int[height + 1];
		//int Avg[512];
		//int HAvg[512];
		int Arsize = ksize * ksize;
		int hconvolstart, hconvolend;
		int rconvolstart, rconvolend;
		int FirstAvg = 0;
		int temp;
		// 0,0 구하기
		for (int h = 0; h <= ksize / 2; h++)
		{
			for (int r = 0; r <= ksize / 2; r++)
			{
				FirstAvg += input_im.at<uchar>(h, r);
			}
		}
		dst.at<uchar>(0, 0) = int(FirstAvg / Arsize);
		Avg[0] = FirstAvg;
		HAvg[0] = FirstAvg;

		int ZZAvg = FirstAvg;
		for (int r = 1; r < row; r++)
		{
			rconvolend = r + ksize / 2;
			rconvolstart = r - ksize / 2;
			for (int ytemp = 0; ytemp <= ksize / 2; ytemp++)
			{
				if (rconvolend >= row || rconvolstart < 0)
				{
					if (rconvolstart - 1 < 0) {
						FirstAvg = FirstAvg + input_im.at<uchar>(ytemp, rconvolend);
					}
					else {
						FirstAvg = FirstAvg - input_im.at<uchar>(ytemp, rconvolstart - 1);
					}
				}
				else
				{
					val = input_im.at<uchar>(ytemp, rconvolend);
					FirstAvg = FirstAvg + val;
					if (rconvolstart - 1 >= 0)FirstAvg = FirstAvg - input_im.at<uchar>(ytemp, rconvolstart - 1);


				}
			}
			if (FirstAvg < 0)
			{
				dst.at<uchar>(0, r) = 0;
				//Avg[r] = 0;
				//cout << "hi" << endl;
			}
			else if (FirstAvg / Arsize > 255)
			{
				dst.at<uchar>(0, r) = 255;
				//Avg[r] = Arsize * 255;
			}
			else
			{
				dst.at<uchar>(0, r) = int(FirstAvg / Arsize);
				//Avg[r] = FirstAvg;

			}
			Avg[r] = FirstAvg;
		}
		int t = input_im.at<uchar>(0, 0);

		//1번째 row 구하기
		for (int h = 1; h < height; h++)
		{
			hconvolstart = h - ksize / 2;
			hconvolend = h + ksize / 2;
			for (int rtemp = 0; rtemp <= ksize / 2; rtemp++)
			{
				if (hconvolend >= height || hconvolstart - 1 < 0)
				{
					if (hconvolstart - 1 < 0) {
						ZZAvg = ZZAvg + input_im.at<uchar>(hconvolend, rtemp);
					}
					else {
						ZZAvg = ZZAvg - input_im.at<uchar>(hconvolstart - 1, rtemp);
					}
				}
				else
				{
					val = input_im.at<uchar>(hconvolend, rtemp);
					ZZAvg = ZZAvg + val;
					if (hconvolstart - 1 >= 0)ZZAvg = ZZAvg - input_im.at<uchar>(hconvolstart - 1, rtemp);


				}
			}
			if (ZZAvg < 0)
			{
				dst.at<uchar>(h, 0) = 0;
				//HAvg[h] = 0;
				//cout << "hi" << endl;
			}
			else if (ZZAvg / Arsize > 255)
			{
				dst.at<uchar>(h, 0) = 255;
				//HAvg[h] = 255 * Arsize;
			}
			else {
				dst.at<uchar>(h, 0) = int(ZZAvg / Arsize);
				//HAvg[h] = ZZAvg;
			}
			HAvg[h] = ZZAvg;
		}
		/*for (int h = 0; h < height; h++)
		{
			cout << h << ":" << HAvg[h] << endl;
		}*/
		//1번째 height 구하기
		int prev;
		int checkim;
		for (int h = 1; h < height; h++)
		{

			for (int r = 1; r < row; r++)
			{
				rconvolend = r + ksize / 2;
				rconvolstart = r - ksize / 2;
				hconvolstart = h - ksize / 2;
				hconvolend = h + ksize / 2;
				if (r == 1)
				{
					prev = Avg[r];
					val = HAvg[h] + Avg[r] - HAvg[h - 1];
				}
				else
				{

					val = Avg[r - 1] + Avg[r] - prev;
					prev = Avg[r];
				}
				temp = 0;
				if (rconvolstart - 1 >= 0 && hconvolstart - 1 >= 0)temp = temp + input_im.at<uchar>(hconvolstart - 1, rconvolstart - 1);
				if (rconvolstart - 1 >= 0 && hconvolend < height)temp = temp - input_im.at<uchar>(hconvolend, rconvolstart - 1);
				if (rconvolend < row && hconvolstart - 1 >= 0)temp = temp - input_im.at<uchar>(hconvolstart - 1, rconvolend);
				if (rconvolend < row && hconvolend < height)temp = temp + input_im.at<uchar>(hconvolend, rconvolend);
				temp = val + temp;
				Avg[r] = temp;
				temp = temp / Arsize;
				if (temp < 0)
				{
					//cout << temp<< endl;
					//Avg[r] = 0;
					temp = 0;
				}
				if (temp > 255)
				{
					temp = 255;
					Avg[r] = 255 * Arsize;
					//cout << "it's called" << endl;
				}
				checkim = input_im.at<uchar>(h, r);
				dst.at<uchar>(h, r) = temp;


			}
		}
		delete[] HAvg;
		delete[] Avg;
		end_time = clock();

		cout << "Kernel Size : " << ksize << ", Exec Time : " << (double)(end_time - start_time) << " (msec)" << endl;
		imshow(name, dst);
	}

}


string type2str(int type) {
	// https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		cout << "[프로그램 사용법]" << endl;
		cout << "명령문 : s151550L02HW.exe imageFile kernalSize<int>" << endl;
		return 0;
	}
	//파일 주소는 argv[1]로 들어옴

	input_im = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (input_im.empty())
	{
		cout << "File open Error!" << endl;
		return -1;
	}
	cout << "Image size :" << input_im.size() << ", Type:" << type2str(input_im.type()) << endl;

	string window_name = "movingAvrg HW";
	namedWindow(window_name);
	long knum = strtol(argv[2],NULL, 10);
	
	//Create track bar to change kernel size
	//createTrackbar("ThresholdValue", window_name, &start_value, max_value, binaryThresholding_opencvCommand, static_cast<void*>(&window_name));
	UserDefinedAvg(knum,window_name);

	//createTrackbar("ThresholdValue", window_name, &start_value, max_value, UserDefinedAvg, static_cast<void*>(&window_name));


	//imshow(window_name, input_im);
	waitKey(0);

	return 0;
}