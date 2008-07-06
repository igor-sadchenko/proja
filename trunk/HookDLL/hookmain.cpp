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
map<HWND, POINT> selectedHandles;
map<HWND, POINT> previousTouch;

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

POINT GetTouchPoint(HWND hWnd, LPARAM lParam)
{
	POINT currentTouch = POINT();
	currentTouch.x = LOWORD(lParam);
	currentTouch.y = HIWORD(lParam);
	ClientToScreen(hWnd, &currentTouch);
	return currentTouch;
}

void PostWindowMessages(MSG* msg, HWND topLevelWindow)
{
	//Perform a hit test at the current position
	LRESULT hittest = DefWindowProc(msg->hwnd, WM_NCHITTEST, msg->wParam, msg->lParam);

	//If the current touch is NOT on the title bar & message is either Down or Up.. then discard
	if( msg->message == WM_LBUTTONDOWN )






	{
		//if(hittest == HTCAPTION)
		//	PostMessage(msg->hwnd, WM_LBUTTONDOWN, msg->wParam, msg->lParam);
		//else
		{
		//	LPPOINT lpoint = (LPPOINT) msg->lParam;
		//	ClientToScreen(msg->hwnd, lpoint);
		//	previousClick[topLevelWindow] = lpoint;
		}

	}
	else if( msg->message == WM_TOUCH_UP ) 
	{
		//if(hittest == HTCAPTION)
		//	PostMessage(msg->hwnd, WM_LBUTTONUP, msg->wParam, msg->lParam);








	}
	else if(msg->message == WM_MOUSEMOVE || msg->message == WM_NCMOUSEMOVE)		//WM_TOUCH_MOVE )
	{
		RECT rect;
		GetWindowRect(topLevelWindow, &rect);
		int width  = rect.right  - rect.left;
		int height = rect.bottom - rect.top;
		int screenwidth  = GetSystemMetrics(SM_CXSCREEN);
		int screenheight = GetSystemMetrics(SM_CYSCREEN);

		if(hittest == HTCAPTION)															//Moving or Resizing Window


		{
			if(previousTouch.find(topLevelWindow) == previousTouch.end())					//No other touches elsewhere on the window
			{																				//.. then perform an ordinary move
				//PostMessage(topLevelWindow, WM_NCMOUSEMOVE, msg->wParam | WM_NCLBUTTONDOWN,	msg->lParam);
				//selectedHandles[topLevelWindow] = GetTouchPoint(msg->hwnd, msg->lParam);	//Update the titlebar touch position
			}
			else
			{
				/*
				POINT preTitleTouch = selectedHandles[topLevelWindow];
				POINT currentTouch = GetTouchPoint(msg->hwnd, msg->lParam);
				long dx = preTitleTouch.x - currentTouch.x;
				long dy = preTitleTouch.y - currentTouch.y;

				if(currentTouch.x < previousTouch[topLevelWindow].x)
				{
					width = width + dx;
					height = height + dy;	
					rect.left = rect.left - dx;
					rect.top = rect.top - dy;
				}
				else
				{
					width = width - dx;
					height = height + dy;			
					rect.top = rect.top - dy;
				}
				
				//Checking for errors
				if(width < 0)				width = 0;
				if(height < 0)				height = 0;
				if(rect.top < 0)			rect.top = 0;
				if(rect.left< 0)			rect.left = 0;
				if(width > screenwidth)		width = screenwidth;
				if(height > screenheight)	height = screenheight;

				//MoveWindow(topLevelWindow, rect.top, rect.left, width, height, TRUE);
				SetWindowPos(topLevelWindow, NULL, rect.left, rect.top, width, height, FALSE);
				selectedHandles[topLevelWindow] = currentTouch;
				*/
			}
		}
		else																				//Window Resizing
		{
			if(previousTouch.find(topLevelWindow) == previousTouch.end())					//First point inside the window
			{
				POINT firstTouch = GetTouchPoint(msg->hwnd, msg->lParam);					
				previousTouch[topLevelWindow] = firstTouch;									//Save it in previousTouch
			}
			else
			{
				POINT preTouch = previousTouch[topLevelWindow];	
				POINT currentTouch = GetTouchPoint(msg->hwnd, msg->lParam);
				long dx = preTouch.x - currentTouch.x;										//Get difference between current & previous touch
				long dy = preTouch.y - currentTouch.y;

				if(currentTouch.x <= selectedHandles[topLevelWindow].x)
				{
					width = width + dx;
					height = height - dy;	
					rect.left = rect.left - dx;
				}
				else
				{
					width = width - dx;
					height = height - dy;			
				}

				//Checking for errors
				if(width < 0)	width = 0;
				if(height < 0)	height = 0;
				if(width > screenwidth)		width = screenwidth;
				if(height > screenheight)	height = screenheight;

				//Actual Resizing
				//MoveWindow(topLevelWindow, rect.top, rect.left, width, height, TRUE);
				SetWindowPos(topLevelWindow, NULL, rect.left, rect.top, width, height, FALSE);
				previousTouch[topLevelWindow] = currentTouch;								//Set current touch as the previous touch
			}
		}

	}
}

void PostMouseMessages(MSG* msg)
{
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

		if(bDoubleClick && msg->hwnd == s_hwndLastWindowHandle)					//In case the user made two successive "Up"s
		{																		//NOTE: Not sure if this should work right with multi-touch or not
			PostMessage(msg->hwnd, WM_LBUTTONDBLCLK , msg->wParam, msg->lParam);//I am not sure wt is the "time" member, 
																				//but we can use it instead of GetTickCount()
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
		PostMessage(msg->hwnd,WM_MOUSEMOVE , msg->wParam, msg->lParam);
	}
}

void PostNonClientMouseMessages(MSG* msg)
{
	//For Double Click
	BOOL bDoubleClick = FALSE;
	DWORD dwTick = 0;

	//Post Messages
	if( msg->message ==  WM_TOUCH_DOWN )
	{
		PostMessage(msg->hwnd, WM_NCLBUTTONDOWN, msg->wParam, msg->lParam);
	}
	else if( msg->message == WM_TOUCH_UP ) 
	{
		dwTick = GetTickCount();
		bDoubleClick = ((dwTick - s_dwLastLTouchUp) <= s_dwDblClickMsecs);

		if(bDoubleClick && msg->hwnd == s_hwndLastWindowHandle)					
		{																		
			PostMessage(msg->hwnd, WM_NCLBUTTONDBLCLK , msg->wParam, msg->lParam);
		}		
		else
		{
			PostMessage(msg->hwnd, WM_NCLBUTTONUP, msg->wParam, msg->lParam);
		}
		s_dwLastLTouchUp = dwTick;
		s_hwndLastWindowHandle = msg->hwnd;
	}
	else if( msg->message == WM_TOUCH_MOVE )
	{
		PostMessage(msg->hwnd,WM_NCMOUSEMOVE , msg->wParam, msg->lParam);
	}
}

bool NonClient(LRESULT hittest)
{
	if(hittest == HTBORDER || 
		hittest == HTBOTTOM || 
		hittest == HTBOTTOMLEFT || 
		hittest == HTBOTTOMRIGHT || 
		hittest == HTCAPTION || 
		hittest == HTCLIENT || 
		hittest == HTCLOSE ||
		hittest == HTERROR || 
		hittest == HTNOWHERE ||
		hittest == HTGROWBOX ||
		hittest == HTHELP ||
		hittest == HTHSCROLL ||
		hittest == HTLEFT ||
		hittest == HTMENU ||
		hittest == HTMAXBUTTON ||
		hittest == HTMINBUTTON ||
		hittest == HTNOWHERE ||
		hittest == HTREDUCE	||
		hittest == HTRIGHT ||
		hittest == HTSIZE ||
		hittest == HTSYSMENU ||
		hittest == HTTOP ||
		hittest == HTTOPLEFT ||
		hittest == HTTOPRIGHT ||
		hittest == HTTRANSPARENT ||
		hittest == HTVSCROLL ||
		hittest == HTZOOM)
		return true;
	return false;
}

LIB LRESULT CALLBACK TWGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam )
{
	if(nCode >= 0 )
	{
		map<HWND,POINT>::iterator _iter;
		int _mode = 1;												//WindowMessages:0 , MouseMessages:1, NonClientMouseMode:2
		MSG* msg = (MSG*) lParam;

		POINT test = POINT();
		POINT touchSCord = GetTouchPoint(msg->hwnd, msg->lParam);	//Convert client coordinates to screen (& from LPARAM to POINT struct)
		HWND currentHandle = WindowFromPoint(touchSCord);			//Get the handle at the current touch position
		HWND topLevel = GetAncestor(currentHandle, GA_ROOTOWNER );	//Get Top Level Window of the current handle

		LRESULT hittest = DefWindowProc(topLevel, WM_NCHITTEST, msg->wParam, MAKELPARAM(touchSCord.x, touchSCord.y)); //Hit Test
																	//HitTest lparam is in screen coordinates


		if(currentHandle == NULL)									//No Window exits at current point







		{
			_mode = 1;												//Mouse Mode
		}
		else
		{
			if(hittest == HTCAPTION)								//On Title bar
			{
				if(msg->message == WM_TOUCH_DOWN)
				{
					_mode = 0;
					selectedHandles[topLevel] = touchSCord;			//Save Handle & current touch(screen coordinates)

				}
				else if(msg->message == WM_TOUCH_MOVE)
				{
					_mode = 0;






				}
				else if(msg->message == WM_TOUCH_UP)
				{
					_mode = 1;
					selectedHandles.erase(topLevel);
				}
			}
			else if(NonClient(hittest))								//Non Client Area
			{
				_mode = 2;		
			}
			else													//Client Area
			{
				_iter = selectedHandles.find(topLevel);
				if(_iter != selectedHandles.end())					//topLevel Hwnd found in the selectedHandles
					_mode = 0;										//another finger is on the title bar
				else
					_mode = 1;
			}
		}

		/*
		HWND test = WindowFromPoint(GetTouchPoint(msg->hwnd, msg->lParam));
		test = GetAncestor(test, GA_ROOTOWNER);
		if(test != GetDesktopWindow() )
			MessageBox(0, L"Window", 0,0);
		

		if(hittest == HTCAPTION)		//On Title Bar
		{
			if(msg->message == WM_NCLBUTTONDOWN) // WM_TOUCH_DOWN)
			{
				_mode = 0;
				selectedHandles[topLevel] = GetTouchPoint(msg->hwnd, msg->lParam);			//add hwd & current pos to the map
			}
			else if(msg->message == WM_TOUCH_MOVE)
			{
				_mode = 0;
			}
			else if(msg->message == WM_NCLBUTTONUP)// || msg->message == WM_NCm) //WM_TOUCH_UP)		
			{
				_mode = 1;
				//selectedHandles.erase(topLevel);
				//previousTouch.erase(topLevel);
			}
		}
		else if(hittest == HTCLIENT)	//Client Area
		{
			_mode = 2;
		}
		else							//Non Client Areas
		{
			_iter = selectedHandles.find(topLevel);
			if(_iter != selectedHandles.end())				//topLevel Hwnd found in the selectedHandles
			{
				
				_mode = 0;									//another finger is on the title bar
			}
			else
				_mode = 1;
		}*/


		switch(_mode)
		{
		case 0:
			PostWindowMessages(msg, topLevel);
			break;
		case 1:
			PostMouseMessages(msg);
			break;
		case 2:
			PostNonClientMouseMessages(msg);
			break;
		}			
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
