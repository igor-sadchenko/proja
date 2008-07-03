#pragma comment (lib, "cv")
#pragma comment (lib, "cxcore")

#include "StdAfx.h"
#include <set>

int BlobDetector::m_Threshold = 145;

ComponentLabelingBlobDetector::~ComponentLabelingBlobDetector(void)
{
	//if(Contour)
	//	delete[] Contour;
	if(m_bmpBitsLabel)
	{
		for(int i = 0; i < m_bitmapInfo->biHeight; i++)
		{
			delete[] m_bmpBitsLabel[i];
		}
		delete[] m_bmpBitsLabel;
	}
}

ComponentLabelingBlobDetector::ComponentLabelingBlobDetector(BITMAPINFOHEADER* b)
{
	//s_Brightness = 150;
	BlobCount = -1;
	m_bitmapInfo = b;
	m_WidthBytes = b->biWidth*3;
	//Contour = new twPoint[m_bitmapInfo->biHeight*m_bitmapInfo->biWidth];
	m_bmpBitsLabel = new BYTE*[m_bitmapInfo->biHeight];
	for(int i = 0; i < m_bitmapInfo->biHeight; i++)   
		m_bmpBitsLabel[i] = new BYTE[m_bitmapInfo->biWidth];
}

bool ComponentLabelingBlobDetector::IsBlob(int y,int x)
{
	/*BYTE b = m_bmpBits[y*m_WidthBytes + x];
	BYTE g = m_bmpBits[y*m_WidthBytes + x+1];
	BYTE r = m_bmpBits[y*m_WidthBytes + x+2];
	int brightness = r + g + b;
	brightness /= 3;*/

	int brightness = m_bmpBits[y*m_WidthBytes + x];
	return brightness > m_Threshold;
}

void ComponentLabelingBlobDetector::InitializeBitmap(BYTE* buffer)
{
	m_bmpBits = buffer;
}

void ComponentLabelingBlobDetector::ApplyMonochrome()
{
	int nh = m_bitmapInfo->biHeight;
	for(int y = 0; y < nh; y++) 
	{
		for(int x = 0; x < m_WidthBytes; x++)
		{
			BYTE b = m_bmpBits[y*m_WidthBytes + x];
			BYTE g = m_bmpBits[y*m_WidthBytes + x+1];
			BYTE r = m_bmpBits[y*m_WidthBytes + x+2];
			int avg = r + g + b;
			avg /= 3;
			m_bmpBits[y*m_WidthBytes + x] = 
			m_bmpBits[y*m_WidthBytes + x+1] =
			m_bmpBits[y*m_WidthBytes + x+2] = avg;
		}
	}
}

void ComponentLabelingBlobDetector::ApplyInversion()
{
	int nh = m_bitmapInfo->biHeight;
	for(int y = 0; y < nh; y++) 
	{
		for(int x = 0; x < m_WidthBytes; x++)
		{
			m_bmpBits[y*m_WidthBytes + x] = //255 - m_bmpBits[y*m_WidthBytes + x];
			m_bmpBits[y*m_WidthBytes + x+1] = //255 - m_bmpBits[y*m_WidthBytes + x+1];
			m_bmpBits[y*m_WidthBytes + x+2] = 255 - m_bmpBits[y*m_WidthBytes + x+2];
		}
	}
}

void ComponentLabelingBlobDetector::ApplyGaussianFilter(int noise)
{
	IplImage* img = cvCreateImage(cvSize(m_bitmapInfo->biWidth, m_bitmapInfo->biHeight), IPL_DEPTH_8U, 3);
	IplImage* tmp = cvCreateImage(cvSize(m_bitmapInfo->biWidth, m_bitmapInfo->biHeight), IPL_DEPTH_8U, 3);
	IplImage* dst = cvCreateImage(cvSize(m_bitmapInfo->biWidth, m_bitmapInfo->biHeight), IPL_DEPTH_8U, 3);

    IplConvKernel* element = cvCreateStructuringElementEx( 3, 3, 0, 0, CV_SHAPE_ELLIPSE, 0 );
    IplConvKernel* element2 = cvCreateStructuringElementEx( 5, 5, 2, 2, CV_SHAPE_ELLIPSE, 0 );

	//img->imageData = (char*) m_bmpBits;
	cvSetData(img, m_bmpBits, m_WidthBytes);

//	cvSmooth(img, img, CV_GAUSSIAN, 7, 7, 0, 0 );
//	cvSub(img, tmp, img);

	cvErode(img, img, element, 1);
	cvSmooth(img, img, CV_GAUSSIAN, 11, 11, 0, 0 );
	cvDilate(img, img, element, 1);
	/*
	cvErode(img, img, element, 2);
	cvSmooth(img, img, CV_GAUSSIAN, 11, 11, 0, 0 );
	cvDilate(img, img, element2, 1);
	cvDilate(img, img, element, 1);
*/
	cvThreshold(img, img, noise, 255, CV_THRESH_TOZERO); 

	//m_bmpBits = (BYTE*) dst->imageData;
	int step;
    CvSize size;
	cvGetRawData(img, (uchar**)&m_bmpBits, &step, &size);
	
	// TODO : Delete pointers ... and change function's name ..
	cvReleaseImage(&img);
	cvReleaseImage(&tmp);
	cvReleaseImage(&dst);
	cvReleaseStructuringElement(&element);
	cvReleaseStructuringElement(&element2); // enta momtazz :Dloool
}

void ComponentLabelingBlobDetector::DetectBlobs(BYTE* Buffer,list<twBlob>& blobList) 
{
	m_bmpBits = Buffer;
	int nh = m_bitmapInfo->biHeight;
	int nw = m_bitmapInfo->biWidth;
	BlobCount = -1; 
	int C = 1, i,j;
	int start;  
	int x_sum = 0, y_sum = 0, all = 0;
	set<twPoint> Contour;
	set<twPoint>::iterator contour_iter;

	//Initialization
	for(i = 0; i < nh; i++)   
	{
		for(j = 0; j < nw; j++)
		{
			m_bmpBitsLabel[i][j] = 0;
		}
	}

	for(int y = 0; y < nh; y++) 
	{
		for(int x = 0; x<m_WidthBytes; x++)
		{
             if (IsBlob(y,x))
             {
				 i = x/3;
					 
				 //The Pixel lies on the external Contour
				if(y != 0 && m_bmpBitsLabel[y][i] == 0  && !IsBlob(y-1,x)) //unlabeled & pixel above it is white, y = 0 && (m_bmpBitsLabel[y][i] == 0))	//First Row, add dummy of white pixels
				 {
					 //Follow Contour Tracing Algo
					 start = 7;
					 int pixelx[] = {3,3,0,-3,-3,-3,0,3};
					 int pixely[] = {0,1,1,1,0,-1,-1,-1};

					 twPoint p = twPoint(x,y);
					 Contour.insert(p);
					 
					 while(!Contour.empty())
					 {
						 contour_iter = Contour.begin();
						 twPoint p = (twPoint)*contour_iter;
						 
						 if(p.m_x < nw*3 && p.m_x >=0 && p.m_y < nh && p.m_y >=0)
						 {
							 if(IsBlob(p.m_y,p.m_x) && m_bmpBitsLabel[p.m_y][p.m_x/3] == 0)
							 {
								 m_bmpBitsLabel[p.m_y][p.m_x/3] = C;						//Label point
								 x_sum += p.m_x;		y_sum += p.m_y;		all++;			//To Calculate the center

								 for(j = 0; j < 8; j++)
								 {
									 int k = (start + j)%8;
									 if((p.m_y + pixely[k] >= 0) && (p.m_y + pixely[k] < nh)	&&
										 (p.m_x + pixelx[k] >= 0) && (p.m_x + pixelx[k] < nw*3) &&
										 m_bmpBitsLabel[p.m_y + pixely[k]][(p.m_x + pixelx[k])/3] == 0)
									 {
										 Contour.insert(twPoint(p.m_x + pixelx[k],p.m_y + pixely[k]));
									 }
									 else if((p.m_y + pixely[k] >= 0) && (p.m_y + pixely[k] < nh) &&(p.m_x + pixelx[k] >= 0) && (p.m_x + pixelx[k] < nw))
									 {
										 //FIXED
										 m_bmpBitsLabel[p.m_y + pixely[k]][(p.m_x + pixelx[k])/3] = -1;
									 }
									 start = k;
								 }

								 //Color it
								 m_bmpBits[p.m_y*m_WidthBytes + p.m_x] = 0;
								 m_bmpBits[p.m_y*m_WidthBytes + p.m_x+1] = 0;
								 m_bmpBits[p.m_y*m_WidthBytes + p.m_x+2] = 255;
							 }
							 else
							 {
								 m_bmpBitsLabel[p.m_y][p.m_x/3] = -1;
							 }
						 }

						 Contour.erase(p); //MAY CHANGE
					 }

					 //Increment C
					 C++;

					 if(C > 1)
					 {
						 BlobCount++;

						 twBlob current = twBlob(BlobCount,twPoint((x_sum/all)/3, nh - y_sum/all));
						 current.m_pointscount = all;
						 list<twBlob>::iterator itr;
						 for(itr= blobList.begin() ; itr != blobList.end(); itr++ )
						 {
							if(itr->m_dist > current.m_dist)
								break;
						 }
						 blobList.insert(itr,current);

						 ////Color Center Green
						 m_bmpBits[(y_sum/all)*m_WidthBytes + (x_sum/all)] = 0;
						 m_bmpBits[(y_sum/all)*m_WidthBytes + (x_sum/all)+1] = 255;
						 m_bmpBits[(y_sum/all)*m_WidthBytes + (x_sum/all)+2] = 0;
						 x_sum = y_sum = all = 0;
					 }
				 }
				 //Pixel lies on internal contour or lies on both
				 else if(!IsBlob(y+1,x) && m_bmpBitsLabel[y+1][i] != -1) //unmarked white pixel
				 {
					 if(x != 0 && m_bmpBitsLabel[y][i] == 0  && m_bmpBitsLabel[y][i - 1] != 0)	
					 {																			
						 m_bmpBitsLabel[y][i] = m_bmpBitsLabel[y][i-1];
					 }
				 }
				 else
				 {
					 m_bmpBitsLabel[y][i] = m_bmpBitsLabel[y][i-1];
				 }
			 }
			 x += 2;
		}
	}

}

void ComponentLabelingBlobDetector::PreprocessBitmap()
{
#ifndef TW_CONFIG

	ApplyMonochrome();
	ApplyGaussianFilter(ModuleManager::getSettings().getNoise());
#endif
}