#pragma once

struct  FormatValues
{
	long Brightness;
	long Contrast;
	long Hue;
	long Saturation;
	long Sharpness;
	long Gamma;
	long ColorEnable;
	long WhiteBalance;
	long BacklightCompensation;
	long Gain;

	long& operator [](int index)
	{
		return ((long*)this)[index];

	}
	FormatValues()
	{
		Brightness = LONG_MIN;
		Contrast = LONG_MIN;
		Hue = LONG_MIN;
		Saturation = LONG_MIN;
		Sharpness = LONG_MIN;
		Gamma = LONG_MIN;
		ColorEnable = LONG_MIN;
		WhiteBalance = LONG_MIN;
		BacklightCompensation = LONG_MIN;
		Gain = LONG_MIN;
		
	}
};