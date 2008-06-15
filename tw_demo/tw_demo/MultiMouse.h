#define _WIN32_WINNT	0x0501

#include <Windows.h>
#include <map>
using namespace std;
#include "MouseCursor.h"

class TouchableNode;

#pragma once
class Mouse;
typedef pair<HANDLE,Mouse> MouseMapEntry;
class Mouse
{
public:
	int xPos;
	int yPos;
	TouchableNode* m_selectedObject;
	int m_position_index;
	int cxScreen;
	int cyScreen;
	bool isLButtonDown;
	bool isRButtonDown;
	HANDLE handle;
	MouseCursor* mMouseCursor ;
	Mouse(HANDLE hDevice,int,int);

	void SendMouseMessage(UINT msg);	
	void Update(RAWMOUSE& raw_mouse);
	MouseMapEntry GetMapEntry()
	{
		return MouseMapEntry(handle,*this);
	}

};
typedef map<HANDLE,Mouse> MouseMap ;


class MultiMouse
{
public:
	MouseMap m_mice;
	int cxScreen;
	int cyScreen;
	MultiMouse(int,int);
	~MultiMouse(void);
	void CaptureDevices(HWND);
	LRESULT HandleInput(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
