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

class CroppingListner: public SampleListener
{
public:
	bool m_isCropMode;
	int m_cropPoints;
	bool m_isNewBlob;
	CroppingListner(){
		m_isCropMode = false;
		m_cropPoints = 0;
		m_isNewBlob = true;
	}
	void OnSampleArrived(BYTE*pdata,long size);
};