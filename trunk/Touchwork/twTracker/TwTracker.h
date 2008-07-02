// Headers for TwDetector Classes ..

// Singleton
class TwTracker: public Singleton<TwTracker>
{
//SINGLETON(TwTracker)

	BlobTracker* m_trackerImp;
public:
	BlobTracker* GetBlobTracker();
	void Initialize();
	list<twBlob>& GetCurrentBlobs();
	list<twBlob>& GetDeletedBlobs();
	int Track(list<twBlob> * newBlobs);
};