#include "VideoDevice.h"
#include "videocap.h"


Video::CVideoCapture g_video;

HRESULT CVideo::GetCaptureDevices( EnumFunc f,void* userdata)
{
	return g_video.GetCaptureDevices(f,userdata);
}

HRESULT CVideo::CaptureFromDevice( int index)
{
	return g_video.CaptureFromDevice(index);
}

HRESULT CVideo::CaptureFromFile( LPTSTR fname)
{
	return g_video.CaptureFromFile(fname);
}

HRESULT CVideo::SetPreviewWindow( HWND hwnd )
{
	return g_video.SetPreviewWindow(hwnd);
}

HRESULT CVideo::Play( SampleListener*  plistener)
{
	return g_video.Play(plistener);
}

HRESULT CVideo::GetFormat(BITMAPINFOHEADER* binfo)
{
	return g_video.GetFormat(binfo);
}

HRESULT CVideo::Stop()
{
	return g_video.Stop();
}

HRESULT CVideo::GetFormatStructs( FormatValues *fmin,FormatValues*fmax,FormatValues *fvals )
{
	return g_video.GetFormatStructs(fmin,fmax,fvals );
}

HRESULT CVideo::SetFormatStructs( FormatValues* formats )
{
	return g_video.SetFormatStructs(formats);
}

HRESULT CVideo::ReleaseDevice()
{
		return g_video.ReleaseDevice();
}
HRESULT CVideo::GetStreamFormats(EnumFunc func_to_call,void* userdata)
{
	return g_video.GetStreamFormats(func_to_call,userdata);
}

HRESULT CVideo::SetStreamFormats(int index)
{
	return g_video.SetStreamFormats(index);
}
HRESULT CVideo::GetStatus()
{
	return g_video.GetStatus();
}
int CVideo::GetFramerate()
{
		return g_video.GetFramerate();
}