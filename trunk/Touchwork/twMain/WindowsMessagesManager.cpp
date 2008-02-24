#include "stdafx.h"

INIT_SINGLETON(WindowsMessagesManager)
int WindowsMessagesManager::m_bruchColor = 0;
void WindowsMessagesManager::SendMouseClickMessage(int x, int y)
{
	SetCursorPos(x,y) ; 

	SendMouseClickDown() ; 
	SendMouseClickUp() ;
	
	//SendMessageW(0, WM_LBUTTONDOWN , MAKELPARAM(x,y) , 0 ) ;
	//SendMessageW(0, WM_LBUTTONUP  , MAKELPARAM(x,y) , 0 ) ; 
}

void WindowsMessagesManager::SendMouseClickDown() 
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));
}

void WindowsMessagesManager::SendMouseClickUp() 
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));
}

void WindowsMessagesManager::DrawBlob(int x,int y,int width,int height)
{
	CWindowDC g(NULL);
	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,0);
	x = (float)x/width * (rect.right - rect.left);
	y = (float)y/height * (rect.bottom - rect.top);
	x = x+ rect.left;
	y = y+ rect.top;

	g.SelectStockBrush(m_bruchColor++);
	if(m_bruchColor ==6)
		m_bruchColor = 0;
	g.Ellipse(x-10,y-10,x+10,y+10);
}

void WindowsMessagesManager::SendMessage(Blob* blob)
{

	int lparam = MAKELPARAM( blob->m_center.m_x, blob->m_center.m_y);
	int wparam = MAKEWPARAM( blob->m_id, blob->m_pointscount );

	
}