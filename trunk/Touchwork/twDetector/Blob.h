#pragma once
#include "Point.h"
class Blob
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
	Blob(void);
	Blob(int,twPoint);
	~Blob(void);
	twPoint CalculateCenter();
	void CalculateDist();
};
