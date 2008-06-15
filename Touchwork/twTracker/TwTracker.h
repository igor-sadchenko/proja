// Headers for TwDetector Classes ..

// Singleton
class TwTracker
{
SINGLETON(TwTracker)
BlobTracker m_blobTracker;
public:
	BlobTracker* GetBlobTracker();
};