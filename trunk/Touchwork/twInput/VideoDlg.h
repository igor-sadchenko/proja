// MainDlg.h : interface of the CVideoDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "FormatDlg.h"
class CVideoDlg : public CDialogImpl<CVideoDlg>, public Singleton<CVideoDlg>
{
public:
	enum { IDD = IDD_DLG_VIDEO};

	BEGIN_MSG_MAP(CVideoDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCLOSE, OnCloseCommand)
		COMMAND_ID_HANDLER(IDCANCEL, OnOK)
		MESSAGE_HANDLER( WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(ID_PAUSE, OnCancel)
		COMMAND_HANDLER(IDC_COMBO1, CBN_SELCHANGE, OnCbnSelchangeCombo1)
		COMMAND_HANDLER(IDC_BTN_FORMAT, BN_CLICKED, On_Format_btn_clicked)
		MESSAGE_HANDLER(WM_DEVICECHANGE,OnDeviceChanged)
		MESSAGE_HANDLER(WM_HSCROLL, OnScroll)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDeviceChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void fillCamerasComb();
LRESULT CVideoDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void CloseDialog(int nVal);
	static bool Combo_AddString(int,LPSTR,void*);
	
	LRESULT OnCbnSelchangeCombo1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	CComboBox m_comb_cams;
	CFormatDlg m_FormatDlg;
	
	LRESULT On_Format_btn_clicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	HWND m_parent;
	CStatic m_actualFrameRate;
	CStatic m_CamFrameRate;
};
