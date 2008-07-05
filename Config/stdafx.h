// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include<DShow.h>
#include "Qedit.h"
#include "AtlBase.h"
#include "geometry.h"

#include<vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include<vector>
#include <math.h>
#include <exception>


#define TW_CONFIG 1
#include <cv.h>
#include "Point.h"
#include "Blob.h"

#include "VideoFormat.h"
#include "videocap.h"
using namespace std;
using namespace Video;

#include "TwSettings.h"
#include "BlobDetector.h"
#include "FrameListner.h"

extern CVideoCapture g_video;
extern TwSettings g_settings;
extern  FrameListner g_Listner;
//todo add here a global instance of the filter .. and copy it (without extern) to the main
extern  MonochromeListner g_Monochrome;
extern  NoiseRemovalListner g_Noise;
extern	CroppingListner g_Crop;

#include "FilterForm.h"
#include "CropForm1.h"
#include "form1.h"


// TODO: reference additional headers your program requires here
