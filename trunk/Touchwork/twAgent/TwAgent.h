// Headers for TwAgent Classes ..
#include<stdafx.h>
#include <list>
using namespace std;

// Singleton
class TwAgent
{
SINGLETON(TwAgent)
public:
	float m_xScreenPerCamera;
	float m_yScreenPerCamera;
	
	void RaiseEvents(list<Blob> &currentBlobs ,list<Blob>& deletedBlobs);
	void NotifyWindowUnderBlob(Blob&,UINT messageType);
	void CameraToScreen(POINT&);
	void UpdateScreenMetrics();

};