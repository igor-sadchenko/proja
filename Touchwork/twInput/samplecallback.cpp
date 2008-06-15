#include "stdafx.h"


using namespace Video;


Video::SampleCallback::SampleCallback()
{
	m_first_time = UINT_MAX;
	m_delay = -1000;

}
void Video::SampleCallback::SetDelay()
{
	bool is_live;
	m_VideoCapture->GetCaptureMode(&is_live);
	DWORD m_delay  = -1000;
	if(is_live)
		m_delay  = 0; 
}

HRESULT STDMETHODCALLTYPE Video::SampleCallback::SampleCB( double SampleTime, IMediaSample *pSample )
{
	/*
	DWORD now =GetTickCount();
	if(m_first_time == UINT_MAX)
		m_first_time = now;
	SampleTime*=1000.0 ;

	ApplicationManager::WriteLine(L" %d < %d\r\n",(DWORD)(m_first_time + SampleTime ),now);	

	if(m_first_time + SampleTime < now + m_delay)
	{
		return S_OK;
	}
	//*/
	//SetListener(m_plistener,m_VideoCapture);
	
	BYTE* pdata;
	pSample->GetPointer(&pdata);
	if(m_plistener != NULL)
	m_plistener->OnSampleArrived( pdata,pSample->GetSize() );

	return S_OK;
}
HRESULT STDMETHODCALLTYPE Video::SampleCallback::BufferCB( double SampleTime, BYTE *pBuffer,long BufferLen )
{
	if(m_plistener != NULL)
	m_plistener->OnSampleArrived( pBuffer ,BufferLen);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Video::SampleCallback::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject )
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE Video::SampleCallback::AddRef( void )
{
	return 0;
}

ULONG STDMETHODCALLTYPE Video::SampleCallback::Release( void )
{
	return 0;
}



void Video::SampleCallback::SetListener(SampleListener*plistener,CVideoCapture* pVideoCapturer)
{
	m_VideoCapture = pVideoCapturer;
	m_plistener = plistener;
	BITMAPINFOHEADER binfo= {sizeof(BITMAPINFOHEADER)};
	m_VideoCapture->GetFormat(&binfo);
	m_plistener->OnFormatChanges(&binfo);
}