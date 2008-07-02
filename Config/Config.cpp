// config2.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "form1.h"
#using "mscorlib.dll"
#using "system.dll"
#using "system.Windows.Forms.dll"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace config;
CVideoCapture g_video;
FrameListner g_Listner;
TwSettings g_settings;
MonochromeListner g_Monochrome;
NoiseRemovalListner g_Noise;
CroppingListner g_Crop;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	Application::Run(gcnew MainForm());
	ofstream fout("config.txt");
	fout<<g_settings;
 	return 0;
 }



