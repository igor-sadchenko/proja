#include "stdafx.h"

INIT_SINGLETON(TwTracker)

BlobTracker* TwTracker::GetBlobTracker()
{
	return m_trackerImp;
}


void TwTracker::Initialize()
{
	m_trackerImp = new BeltRangeTracker();
}

int TwTracker::Track( list<twBlob> * newBlobs )
{
	return m_trackerImp->UpdateBlobs(newBlobs);
}

list<twBlob>& TwTracker::GetDeletedBlobs()
{
	return m_trackerImp->deletedBlobs;
}

list<twBlob>& TwTracker::GetCurrentBlobs()
{
	return m_trackerImp->currentBlobs;
}