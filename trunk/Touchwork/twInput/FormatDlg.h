#pragma once
#include "stdafx.h"
#include "VideoDevice.h"


class CFormatDlg :
	public CDialogImpl<CFormatDlg>
{
public:
	void SetValues(FormatValues&,FormatValues&,FormatValues&);
	enum { IDD = IDD_FORMAT};

	BEGIN_MSG_MAP(CFormatDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_HSCROLL, OnScroll)
		COMMAND_HANDLER(IDC_COMB_FORMAT, CBN_SELCHANGE, OnCbnSelchangeCombFormat)
	END_MSG_MAP()
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, 
	LPARAM lParam, BOOL& bHandled);

//	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void CloseDialog(int nVal);
	LRESULT OnScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	CComboBox m_comb_Format;
	void fillFormatComb();
	static bool Combo_AddString( int i,LPSTR str,void* userdata);
	void FillFormats();
	LRESULT OnCbnSelchangeCombFormat(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
