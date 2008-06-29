// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once
#define _ATL_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4995)

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include "atlmisc.h"

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include "resource.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>
#include <math.h>
#include <sstream>	
#include <exception>
#include <windows.h>

using namespace std;

#include <DShow.h>
#include "Qedit.h"
#include "AtlBase.h"
#include "cv.h"

#include "singleton.h"
#include "resource.h"
#include "VideoFormat.h"
#include "videocap.h"
using namespace Video;
#include "Blob.h"
#include "Point.h"
#include "BlobTracker.h"

#include "TwSettings.h"
#include "twMessages.h"
#include "TwInput.h"
#include "TwAgent.h"
#include "TwDetector.h"
#include "TwTracker.h"
#include "ApplicationManager.h"

#include "Touchwork.h"





//void WriteLine (TCHAR * szFormat, ...);
#define show(x) ::MessageBox(0,L#x,0,0);
