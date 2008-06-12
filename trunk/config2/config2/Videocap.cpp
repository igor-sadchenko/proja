#include "stdafx.h"
#include "AtlBase.h"
#include <math.h>
#include <sstream>	
using namespace std;
using namespace Video;

HRESULT CVideoCapture::GetCaptureDevices( EnumFunc functoCall,void* userdata)
{
	EnterCriticalSection(&m_CriticalSection);
	ICreateDevEnum *pDevEnum = NULL;
	IEnumMoniker *pEnum = NULL;

	// Create the System Device Enumerator.
	m_hResult = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
		reinterpret_cast<void**>(&pDevEnum));
	if (FAILED(m_hResult))
		return m_hResult;
	
	// Create an enumerator for the video capture category.
	m_hResult = pDevEnum->CreateClassEnumerator(
		CLSID_VideoInputDeviceCategory,
		&pEnum, 0);
	
	if (m_hResult == S_FALSE)
		m_hResult = E_FAIL;
	if (FAILED(m_hResult))
		return m_hResult;

	IMoniker *pMoniker = NULL;
	bool bcont = true;
	if(devices != NULL)
	{
		devices->clear();
	}
	else
		devices = new vector<IMoniker*>();
	HRESULT hr;
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK  )
	{
		devices->push_back(pMoniker);
		IPropertyBag *pPropBag;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
			(void**)(&pPropBag));
		if (FAILED(hr))
		{
			pMoniker->Release();
			continue;  // Skip this one, maybe the next one will work.
		} 
		// Find the description or friendly name.
		VARIANT varName;
		VariantInit(&varName);
		hr = pPropBag->Read(L"Description", &varName, 0);
		if (FAILED(hr))
		{
			hr = pPropBag->Read(L"FriendlyName", &varName, 0);
		}
		if (SUCCEEDED(hr))
		{
			// Add it to the application's list box.
			USES_CONVERSION;
			if(functoCall !=NULL && bcont)
				bcont = functoCall(0,OLE2T(varName.bstrVal),userdata);
			VariantClear(&varName); 
		}
		pPropBag->Release();
		pMoniker->Release();
	}
	LeaveCriticalSection(&m_CriticalSection);
	return hr;
}

Video::CVideoCapture::CVideoCapture():m_hResult(E_FAIL)
{
	m_isInPreviewMode = false;
	m_filter_count = 0;
	m_last_out_pin = NULL;


	CoUninitialize();
	InitializeCriticalSection(&m_CriticalSection);
}


HRESULT Video::CVideoCapture::SetPreviewWindow(HWND hwnd)
{
	if(FAILED(m_hResult))
		return m_hResult;

	RECT rc;

	HRESULT hr;

	// Set the video window to be a child of the main window
	hr = m_pVWnd->put_Owner((OAHWND)hwnd);

	if (FAILED(hr))
		return hr;

	hr = m_pVWnd->put_MessageDrain((OAHWND)hwnd);
	if (FAILED(hr))
		return hr;

	// Set video window style
	hr = m_pVWnd->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
	if (FAILED(hr))
		return hr;

	//position video window in client rect of main application window
	GetClientRect(hwnd, &rc);
	
	m_pVWnd->SetWindowPosition(0, 0, rc.right, rc.bottom);

	// Make the video window visible, now that it is properly positioned
	hr = m_pVWnd->put_Visible(OATRUE);
	if (FAILED(hr))
		return hr;

	m_isInPreviewMode= true;

	return hr;
}

HRESULT  Video::CVideoCapture::CaptureFromDevice( UINT index )
{
	m_hResult = InitCaptureGraphBuilder();
	if(FAILED(m_hResult))
	{
		return m_hResult;
	}
	if(devices == NULL) //not filled .. user never called get devices call it
		m_hResult = GetCaptureDevices(NULL,NULL);
	if(FAILED(m_hResult))
	{
		return m_hResult;
	}
		
	if(devices->size() < index + 1)
		return E_FAIL;
		
	IMoniker* pMoniker = (*devices)[index];
	HRESULT m_hResult = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&m_pCap);
	if (FAILED(m_hResult))
	{
		return m_hResult;
	}
	
	if (FAILED(m_pCap->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp)))
	{
		m_pProcAmp = NULL;
	}

	if (SUCCEEDED(m_hResult))
	{
		m_hResult = m_pGraph->AddFilter(m_pCap, L"Capture Filter");

		if (FAILED(m_hResult))
		{
			return m_hResult;
		}
		m_hResult = m_pBuilder->FindPin(m_pCap,PIN_DIRECTION::PINDIR_OUTPUT,&PIN_CATEGORY_CAPTURE,NULL,FALSE,0,&m_last_out_pin);
		if(FAILED(m_hResult))
			return m_hResult;

		IBaseFilter *p_grabber_base;
		
		m_hResult = m_pSGrab->QueryInterface(IID_IBaseFilter,(LPVOID*)&p_grabber_base);
		if (FAILED(m_hResult))
		{
			return m_hResult;
		}
/*
		IBaseFilter *p_renderer_base;
		m_hResult = m_pVWnd->QueryInterface(IID_IBaseFilter,(LPVOID*)&p_renderer_base);
		if (FAILED(m_hResult))
		{
			return m_hResult;
		}
//*/
		GetStreamFormats(NULL,NULL);
		if (FAILED(SetStreamFormats(-1)))
		{
			//MessageBox(0,L"The Device does not support any RGB-24 media format",0,0);
			AM_MEDIA_TYPE format = {0};
			format.majortype = MEDIATYPE_Video;
			format.subtype = MEDIASUBTYPE_RGB24;
			m_pSGrab->SetMediaType(&format);
		//SetStreamFormats(0);
		//	m_hResult = E_FAIL;
		//	return m_hResult;
		}
		else
		{
			m_pSGrab->SetMediaType(m_CurrentFormat);
		}	
		IPin* pInPin;
		HRESULT hr = m_pBuilder->FindPin(m_pSGrab,PIN_DIRECTION::PINDIR_INPUT,NULL,NULL,FALSE,0,&pInPin);
		if(FAILED(hr))
			return hr;

		hr = m_pGraph->Connect(m_last_out_pin,pInPin);
		if(FAILED(hr))
			return hr;

		hr = m_pBuilder->FindPin(m_pSGrab,PIN_DIRECTION::PINDIR_OUTPUT,NULL,NULL,FALSE,0,&m_last_out_pin);
		if(FAILED(hr))
			return hr;

		//m_hResult = m_pBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pCap,p_grabber_base , NULL/*p_renderer_base*/);
		m_isLive = true;

	}
	return m_hResult;
}

HRESULT Video::CVideoCapture::CaptureFromFile( LPTSTR fname)
{
	IMediaEvent   *pEvent = NULL;

	// Initialize the COM library.
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		return hr;
	}
	m_hResult = InitCaptureGraphBuilder();
	if(FAILED(m_hResult))
	{
		return m_hResult;
	}

	hr = m_pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

	// Build the graph. IMPORTANT: Change this string to a file on your system.
	hr = m_pGraph->RenderFile(fname, NULL);
	
	m_isLive = false;
	return S_OK;
}

HRESULT Video::CVideoCapture::Play(SampleListener* plistener)
{
	if(FAILED(m_hResult))
		return m_hResult;

	//SetSourceFramerate(60);
	m_ISampleCB.SetListener(plistener,this);
	m_ISampleCB.SetDelay();
	m_pSGrab->SetCallback(&m_ISampleCB,0);

	return m_pMCtrl->Run();
}

HRESULT Video::CVideoCapture::Play()
{
	if(FAILED(m_hResult))
		return m_hResult;

	if(m_last_out_pin != NULL)
	{
		m_hResult = AppendSink();
		if(FAILED(m_hResult))
			return m_hResult;

	}
	return m_pMCtrl->Run();
}
HRESULT Video::CVideoCapture::GetFormat(BITMAPINFOHEADER* binfo)
{
	AM_MEDIA_TYPE mtype;
	HRESULT hr = m_pSGrab->GetConnectedMediaType(&mtype);

	if(SUCCEEDED(hr) && mtype.formattype== FORMAT_VideoInfo&& mtype.cbFormat >= sizeof(VIDEOINFOHEADER))
	{
		VIDEOINFOHEADER* vi=(VIDEOINFOHEADER*)mtype.pbFormat;
		*binfo = vi->bmiHeader;
	}
	else 
	{
		hr = E_FAIL;
		::MessageBox(0,L"getformat failed",0,0);
	}
	return hr;
}

void GetSubtybeName(GUID& type,wostringstream & strDescription)
{
#define STRING_CASE(X,Y) if(type== X)  strDescription<<Y;
		STRING_CASE(MEDIASUBTYPE_None,"--No Subtype--")
		STRING_CASE(MEDIASUBTYPE_RGB1, "RGB, 1 bit per pixel (bpp), palettized")
		STRING_CASE(MEDIASUBTYPE_RGB4,  "RGB, 4 bpp, palettized")
		STRING_CASE(MEDIASUBTYPE_RGB8, "RGB, 8 bpp")
		STRING_CASE(MEDIASUBTYPE_RGB555, "RGB 555, 16 bpp")
		STRING_CASE(MEDIASUBTYPE_RGB565,  "RGB 565, 16 bpp")
		STRING_CASE(MEDIASUBTYPE_RGB24, "RGB, 24 bpp")
		STRING_CASE(MEDIASUBTYPE_RGB32, "RGB, 32 bpp")
		STRING_CASE(MEDIASUBTYPE_ARGB1555, "RGB 555 with alpha channel")
		STRING_CASE(MEDIASUBTYPE_ARGB32, "RGB 32 with alpha channel")
		STRING_CASE(MEDIASUBTYPE_ARGB4444, "16-bit RGB with alpha channel; 4 bits per channel")
		STRING_CASE(MEDIASUBTYPE_A2R10G10B10, "32-bit RGB with alpha channel; 10 bits per RGB channel plus 2 bits for alpha")
		STRING_CASE(MEDIASUBTYPE_A2B10G10R10, "32-bit RGB with alpha channel; 10 bits per RGB channel plus 2 bits for alpha")
		STRING_CASE(MEDIASUBTYPE_AYUV, "4:4:4 YUV format");
		STRING_CASE(MEDIASUBTYPE_UYVY , "UYVY (packed 4:2:2) ");
		STRING_CASE(MEDIASUBTYPE_Y411  ,"Same as Y41P ");
		STRING_CASE(MEDIASUBTYPE_Y41P  ,"Y41P (packed 4:1:1) ");
		STRING_CASE(MEDIASUBTYPE_Y211  ,"Y211 ");
		STRING_CASE(MEDIASUBTYPE_YUY2  ,"YUY2 (packed 4:2:2) ");
		STRING_CASE(MEDIASUBTYPE_YVYU  ,"YVYU (packed 4:2:2) ");
		STRING_CASE(MEDIASUBTYPE_YUYV ,"Same as YUY2. (Used by Canopus; FOURCC 'YUYV') ");
		STRING_CASE(MEDIASUBTYPE_RGB32_D3D_DX7_RT ,"32-bit RGB render target.");
		STRING_CASE(MEDIASUBTYPE_RGB16_D3D_DX7_RT ,"16-bit RGB render target.");
		STRING_CASE(MEDIASUBTYPE_ARGB32_D3D_DX7_RT ,"32-bit ARGB render target.");
		STRING_CASE(MEDIASUBTYPE_ARGB4444_D3D_DX7_RT ,"ARGB4444 render target. For subpicture graphics.");
		STRING_CASE(MEDIASUBTYPE_ARGB1555_D3D_DX7_RT ,"ARGB1555 render target. For subpicture graphics.");
		STRING_CASE(MEDIASUBTYPE_RGB32_D3D_DX9_RT ,"32-bit RGB render target.");
		STRING_CASE(MEDIASUBTYPE_RGB16_D3D_DX9_RT ,"16-bit RGB render target.");
		STRING_CASE(MEDIASUBTYPE_ARGB32_D3D_DX9_RT ,"32-bit ARGB render target.");
		STRING_CASE(MEDIASUBTYPE_ARGB4444_D3D_DX9_RT ,"ARGB4444 render target. For subpicture graphics.");
		STRING_CASE(MEDIASUBTYPE_ARGB1555_D3D_DX9_RT ,"ARGB1555 render target. For subpicture graphics.");
//
//		STRING_CASE(MEDIASUBTYPE_CFCC ,"MJPG format produced by some cards. (FOURCC 'CFCC') ");
//		STRING_CASE(MEDIASUBTYPE_CLJR ,"Cirrus Logic CLJR format. (FOURCC 'CLJR') ");
//		STRING_CASE(MEDIASUBTYPE_CPLA ,"Cinepak UYVY format. (FOURCC 'CPLA') ");
//		STRING_CASE(MEDIASUBTYPE_CLPL ,"A YUV format supported by some Cirrus Logic drivers. (FOURCC 'CLPL') ");
//		STRING_CASE(MEDIASUBTYPE_IJPG ," Intergraph JPEG format. (FOURCC 'IJPG') ");
//		STRING_CASE(MEDIASUBTYPE_MDVF," A DV encoding format. (FOURCC 'MDVF') ");
//		STRING_CASE(MEDIASUBTYPE_MJPG,"  Motion JPEG (MJPG) compressed video. (FOURCC 'MJPG')  ");
//		STRING_CASE(MEDIASUBTYPE_MPEG1Packet  ,"MPEG1 Video Packet.  ");
//		STRING_CASE(MEDIASUBTYPE_MPEG1Payload  ,"MPEG1 Video Payload.  ");
//		STRING_CASE(MEDIASUBTYPE_Overlay  ,"Video delivered using hardware overlay.  ");
//		STRING_CASE(MEDIASUBTYPE_Plum  ,"Plum MJPG format. (FOURCC 'Plum') ");
//		STRING_CASE(MEDIASUBTYPE_QTJpeg ," QuickTime JPEG compressed data. " );
//		STRING_CASE(MEDIASUBTYPE_QTMovie ," Apple® QuickTime® compression. " );
//		STRING_CASE(MEDIASUBTYPE_QTRle ," QuickTime RLE compressed data.  ");
//		STRING_CASE(MEDIASUBTYPE_QTRpza ," QuickTime RPZA compressed data. " );
//		STRING_CASE(MEDIASUBTYPE_QTSmc ," QuickTime SMC compressed data.  ");
//		STRING_CASE(MEDIASUBTYPE_TVMJ ," TrueVision MJPG format. (FOURCC 'TVMJ') ");
////		STRING_CASE(MEDIASUBTYPE_VideoPort ,"Video port data, used with DVD. ");
////		STRING_CASE(MEDIASUBTYPE_VPVBI Video ,"port vertical blanking interval (VBI) data. ");
//		STRING_CASE(MEDIASUBTYPE_VPVideo ,"Video port video data. ");
//		STRING_CASE(MEDIASUBTYPE_WAKE  ,"MJPG format produced by some cards. (FOURCC 'WAKE') ");
//		STRING_CASE(MEDIASUBTYPE_MPEG2_VIDEO ,"MPEG 2");
//		//STRING_CASE();
}

HRESULT Video::CVideoCapture::GetStreamFormats(EnumFunc functoCall,void* userdata)
{		
	if(FAILED(m_hResult))
		return m_hResult;
	HRESULT hr ;
	if(m_pConfig == NULL)
	{
	hr = m_pBuilder->FindInterface(
		&PIN_CATEGORY_CAPTURE, // Preview pin.
		0,    // Any media type.
		m_pCap, // Pointer to the capture filter.
		IID_IAMStreamConfig, (void**)&m_pConfig);
	}
	m_pConfig->GetFormat(&m_CurrentFormat);
	int iCount = 0, iSize = 0;
	hr = m_pConfig->GetNumberOfCapabilities(&iCount, &iSize);
	AM_MEDIA_TYPE *pmtConfig;
	
	if (m_vFormats != NULL)
	{
		m_vFormats->clear();
	}
	else
		m_vFormats = new vector<AM_MEDIA_TYPE *>(iCount);

	// Check the size to make sure we pass in the correct structure.
	if (iSize >= sizeof(VIDEO_STREAM_CONFIG_CAPS))
	{
		
		//Use the video capabilities structure.
		bool bcont = true;
 		for (int iFormat = 0; iFormat < iCount; iFormat++)
		{
			wostringstream strFormatDesc;
			
			VIDEO_STREAM_CONFIG_CAPS scc;
			hr = m_pConfig->GetStreamCaps(iFormat, &pmtConfig, (BYTE*)&scc);
			if (SUCCEEDED(hr))
			{
				GetSubtybeName(pmtConfig->subtype , strFormatDesc);
				//if (strFormatDesc.str().empty())
				//{
				//	FreeMediaType(*pmtConfig);
				//	continue;
				//}

				m_vFormats->push_back(pmtConfig);
				//hr = pConfig->SetFormat(pmtConfig);

				
				if(functoCall !=NULL && bcont)
				{
					bcont = functoCall(0, (LPTSTR)strFormatDesc.str().c_str() ,userdata);
				}
				// Delete the media type when you are done.
				//FreeMediaType(*pmtConfig);
			}
		}

		
	}
	return hr;
}

void Video::CVideoCapture::FreeMediaType(AM_MEDIA_TYPE& mt)
{
	if (mt.cbFormat != 0)
	{
		CoTaskMemFree((PVOID)mt.pbFormat);
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL)
	{
		// Unecessary because pUnk should not be used, but safest.
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}
}

HRESULT Video::CVideoCapture::Stop()
{
	if(FAILED(m_hResult))
		return m_hResult;
	return m_pMCtrl->Stop();
}
	
HRESULT CVideoCapture::InitCaptureGraphBuilder()
{
	if (m_pGraph || m_pBuilder)
	{
		return E_POINTER;
	}

	// Create the Capture Graph Builder.
	HRESULT hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, 
		CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&m_pBuilder );
	if (SUCCEEDED(hr))
	{
		// Create the Filter Graph Manager.
		hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER,
			IID_IGraphBuilder, (void**)&m_pGraph);
		if (SUCCEEDED(hr))
		{
			// Initialize the Capture Graph Builder.
			m_pBuilder->SetFiltergraph(m_pGraph);

			hr = m_pGraph->QueryInterface(IID_IMediaControl,(LPVOID *) &m_pMCtrl);
			if (FAILED(hr))
				return hr;


			hr = m_pGraph->QueryInterface(IID_IMediaEvent, (LPVOID *) &m_pMEvent);
			if (FAILED(hr))
				return hr;

			//get a video renderer
			hr = m_pGraph->QueryInterface(IID_IVideoWindow, (LPVOID *) &m_pVWnd);
			if (FAILED(hr))
			{
				return hr;
			}

			//get a NULL renderer
			IBaseFilter *pGrabberF = NULL;
			hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter, (void**)&pGrabberF);
			if (FAILED(hr))
			{
				return hr;
			}
			hr = m_pGraph->AddFilter(pGrabberF,L"Grabber");
			if(FAILED(hr))
				return hr;
			pGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&m_pSGrab);
/*
			hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter, (void**)&m_pNullRenderer);
			if (FAILED(hr))
			{
				return hr;
			}


			hr = m_pGraph->AddFilter(m_pNullRenderer,L"NullRenderer");
			if(FAILED(hr))
				return hr;
*/
			//hr = m_pMEvent->SetNotifyWindow((OAHWND), WM_GRAPHNOTIFY, 0);

			return S_OK;
		}
		else
		{
			m_pBuilder->Release();
		}

		
	}

	return hr; // Failed
}


HRESULT Video::CVideoCapture::GetFormatStructs(FormatValues *fmin,FormatValues*fmax,FormatValues *fvals)
{	if(FAILED(m_hResult))
		return m_hResult;

	FormatValues temp;
	if (m_pProcAmp)
	{

		if (fmin == NULL && fmax == NULL && fvals == NULL)
			return S_OK;
		if(fmin == NULL)
			fmin =&temp;
		if(fmax == NULL)
			fmax =&temp;
		if(fvals == NULL)
			fvals =&temp;

		long Step, Flags ;

		HRESULT	hr;
		
		for (int iprop = 0; iprop <= VideoProcAmp_Gain; iprop++)
		{
		
			hr = m_pProcAmp->GetRange(iprop, &m_fv_min[iprop], &m_fv_max[iprop], &Step,
				&m_fv_def[iprop], &Flags);

			if (SUCCEEDED(hr))
			{
				// Get the current value.
				hr = m_pProcAmp->Get(iprop, &m_fv_vals[iprop], &Flags);
				if (FAILED(hr) || Flags&VideoProcAmp_Flags_Auto)
				{
					m_fv_min[iprop] = LONG_MIN;
				}
			}
			else
			{
				m_fv_min[iprop] = LONG_MIN;
			}

			*fmin= m_fv_min;
			*fmax= m_fv_max;
			*fvals= m_fv_vals;
		}
		return S_OK;
	}


	return E_FAIL;
}

HRESULT Video::CVideoCapture::SetFormatStructs(FormatValues* formats)
{
	if(FAILED(m_hResult))
		return m_hResult;

	if (m_pProcAmp)
	{
		long Flags= VideoProcAmp_Flags_Manual;
		
		HRESULT	hr ;
		if (formats == NULL)
		{
			formats = &m_fv_def;
		}
		m_fv_vals = *formats;
		for (int iprop = 0; iprop <= VideoProcAmp_Gain; iprop++)
		{
			if (m_fv_min[iprop]!= LONG_MIN)
			{
				// Get the current value.
				hr = m_pProcAmp->Set(iprop, m_fv_vals[iprop], Flags);
			}
		}
		return S_OK;
	}


	return E_FAIL;
}

HRESULT Video::CVideoCapture::ReleaseDevice()
{
	if(m_pMCtrl != NULL)
		m_pMCtrl->Stop();
	if (m_pCap != NULL)
	{
		//clear all device dependent values
		if (m_pProcAmp != NULL)
		{
			m_pProcAmp->Release();
			m_pProcAmp = NULL;
		}
		//remove form the graph
		if(m_pGraph != NULL)
		{
			m_hResult = m_pGraph->RemoveFilter(m_pCap);
			if(FAILED(m_hResult))
				return m_hResult;
		}
		m_pCap->Release();
		m_pCap = NULL;
	}
	return S_OK;
}

HRESULT Video::CVideoCapture::SetStreamFormats( int index )
{
	if(FAILED(m_hResult))
		return m_hResult;

	if (m_vFormats != NULL && SUCCEEDED(GetStreamFormats(NULL,NULL)))
	{
		if(index >= (int)m_vFormats->size() || index < -1)
			return E_INVALIDARG;
		
		if (index == -1)
		{
			for (index =0;index < (int)m_vFormats->size();index++)
			{
				AM_MEDIA_TYPE* pformat = (*m_vFormats)[index];
				if(pformat->subtype == MEDIASUBTYPE_RGB24)
					break;
			}
			if (index >=  (int)m_vFormats->size())
			{
				return E_FAIL;
			}
		}
		else
			if (index >=  (int)m_vFormats->size())
			{
				return E_FAIL;
			}
		
		return SetStreamFormats((*m_vFormats)[index]);
	}
	else return E_FAIL;
}

HRESULT Video::CVideoCapture::SetStreamFormats( AM_MEDIA_TYPE* pformat )
{

	HRESULT hr =m_pConfig->SetFormat(pformat);
	if (SUCCEEDED(hr))
	{
		hr = m_pSGrab->SetMediaType(pformat);
		if(SUCCEEDED(hr))
			m_CurrentFormat = pformat;
	}
	//m_CurrentFormat->pbFormat
	return hr;
}
HRESULT Video::CVideoCapture::GetStatus()
{
	return m_hResult;
}

double Video::CVideoCapture::GetSourceFramerate()
{
	HRESULT hr = GetCaptureMode(NULL);
	if(FAILED(hr))
		return -1.0;

	double timePerFrame ;
	if (true ||hr == S_FALSE) //recorded video from file
	{
		IBasicVideo* p_vid;
		REFTIME avgTime;
		IBaseFilter* pVidoRenderer;
		hr = m_pGraph->FindFilterByName(L"Video Renderer",&pVidoRenderer);
		if(FAILED(hr))
		{
			DebugBreak();
			return -1.0;
		}
		hr = pVidoRenderer->QueryInterface(IID_IBasicVideo,(void**)&p_vid);
		if(FAILED(hr))
		{
			return -1.0;
		}
		
		p_vid->get_AvgTimePerFrame(&avgTime);
		p_vid->Release();
		timePerFrame = (double)avgTime;
	}
	else //hr == S_OK
	{
		VIDEOINFOHEADER* inf = (VIDEOINFOHEADER*) m_CurrentFormat->pbFormat;
		timePerFrame = (double)inf->AvgTimePerFrame / pow(10.0,7);
	}


	return 1.0/timePerFrame; 
}

HRESULT Video::CVideoCapture::SetSourceFramerate( double nframes_per_second )
{
	VIDEOINFOHEADER* inf = (VIDEOINFOHEADER*) m_CurrentFormat->pbFormat;
	inf->AvgTimePerFrame = (REFERENCE_TIME)(nframes_per_second * pow(10.0,9.0) * 100.0); 
	return SetStreamFormats( m_CurrentFormat );
}

double Video::CVideoCapture::GetActualFramerate()
{
	HRESULT hr = GetCaptureMode(NULL);
	if(FAILED(hr))
		return -1.0;

	if (true ||hr == S_FALSE) //recorded video from file
	{
		IQualProp *p_vid;
		int avgTime;

		IBaseFilter* pVidoRenderer;
		hr = m_pGraph->FindFilterByName(L"Video Renderer",&pVidoRenderer);
		if(FAILED(hr))
		{
			DebugBreak();
			return -1.0;
		}
		hr = pVidoRenderer->QueryInterface(IID_IQualProp,(void**)&p_vid);
		if(FAILED(hr))
		{
			return -1.0;
		}
		p_vid->get_AvgFrameRate(&avgTime) ;
		
		p_vid->Release();
		return (double)avgTime/ 100.0;
		
	}
	else //hr == S_OK
	{
		
		return -1.0;
	}
}


HRESULT Video::CVideoCapture::GetPreviewMode( bool* isPreviewed )
{
	if (FAILED(m_hResult))
		return m_hResult;

	if(isPreviewed)
		*isPreviewed = m_isInPreviewMode;
	if(m_isInPreviewMode)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

HRESULT Video::CVideoCapture::GetCaptureMode( bool* isLiveFromCam )
{
	if (FAILED(m_hResult))
		return m_hResult;
	if(isLiveFromCam)
		*isLiveFromCam = m_isLive;
	if(m_isLive)
		return S_OK;
	else
		return S_FALSE;
}

HRESULT Video::CVideoCapture::SetCrop( int x1,int y1,int x2,int y2 )
{
	RECT rcSource = {x1,y1,x2,y2};
	return SetCrop(rcSource);
}

HRESULT Video::CVideoCapture::SetCrop( RECT& rcSource )
{
	GetStreamFormats(NULL,NULL);
	
	VIDEOINFOHEADER* inf = (VIDEOINFOHEADER*) m_CurrentFormat->pbFormat;
	RECT rcTarget = {0,0,0,0};
//  	inf->bmiHeader.biWidth = rcSource.right-rcSource.left;
//  	inf->bmiHeader.biHeight = rcSource.bottom - rcSource.top;
//  	inf->bmiHeader.biSizeImage = DIBSIZE(inf->bmiHeader);

	inf->rcSource = rcSource; 
	inf->rcTarget = rcTarget;

	m_pMCtrl->Stop();
	HRESULT hr = SetStreamFormats( m_CurrentFormat );

	m_pMCtrl->Run();
	return hr;
}

HRESULT Video::CVideoCapture::AppendSampleGrabber( SampleListener* plistener)
{
	int i = m_filter_count;

	IBaseFilter *pGrabberF = NULL;
	HRESULT hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (void**)&pGrabberF);
	if (FAILED(hr))
	{
		return hr;
	}
	wostringstream sout;
	sout<<"Grabber"<<i;
	hr = m_pGraph->AddFilter(pGrabberF,sout.str().c_str());
	if(FAILED(hr))
		return hr;
	ISampleGrabber * pSGrab;
	hr = pGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&pSGrab);
	if(FAILED(hr))
		return hr;
	
	SampleCallback* pISampleCB = new SampleCallback();
	m_sampleCallbacks.push_back(pISampleCB);
	pISampleCB->SetListener(plistener,this);
	pISampleCB->SetDelay();
	pSGrab->SetCallback(pISampleCB,0);
	IPin* pInPin;
	hr = m_pBuilder->FindPin(pSGrab,PIN_DIRECTION::PINDIR_INPUT,NULL,NULL,FALSE,0,&pInPin);
	if(FAILED(hr))
		return hr;

	hr = m_pGraph->Connect(m_last_out_pin,pInPin);
		if(FAILED(hr))
			return hr;

	hr = m_pBuilder->FindPin(pSGrab,PIN_DIRECTION::PINDIR_OUTPUT,NULL,NULL,FALSE,0,&m_last_out_pin);
	if(FAILED(hr))
		return hr;

	m_filter_count++;
	return S_OK;
}

HRESULT Video::CVideoCapture::AppendPreview( HWND hwnd)
{
	int i = m_filter_count;
	wostringstream sout1;
	sout1<<"Tee"<<i;

	//crete a deMUX
	IBaseFilter *pTee = NULL;
	HRESULT hr = CoCreateInstance(CLSID_InfTee, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (void**)&pTee);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_pGraph->AddFilter(pTee,sout1.str().c_str());
	if(FAILED(hr))
		return hr;

	IPin* pInPin;
	hr = m_pBuilder->FindPin(pTee,PIN_DIRECTION::PINDIR_INPUT,NULL,NULL,FALSE,0,&pInPin);
	if(FAILED(hr))
		return hr;

	hr = m_pGraph->Connect(m_last_out_pin,pInPin);
	if(FAILED(hr))
		return hr;


	m_filter_count++;

	//Create a renderer window filter
	i = m_filter_count;
	wostringstream sout2;
	sout1<<"Wnd"<<i;


	IBaseFilter *pRnd = NULL;
	hr = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (void**)&pRnd);
	if (FAILED(hr))
	{
		return hr;
	}
	
	IVideoWindow* pVWnd;
	hr = pRnd->QueryInterface(IID_IVideoWindow, (LPVOID *) &pVWnd);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pGraph->AddFilter(pRnd,sout2.str().c_str());
	if(FAILED(hr))
		return hr;

	hr = m_pBuilder->FindPin(pRnd,PIN_DIRECTION::PINDIR_INPUT,NULL,NULL,FALSE,0,&pInPin);
	if(FAILED(hr))
		return hr;

	hr = m_pGraph->Connect(m_last_out_pin,pInPin);
	if(FAILED(hr))
		return hr;


	hr = m_pBuilder->FindPin(pTee,PIN_DIRECTION::PINDIR_OUTPUT,NULL,NULL,FALSE,0,&m_last_out_pin);
	if(FAILED(hr))
		return hr;

	m_filter_count++;

	//add window info 
	RECT rc;

	hr = pVWnd->put_Owner((OAHWND)hwnd);

	if (FAILED(hr))
		return hr;

	hr = pVWnd->put_MessageDrain((OAHWND)hwnd);
	if (FAILED(hr))
		return hr;

	hr = pVWnd->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
	if (FAILED(hr))
		return hr;

	GetClientRect(hwnd, &rc);

	pVWnd->SetWindowPosition(0, 0, rc.right, rc.bottom);

	// Make the video window visible, now that it is properly positioned
	hr = pVWnd->put_Visible(OATRUE);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Video::CVideoCapture::AppendSink()
{
	if(m_last_out_pin == NULL)
		return E_FAIL;

	IBaseFilter *pNull = NULL;
	HRESULT hr = CoCreateInstance(	CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (void**)&pNull );
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_pGraph->AddFilter(pNull ,L"Sink");
	if(FAILED(hr))
		return hr;

	IPin* pInPin;
	hr = m_pBuilder->FindPin(pNull ,PIN_DIRECTION::PINDIR_INPUT,NULL,NULL,FALSE,0,&pInPin);
	if(FAILED(hr))
		return hr;

	hr = m_pGraph->Connect(m_last_out_pin,pInPin);
	if(FAILED(hr))
		return hr;

	m_last_out_pin = NULL;

	return S_OK;
}