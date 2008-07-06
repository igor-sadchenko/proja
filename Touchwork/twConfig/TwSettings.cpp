#include "StdAfx.h"
#include "TwSettings.h"

istream& operator >>(istream &is,TwSettings &obj)
{

	while (is.good())
	{
		string str;
		is>>str;
		if(str == "crop")
			is>>obj.m_Crop.left>>obj.m_Crop.top>>obj.m_Crop.right>>obj.m_Crop.bottom;
		else if (str == "noise")
			is>>*(obj.m_valNoise);
		else if (str == "Xflip")
			is>>obj.m_x_flip;
		else if (str == "Yflip")
			is>>obj.m_y_flip;
		else if (str == "CamraTrapozoid")
		{
			for(int i =0;i<4 ;i++)
			{
				is>>obj.m_trapozoid[i][0]>>obj.m_trapozoid[i][1];
			}
		}
		else if (str == "threshold")
			is>>obj.m_threshold;
	}
	return is;
}


ostream& operator <<(ostream &os,const TwSettings &obj)
{
	os<<"crop "<< obj.m_Crop.left<<" "<<obj.m_Crop.top<<" "<<obj.m_Crop.right<<" "<<obj.m_Crop.bottom<<endl;
	os<<"noise "<< *(obj.m_valNoise)<<endl;
	os<<"Xflip "<< obj.m_x_flip<<endl;
	os<<"Yflip "<< obj.m_y_flip<<endl;
	os<<"CamraTrapozoid"<<endl;
	for(int i =0;i<4 ;i++)
	{
		os<<obj.m_trapozoid[i][0]<<" "<<obj.m_trapozoid[i][1]<<endl;
	}
	os<<"threshold "<<obj.m_threshold<<endl;

	return os;
}

int TwSettings::getNoise()
{
	return * m_valNoise;
}