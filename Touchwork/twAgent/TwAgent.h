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
	class TwAgent:public Singleton<TwAgent>
{
//SINGLETON(TwAgent)
public: 
	TwAgent():Singleton<TwAgent>()
	{
		InitializeWindowsMessages()  ; 
	} ; 

public:
	HHOOK m_hook ;

	float m_xCamera;
	float m_yCamera;
	
	float m_xCrop;
	float m_yCrop;

	float m_xScreen;
	float m_yScreen;


	UINT WM_TOUCH_UP ; 
	UINT WM_TOUCH_DOWN;
	UINT WM_TOUCH_MOVE;

	float m_xScreenPerCamera;
	float m_yScreenPerCamera;
	float m_xScreenPerCrop;
	float m_yScreenPerCrop;

	~TwAgent()
	{
		UnhookWindowsHookEx(m_hook);
	};
	
	void RaiseEvents(list<twBlob> &currentBlobs ,list<twBlob>& deletedBlobs);
	void NotifyWindowUnderBlob(twBlob&,UINT messageType);
	void CameraToScreen(POINT&);
	void UpdateScreenMetrics();
	int InitializeHookDll() ; 
	void InitializeWindowsMessages();
	bool ClipPoint(POINT &);
	void FlipPoint(POINT& pt);
	void DrawScreenPoint(POINT pt);
	void CropAreaToScreen( POINT& pt);
};