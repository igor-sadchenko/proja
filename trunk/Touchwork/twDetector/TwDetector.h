// Headers for TwDetector Classes ..
#include "BlobDetector.h"

// Singleton
class TwDetector :public Singleton<TwDetector>
{
	
private:
	BlobDetector* m_blobDetector;
	

public:
	BlobDetector* GetBlobDetector();
	void Initialize();
	void Detect(BITMAPINFOHEADER* binfo, BYTE* pdata ,list<Blob>&);
	
	void SetFormat(BITMAPINFOHEADER*);
};