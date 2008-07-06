#include "stdafx.h"


INIT_SINGLETON(TwDetector)

void TwDetector::SetFormat( BITMAPINFOHEADER* pbmpinfo)
{
	m_blobDetector = new ComponentLabelingBlobDetector(pbmpinfo);
}

BlobDetector* TwDetector::GetBlobDetector()
{
	return TwDetector::m_blobDetector;
}



void TwDetector::Detect(BITMAPINFOHEADER* bmpinfo, BYTE* pdata ,list<twBlob>& blobList)
{
	

	//image processing
	m_blobDetector->InitializeBitmap(pdata);
	m_blobDetector->PreprocessBitmap();
	
	//detection
	//thats copying a list!!!!!!!!!! 
	m_blobDetector->DetectBlobs(pdata,blobList);

}

void TwDetector::Initialize()
{
	BlobDetector::m_Threshold = ModuleManager::getSettings().m_threshold;
}

void TwDetector::DeInitialize()
{
	delete m_blobDetector;	
}