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
