#pragma once

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
	twPoint* Contour;
	BYTE* m_bmpBits;
	
	BYTE** m_bmpBitsLabel;
public:

	~BlobDetector(void);
	BlobDetector(BITMAPINFOHEADER* b);
	list<Blob> DetectBlobs(BYTE* Buffer);
	bool IsBlob(int y,int x);
	void InitializeBitmap(BYTE* buffer);
	void ApplyMonochrome();
	void ApplyInversion();
	void ApplyGaussianFilter();
};



/*
twDetector:
===========
ctor(BITMAPINFOHEADER);							// Initialize();
int Detect(BYTE*);								// mycallbackfunciton();
void SetBitmapInfoHeader(BITMAPINFOHEADER);		// setter
Blob* GetBlobs();
*/