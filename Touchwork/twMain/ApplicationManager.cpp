#include "stdafx.h"
#include "VideoDlg.h"

HWND ApplicationManager::txtConsole = 0;
INIT_SINGLETON(ApplicationManager)

void ApplicationManager::WriteLine(TCHAR *szFormat, ...)
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

void ApplicationManager::SetTextConsoleHandle(HWND handle)
{
	txtConsole = handle;
}

void ApplicationManager::InitializeApplication()
{

	ifstream fin("config.txt");
	fin>>m_settings;
	ostringstream sout;
	sout<<m_settings;
	MessageBoxA(0,sout.str().c_str(),0,0);
	m_twInput = TwInput::getInstancePtr();
	m_twAgent = TwAgent::getInstancePtr();
	m_twTracker = TwTracker::getInstancePtr();
	//m_WindowsMessagesMgr = WindowsMessagesManager::getInstancePtr();
	m_blobTracker = m_twTracker->GetBlobTracker();

	m_twInput->Start(this);
}


void ApplicationManager::OnSampleArrived(BYTE* pdata,long size)
{
	//UpdateFramerates();
	
	//select one or more of the next
	//DebugBreak();			
	//Sleep(100);	//emulate slow/fast detection/tracking/agent
	OnFrame(pdata,size);
}

void ApplicationManager::OnFrame(BYTE* pdata,int size)
{

	//-----detect
	BlobDetector* myblobDetector = new BlobDetector(&m_bmpinfo);
	
	
	WriteLine(L"rect: %d %d\r\n",m_bmpinfo.biWidth,
		m_bmpinfo.biHeight
		);

	//thats copying a list!!!!!!!!!! 
//	list<Blob> blobList = myblobDetector->DetectBlobs(pdata);
	
	myblobDetector->InitializeBitmap(pdata);

	return;

	//-----track
//	int k = m_blobTracker->UpdateBlobs( &blobList ); // dah fine .. its a pointer .. no copy constructors called

	//------agent ... act ?
		
//	m_twAgent->RaiseEvents(m_blobTracker->currentBlobs,m_blobTracker->deletedBlobs);				
//	WriteLine(L"Number of Blobs: %d\r\n",m_blobTracker->currentBlobs.size());
/*
	WriteLine(L"IDs ");
	list<Blob>::iterator itr;
	for(itr = m_blobTracker->currentBlobs.begin() ; itr != m_blobTracker->currentBlobs.end(); itr++ )
	{
		WriteLine(L" - %d %d", itr->m_id, BlobDetector::s_Brightness);
	}
*/
	


	
}

void ApplicationManager::UpdateFramerates()
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

void ApplicationManager::OnFormatChanges( BITMAPINFOHEADER* pbmpinfo )
{
	SampleListener::OnFormatChanges(pbmpinfo);
	if(m_bmpinfo.biCompression == BI_RGB && m_bmpinfo.biBitCount == 24)
	{
		TwDetector::getInstance().SetFormat(&m_bmpinfo);
		ApplicationManager::WriteLine(TEXT("blob detector initialized\r\n"));
	}
	else
		MessageBox(0,TEXT("this format is not Uncompressed RGB-24, choose another video"),0,0);

	OnScreenSizeChanges();
}

void ApplicationManager::OnScreenSizeChanges()
{
	//get camera size , and screen size and set the Agent mapping
	
	float xCamera = m_bmpinfo.biWidth;
	float yCamera = m_bmpinfo.biHeight;
	float xScreen = GetSystemMetrics(SM_CXSCREEN);
	float yScreen = GetSystemMetrics(SM_CYSCREEN);

		
	m_twAgent->m_xScreenPerCamera = xScreen / xCamera;
	m_twAgent->m_yScreenPerCamera = yScreen / yCamera;

}