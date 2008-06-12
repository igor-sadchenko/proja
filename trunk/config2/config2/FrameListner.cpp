#include "StdAfx.h"


void FrameListner::OnSampleArrived( BYTE*pdata,long size )
{
	//the frame listener code goes here // use m_bmpinfo
	//this->m_pbmpinfo  // aho :D
	for(int i = 0;i < size ; i++)
		pdata[i] = 255;

}


void HighPassListner::OnSampleArrived( BYTE*pdata,long size )
{
	//the highpass code goes here

	//this some code can use g_settings .. which will be saved to the filllle :D
	for(int i = 0;i < size ; i++)
		pdata[i] = 0;
}

