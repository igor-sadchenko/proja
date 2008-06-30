// Headers for TwAgent Classes ..
#pragma once

// Singleton
class TwInput:public Singleton<TwInput>
{
	
public:
	SampleListener* m_SampleListener;

	Video::CVideoCapture m_video;
	void Start(SampleListener*);
};