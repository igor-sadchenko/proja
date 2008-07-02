#include "stdafx.h"
#include "VideoDlg.h"

HWND ModuleManager::txtConsole = 0;
INIT_SINGLETON(ModuleManager)

void ModuleManager::WriteLine(TCHAR *szFormat, ...)
{
	TCHAR   szBuffer [1024] ;
	va_list pArgList ;

	va_start (pArgList, szFormat) ;
	wvsprintf (szBuffer, szFormat, pArgList) ;
	va_end (pArgList) ;

	::SendMessage (txtConsole, EM_SETSEL, (WPARAM) -1, (LPARAM) -1) ;
	::SendMessage (txtConsole, EM_REPLACESEL, FALSE, (LPARAM) szBuffer) ;
	::SendMessage (txtConsole, EM_SCROLLCARET, 0, 0) ;
	CEdit ctrl(txtConsole);
	if(ctrl.GetLineCount() > 200)
		ctrl.Clear();

}

void ModuleManager::SetTextConsoleHandle(HWND handle)
{
	txtConsole = handle;
}

void ModuleManager::InitializeApplication()
{

	LoadSettings();
// 	m_twInput = TwInput::getInstancePtr();
// 	
// 	m_twAgent = TwAgent::getInstancePtr();
// 	m_twTracker = TwTracker::getInstancePtr();
// 	m_twDetector = TwDetector::getInstancePtr();
// 	
	//m_twAgent->InitializeHookDll() ;
	m_twTracker.Initialize();
	m_twInput.Start(this);
}


void ModuleManager::OnSampleArrived(BYTE* pdata,long size)
{
	//UpdateFramerates();
	OnFrame(pdata,size);
}

void ModuleManager::OnFrame(BYTE* pdata,int size)
{
	list<twBlob>  blobList;
	//-----detect
	m_twDetector.Detect(&m_bmpinfo, pdata, blobList);
	
	//-----track
	int k = m_twTracker.Track(&blobList );
	
	//------agent 
	m_twAgent.RaiseEvents(m_twTracker.GetCurrentBlobs(),m_twTracker.GetDeletedBlobs());		

	DisplayDetectionResults();
}

void ModuleManager::UpdateFramerates()
{
	static float lasttime = 0.0f;
	static float timespan = 0;

	float now = ((float)GetTickCount())/1000.0f;
	float framerate = 1.0f /(now-lasttime);

	timespan += now-lasttime;
	lasttime = now;

	if(timespan >= 1.0f)
	{
		timespan = 0;
		wostringstream str;
		str<<setiosflags( ios::fixed )<< framerate ;//(float)TwInput::getInstance().m_video.GetSourceFramerate();
		CVideoDlg::getInstance().m_CamFrameRate.SetWindowText(str.str().c_str());
		wostringstream str2;
		str2<<setiosflags( ios::fixed )<< (float)TwInput::getInstance().m_video.GetActualFramerate();
		CVideoDlg::getInstance().m_actualFrameRate.SetWindowText(str2.str().c_str());

	}	
}

void ModuleManager::OnFormatChanges( BITMAPINFOHEADER* pbmpinfo )
{
	SampleListener::OnFormatChanges(pbmpinfo);
	if(m_bmpinfo.biCompression == BI_RGB && m_bmpinfo.biBitCount == 24)
	{
		TwDetector::getInstance().SetFormat(&m_bmpinfo);
		ModuleManager::WriteLine(TEXT("blob detector initialized\r\n"));
	}
	else
		MessageBox(0,TEXT("this format is not Uncompressed RGB-24, choose another video"),0,0);

	OnScreenSizeChanges();
}

void ModuleManager::OnScreenSizeChanges()
{
	//get camera size , and screen size and set the Agent mapping
	
	m_twAgent.m_xCamera = m_bmpinfo.biWidth;
	m_twAgent.m_yCamera = m_bmpinfo.biHeight;
	m_twAgent.m_xScreen = GetSystemMetrics(SM_CXSCREEN);
	m_twAgent.m_yScreen = GetSystemMetrics(SM_CYSCREEN);

	m_twAgent.m_xCrop = m_twSettings.m_Crop.right - m_twSettings.m_Crop.left;
	m_twAgent.m_yCrop = m_twSettings.m_Crop.bottom - m_twSettings.m_Crop.top;
		
 	m_twAgent.m_xScreenPerCamera = m_twAgent.m_xScreen / m_twAgent.m_xCamera;
 	m_twAgent.m_yScreenPerCamera = m_twAgent.m_yScreen / m_twAgent.m_yCamera;
	m_twAgent.m_xScreenPerCrop = m_twAgent.m_xScreen / m_twAgent.m_xCrop ;
	m_twAgent.m_yScreenPerCrop = m_twAgent.m_yScreen / m_twAgent.m_yCrop ;

}

void ModuleManager::LoadSettings()
{
	ifstream fin("config.txt");
	fin>>m_twSettings;
	ostringstream sout;
	sout<<m_twSettings;
	//MessageBoxA(0,sout.str().c_str(),0,0);

}

TwSettings& ModuleManager::getSettings()
{
	return ModuleManager::getInstance().m_twSettings;
}

void ModuleManager::DisplayDetectionResults()
{
	//WriteLine(L"Number of Blobs: %d\r\n",m_blobTracker->currentBlobs.size());
	
	WriteLine(L"\r\nIDs ");
	list<twBlob>::iterator itr;
	for(itr = m_twTracker.GetCurrentBlobs().begin() ; itr != m_twTracker.GetCurrentBlobs().end(); itr++ )
	{
	WriteLine(L" - %d", itr->m_id);
	}
	//*/

}