// Headers for TwDetector Classes ..
#include "BlobDetector.h"

// Singleton
class TwDetector 
{
	SINGLETON(TwDetector)
private:
	BlobDetector* m_blobDetector;
	

public:
	BlobDetector* GetBlobDetector();
	void SetFormat(BITMAPINFOHEADER*);
	void SetBlobBrightness(int value);
	int GetBlobBrightness();
};