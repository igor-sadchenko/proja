// MainDlg.cpp : implementation of the CVideoDlg class
//
/////////////////////////////////////////////////////////////////////////////

//#pragma comment(linker, "/NODEFAULTLIB:msVCRTD.LIB")

#include "stdafx.h"

#include <Dbt.h>
#include <AtlCtrls.h>

#include "VideoDlg.h"

CVideoDlg* Singleton<CVideoDlg>::m_instance;

LRESULT CVideoDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);


	//set the picture box
	
	m_pic_box.m_hWnd =  GetDlgItem(IDC_STATIC1);;
	
	

	//---
	ModuleManager::txtConsole =GetDlgItem(IDC_Monitor);
	m_comb_cams.m_hWnd = GetDlgItem(IDC_COMBO1);
	fillCamerasComb();

//	m_FormatDlg.m_hWnd =
	m_FormatDlg.Create(NULL) ;
	m_CamFrameRate.m_hWnd = GetDlgItem(IDC_CAM_FR);
	m_actualFrameRate.m_hWnd = GetDlgItem(IDC_ACTUAL_FR);

	//-----set the brightness scroll bar
	CTrackBarCtrl slider;
	slider.m_hWnd = ::GetDlgItem(m_hWnd,IDC_SLIDER_BRIGHTNESS);
	slider.SetRangeMin(0,FALSE);
	slider.SetRangeMax(255,FALSE);
	slider.SetPos(BlobDetector::m_Threshold);
	CStatic ctrl_static;
	ctrl_static.m_hWnd = ::GetDlgItem(m_hWnd,IDC_LABEL_Brightness);
	wostringstream str ;
	str<< BlobDetector::m_Threshold;
	ctrl_static.SetWindowText(str.str().c_str());

	return TRUE;
}

void CVideoDlg::fillCamerasComb()
{
	m_comb_cams.ResetContent();
	m_comb_cams.AddString(L"<Browse...>");

	TwInput::getInstance().m_video.GetCaptureDevices((EnumFunc)Combo_AddString,&m_comb_cams);
}
LRESULT CVideoDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_FormatDlg.DestroyWindow();
	// unregister message filtering and idle updates
	
	return 0;
}
LRESULT CVideoDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	ShowWindow(SW_HIDE);
	ModuleManager::getInstance().FinalizeApplication();
	CloseDialog(0);

	DestroyWindow();

	return 0;
}

LRESULT CVideoDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ModuleManager::WriteLine(L"b4 OK %d\r\n", SUCCEEDED(TwInput::getInstance().m_video.GetStatus()) );
	int sel = m_comb_cams.GetCurSel();
	HWND hpicture_box = GetDlgItem(IDC_STATIC1);
	
	ModuleManager::WriteLine(L"%d\r\n",m_pic_box.m_hWnd);
	if(FAILED( TwInput::getInstance().m_video.AppendSampleGrabber(TwInput::getInstance().m_SampleListener)))
	{
		show("Grabber init failed");
	}

	if(FAILED(TwInput::getInstance().m_video.AppendPreview(m_pic_box.m_hWnd)))
	{
		show("window init failed");
	}
	
	if(FAILED(TwInput::getInstance().m_video.Play()))
	{
		show("Play failed");
	}
	return 0;
}

LRESULT CVideoDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TwInput::getInstance().m_video.Stop();
	return 0;
}

void CVideoDlg::CloseDialog(int nVal)
{
	CWindow owner(m_parent);
	owner.SendMessage(WM_CLOSE);
	//DestroyWindow();
	//::PostQuitMessage(nVal);
}

bool CVideoDlg::Combo_AddString( int i,LPSTR str,void* userdata)
{
	CComboBox* combo =(CComboBox*)userdata;
	combo->AddString((LPCTSTR)str);
	return true;
}
LRESULT CVideoDlg::OnCbnSelchangeCombo1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	TwInput::getInstance().m_video.ReleaseDevice();
	int sel = m_comb_cams.GetCurSel();
	
	if(sel== 0)
	{
		
		CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST,L"AVI Video (*.avi)\0*.avi\0All Files (*.*)\0*.*");
		if(IDOK ==  dlg.DoModal())
		{
			if(FAILED(TwInput::getInstance().m_video.CaptureFromFile( dlg.m_ofn.lpstrFile)))
				MessageBox(L"file failed",0,0);
			
		}
	}
	else if(sel > 0)
	{
		if( FAILED(TwInput::getInstance().m_video.CaptureFromDevice(sel-1)))
		{
			ModuleManager::WriteLine(L"camera device init failed\r\n");
		}
	}
	CButton fmtBtn;
	fmtBtn.m_hWnd = ::GetDlgItem(m_hWnd,IDC_BTN_FORMAT);
	ModuleManager::WriteLine(L"b4 format %d\r\n", SUCCEEDED(TwInput::getInstance().m_video.GetStatus()) );
	fmtBtn.EnableWindow((SUCCEEDED(TwInput::getInstance().m_video.GetFormatStructs(NULL,NULL,NULL))));
	ModuleManager::WriteLine(L"after format %d\r\n", SUCCEEDED(TwInput::getInstance().m_video.GetStatus()) );
	return 0;
}


LRESULT CVideoDlg::On_Format_btn_clicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	FormatValues fmin,fmax,fvals;
	if(FAILED(TwInput::getInstance().m_video.GetFormatStructs(&fmin,&fmax,&fvals)))
	{
		show("filed to init format dialog");
		return E_FAIL;
	}
	m_FormatDlg.ShowWindow(SW_SHOW);
	m_FormatDlg.SetValues(fmin,fmax,fvals);
	m_FormatDlg.FillFormats();
	

	return 0;
}


LRESULT CVideoDlg::OnDeviceChanged( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	if(wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVICEREMOVECOMPLETE || wParam == DBT_DEVNODES_CHANGED)
	{
		ModuleManager::WriteLine(L"device inserted, code= %x\r\n",wParam);	
		fillCamerasComb();
		ModuleManager::WriteLine(L"device inserted handled, code= %x\r\n",wParam);	
	}
	return TRUE;
}

LRESULT CVideoDlg::OnCloseCommand( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	CloseDialog(0);
	return TRUE;
}

LRESULT CVideoDlg::OnScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if(wParam == SB_ENDSCROLL || wParam == SB_THUMBTRACK )
	{
		CTrackBarCtrl slider;
		if (lParam != NULL)
		{
			slider.m_hWnd = (HWND)lParam;
			CStatic ctrl_static;
			ctrl_static.m_hWnd = ::GetDlgItem(m_hWnd,IDC_LABEL_Brightness);
			BlobDetector::m_Threshold = slider.GetPos();
			wostringstream str ;
			str<< BlobDetector::m_Threshold;
			ctrl_static.SetWindowText(str.str().c_str());
			
		}

	}
	return TRUE;
}

