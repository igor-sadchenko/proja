#pragma once

class TwSettings
{
public:
	RECT m_Crop;
	int m_Highpass;
	int* m_valNoise;

	TwSettings(void)
	{
		m_valNoise = new int();
		*m_valNoise = 100;
	}
};
