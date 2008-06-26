#include<windows.h>
#include<atlbase.h>
#include <atlwin.h>
#include <string>
#include <sstream>
#include <set>
using namespace std;

#pragma region shared
#pragma data_seg("TWshare")
HWND s_hwndMaster = NULL;
HHOOK s_hook = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:TWshare,rws")
#pragma endregion shared
#define LIB extern "C" __declspec(dllexport)

HINSTANCE g_hinstDll = NULL;

set<HWND> selectedHandles;
DWORD  m_dwLastLButtonDown = ULONG_MAX;
DWORD  m_dwDblClickMsecs   = GetDoubleClickTime();

 
BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, PVOID fImpLoad) {

	switch (fdwReason) {

	  case DLL_PROCESS_ATTACH:
		  // DLL is attaching to the address space of the current process.
		  g_hinstDll = hinstDll;
		  break;

	  case DLL_THREAD_ATTACH:
		  // A new thread is being created in the current process.
		  break;

	  case DLL_THREAD_DETACH:
		  // A thread is exiting cleanly.
		  break;

	  case DLL_PROCESS_DETACH:
		  // The calling process is detaching the DLL from its address space.
		  break;
	}
	return(TRUE);
}


UINT WM_TOUCH_UP;	
UINT WM_TOUCH_DOWN;	
UINT WM_TOUCH_MOVE;	


LIB LRESULT CALLBACK TWCallWndProc(int nCode, WPARAM wParam, LPARAM lParam )
{
	if(nCode >= 0 )
	{
		CWPRETSTRUCT* msg = (CWPRETSTRUCT*) lParam ;
	
		if( msg->message == WM_TOUCH_DOWN )
		{
			MessageBox(0,L"Click",0,0);
			PostMessage(msg->hwnd, WM_LBUTTONDOWN, wParam, lParam);
		}
		else if( msg->message == WM_TOUCH_UP ) 
		{
			MessageBox(0,L"Click",0,0);
			PostMessage(msg->hwnd, WM_LBUTTONUP, wParam, lParam);
		}
		else if( msg->message == WM_TOUCH_MOVE )
		{
			MessageBox(0,L"Click",0,0);
			PostMessage(msg->hwnd,WM_MOUSEMOVE , wParam, lParam);
		}		
	}
	return CallNextHookEx(s_hook ,nCode, wParam, lParam);
}

void PostWindowMessages(MSG* msg)
{
	if( msg->message == WM_TOUCH_DOWN )
	{
		PostMessage(msg->hwnd, WM_LBUTTONDOWN, msg->wParam, msg->lParam);
	}
	else if( msg->message == WM_TOUCH_UP ) 
	{
		PostMessage(msg->hwnd, WM_LBUTTONUP, msg->wParam, msg->lParam);
	}
	else if( msg->message == WM_TOUCH_MOVE )
	{
		PostMessage(msg->hwnd,WM_MOUSEMOVE , msg->wParam, msg->lParam);
	}
}

void PostMouseMessages(MSG* msg)
{
	//Activate the window first
	SetActiveWindow(msg->hwnd);

	//For Double Click
	BOOL bDoubleClick = FALSE;
	DWORD dwTick = 0;

	//Send Messages
	if( msg->message == WM_TOUCH_DOWN )
	{
		PostMessage(msg->hwnd, WM_LBUTTONDOWN, msg->wParam, msg->lParam);
		dwTick = GetTickCount();
		bDoubleClick = ((dwTick - m_dwLastLButtonDown) <= m_dwDblClickMsecs);
        m_dwLastLButtonDown = dwTick;
	}
	else if( msg->message == WM_TOUCH_UP ) 
	{
		PostMessage(msg->hwnd, WM_LBUTTONUP, msg->wParam, msg->lParam);
	}
	else if( msg->message == WM_TOUCH_MOVE )
	{
		PostMessage(msg->hwnd,WM_MOUSEMOVE , msg->wParam, msg->lParam);
	}
	if(bDoubleClick)
		PostMessage(  msg->hwnd, WM_LBUTTONDBLCLK, msg->wParam, msg->lParam);
}

LIB LRESULT CALLBACK TWGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam )
{
	if(nCode >= 0 )
	{
		MSG* msg = (MSG*) lParam ;

		set<HWND>::iterator iter;
		bool windowMode = true;
		HWND topLevel = GetAncestor(msg->hwnd, GA_ROOT); //OR GA_ROOTOWNER, Still not sure which to use

		if(msg->hwnd == topLevel)
		{
			//Hit Test
			LRESULT hittest = DefWindowProc(msg->hwnd, WM_NCHITTEST, msg->wParam, lParam);
			if(hittest == HTCAPTION)
			{
				if(msg->message == WM_TOUCH_DOWN || msg->message == WM_TOUCH_MOVE)
				{
					windowMode = true;
					iter = selectedHandles.find(msg->hwnd);
					if(iter == selectedHandles.end())			//if handle wasn't stored before
						selectedHandles.insert(msg->hwnd);		//add it to the set
				}
				else											//In case of WM_TOUCH_UP
				{
					windowMode = false;
					selectedHandles.erase(msg->hwnd);
				}
			}
			else
			{
				windowMode = false;
			}
		}
		else
		{
			iter = selectedHandles.find(topLevel);
			if(iter != selectedHandles.end())					//toplevel Hwnd found in the selectedHandles
				windowMode = true;
			else
				windowMode = false;
		}

		if(windowMode)
			PostWindowMessages(msg);
		else
			PostMouseMessages(msg);

		//Pseudocode
		//Get TopLevelWindow
		//if( current Handle is a TopLevel )
		//	Perform a Hit Test
		//	if(Hit Test result == HTCAPTION)
		//		if(blobType == WM_TouchDown || blobType == WM_TouchMoveMouseMode )
		//			Store this handle in the SelectedHandles List (Check that this Hwnd doesn't exit before)
		//			Mode = WindowMode
		//		else
		//			Remove that Hwnd if it existed before in the SelectedHandles List
		//			Mode = MouseMode;
		//else
		//	Check the TopLevelWindow in the SelectedHandles list
		//	if( TopLevel found)
		//		Mode = WindowMode;
		//	else
		//		Mode = MouseMode;

		//if(WindowMode)
		//	PostWindowMessages(..)
		//else
		//	PostMouseMessages(..)
	}
	return CallNextHookEx(s_hook ,nCode, wParam, lParam);
}

LIB void SetMasterWindow(HHOOK hook)
{
	s_hook = hook;

	WM_TOUCH_UP = RegisterWindowMessageA( "WM_TOUCH_UP" ) ;
	WM_TOUCH_DOWN = RegisterWindowMessageA("WM_TOUCH_DOWN" ) ; 
	WM_TOUCH_MOVE = RegisterWindowMessageA( "WM_TOUCH_MOVE" ) ; 
}

