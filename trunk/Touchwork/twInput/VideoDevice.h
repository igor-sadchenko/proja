#pragma once
#define _ATL_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4995)


#include <Windows.h>
#include "VideoFormat.h"
typedef bool (*EnumFunc)(int,LPTSTR,void*);
typedef void (*SampleCallbackFunc)(BYTE*pdata,int size,BITMAPINFOHEADER* pbitmapinfo);
