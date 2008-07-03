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

bool twPoint::operator < (twPoint p2) const
{
	if(m_x < p2.m_x)
		return true;
	else if(m_x == p2.m_x)
		if(m_y < p2.m_y)
			return true;
	return false;
}