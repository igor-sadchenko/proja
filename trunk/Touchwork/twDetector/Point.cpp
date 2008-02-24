#include "StdAfx.h"
#include "Point.h"

Point::Point(void)
{
}

Point::Point(int x,int y)
{
	m_x = x;
	m_y = y;
}

Point::~Point(void)
{
}

POINT Point::getPOINT()
{
	POINT pt = {m_x,m_y};
	return pt;
}