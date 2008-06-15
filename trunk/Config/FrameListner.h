#pragma once

#include <cv.h>

class FrameListner: public SampleListener
{
public:
	int m_WidthBytes;
	BYTE* m_bmpBits;





public:
	void OnSampleArrived(BYTE*pdata,long size);
};

class MonochromeListner: public SampleListener
{
public:
	void OnSampleArrived(BYTE*pdata,long size);
};

class NoiseRemovalListner: public SampleListener
{
public:
	void OnSampleArrived(BYTE*pdata,long size);
};
