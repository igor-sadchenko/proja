#include "MultiMouse.h"
#include "tw_demo.h"

int MouseCursor::s_num_mice = 0;
MultiMouse::MultiMouse(int x,int y)
{
	cxScreen = x;
	cyScreen = y;
}

MultiMouse::~MultiMouse(void)
{
}

void MultiMouse::CaptureDevices( HWND hwnd )
{

	RAWINPUTDEVICE Rid[1];

	//----------capture mouse
	Rid[0].usUsagePage = 01; 
	Rid[0].usUsage = 02; 
	Rid[0].dwFlags = RIDEV_NOLEGACY;
	Rid[0].hwndTarget = hwnd;

	if (RegisterRawInputDevices(Rid, 1, sizeof (Rid[0])) == FALSE) 
	{
		MessageBox(0,"failed to Register RawInput",0,0);
	}
}

LRESULT MultiMouse::HandleInput( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
		UINT dwSize;

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, 
			sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL) 
		{
			return 0;
		} 

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, 
			sizeof(RAWINPUTHEADER)) != dwSize )
			OutputDebugString (TEXT("GetRawInputData doesn't return correct size !\n")); 

		RAWINPUT* raw = (RAWINPUT*)lpb;
		HWND hwndFocus = NULL;
		if (raw->header.dwType == RIM_TYPEMOUSE) 
		{
			MouseMap::iterator mouse = m_mice.find(raw->header.hDevice);
			if (mouse == m_mice.end())
			{
				Mouse newmouse(raw->header.hDevice,cxScreen,cyScreen);
				mouse = m_mice.insert(m_mice.end(),newmouse.GetMapEntry());
				//OnKeyBoardAdded(mouse->second);
			}


			mouse->second.Update(raw->data.mouse);
		} 



		delete[] lpb; 
		return 0;

}

void Mouse::SendMouseMessage( UINT cMsg)
{
	switch (cMsg)
	{
	case WM_MOUSEMOVE:
		tw_demoApp::getSingleton().MultiMouseMoved(*this);
		break;
	case WM_LBUTTONDOWN:
		tw_demoApp::getSingleton().MultiMouseLPressed(*this);
		break;
	case WM_LBUTTONUP:
		tw_demoApp::getSingleton().MultiMouseLReleased(*this);
		break;
	}

}

Mouse::Mouse( HANDLE hDevice, int x,int y ):handle(hDevice)
{
	yPos = 0;
	xPos = 0;
	cxScreen = x;
	cyScreen = y;
	isLButtonDown = false;
	isRButtonDown = false;
	mMouseCursor = new MouseCursor();
	mMouseCursor->setImage("entis.png");
	mMouseCursor->setVisible(true);
	mMouseCursor->setWindowDimensions(x,y);
	m_selectedObject = NULL;

}
void Mouse::Update( RAWMOUSE& raw_mouse )
{
	if(raw_mouse.usFlags == MOUSE_MOVE_RELATIVE)
	{
		xPos = xPos + raw_mouse.lLastX;
		yPos = yPos + raw_mouse.lLastY;

		xPos = max(xPos,0);
		yPos = max(yPos,0);

		xPos = min(xPos, cxScreen);
		yPos = min(yPos, cyScreen);
	}
	else if(raw_mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
	{
		xPos = raw_mouse.lLastX;
		yPos = raw_mouse.lLastY;
	}

	if(raw_mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
	{
		isLButtonDown = true;
		SendMouseMessage(WM_LBUTTONDOWN);
	}
	else if(raw_mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
	{	
		isLButtonDown = false;
		SendMouseMessage(WM_LBUTTONUP);
	}
	else if(raw_mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
	{	
		isRButtonDown = true;
		SendMouseMessage(WM_RBUTTONDOWN);
	}
	else if(raw_mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
	{	
		isRButtonDown = false;
		SendMouseMessage(WM_RBUTTONUP);
	}
	else
	{	
		SendMouseMessage(WM_MOUSEMOVE);
	}
	mMouseCursor->updatePosition(xPos,yPos);
}


