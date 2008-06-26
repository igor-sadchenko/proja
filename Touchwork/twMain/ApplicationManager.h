#pragma once
// Singleton
class ApplicationManager:public SampleListener
{
	SINGLETON(ApplicationManager)
public:
	
	// Text Monitor
	static HWND txtConsole;
private:
	
	//WindowsMessagesManager* m_WindowsMessagesMgr;
	TwInput* m_twInput;
	TwAgent* m_twAgent;
	TwTracker* m_twTracker;
	BlobTracker* m_blobTracker;
public:
	TwSettings m_settings;
	


public:
	// Text Monitor
	static void WriteLine(TCHAR * szFormat, ...);
	static void SetTextConsoleHandle(HWND handle);
	//static void SetApplicationDlg(----);
	void OnSampleArrived(BYTE* pdata,long size);
	void OnFormatChanges(BITMAPINFOHEADER* pbmpinfo);
	void OnFrame(BYTE* pdata,int size);
	void InitializeApplication();
	void UpdateFramerates();
	void OnScreenSizeChanges();
	
};

