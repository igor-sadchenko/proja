// Headers for TwAgent Classes ..
#pragma once
#include "stdafx.h"

// Singleton
class TwInput
{
SINGLETON(TwInput)
	

public:
	SampleListener* m_SampleListener;

	Video::CVideoCapture m_video;
	void Start(SampleListener*);
};