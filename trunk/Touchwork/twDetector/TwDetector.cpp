#include "stdafx.h"


INIT_SINGLETON(TwDetector)

void TwDetector::SetFormat( BITMAPINFOHEADER* pbmpinfo)
{
	if(m_blobDetector != NULL)
		delete m_blobDetector;
	m_blobDetector = new BlobDetector(pbmpinfo);
}

BlobDetector* TwDetector::GetBlobDetector()
{
	return TwDetector::m_blobDetector;
}

void TwDetector::SetBlobBrightness( int value )
{
	BlobDetector::s_Brightness = value;
}


int TwDetector::GetBlobBrightness()
{
	return BlobDetector::s_Brightness;
}