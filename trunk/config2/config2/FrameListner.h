#pragma once

class FrameListner: public SampleListener
{
public:
	void OnSampleArrived(BYTE*pdata,long size);
	void OnFormatChanges(BITMAPINFOHEADER* pbmpinfo);

};

class HighPassListner: public SampleListener
{
public:
	void OnSampleArrived(BYTE*pdata,long size);
	void OnFormatChanges(BITMAPINFOHEADER* pbmpinfo);
};
