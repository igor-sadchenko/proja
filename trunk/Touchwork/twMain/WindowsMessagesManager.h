#ifndef WINDOWS_MESSAGES_MANAGER
#define WINDOWS_MESSAGES_MANAGER
#include <stdafx.h>
class WindowsMessagesManager
{
	SINGLETON(WindowsMessagesManager)
public:
	void SendMouseClickMessage( int x , int y ) ; 
	void DrawBlob(int x,int y,int width,int height);
private:
	static int m_bruchColor;
	void SendMouseClickDown() ; 
	void SendMouseClickUp() ; 
	void SendMessage(Blob*);	
};

#endif