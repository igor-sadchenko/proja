#pragma once
#include "Point.h"
#include "Blob.h"
#include <list>
using namespace std;

class BlobDetector
{
//Variables
public:
	//Getters & setters
	//Bitmap
	BITMAPINFOHEADER* m_bitmapInfo;
	int m_WidthBytes;
	int BlobCount;
	static int s_Brightness;
	Point* Contour;
	BYTE* m_bmpBits;
	
	BYTE** m_bmpBitsLabel;
public:

	~BlobDetector(void);
	BlobDetector(BITMAPINFOHEADER* b);
	list<Blob> DetectBlobs(BYTE* Buffer) ;
	bool IsBlob(int y,int x);
};



/*
twDetector:
===========
ctor(BITMAPINFOHEADER);							// Initialize();
int Detect(BYTE*);								// mycallbackfunciton();
void SetBitmapInfoHeader(BITMAPINFOHEADER);		// setter
Blob* GetBlobs();
*/