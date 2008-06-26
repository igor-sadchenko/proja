#include "StdAfx.h"
#include "Point.h"

twPoint::twPoint(void)
{
}

twPoint::twPoint(int x,int y)
{
	m_x = x;
	m_y = y;
}

twPoint::~twPoint(void)
{
}

POINT twPoint::getPOINT()
{
	POINT pt = {m_x,m_y};
	return pt;
}