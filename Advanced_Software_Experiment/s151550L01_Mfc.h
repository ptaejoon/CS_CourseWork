#include <opencv2/opencv.hpp>
#include "MsgView.h"
#include "CDIB.h"

using namespace std;
using namespace cv;

string type2str(int type);

class SWL01 {
private:
	BYTE    tmp[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
protected:
	// DIB data (read from a .bmp ÆÄÀÏ
	CDib	m_dibFile1;
	LONG	m_width1, m_height1;	// height and width
	int     m_depth1;				// bits per pixel
	LPBITMAPINFO mg_lpBMIH1;
	Mat     m_Mat1;		// Mat corresponding to image1

	CDib	m_dibFile2;
	LONG	m_width2, m_height2;	// height and width
	int     m_depth2;				// bits per pixel
	LPBITMAPINFO mg_lpBMIH2;
	Mat     m_Mat2;		// Mat corresponding to image2

	Mat     m_MatR;		// Resulting Mat after processing something
	LPBITMAPINFO mg_lpBMIHR = (LPBITMAPINFO)&tmp;

public:
	bool readImageF1;	// set true after reading an image
	bool readImageF2;	// set true after reading an image
	bool processedF;    // set true when any processing is done

	SWL01() {
		readImageF1 = false;
		readImageF2 = false;
		processedF = false;
	}

	~SWL01() {
	}

	void Create_bmiInfoHeader(cv::Mat *image);

	// read an image & store it to img_org
	void readImage(CString pathName, int which);

	void drawImage(CDC *pDC, int dcLTx, int dcLTy, int which);
	void drawMatPopUp(int which);

	void DIBtoMat(int which);
	void DarkenGrayscaleImage(void);
	void saveImage(CString pathName);
	void BrightenGrayscaleImage();
	void Color24toGrayscale();
	void Color16toGrayscale();
};