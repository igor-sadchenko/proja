#pragma once
#include "Point.h"
class Blob
{
//Attributes
public:
	int m_id;
	int m_pointscount;
	float m_dist;
	Point m_center;
	bool m_checked;
	bool m_isOld;
	bool m_isMoved;
	//Point* m_points;

//Methods
public:
	Blob(void);
	Blob(int,Point);
	~Blob(void);
	Point CalculateCenter();
	void CalculateDist();
};
