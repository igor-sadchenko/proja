#pragma once

#include <Windows.h>
class twPoint
{
public:
	int m_x;
	int m_y;
public:
	twPoint(void);
	~twPoint(void);
	twPoint(int x,int y);
	POINT getPOINT();
};
