#include<windows.h>
#include<atlbase.h>
#include <atlwin.h>
#include <string>
#include <sstream>
using namespace std;

#pragma region shared
#pragma data_seg("TWshare")
HWND s_hwndMaster = NULL;
HHOOK s_hook = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:TWshare,rws")
#pragma endregion shared

#define LIB extern "C" __declspec(dllexport)

HINSTANCE g_hinstDll=NULL;

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
		BOOL ret;
		CWPRETSTRUCT* msg = (CWPRETSTRUCT*) lParam ;
		//switch(msg->message)
		//{
		//	//DONT WRITE HEREs
		//	case WM_TOUCH_DOWN:
		//		MessageBox(0,L"Click",0,0);
		//		PostMessage(msg->hwnd, WM_LBUTTONDOWN, wParam, lParam);
		//		break;
		//	case WM_TOUCH_UP:
		//		MessageBox(0,L"Click",0,0);
		//		PostMessage(msg->hwnd, WM_LBUTTONUP, wParam, lParam);
		//		break;
		//	case WM_TOUCH_MOVE:
		//		MessageBox(0,L"Click",0,0);
		//		PostMessage(msg->hwnd,WM_MOUSEMOVE , wParam, lParam);
		//		break;
		//}
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


LIB LRESULT CALLBACK TWGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam )
{

	if(nCode >= 0 )
	{
		BOOL ret;
		MSG* msg = (MSG*) lParam ;
		/*switch(msg->message)
		{
		case WM_TOUCH_DOWN:
			MessageBox(0,L"Click",0,0);
			PostMessage(msg->hwnd, WM_LBUTTONDOWN, wParam, lParam);
			break;
		case WM_TOUCH_UP:
			MessageBox(0,L"Click",0,0);
			PostMessage(msg->hwnd, WM_LBUTTONUP, wParam, lParam);
			break;
		case WM_TOUCH_MOVE:
			MessageBox(0,L"Click",0,0);
			PostMessage(msg->hwnd,WM_MOUSEMOVE , wParam, lParam);
			break;
		}*/

		if( msg->message == WM_TOUCH_DOWN )
		{
			MessageBox(0,L"GNClick",0,0);
			PostMessage(msg->hwnd, WM_LBUTTONDOWN, wParam, lParam);
		}
		else if( msg->message == WM_TOUCH_UP ) 
		{
			MessageBox(0,L"GMClick",0,0);
			PostMessage(msg->hwnd, WM_LBUTTONUP, wParam, lParam);
		}
		else if( msg->message == WM_TOUCH_MOVE )
		{
			MessageBox(0,L"GMClick",0,0);
			PostMessage(msg->hwnd,WM_MOUSEMOVE , wParam, lParam);
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



