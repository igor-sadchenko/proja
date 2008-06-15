#include "stdafx.h"
using namespace Video;

INIT_SINGLETON(TwInput)
void TwInput::Start( SampleListener* plistener)
{
	m_SampleListener = plistener;
}