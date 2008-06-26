#pragma once

class TwSettings
{
public:
	RECT m_Crop;
	int m_Highpass;
	int* m_valNoise;
	bool m_y_flip;
	bool m_x_flip;

	friend istream& operator >>(istream &is,TwSettings &obj);
    friend ostream& operator <<(ostream &os,const TwSettings &obj);

	friend wistream& operator >>(wistream &is,TwSettings &obj);
    friend wostream& operator <<(wostream &os,const TwSettings &obj);

	TwSettings(void)
	{
		m_valNoise = new int();
		*m_valNoise = 100;
	}
};