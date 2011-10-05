// UltraIE - Yet Another IE Add-on
// Copyright (C) 2006-2010
// Simon Wu Fuheng (simonwoo2000 AT gmail.com), Singapore
// Homepage: http://www.linkedin.com/in/simonwoo
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __MY_NOTE_BHO_H__
#define __MY_NOTE_BHO_H__


enum { BTN_NOTEBOOK_WIDTH = 400, BTN_NOTEBOOK_HEIGHT = 20 };//67

class CMyNoteBHOWindow:
	public CWindowImpl<CMyNoteBHOWindow>
{	
public:
	//STATUSCLASSNAME;#define STATUSCLASSNAMEW        L"msctls_statusbar32"
	DECLARE_WND_SUPERCLASS(_T("CMyNoteBHOWindow"), GetWndClassName())
	CMyNoteBHOWindow(){
		SBRect.left = -1;
		SBRect.right= BTN_NOTEBOOK_WIDTH;
		SBRect.top  = 0;
		SBRect.bottom = BTN_NOTEBOOK_HEIGHT;
	}
	virtual ~CMyNoteBHOWindow(){}

public:
	BEGIN_MSG_MAP(CMyNoteBHOWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		COMMAND_ID_HANDLER(IDC_BTN_NOTEBOOK, OnBtnNoteBook)
	END_MSG_MAP()

public:
	CButton m_btnNotebook;
	//CStatic m_static;
	CStatusBarCtrl  m_StatusBar;
	RECT SBRect;

public:
	/*
	BOOL CreateSimpleStatusBar(LPCTSTR lpstrText,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP,
		UINT nID = ATL_IDW_STATUS_BAR)
	{
		m_status.m_hWnd = ::CreateStatusWindow(dwStyle, lpstrText, m_hWnd, nID);
		return (m_status.m_hWnd != NULL);
	}
	*/
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 1;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{	
		CPaintDC dc(m_hWnd);
		return 0;
	}

	

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{

		
		if(m_StatusBar.Create(m_hWnd, SBRect, _T(""), WS_CHILD|WS_VISIBLE|SB_BOTTOM/*|SB_BOTTOM|SB_LEFT|SB_RIGHT*/,SBT_TOOLTIPS, ATL_IDW_STATUS_BAR)==NULL){
			MessageBox(_T("Welcome to http://www.cnitblog.com/linghuye"), _T("ERROR"), MB_OK | MB_ICONINFORMATION);
		}

		int arrPanes[] = { 2, BTN_NOTEBOOK_WIDTH/4, BTN_NOTEBOOK_WIDTH*2/4, BTN_NOTEBOOK_WIDTH*3/4, BTN_NOTEBOOK_WIDTH };
		if(! m_StatusBar.SetParts(sizeof(arrPanes) / sizeof(int),arrPanes)){
			MessageBox(_T("Welcome to http://www.cnitblog.com/linghuye"), _T("ERROR2"), MB_OK | MB_ICONINFORMATION);
		}
		m_StatusBar.SetText(1,_T("simon"));
		m_StatusBar.SetText(2,_T("woo"));
		m_StatusBar.SetText(3,_T("wu"));
		m_StatusBar.SetText(4,_T("fuheng"));
		m_StatusBar.SetMinHeight(18);
		return 0;
	}

	/*
	void SetPaneWidths(int* arrWidths, int nPanes)
	{ 
		// find the size of the borders
		int arrBorders[3];
		m_status.GetBorders(arrBorders);
		// calculate right edge of default pane (0)
		arrWidths[0] += arrBorders[2];
		for (int i = 1; i < nPanes; i++)
			arrWidths[0] += arrWidths[i];
		// calculate right edge of remaining panes (1 thru nPanes-1)
		for (int j = 1; j < nPanes; j++)
			arrWidths[j] += arrBorders[2] + arrWidths[j - 1];
		// set the pane widths
		m_status.SetParts(m_status.m_nPanes, arrWidths); 
	}

	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&)
	{ 
		// get the current date and time
		SYSTEMTIME st;
		::GetLocalTime(&st);
		ATL::CString str;
		// Pane 1: Display the date
		str.Format(_T("%i/%i/%i"), st.wMonth, st.wDay, st.wYear);
		m_status.SetPaneText(IDR_DATE, str);

		// Pane 2: Display the time
		str.Format(_T("%i:%02i"), st.wHour, st.wMinute);
		m_status.SetPaneText(IDR_TIME, str);
		return 0; 
	}
	*/

	LRESULT OnBtnNoteBook(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{	
		MessageBox(_T("Welcome to http://www.cnitblog.com/linghuye"), _T("OK"), MB_OK | MB_ICONINFORMATION);
		return 0;
	}
};	

#endif//__MY_NOTE_BHO_H__