#include "StdAfx.h"


void FrameListner::OnSampleArrived( BYTE*pdata,long size )
{
	m_WidthBytes = m_bmpinfo.biWidth*3;
	m_bmpBits = pdata;
}


void MonochromeListner::OnSampleArrived( BYTE*pdata,long size )
{
	ComponentLabelingBlobDetector detector(&m_bmpinfo);
	detector.InitializeBitmap(pdata);
	detector.ApplyMonochrome();
}

void NoiseRemovalListner::OnSampleArrived( BYTE*pdata,long size )
{
	ComponentLabelingBlobDetector detector(&m_bmpinfo);
	detector.InitializeBitmap(pdata);
	detector.ApplyGaussianFilter(g_settings.getNoise());
}

