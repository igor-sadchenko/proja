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
		else if (str == "highpass")
			is>>obj.m_Highpass;
		else if (str == "noise")
			is>>*(obj.m_valNoise);
		else if (str == "Xflip")
			is>>obj.m_x_flip;
		else if (str == "Yflip")
			is>>obj.m_y_flip;
	}
	return is;
}


ostream& operator <<(ostream &os,const TwSettings &obj)
{
	os<<"crop "<< obj.m_Crop.left<<" "<<obj.m_Crop.top<<" "<<obj.m_Crop.right<<" "<<obj.m_Crop.bottom<<endl;
	os<<"highpass "<< obj.m_Highpass<<endl;
	os<<"noise "<< *(obj.m_valNoise)<<endl;
	os<<"Xflip "<< obj.m_x_flip<<endl;
	os<<"Yflip "<< obj.m_y_flip<<endl;

	return os;
}
