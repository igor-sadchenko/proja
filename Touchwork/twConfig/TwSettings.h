#pragma once

class TwSettings
{
public:
	RECT m_Crop;
	point m_trapozoid[4];
	int* m_valNoise;
	bool m_y_flip;
	bool m_x_flip;
	int m_threshold;

	friend istream& operator >>(istream &is,TwSettings &obj);
    friend ostream& operator <<(ostream &os,const TwSettings &obj);

	friend wistream& operator >>(wistream &is,TwSettings &obj);
    friend wostream& operator <<(wostream &os,const TwSettings &obj);

	TwSettings(void)
	{
		m_valNoise = new int();
		*m_valNoise = 100;
		m_threshold = 145;
	}
	~TwSettings(void)
	{
		delete m_valNoise;
		
	}
	
	int getNoise();
};