// config2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "config2.h"
#include "CropForm.h"
#using "mscorlib.dll"
#using "system.dll"
#using "system.Windows.Forms.dll"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace config2;
CVideoCapture g_video;
FrameListner g_Listner;
TwSettings g_settings;
HighPassListner g_Highpass;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	Application::Run(gcnew CropForm());
	return 0;
}


