#include "stdafx.h"

INIT_SINGLETON(TwAgent)

void TwAgent::InitializeWindowsMessages()
{
	WM_TOUCH_UP = RegisterWindowMessageA( "WM_TOUCH_UP" ) ; 
	WM_TOUCH_DOWN = RegisterWindowMessageA( "WM_TOUCH_DOWN" ) ;
	WM_TOUCH_MOVE = RegisterWindowMessageA( "WM_TOUCH_MOVE" ) ; 
}

void TwAgent::RaiseEvents( list<twBlob> &currentBlobs ,list<twBlob>& deletedBlobs )
{
	list<twBlob>::iterator itr;
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

void TwAgent::NotifyWindowUnderBlob(twBlob& blob, UINT messageType)
{
	//get the blob place
	POINT pt = blob.m_center.getPOINT();
	//clip Point
	if(ClipPoint(pt))
		return;
	//flip point
	FlipPoint(pt);
	//map it to the screen space
	CropAreaToScreen(pt);

	//draw a point ...
	DrawScreenPoint(pt);

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
		PostMessage(win.m_hWnd,messageType,wparam,lparam);
	}
}

void TwAgent::CameraToScreen( POINT& pt)
{
	pt.x = pt.x * m_xScreenPerCamera;
	pt.y = pt.y * m_yScreenPerCamera;
}
void TwAgent::CropAreaToScreen( POINT& pt)
{
	pt.x = pt.x * m_xScreenPerCrop;
	pt.y = pt.y * m_yScreenPerCrop;
}

int TwAgent::InitializeHookDll()
{
	HMODULE hmod = LoadLibrary(TEXT("HookDLL.dll"));

	//HHOOK hook1 = SetWindowsHookEx(WH_CALLWNDPROCRET,TWCallWndProc,hmod,NULL);	
	m_hook = SetWindowsHookEx(WH_GETMESSAGE,TWGetMsgProc,hmod,NULL);	

	if(m_hook  == NULL)
	{
		MessageBox(0,L"Hook failed",0,0);
	}
	else
	{
		MessageBox(0,L"Hook Succeeded" , 0 , 0 ) ; 
		SetMasterWindow(m_hook);
		//TWCallWndProc(0,0,0);
	}

    return 0;
}	

bool TwAgent::ClipPoint( POINT & pt)
{
	RECT rc = ModuleManager::getSettings().m_Crop;
	pt.x = pt.x - rc.left;
	pt.y = pt.y - rc.top;
	if(pt.x > rc.right || pt.y > rc.bottom)
		return true;
	else
		return false;
}

void TwAgent::FlipPoint( POINT& pt )
{
	if(ModuleManager::getSettings().m_x_flip)
	{
		pt.x = m_xCrop - pt.x;
	}

	if(ModuleManager::getSettings().m_y_flip)
	{
		pt.y = m_yCrop - pt.y;
	}
}

void TwAgent::DrawScreenPoint( POINT pt )
{
	HDC hdc = GetDC(NULL);
	int r = 100;
	Ellipse(hdc, pt.x - r , pt.y - r , pt.x + r, pt.y + r);
	ReleaseDC(NULL, hdc);
}