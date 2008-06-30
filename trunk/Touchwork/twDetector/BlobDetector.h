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
	twPoint* Contour;
	BYTE* m_bmpBits;

	BYTE** m_bmpBitsLabel;
	static int m_Threshold;
public:

	virtual void DetectBlobs(BYTE* Buffer,list<Blob>& blobList)=0;
	virtual void InitializeBitmap(BYTE* buffer)=0;
	virtual void PreprocessBitmap()=0;
};





class ComponentLabelingBlobDetector:public BlobDetector
{

public:

	~ComponentLabelingBlobDetector(void);
	ComponentLabelingBlobDetector(BITMAPINFOHEADER* b);
	void DetectBlobs(BYTE* Buffer,list<Blob>& blobList);
	bool IsBlob(int y,int x);
	void InitializeBitmap(BYTE* buffer);
	void ApplyMonochrome();
	void ApplyInversion();
	void ApplyGaussianFilter(int noise);
	virtual void PreprocessBitmap();
};


