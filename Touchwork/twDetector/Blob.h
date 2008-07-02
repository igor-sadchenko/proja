#pragma once
#include "Point.h"
class twBlob
{
//Attributes
public:
	int m_id;
	int m_pointscount;
	float m_dist;
	twPoint m_center;
	bool m_checked;
	bool m_isOld;
	bool m_isMoved;
	//Point* m_points;

//Methods
public:
	twBlob(void);
	twBlob(int,twPoint);
	~twBlob(void);
	twPoint CalculateCenter();
	void CalculateDist();
};
