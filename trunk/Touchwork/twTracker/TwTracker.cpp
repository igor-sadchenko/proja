#include "stdafx.h"

INIT_SINGLETON(TwTracker)

BlobTracker* TwTracker::GetBlobTracker()
{
	return m_tracker;
}


void TwTracker::Initialize()
{
	m_tracker = new RangeBlobTracker();
}

int TwTracker::Track( list<Blob> * newBlobs )
{
	return m_tracker->UpdateBlobs(newBlobs);
}

list<Blob>& TwTracker::GetDeletedBlobs()
{
	return m_tracker->deletedBlobs;
}

list<Blob>& TwTracker::GetCurrentBlobs()
{
	return m_tracker->currentBlobs;
}