// Headers for TwDetector Classes ..

// Singleton
class TwTracker: public Singleton<TwTracker>
{
//SINGLETON(TwTracker)

	BlobTracker* m_tracker;
public:
	BlobTracker* GetBlobTracker();
	void Initialize();
	list<Blob>& GetCurrentBlobs();
	list<Blob>& GetDeletedBlobs();
	int Track(list<Blob> * newBlobs);
};