#include "StdAfx.h"
#include "Point.h"
#include "Blob.h"
#include <math.h>

Blob::Blob(void)
{
	m_id = 0;
	m_dist = -1;
	m_pointscount = 0;
	m_center.m_x = 0;
	m_center.m_y = 0;
	CalculateDist();
	m_checked = false;
	m_isOld = false;
	m_isMoved = false;
}

Blob::Blob(int id,Point center)
{
	m_id = id;
	m_dist = -1;
	m_pointscount = 0;
	m_center.m_x = center.m_x;
	m_center.m_y = center.m_y;
	CalculateDist();
	m_checked = false;
	m_isOld = false;
	m_isMoved = false;
}

Blob::~Blob(void)
{
}

Point Blob::CalculateCenter()
{
	//int i;
	Point center;
	return center;
}
void Blob::CalculateDist()
{
	m_dist = sqrtf(m_center.m_x*m_center.m_x + m_center.m_y*m_center.m_y);
}
