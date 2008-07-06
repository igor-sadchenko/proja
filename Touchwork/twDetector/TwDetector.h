// Headers for TwDetector Classes ..
#include "BlobDetector.h"

// Singleton
class TwDetector :public Singleton<TwDetector>
{
	
private:
	ComponentLabelingBlobDetector* m_blobDetector ;

public:
	BlobDetector* GetBlobDetector();
	void Initialize();
	void Detect(BITMAPINFOHEADER* binfo, BYTE* pdata ,list<twBlob>&);
	
	void SetFormat(BITMAPINFOHEADER*);
	void DeInitialize();
};