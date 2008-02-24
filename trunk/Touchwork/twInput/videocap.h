#pragma once
#include "stdafx.h"
#include<DShow.h>
#include "Qedit.h"
#pragma comment(lib,"Strmiids")

#include<vector>
using namespace std;


typedef bool (*FormatAccepted)();
typedef bool (*EnumFunc)(int,LPTSTR,void*);
typedef void (*SampleCallbackFunc)(BYTE*pdata,int size,BITMAPINFOHEADER* pbitmapinfo);

class SampleListener
{
public:
	virtual void OnSampleArrived(BYTE*pdata,long size)=0;
	virtual void OnFormatChanges(BITMAPINFOHEADER* pbmpinfo)=0;
};

namespace Video
{
	class CVideoCapture;

	class SampleCallback:public ISampleGrabberCB
	{
	
//		SampleCallbackFunc m_pCallback;
		DWORD m_delay ;
		DWORD m_first_time;
		SampleListener* m_plistener;
		CVideoCapture* m_VideoCapture;
	public:
		
		SampleCallback();
		HRESULT STDMETHODCALLTYPE SampleCB(double SampleTime, IMediaSample *pSample);
		HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime, BYTE *pBuffer,long BufferLen);
		HRESULT STDMETHODCALLTYPE QueryInterface( 
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

		virtual ULONG STDMETHODCALLTYPE AddRef( void);

		virtual ULONG STDMETHODCALLTYPE Release( void);
		void SetDelay();
		void SetListener(SampleListener*,CVideoCapture*);
	};

	class CVideoCapture
	{
	private:
		
		bool m_isLive;
		bool m_isInPreviewMode;
		HRESULT m_hResult;
		IBaseFilter *m_pCap ;
		IGraphBuilder* m_pGraph;
		ICaptureGraphBuilder2* m_pBuilder;
		vector<IMoniker*>* devices;
		vector<AM_MEDIA_TYPE*>* m_vFormats;
		IMediaControl * m_pMCtrl;
		IMediaEvent* m_pMEvent;
		IVideoWindow* m_pVWnd;
		ISampleGrabber* m_pSGrab;
		IAMVideoProcAmp* m_pProcAmp ;
		IAMStreamConfig *m_pConfig;
		IBaseFilter* m_pNullRenderer;
		AM_MEDIA_TYPE* m_CurrentFormat;
		SampleCallback m_ISampleCB;

		FormatValues m_fv_max,m_fv_min,m_fv_vals,m_fv_def;
		CRITICAL_SECTION m_CriticalSection;
	private://helpers
		HRESULT InitCaptureGraphBuilder();
		void FreeMediaType(AM_MEDIA_TYPE& mt);
	public:
		CVideoCapture();
		HRESULT GetCaptureDevices(EnumFunc,void*);
		HRESULT CaptureFromDevice(UINT);
		HRESULT CaptureFromFile(LPTSTR);
		HRESULT SetPreviewWindow(HWND hwnd);
		HRESULT Play(SampleListener* plistener);
		HRESULT GetFormat(BITMAPINFOHEADER*);
		HRESULT Stop();
		HRESULT GetFormatStructs(FormatValues *fmin,FormatValues*fmax,FormatValues *fvals);
		HRESULT SetFormatStructs(FormatValues* formats);
		HRESULT ReleaseDevice();
		HRESULT GetStreamFormats(EnumFunc func_to_call,void* userdata);
		HRESULT SetStreamFormats(int index);
		HRESULT SetStreamFormats(AM_MEDIA_TYPE* pformat);
		HRESULT GetStatus();
		double GetSourceFramerate();
		double GetActualFramerate();
		HRESULT	SetSourceFramerate(double nframes_per_second);
		HRESULT GetPreviewMode(bool* isPreview);
		HRESULT GetCaptureMode(bool* isLiveFromCam);
	};
}
