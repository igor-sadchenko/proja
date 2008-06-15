#include "StdAfx.h"
#include "FormatDlg.h"
LRESULT CFormatDlg::OnInitDialog(UINT uMsg, WPARAM wParam, 
					 LPARAM lParam, BOOL& bHandled)
{
	// Do some initialization code
	m_comb_Format.m_hWnd = GetDlgItem(IDC_COMB_FORMAT);
	return TRUE;
}

LRESULT CFormatDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CFormatDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	FormatValues fmin,fmax,fvals;
	TwInput::getInstance().m_video.SetFormatStructs(NULL);
	TwInput::getInstance().m_video.GetFormatStructs(&fmin,&fmax,&fvals);
	
	SetValues(fmin,fmax,fvals);
	return 0;
}


void CFormatDlg::CloseDialog(int nVal)
{
}

void CFormatDlg::fillFormatComb()
{
	m_comb_Format.ResetContent();
	TwInput::getInstance().m_video.GetStreamFormats((EnumFunc)Combo_AddString,&m_comb_Format);
}

void CFormatDlg::SetValues( FormatValues&fmin,FormatValues&fmax,FormatValues& fval)
{
	CTrackBarCtrl slider;
	for (int i=0;i<10 ;i++)
	{
		slider.m_hWnd= GetDlgItem(IDC_SLIDER1+i);
		
		if( fmin[i] == LONG_MIN ) 
		{
			slider.SetScrollPos(SB_CTL,0);
			slider.EnableWindow( FALSE);
			continue;
		}
		slider.EnableWindow( TRUE);
		slider.SetRangeMin(fmin[i],FALSE);
		slider.SetRangeMax( fmax[i],FALSE);
		slider.SetPos(fval[i]);
	}
}

LRESULT CFormatDlg::OnScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(wParam == SB_ENDSCROLL || wParam == SB_THUMBTRACK )
	{

	
	FormatValues fvals;
	CTrackBarCtrl slider;
	if (lParam != NULL)
	{
		slider.m_hWnd = (HWND)lParam;
		TwInput::getInstance().m_video.GetFormatStructs(NULL,NULL,&fvals);
		fvals[slider.GetDlgCtrlID() - IDC_SLIDER1] = slider.GetPos();
		TwInput::getInstance().m_video.SetFormatStructs(&fvals);
		ApplicationManager::WriteLine(L"%d\r\n",slider.GetPos());
	}
	
	}
	return TRUE;
}
bool CFormatDlg::Combo_AddString( int i,LPSTR str,void* userdata)
{
	CComboBox* combo =(CComboBox*)userdata;
	combo->AddString((LPCTSTR)str);
	return true;
}

void CFormatDlg::FillFormats()
{
	fillFormatComb();
}
LRESULT CFormatDlg::OnCbnSelchangeCombFormat(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int sel = m_comb_Format.GetCurSel();

	if(sel > 0)
	{
		TwInput::getInstance().m_video.SetStreamFormats(sel);
	}

	return 0;
}
