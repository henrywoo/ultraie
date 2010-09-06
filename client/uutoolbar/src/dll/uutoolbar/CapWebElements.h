#pragma once
#include "stdafx.h"
#include "resource.h"
#include "fstream"

class CCapWebElements:
	public CDialogImpl<CCapWebElements>
{
public:
	enum { IDD = IDD_CAPWEBPAGEELEMENTS };
	CCapWebElements(void);
	~CCapWebElements(void);

	BEGIN_MSG_MAP(CCapWebElements)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL,   OnCancel    )
		MESSAGE_HANDLER(WM_MOUSEMOVE,  OnMouseMove )
		MESSAGE_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP,  OnLButtonUp)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp  (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	bool GetElementLocation(IHTMLElement *pElement, RECT *dRect);

	IHTMLDocument2Ptr webcap_pHtmlDoc2;
	IWebBrowser2Ptr p_IWebBrowser2;

	HICON m_hIcon;
	short m_count;
	HPEN  g_hRectanglePen;
	bool  m_start;

	std::ofstream ofile;
	time_t mytime;
	long tempID;
	BSTR p;
	BSTR tag;
	//IDC_CORDINFO

	CWindow m_IDC_CORDINFO;
};
