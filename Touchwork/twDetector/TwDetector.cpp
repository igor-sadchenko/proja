#include "stdafx.h"


INIT_SINGLETON(TwDetector)

void TwDetector::SetFormat( BITMAPINFOHEADER* pbmpinfo)
{
}

BlobDetector* TwDetector::GetBlobDetector()
{
	return TwDetector::m_blobDetector;
}



void TwDetector::Detect(BITMAPINFOHEADER* bmpinfo, BYTE* pdata ,list<Blob>& blobList)
{
	BlobDetector* myblobDetector = new ComponentLabelingBlobDetector(bmpinfo);

	//image processing
	myblobDetector->InitializeBitmap(pdata);
	myblobDetector->PreprocessBitmap();
	
	//detection
	//thats copying a list!!!!!!!!!! 
	myblobDetector->DetectBlobs(pdata,blobList);
	delete myblobDetector;
}

void TwDetector::Initialize()
{

}
