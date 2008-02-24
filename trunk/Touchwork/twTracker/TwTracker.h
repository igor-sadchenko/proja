// Headers for TwDetector Classes ..
#include "BlobTracker.h"

// Singleton
class TwTracker
{
SINGLETON(TwTracker)
BlobTracker m_blobTracker;
public:
	BlobTracker* GetBlobTracker();
};