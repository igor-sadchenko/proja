#include <windows.h>
#include <atlbase.h>
#include <atlwin.h>
#include <string>
#include <sstream>
#include <set>
#include <map>
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
map<HWND, POINT> previousClick;

//For Double Click
DWORD  s_dwLastLTouchUp = ULONG_MAX;
DWORD  s_dwDblClickMsecs = GetDoubleClickTime();
HWND   s_hwndLastWindowHandle = 0;
 
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
	return CallNextHookEx(s_hook ,nCode, wParam, lParam);
}

void PostWindowMessages(MSG* msg, HWND topLevelWindow)
{
	//Perform a hit test at the current position
	LRESULT hittest = DefWindowProc(msg->hwnd, WM_NCHITTEST, msg->wParam, msg->lParam);

	//If the current touch is NOT on the title bar & message is either Down or Up.. then discard (use for resizing)
	if( msg->message == WM_TOUCH_DOWN )
	{
		if(hittest == HTCAPTION)
			PostMessage(msg->hwnd, WM_NCLBUTTONDOWN, msg->wParam, msg->lParam);
		else
		{
			previousClick[topLevelWindow].x = LOWORD(msg->lParam);
			previousClick[topLevelWindow].y = HIWORD(msg->lParam);
		}
	}
	else if( msg->message == WM_TOUCH_UP ) 
	{
		if(hittest == HTCAPTION)
			PostMessage(msg->hwnd, WM_NCLBUTTONUP, msg->wParam, msg->lParam);
		else
		{
			previousClick.erase(topLevelWindow);
		}
	}
	else if( msg->message == WM_TOUCH_MOVE )
	{
		if(hittest == HTCAPTION)	//Moving Window
			PostMessage(msg->hwnd ,WM_NCMOUSEMOVE , msg->wParam, msg->lParam);
		else						//Resizing Window 
		{
			RECT rect;
			GetWindowRect(topLevelWindow, &rect);
			int width  = rect.right  - rect.left;
			int height = rect.bottom - rect.top;

			POINT previousTouch = previousClick[topLevelWindow];
			long dx = previousTouch.x - LOWORD(msg->lParam);
			long dy = previousTouch.y - HIWORD(msg->lParam);

			width = width - dx;
			height = height - dy;

			SetWindowPos(topLevelWindow, NULL, LOWORD(rect.top), LOWORD(rect.top), width, height, FALSE);
			previousClick[topLevelWindow].x = LOWORD(msg->lParam);
			previousClick[topLevelWindow].y = HIWORD(msg->lParam);
		}
	}
}

void PostMouseMessages(MSG* msg)
{
	//Activate the window first, if it is deactivated
	//HWND active = GetActiveWindow();
	//if(active != msg->hwnd)
		//SetActiveWindow(msg->hwnd);

	//For Double Click
	BOOL bDoubleClick = FALSE;
	DWORD dwTick = 0;

	//Post Messages
	if( msg->message ==  WM_TOUCH_DOWN )
	{
		PostMessage(msg->hwnd, WM_LBUTTONDOWN, msg->wParam, msg->lParam);
	}
	else if( msg->message == WM_TOUCH_UP ) 
	{
		dwTick = GetTickCount();
		bDoubleClick = ((dwTick - s_dwLastLTouchUp) <= s_dwDblClickMsecs);

		if(bDoubleClick && msg->hwnd == s_hwndLastWindowHandle)						//In case the user made two successive "Up"s
		{																			//NOTE: Not sure if this should work right with multi-touch or not, tested on mouse.
			PostMessage(msg->hwnd, WM_LBUTTONDBLCLK , msg->wParam, msg->lParam);	//I am not sure wt is the "time" member, but we can use it instead of GetTickCount()
		}		
		else
		{
			PostMessage(msg->hwnd, WM_LBUTTONUP, msg->wParam, msg->lParam);
		}
		s_dwLastLTouchUp = dwTick;
		s_hwndLastWindowHandle = msg->hwnd;
	}
	else if( msg->message == WM_TOUCH_MOVE )
	{
		PostMessage(msg->hwnd, WM_MOUSEMOVE, msg->wParam, msg->lParam);
	}
}

LIB LRESULT CALLBACK TWGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam )
{
	if(nCode >= 0 )
	{
		set<HWND>::iterator iter;
		bool windowMode = true;

		MSG* msg = (MSG*) lParam ;
		HWND topLevel = GetAncestor(msg->hwnd, GA_ROOTOWNER ); //MODIFIED: GA_ROOTOWNER not GA_ROOT

		if(msg->hwnd == topLevel)
		{
			//Hit Test
			LRESULT hittest = DefWindowProc(msg->hwnd, WM_NCHITTEST, msg->wParam, msg->lParam);
			if(hittest == HTCAPTION)
			{
				if(msg->message == WM_TOUCH_DOWN || msg->message == WM_TOUCH_MOVE)
				{
					windowMode = true;
					//iter = selectedHandles.find(msg->hwnd);	//MODIFIED : No need for checking as we are using a set
					//if(iter == selectedHandles.end())			//if handle wasn't stored before
					selectedHandles.insert(msg->hwnd);			//add hwd it to the set
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
			if(iter != selectedHandles.end())					//topLevel Hwnd found in the selectedHandles
				windowMode = true;								//another finger is on the title bar
			else
				windowMode = false;
		}

		if(windowMode)
			PostWindowMessages(msg, topLevel);
		else
			PostMouseMessages(msg);

		/*Pseudocode
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
		*/
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