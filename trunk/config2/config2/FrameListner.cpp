#include "StdAfx.h"


void FrameListner::OnSampleArrived( BYTE*pdata,long size )
{
	m_WidthBytes = m_bmpinfo.biWidth*3;
	m_bmpBits = pdata;
}


void MonochromeListner::OnSampleArrived( BYTE*pdata,long size )
{
	
	int nw = m_bmpinfo.biWidth*3;
	int nh = m_bmpinfo.biHeight;

	for(int y = 0; y < nh; y++) 
	{
		for(int x = 0; x < nw; x++)
		{
			BYTE b = pdata[y*nw + x];
			BYTE g = pdata[y*nw + x+1];
			BYTE r = pdata[y*nw + x+2];
			int avg = r + g + b;
			avg /= 3;
			pdata[y*nw + x] = 
			pdata[y*nw + x+1] =
			pdata[y*nw + x+2] = avg;
		}
	}

}

void NoiseRemovalListner::OnSampleArrived( BYTE*pdata,long size )
{
	
	IplImage* img = cvCreateImage(cvSize(m_bmpinfo.biWidth, m_bmpinfo.biHeight), IPL_DEPTH_8U, 3);
	IplImage* tmp = cvCreateImage(cvSize(m_bmpinfo.biWidth, m_bmpinfo.biHeight), IPL_DEPTH_8U, 3);
	IplImage* dst = cvCreateImage(cvSize(m_bmpinfo.biWidth, m_bmpinfo.biHeight), IPL_DEPTH_8U, 3);

    IplConvKernel* element = cvCreateStructuringElementEx( 3, 3, 0, 0, CV_SHAPE_ELLIPSE, 0 );
    IplConvKernel* element2 = cvCreateStructuringElementEx( 5, 5, 2, 2, CV_SHAPE_ELLIPSE, 0 );

	int nw = m_bmpinfo.biWidth*3;

	//img->imageData = (char*) m_bmpBits;
	cvSetData(img, pdata, nw);

//	cvSmooth(img, img, CV_GAUSSIAN, 7, 7, 0, 0 );
//	cvSub(img, tmp, img);

	cvErode(img, img, element, 1);
	cvSmooth(img, img, CV_GAUSSIAN, 11, 11, 0, 0 );
	cvDilate(img, img, element, 1);
//	g_settings
	/*
	cvErode(img, img, element, 2);
	cvSmooth(img, img, CV_GAUSSIAN, 11, 11, 0, 0 );
	cvDilate(img, img, element2, 1);
	cvDilate(img, img, element, 1);
*/
	
	cvThreshold(img, img, (double)(*g_settings.m_valNoise), 255, CV_THRESH_TOZERO); 

	//m_bmpBits = (BYTE*) dst->imageData;
	int step;
    CvSize size2;
	cvGetRawData(img, (uchar**)&pdata, &step, &size2);
	
	// TODO : Delete pointers ... and change function's name ..
	cvReleaseImage(&img);
	cvReleaseImage(&tmp);
	cvReleaseImage(&dst);
	cvReleaseStructuringElement(&element);
	cvReleaseStructuringElement(&element2); // enta momtazz :Dloool
}

