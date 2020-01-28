#include "stdafx.h"
#include "MainFrm.h"
#include "swLab19fDoc.h"
#include "swLab19fView.h"
#include "s151550L01_Mfc.h"
#include "s151550L01_ext.h"


void SWL01::DarkenGrayscaleImage(void) {
	
	if (SWL01_inst.readImageF1 == TRUE)
	{
		string s1 = "8UC1";
		Mat *pMat;
		Mat *pMatR;
		pMat = &m_Mat1;
		pMatR = &m_MatR;
		int   w, h, depth;
		w = m_width1; h = m_height1; depth = m_depth1;

		*pMatR = Mat(h, w, CV_8UC1);
		string st = type2str((*pMat).type());
		
		if (st.compare(s1) != 0) {
			AfxMessageBox(L"Only 8bit grayscale can be shown!", MB_OK, 0);
			return;
		}
		
		unsigned char *pMatData = (*pMat).data;
		unsigned char *pMatRData = (*pMatR).data;
		for (int p = 0; p < h * w * (*pMat).channels(); p++) {
			*pMatRData++ = *pMatData++/2;
		}
		Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
		processedF = true; // 처리 완료를 flag을 통하여 알린다
		g_pView->Invalidate(); // OnDraw를 호출한다

	}
	else
	{
		AfxMessageBox(L"Image is not Uploaded!", MB_OK, 0);
		return;
	}
}

void SWL01::BrightenGrayscaleImage(void)
{
	if (SWL01_inst.readImageF1 == TRUE)
	{
		string s1 = "8UC1";
		Mat *pMat;
		Mat *pMatR;
		pMat = &m_Mat1;
		pMatR = &m_MatR;
		int   w, h, depth;
		w = m_width1; h = m_height1; depth = m_depth1;

		*pMatR = Mat(h, w, CV_8UC1);
		string st = type2str((*pMat).type());

		if (st.compare(s1) != 0) {
			AfxMessageBox(L"Only 8bit grayscale can be shown!", MB_OK, 0);
			return;
		}

		unsigned char *pMatData = (*pMat).data;
		unsigned char *pMatRData = (*pMatR).data;
		for (int p = 0; p < h * w * (*pMat).channels(); p++) {
			*pMatRData++ = (*pMatData++ +255) / 2;
		}
		Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
		processedF = true; // 처리 완료를 flag을 통하여 알린다
		g_pView->Invalidate(); // OnDraw를 호출한다

	}
	else
	{
		AfxMessageBox(L"Image is not Uploaded!", MB_OK, 0);
		return;
	}
}

void SWL01::Color24toGrayscale(void) {
	if (SWL01_inst.readImageF1 == TRUE)
	{
		string s1 = "8UC3";
		Mat *pMat;
		Mat *pMatR;
		pMat = &m_Mat1;
		pMatR = &m_MatR;
		int   w, h, depth;
		w = m_width1; h = m_height1; depth = m_depth1;

		*pMatR = Mat(h, w, CV_8UC1);
		string st = type2str((*pMat).type());

		if (st.compare(s1) != 0) {
			AfxMessageBox(L"Only 8bit grayscale can be shown!", MB_OK, 0);
			return;
		}

		unsigned char *pMatData = (*pMat).data;
		unsigned char *pMatRData = (*pMatR).data;

		for (int p = 0; p < h * w * (*pMat).channels(); p = p +3)
		{
			pMatRData[p/3] = (0.114*float(pMatData[p]) + 0.587*float(pMatData[p + 1]) + 0.299*float(pMatData[p + 2]))/3;

		
		}



		Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
		processedF = true; // 처리 완료를 flag을 통하여 알린다
		g_pView->Invalidate(); // OnDraw를 호출한다
		
		
	}
	else
	{
		AfxMessageBox(L"Image is not Uploaded!", MB_OK, 0);
		return;
	}
}

void SWL01::Color16toGrayscale(void)
{
	if (SWL01_inst.readImageF1 == TRUE)
	{
		string s1 = "16UC1";
		Mat *pMat;
		Mat *pMatR;
		pMat = &m_Mat1;
		pMatR = &m_MatR;
		int   w, h, depth;
		w = m_width1; h = m_height1; depth = m_depth1;

		*pMatR = Mat(h, w, CV_8UC1);
		string st = type2str((*pMat).type());

		if (st.compare(s1) != 0) {
			AfxMessageBox(L"Only 8bit grayscale can be shown!", MB_OK, 0);
			return;
		}
		
	
			
		unsigned char *pMatData = (*pMat).data;
		unsigned char *pMatRData = (*pMatR).data;

		unsigned short temp;
		unsigned short R, G, B;
		for (int r = 0; r < h; r++) {
			for (int c = 0; c < w; c++) {
				temp = (*pMat).at<ushort>(r, c);
				R = 31744 & temp;
				R = R >> 10;
				G = 992 & temp;
				G = G >> 5;
				B = 31 & temp;
				*pMatRData++ = (float(R)*0.299 + 0.587*float(G) + 0.114*float(B))*8;
			}
		}

		Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
		processedF = true; // 처리 완료를 flag을 통하여 알린다
		g_pView->Invalidate(); // OnDraw를 호출한다


	}
	else
	{
		AfxMessageBox(L"Image is not Uploaded!", MB_OK, 0);
		return;
	}
}