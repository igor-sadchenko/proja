#include "stdafx.h"


INIT_SINGLETON(TwDetector)

void TwDetector::SetFormat( BITMAPINFOHEADER* pbmpinfo)
{
}

BlobDetector* TwDetector::GetBlobDetector()
{
	return TwDetector::m_detectorImp;
}



void TwDetector::Detect(BITMAPINFOHEADER* bmpinfo, BYTE* pdata ,list<twBlob>& blobList)
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
