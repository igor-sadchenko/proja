#include "stdafx.h"
//INIT_SINGLETON(TwAgent)

TwAgent TwAgent::m_instance;


void TwAgent::InitializeWindowsMessages()
{
	WM_TOUCH_UP = RegisterWindowMessageA( "WM_TOUCH_UP" ) ; 
	WM_TOUCH_DOWN = RegisterWindowMessageA( "WM_TOUCH_DOWN" ) ;
	WM_TOUCH_MOVE = RegisterWindowMessageA( "WM_TOUCH_MOVE" ) ; 
}

void TwAgent::RaiseEvents( list<Blob> &currentBlobs ,list<Blob>& deletedBlobs )
{
	list<Blob>::iterator itr;
	for(itr = currentBlobs.begin() ; itr != currentBlobs.end(); itr++ )
	{
		if (itr->m_isMoved)
		{
			//move
			NotifyWindowUnderBlob(*itr,WM_TOUCH_MOVE);
		}
		else if( !itr->m_isOld )
		{
			//new //touchDOWN
			NotifyWindowUnderBlob(*itr,WM_TOUCH_DOWN);
		}
	}

	for(itr = deletedBlobs.begin() ; itr != deletedBlobs.end(); itr++ )
	{
		//delete // TOUCHUP
		NotifyWindowUnderBlob(*itr,WM_TOUCH_UP);
	}
}

void TwAgent::NotifyWindowUnderBlob(Blob& blob, UINT messageType)
{
	//get the blob place
	POINT pt = blob.m_center.getPOINT();
	//map it to the screen space
	CameraToScreen(pt);
	//locate the window under it
	HWND hwnd = WindowFromPoint(pt);
	if(hwnd)
	{
		CWindow win(hwnd);
		//win = win.GetTopLevelParent();	//this should not be the case we ust send to any window ..and maybe it notifies its own parent later
		//map screen space to window client space
		ScreenToClient(win.m_hWnd,&pt);
		WPARAM wparam = MAKEWPARAM( blob.m_id, blob.m_pointscount );
		LPARAM lparam = MAKELPARAM( pt.x,pt.y);
		//notify the screen of the new stuff
		SendMessage(win.m_hWnd,messageType,wparam,lparam);
	}
}

void TwAgent::CameraToScreen( POINT& pt)
{
	pt.x = pt.x * m_xScreenPerCamera;
	pt.y = pt.y * m_yScreenPerCamera;
}

int TwAgent::InitializeHookDll()
{
	HMODULE hmod = LoadLibrary(TEXT("HookDLL.dll"));

	//HHOOK hook1 = SetWindowsHookEx(WH_CALLWNDPROCRET,TWCallWndProc,hmod,NULL);	
	hook2 = SetWindowsHookEx(WH_GETMESSAGE,TWGetMsgProc,hmod,NULL);	

	if(hook2  == NULL)
	{
		MessageBox(0,L"Hook failed",0,0);
	}
	else
	{
		MessageBox(0,L"Hook Succeeded" , 0 , 0 ) ; 
		SetMasterWindow(hook2);
		//TWCallWndProc(0,0,0);
	}

	//put unhook dih in other func ..destructor wala 7aga
	//UnhookWindowsHookEx(hook1);
    return 0;
}	
