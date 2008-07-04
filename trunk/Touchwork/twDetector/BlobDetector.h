#pragma once

class BlobDetector
{
	//Variables
public:

	//Bitmap
	BITMAPINFOHEADER* m_bitmapInfo;
	int m_WidthBytes;
	int BlobCount;
	BYTE* m_bmpBits;
	BYTE** m_bmpBitsLabel;
	static int m_Threshold;
public:
	virtual ~BlobDetector() = 0;
	virtual void DetectBlobs(BYTE* Buffer,list<twBlob>& blobList)=0;
	virtual void InitializeBitmap(BYTE* buffer)=0;
	virtual void PreprocessBitmap()=0;
};





class ComponentLabelingBlobDetector:public BlobDetector
{

public:
	
	~ComponentLabelingBlobDetector();
	ComponentLabelingBlobDetector(BITMAPINFOHEADER* b);
	void DetectBlobs(BYTE* Buffer,list<twBlob>& blobList);
	bool IsBlob(int y,int x);
	void InitializeBitmap(BYTE* buffer);
	void ApplyMonochrome();
	void ApplyInversion();
	void ApplyGaussianFilter(int noise);
	virtual void PreprocessBitmap();
};


