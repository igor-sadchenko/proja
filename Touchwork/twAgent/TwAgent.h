// Headers for TwAgent Classes ..
#include<stdafx.h>
#include <list>
using namespace std;


#define LIB extern "C" __declspec(dllimport)
	LIB LRESULT CALLBACK TWCallWndProc(int nCode,WPARAM wParam,LPARAM lParam );
	LIB LRESULT CALLBACK TWGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam );
	LIB void SetMasterWindow(HHOOK hook);

	LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


// Singleton
class TwAgent
{
//SINGLETON(TwAgent)
private: 
	static TwAgent m_instance;
	TwAgent()
	{
		InitializeWindowsMessages()  ; 
	} ; 
public: 
	static TwAgent& getInstance(void)
	{return m_instance;}
	static TwAgent* getInstancePtr(void)
	{return &m_instance;}

public:
	HHOOK hook2 ;

	UINT WM_TOUCH_UP ; 
	UINT WM_TOUCH_DOWN;
	UINT WM_TOUCH_MOVE;

	float m_xScreenPerCamera;
	float m_yScreenPerCamera;

	~TwAgent()
	{
		UnhookWindowsHookEx(hook2);
	};
	
	void RaiseEvents(list<Blob> &currentBlobs ,list<Blob>& deletedBlobs);
	void NotifyWindowUnderBlob(Blob&,UINT messageType);
	void CameraToScreen(POINT&);
	void UpdateScreenMetrics();
	int InitializeHookDll() ; 
	void InitializeWindowsMessages();

};