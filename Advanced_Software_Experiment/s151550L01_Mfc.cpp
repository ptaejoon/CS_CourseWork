#include "stdafx.h"

#include "swLab19fDoc.h"
#include "swLab19fView.h"
#include "MainFrm.h"

#include "s151550L01_Mfc.h"
#include "CDIB.h"

SWL01			SWL01_inst;
CswLab19fView	*g_pView = NULL;
CMainFrame		*g_pMainF = NULL;

void SWL01::readImage(CString pathName, int which) {
	// read an image & store it to img_org
	CFile file;
	LPCTSTR str = pathName;
	CDib  *pDibF;
	LONG  *width, *height;
	int   *depth;
	LPBITMAPINFO *bmInfoHd;

	if (which == 1) {
		pDibF = &m_dibFile1;	// when reading image1
		width = &m_width1; height = &m_height1;
		depth = &m_depth1;  bmInfoHd = &mg_lpBMIH1;
		readImageF1 = true;
	}
	else if (which == 2) {
		if (readImageF1 == false) {
			AfxMessageBox(L"Read image1 first!", MB_OK, 0);
			return;
		}
		pDibF = &m_dibFile2;	// when reading addtional image2
		width = &m_width2; height = &m_height2;
		depth = &m_depth2;  bmInfoHd = &mg_lpBMIH2;
		readImageF2 = true;
	}

	(*pDibF).Empty();	// clear any previouse read image
	file.Open(str, CFile::modeRead);
	if ((*pDibF).Read(&file) != TRUE) {
		file.Close();
		return;
	}
	file.Close();
	*width = ((*pDibF).m_lpBMIH)->biWidth;
	*height = ((*pDibF).m_lpBMIH)->biHeight;
	*depth = ((*pDibF).m_lpBMIH)->biBitCount;
	*bmInfoHd = (LPBITMAPINFO)((*pDibF).m_lpBMIH);

	DIBtoMat(which);

	processedF = false;	// will be true after doing somthing

	CString msga, msgb;
	msga.Format(L"Read an image from %s and save it to image %d", pathName, which);
	msgb.Format(L"  Its size and depth are (w = %d, h = %d, depth = %d)",
		*width, *height, *depth);
	showMessage(msga); showMessage(msgb);

	g_pView->Invalidate();
}

void SWL01::drawImage(CDC *pDC, int dcLTx, int dcLTy, int which) {
	if (which == 1) {
		m_dibFile1.Draw(pDC, CPoint(dcLTx, dcLTy), CSize(m_width1, m_height1));
	}
	else if (which == 2) {
		m_dibFile2.Draw(pDC, CPoint(dcLTx + 20 + m_width1, dcLTy), CSize(m_width2, m_height2));
	}
	else if (which == 3) {
		StretchDIBits(pDC->GetSafeHdc(),		// dc handle
			dcLTx + 20 + m_width1, dcLTy,		// 화면의 좌상귀 좌표
			m_MatR.cols, m_MatR.rows,	// 화면의 폭과 높이
			0, 0,						// Mat 배열의 좌상귀
			m_MatR.cols, m_MatR.rows,	// Mat 배열의 폭과 높이
			m_MatR.data,				// image data to display
			mg_lpBMIHR,			// BITMAPINFO 시작 주소 
			DIB_RGB_COLORS,		// RGB 또는 색상 테이블 인덱스
			SRCCOPY				// 래스터 연산 방법
		);
	}
}

void SWL01::drawMatPopUp(int which) {
	string str("Input Image ");
	const char *cstr;
	const char *Rstr = "Output Image";
	Mat *pMat;

	if (which == 1) {
		str.append("1");
		cstr = str.c_str();		// conversion from string to (const char *)
		pMat = &m_Mat1;
	}
	else if (which == 2) {
		str.append("2");
		cstr = str.c_str();
		pMat = &m_Mat2;
	}
	else if (which == 3) {
		cstr = Rstr;
		pMat = &m_MatR;
	}
	string s1 = "8UC1", s3 = "8UC3";
	string st = type2str((*pMat).type());
	if (st.compare(s1) != 0 && st.compare(s3) != 0) {
		AfxMessageBox(L"Only 8bit grayscale or 24bit color can be shown!", MB_OK, 0);
		return;
	}
	//namedWindow(cstr, WINDOW_AUTOSIZE);
	imshow(cstr, *pMat);
	waitKey(0);
}

void SWL01::DIBtoMat(int which) {
	CDib  *pDibF;
	Mat   *pMat;
	int   w, h, depth;
	if (which == 1) {
		pDibF = &m_dibFile1; pMat = &m_Mat1;
		w = m_width1; h = m_height1; depth = m_depth1;
	}
	else if (which == 2) {
		pDibF = &m_dibFile2; pMat = &m_Mat2;
		w = m_width2; h = m_height2; depth = m_depth2;
	}
	(*pMat).release();		// clear any previous image data

	switch (depth) {
	case 8:
		*pMat = Mat(h, w, CV_8UC1);	// allocate Mat			
		break;
	case 16:	// in this case we create CV_16UC1 Mat(drawable? anyway...)
		*pMat = Mat(h, w, CV_16UC1);
		break;
	case 24:
		*pMat = Mat(h, w, CV_8UC3);
	};
	unsigned char *pMatData = (*pMat).data;
	unsigned char *pDibData = (*pDibF).m_lpImage;
	//***아래에 실제 이미지를 복사하는 빈 부분을 짥게 작성하자***
	//***   8,16,24 모든 이미지를 볼 수 있어야 한다                ***
	/*for (int r = 0; r < h; r++)
		for (int c = 0; c < w; c++) {
			pMatData[r * h * 3 + c * 3] = *pDibData++;
			pMatData[r * h * 3 + c * 3 + 1] = *pDibData++;
			pMatData[r * h * 3 + c * 3 + 2] = *pDibData++;
		}*/
	//***
	switch (depth) {
		case 8:
			for (int r = 0; r < h; r++)
				for (int c = 0; c < w; c++) {
					pMatData[r * h + c ] = *pDibData++;
				}
			break;
		case 16:
			for (int r = 0; r < h; r++)
				for (int c = 0; c < w; c++) {
					pMatData[r * w *2 + c * 2] = *pDibData++;
					pMatData[r * w *2 + c * 2 + 1] = *pDibData++;
				}
			break;
		case 24:
			for (int r = 0; r < h; r++)
				for (int c = 0; c < w; c++) {
					*pMatData++ = *pDibData++;
					*pMatData++ = *pDibData++;
					*pMatData++ = *pDibData++;
				}
	}
	
	flip((*pMat), (*pMat), 0);	// upside down
	
}


//
// Global Functions
//
/*
+--------+----+----+----+----+------+------+------+------+
|        | C1 | C2 | C3 | C4 | C(5) | C(6) | C(7) | C(8) |
+--------+----+----+----+----+------+------+------+------+
| CV_8U  |  0 |  8 | 16 | 24 |   32 |   40 |   48 |   56 |
| CV_8S  |  1 |  9 | 17 | 25 |   33 |   41 |   49 |   57 |
| CV_16U |  2 | 10 | 18 | 26 |   34 |   42 |   50 |   58 |
| CV_16S |  3 | 11 | 19 | 27 |   35 |   43 |   51 |   59 |
| CV_32S |  4 | 12 | 20 | 28 |   36 |   44 |   52 |   60 |
| CV_32F |  5 | 13 | 21 | 29 |   37 |   45 |   53 |   61 |
| CV_64F |  6 | 14 | 22 | 30 |   38 |   46 |   54 |   62 |
+--------+----+----+----+----+------+------+------+------+
*/
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

void SWL01::Create_bmiInfoHeader(cv::Mat *image) {
	// https://luckygg.tistory.com/117
	int bpp = image->channels() * 8;
	int w = image->cols, h = image->rows;

	memset(mg_lpBMIHR, 0, sizeof(BITMAPINFO));
	mg_lpBMIHR->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	mg_lpBMIHR->bmiHeader.biPlanes = 1;
	mg_lpBMIHR->bmiHeader.biBitCount = bpp;
	mg_lpBMIHR->bmiHeader.biCompression = BI_RGB;
	mg_lpBMIHR->bmiHeader.biWidth = w;
	mg_lpBMIHR->bmiHeader.biHeight = -h;
	mg_lpBMIHR->bmiHeader.biSizeImage = w * h * 1;

	switch (bpp) {
	case 8:
		for (int i = 0; i < 256; i++) {
			mg_lpBMIHR->bmiColors[i].rgbBlue = (BYTE)i;
			mg_lpBMIHR->bmiColors[i].rgbGreen = (BYTE)i;
			mg_lpBMIHR->bmiColors[i].rgbRed = (BYTE)i;
			mg_lpBMIHR->bmiColors[i].rgbReserved = 0;
		}
		break;
	case 32:
	case 24:
		((DWORD*)mg_lpBMIHR->bmiColors)[0] = 0x00FF0000; /* red mask  */
		((DWORD*)mg_lpBMIHR->bmiColors)[1] = 0x0000FF00; /* green mask */
		((DWORD*)mg_lpBMIHR->bmiColors)[2] = 0x000000FF; /* blue mask  */
		break;
	}
}

void SWL01::saveImage(CString pathName) {
	// save the processed image to a specific file
	// possible extension: .bmp, ,jpg, etc. For more see
	// https://docs.opencv.org/3.4.7/d4/da8/group__imgcodecs.html#ga288b8b3da0892bd651fce07b3bbd3a56
	string str = CT2CA(pathName);     // convert CString to string
	const char *cstr = str.c_str();

	bool result = imwrite(cstr, m_MatR);
	if (!result) {
		AfxMessageBox(L"ERROR: Cannot Save The Image", MB_OK, 0);
	}
	g_pView->Invalidate(TRUE);
	return;
}