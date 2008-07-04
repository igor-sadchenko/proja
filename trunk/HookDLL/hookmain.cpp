#include <windows.h>
#include <atlbase.h>
#include <atlwin.h>
#include <string>
#include <sstream>
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


UINT WM_TOUCH_UP;	
UINT WM_TOUCH_DOWN;	
UINT WM_TOUCH_MOVE;
UINT WM_IS_TOUCHABLE;

LIB void SetMasterWindow(HHOOK hook)
{
	s_hook = hook;

	WM_TOUCH_UP = RegisterWindowMessageA( "WM_TOUCH_UP" ) ;
	WM_TOUCH_DOWN = RegisterWindowMessageA("WM_TOUCH_DOWN" ) ; 
	WM_TOUCH_MOVE = RegisterWindowMessageA( "WM_TOUCH_MOVE" ) ; 
	WM_IS_TOUCHABLE = RegisterWindowMessageA( "WM_IS_TOUCHABLE" );
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, PVOID fImpLoad) {

	switch (fdwReason) {
	  case DLL_PROCESS_ATTACH:
		  // DLL is attaching to the address space of the current process.
		  g_hinstDll = hinstDll;
		  SetMasterWindow(s_hook);
		  break;

	  case DLL_THREAD_ATTACH:
		  SetMasterWindow(s_hook);
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

void PostWindowMessages(MSG msg, HWND topLevelWindow)
{
	//This is called for 2 reasons:: 1)Touch on title bar	or  2)Touch inside window while another one is already on the title bar
	LRESULT hittest = DefWindowProc(topLevelWindow, WM_NCHITTEST, msg.wParam, msg.lParam);	//Perform a hit test at the current position
	POINT currentTouch = POINT();															
	currentTouch.x = LOWORD(msg.lParam);	currentTouch.y = HIWORD(msg.lParam);			//In Screen Coordinates

	if( msg.message == WM_TOUCH_DOWN )
	{
		if(hittest == HTCAPTION)															//The Touch_Down was on the title bar
		{
			PostMessage(topLevelWindow, WM_NCLBUTTONDOWN, MK_LBUTTON, msg.lParam);
			selectedHandles[topLevelWindow] = currentTouch;									//Update point on title bar
		}
		else
		{
			previousTouch[topLevelWindow] = currentTouch;									//First touch inside window
		}
	}
	else if( msg.message == WM_TOUCH_UP ) 
	{
		if(hittest == HTCAPTION)
		{
			PostMessage(topLevelWindow, WM_LBUTTONUP, MK_LBUTTON, msg.lParam);
			selectedHandles.erase(topLevelWindow);											//This is supposed to be done by the caller
		}
		else
		{
			previousTouch.erase(topLevelWindow);
		}
	}
	else if(msg.message == WM_TOUCH_MOVE )
	{
		//We have 2 cases:	1)The touch moving is the one on the title bar (either move or resize)
		//					2)The touch moving is inside the window (resize)
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
				PostMessage(topLevelWindow, WM_MOUSEMOVE, MK_LBUTTON, msg.lParam);
				selectedHandles[topLevelWindow] = currentTouch;								//Update the titlebar touch position
			}
			else
			{
				POINT preTitleTouch = selectedHandles[topLevelWindow];						//Use the previous value of touch on title bar
				long dx = preTitleTouch.x - currentTouch.x;									//to determine the resizing factor
				long dy = preTitleTouch.y - currentTouch.y;

				if(currentTouch.x < previousTouch[topLevelWindow].x)						//resizing in / direction
				{
					width = width + dx;
					height = height + dy;	
					rect.left = rect.left - dx;
					rect.top = rect.top - dy;
				}
				else																		//resizing in \ direction
				{
					width = width - dx;
					height = height + dy;			
					rect.top = rect.top - dy;
				}
				
				//Checking
				if(width < 0)				width = 0;
				if(height < 0)				height = 0;
				//if(rect.top < 0)			rect.top = 0;
				//if(rect.left< 0)			rect.left = 0;
				if(width > screenwidth)		width = screenwidth;
				if(height > screenheight)	height = screenheight;

				//MoveWindow(topLevelWindow, rect.top, rect.left, width, height, TRUE);
				SetWindowPos(topLevelWindow, NULL, rect.left, rect.top, width, height, FALSE);
				selectedHandles[topLevelWindow] = currentTouch;								//Update the title bar touch position
			}
		}
		else																				//Window Resizing
		{
			POINT preTouch = previousTouch[topLevelWindow];	
			long dx = preTouch.x - currentTouch.x;											//Get difference between current & previous touch
			long dy = preTouch.y - currentTouch.y;

			if(currentTouch.x < selectedHandles[topLevelWindow].x)							//resizing in / direction
			{
				width = width + dx;
				height = height - dy;	
				rect.left = rect.left - dx;
			}
			else																			//resizing in \ direction
			{
				width = width - dx;
				height = height - dy;			
			}

			//Checking
			if(width < 0)	width = 0;
			if(height < 0)	height = 0;
			//if(rect.top < 0)			rect.top = 0;
			//if(rect.left< 0)			rect.left = 0;
			if(width > screenwidth)		width = screenwidth;
			if(height > screenheight)	height = screenheight;

			//Actual Resizing
			//MoveWindow(topLevelWindow, rect.top, rect.left, width, height, TRUE);
			SetWindowPos(topLevelWindow, NULL, rect.left, rect.top, width, height, FALSE);
			previousTouch[topLevelWindow] = currentTouch;									//Set current touch as the previous touch
		}
	}
}

void PostMouseMessages(MSG msg)
{
	//For Double Click
	BOOL bDoubleClick = FALSE;
	DWORD dwTick = 0;

	if( msg.message ==  WM_TOUCH_DOWN )
	{
		PostMessage(msg.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, msg.lParam);
	}
	else if( msg.message == WM_TOUCH_UP ) 
	{
		dwTick = GetTickCount();												//Maybe we can use the "time" member instead of GetTickCount()
		bDoubleClick = ((dwTick - s_dwLastLTouchUp) <= s_dwDblClickMsecs);

		if(bDoubleClick && msg.hwnd == s_hwndLastWindowHandle)					//Elapsed time does not exceed the double-click time-out value
		{																		//NOTE: If that doesn't work, try posting an LButtonUP after the 
			PostMessage(msg.hwnd, WM_LBUTTONDBLCLK , MK_LBUTTON, msg.lParam);	//DoubleClick message
		}		
		else
		{
			PostMessage(msg.hwnd, WM_LBUTTONUP, MK_LBUTTON , msg.lParam);
		}
		s_dwLastLTouchUp = dwTick;
		s_hwndLastWindowHandle = msg.hwnd;
	}
	else if( msg.message == WM_TOUCH_MOVE )
	{
		PostMessage(msg.hwnd,WM_MOUSEMOVE , MK_LBUTTON, msg.lParam);
	}
}

void PostNonClientMouseMessages(MSG msg)
{
	BOOL bDoubleClick = FALSE;
	DWORD dwTick = 0;

	if( msg.message ==  WM_TOUCH_DOWN )
	{
		PostMessage(msg.hwnd, WM_NCLBUTTONDOWN, MK_LBUTTON, msg.lParam);
	}
	else if( msg.message == WM_TOUCH_UP ) 
	{
		dwTick = GetTickCount();
		bDoubleClick = ((dwTick - s_dwLastLTouchUp) <= s_dwDblClickMsecs);

		if(bDoubleClick && msg.hwnd == s_hwndLastWindowHandle)					
		{																		
			PostMessage(msg.hwnd, WM_NCLBUTTONDBLCLK , MK_LBUTTON, msg.lParam);
		}		
		else
		{
			switch(msg.wParam)
			{
			case HTMINBUTTON:
				if(IsIconic(msg.hwnd))
					PostMessage(msg.hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				else
					PostMessage(msg.hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
				break;
			case HTMAXBUTTON:
				if(IsZoomed(msg.hwnd))
					PostMessage(msg.hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				else
					PostMessage(msg.hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				break;
			case HTCLOSE:
				PostMessage(msg.hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
				break;
			/*case HTVSCROLL:
				PostMessage(msg.hwnd, WM_SYSCOMMAND, SC_VSCROLL, 0);
				break;
			case HTHSCROLL:
				PostMessage(msg.hwnd, WM_SYSCOMMAND, SC_HSCROLL, 0);
				break;*/
			default:
				PostMessage(msg.hwnd, WM_LBUTTONUP, MK_LBUTTON, msg.lParam);
				break;
			}
		}
		s_dwLastLTouchUp = dwTick;
		s_hwndLastWindowHandle = msg.hwnd;
	}
	else if( msg.message == WM_TOUCH_MOVE )
	{
		PostMessage(msg.hwnd,WM_NCMOUSEMOVE , MK_LBUTTON, msg.lParam);
	}
}

bool NonClient(LRESULT hittest)
{
	if(hittest == HTBORDER || 
		hittest == HTBOTTOM || 
		hittest == HTBOTTOMLEFT || 
		hittest == HTBOTTOMRIGHT || 
		hittest == HTCAPTION || 
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
	if(nCode >= 0 & nCode == HC_ACTION)
	{
		map<HWND,POINT>::iterator _iter;
		int _mode = 1;												//WindowMessages:0 , MouseMessages:1, NonClientMouseMessages:2
		MSG* msg = (MSG*) lParam;

		if(SendMessage(msg->hwnd, WM_IS_TOUCHABLE, 0, 0) == 5)
		{
			return CallNextHookEx(s_hook ,nCode, wParam, lParam);
		}

		POINT touchSCord = GetTouchPoint(msg->hwnd, msg->lParam);	//Convert client coordinates to screen (takes LPARAM,return POINT struct)
		HWND currentHandle = msg->hwnd; //WindowFromPoint(touchSCord);			//Get the handle at the current touch position
		HWND topLevel = GetAncestor(currentHandle, GA_ROOTOWNER );	//Get Top Level Window of the current handle

		if((msg->message == WM_TOUCH_DOWN || msg->message == WM_TOUCH_MOVE) && 
			GetForegroundWindow() != topLevel)						//If window is not currently on the top, 
			SetForegroundWindow(topLevel);							//then bring it to the top

		LRESULT hittest = DefWindowProc(topLevel, WM_NCHITTEST, msg->wParam, MAKELPARAM(touchSCord.x, touchSCord.y)); //Hit Test
																	//HitTest lparam is in screen coordinates

		if(currentHandle == NULL)									//No Window exits at current point (WindowFromPoint).. impossible case
		{
			_mode = 1;												//Mouse Mode
		}
		else														//Else, we have 3 cases:: 1)On Title bar, 2)Client Area, 3)Non Client Area
		{
			if(hittest == HTCAPTION)								//On Title bar
			{
				if(msg->message == WM_TOUCH_DOWN)
				{
					_mode = 0;										//Send Window Level Messages
					selectedHandles[topLevel] = touchSCord;			//Save Handle & current title bar touch(screen coordinates)
				}
				else if(msg->message == WM_TOUCH_MOVE)
				{
					_mode = 0;										//Updating the selectedHandles will happen in Posting the msg
				}													//after knowing the type of move (resize or move(we need previous value))
				else if(msg->message == WM_TOUCH_UP)
				{
					_mode = 1;
					selectedHandles.erase(topLevel);				//Delete Handle from the selected handles
					previousTouch.erase(topLevel);					//Delete any previous stored points inside window (used for resizing)
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
					_mode = 0;										//another finger is on the title bar, therefore send Window Messages
				else
					_mode = 1;										//else, send Client Mouse Messages
			}
		}

		MSG twMsg = *msg;
		switch(_mode){
			case 0:
				{
					twMsg.lParam = MAKELPARAM(touchSCord.x, touchSCord.y);	//Window Level Messages are sent in screen coordinates
					twMsg.hwnd = currentHandle;
					PostWindowMessages(twMsg, topLevel);
				}
				break;
			case 1:
				{
					ScreenToClient(currentHandle, &touchSCord);				//lParam should be modified to have the current handle coordinates
					twMsg.lParam = MAKELPARAM(touchSCord.x, touchSCord.y);
					twMsg.hwnd = currentHandle;
					PostMouseMessages(twMsg);
				}
				break;
			case 2:
				{
					twMsg.wParam = hittest;									//The result of hittest is placed in the wparam
					//twMsg.lParam = MAKELPARAM(touchSCord.x, touchSCord.y);	//Non client messages are sent in screen coordinates			
					PostNonClientMouseMessages(twMsg);
				}
				break;
		}
	}
	return CallNextHookEx(s_hook ,nCode, wParam, lParam);
}

