#pragma once

#include <Windows.h>
class Point
{
public:
	int m_x;
	int m_y;
public:
	Point(void);
	~Point(void);
	Point(int x,int y);
	POINT getPOINT();
};
