#include "StdAfx.h"
#include <list>
#include <math.h>
using namespace std;

int BlobTracker::DIST_THRESHOLD = 20;
int BlobTracker::MOVE_THRESHOLD = 5;
int BlobTracker::m_GlobalID = 0;

BlobTracker::BlobTracker(void)
{
}

BlobTracker::~BlobTracker(void)
{
}

int BlobTracker::UpdateBlobs(list<Blob> *newBlobs)
{
	deletedBlobs.clear(); //new frame, new blobs to delete

	bool matchFound = false;
	float minDist = INT_MAX;
	float minBoundry, maxBoundry, diff;

	list<Blob>::iterator match; // newBlob
	Blob* newMatch;

	list<Blob>::iterator nb; // newBlob
	list<Blob>::iterator ob; // oldBlob


	for(ob = currentBlobs.begin(); ob != currentBlobs.end(); ob++)
	{
		ob->m_checked = false;
	}

	// foreach new blob ..
	for(nb = newBlobs->begin(); nb != newBlobs->end(); nb++)
	{
		minBoundry = nb->m_dist - DIST_THRESHOLD;
		maxBoundry = nb->m_dist + DIST_THRESHOLD;
		
		matchFound = false;
		minDist = INT_MAX;

		// foreach old blob ..
		if(currentBlobs.size() > 0)
		{
			for(ob = currentBlobs.begin(); ob != currentBlobs.end(); ob++)
			{
				if(ob->m_dist > maxBoundry)
					break;
				
				if(ob->m_dist > minBoundry)
				{
					diff = sqrtf( (nb->m_center.m_x - ob->m_center.m_x) * (nb->m_center.m_x - ob->m_center.m_x) +
						(nb->m_center.m_y - ob->m_center.m_y) * (nb->m_center.m_y - ob->m_center.m_y) );
					
					if(diff < DIST_THRESHOLD && minDist > diff)
					{
						matchFound = true;
						match = ob;
						minDist = diff;
					}
				}
			}
		}
		if(matchFound)
		{
			// Update ..

			if(minDist > MOVE_THRESHOLD)
				match->m_isMoved = true;

			match->m_checked = true;
			match->m_isOld = true;
			match->m_center = nb->m_center;
			match->m_pointscount = nb->m_pointscount;
		}
		else
		{			
			// New ..
			list<Blob>::iterator itr;
			for(itr = currentBlobs.begin() ; itr != currentBlobs.end(); itr++ )
			{
				if(itr->m_dist > nb->m_dist)
					break;
			}
			nb->m_id = m_GlobalID++;
			nb->m_checked = true;
			currentBlobs.insert(itr,(Blob)(*nb));
		}
	}

	
	for(ob = currentBlobs.begin(); ob != currentBlobs.end();)
	{
		nb = ob++;
		if(!nb->m_checked)
		{
			// Delete ..
			deletedBlobs.push_back(*nb);
			currentBlobs.erase(nb);

		}
	}

	if(m_GlobalID > 1000)
		m_GlobalID = 0;

	return currentBlobs.size();
}