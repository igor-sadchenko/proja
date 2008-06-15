#include "stdafx.h"

INIT_SINGLETON(TwTracker)

BlobTracker* TwTracker::GetBlobTracker()
{
	return &m_blobTracker;
}