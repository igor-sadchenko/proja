#pragma once

class FrameListner: public SampleListener
{
public:
	void OnSampleArrived(BYTE*pdata,long size);
};

class HighPassListner: public SampleListener
{
public:
	void OnSampleArrived(BYTE*pdata,long size);
};
