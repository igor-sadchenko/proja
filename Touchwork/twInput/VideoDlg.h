// MainDlg.h : interface of the CVideoDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "FormatDlg.h"

class CPictureBox : public CWindowImpl< CPictureBox , CStatic>
{
public:

	DECLARE_WND_SUPERCLASS(NULL, CStatic::GetWndClassName())

	//enum { IDD = IDC_STATIC1};
	// overridden to provide proper initialization
//  	BOOL SubclassWindow(HWND hWnd)
// 	{
// 
// 		BOOL bRet = ATL::CWindowImpl< CPictureBox , CStatic, ATL::CControlWinTraits>::SubclassWindow(hWnd);
// 		if(bRet)
// 			Init();
// 		return bRet;
// 	}

	// Overrideables
	void DoPaint(CDCHandle dc)
	{

	}

	// Message map and handlers
	BEGIN_MSG_MAP(CPicturebox)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)

	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		Init();
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{

		bHandled = FALSE;
		return 1;
	}



	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 1;   // no background needed
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{

		CDCHandle dc;
		if(wParam)
			dc.m_hDC = (HDC) wParam;
		else
		{
			CPaintDC dc2(m_hWnd);
			dc.m_hDC = dc2;
		}
		RECT r = {50,50,400,200};
		
		
		SelectBrush(dc,GetSysColorBrush(BLACK_BRUSH));
		
		::FillRect(dc,&r,dc.GetCurrentBrush());

		bHandled = true;
		return 1;
	}

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		::MessageBox(0,0,0,0);
		return 1;
	}

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{

		return 1;
	}


	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{

			POINT ptCursor = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			ClientToScreen(&ptCursor);
			RECT rect = { 0 };
			GetWindowRect(&rect);
			unsigned int uPressed = ::PtInRect(&rect, ptCursor) ? 1 : 0;
		
		bHandled = FALSE;
		return 1;
	}



	void Init()
	{
		//SubclassWindow(m_hWnd);
		// We need this style to prevent Windows from painting the button
		ModifyStyle(0, BS_OWNERDRAW);

	
	}


};


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
	CStatic m_pic_box;
	LRESULT OnTtnGetDispInfoCustom1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);


};




