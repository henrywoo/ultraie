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

#ifndef __MY_STATWNDCONFIG_H__
#define __MY_STATWNDCONFIG_H__

#include <6Beeres.h>
#include <6BeeCommons.h>

enum {STATWNDCONFIG_WIDTH = 60,RIGHT_OFFSET=60+93};//87

class C6BeeStatusConfigWnd:
	public CWindowImpl<C6BeeStatusConfigWnd>//,
	//public CToolTipDialog<C6BeeStatusConfigWnd>
{	
public:
	DECLARE_WND_SUPERCLASS(_T("C6BeeStatusConfigWnd"), GetWndClassName())

	C6BeeStatusConfigWnd(){
		URL = _T("李少嫦");
		SBRect.left = -1;
		SBRect.right= STATWNDCONFIG_WIDTH;
		SBRect.top  = 0;
		SBRect.bottom = 20;
	}
	virtual ~C6BeeStatusConfigWnd()
	{
		if(IsWindow())
			DestroyWindow();
	}

public:
	BEGIN_MSG_MAP(C6BeeStatusConfigWnd)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

public:
	//CButton m_btnNotebook;
	CStatusBarCtrl  m_StatusConfigBar;
	ATL::CString URL;
	RECT SBRect;

public:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{

		//if(m_StatusConfigBar.Create(m_hWnd, SBRect, _T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|SBT_TOOLTIPS|SBARS_TOOLTIPS)==NULL){
		if(m_StatusConfigBar.Create(m_hWnd, SBRect, _T(""), WS_CHILD|WS_VISIBLE|SB_BOTTOM|SBT_TOOLTIPS|SBARS_TOOLTIPS)==NULL){
			MessageBox(_T("FAILED"), _T("ERROR"), MB_OK | MB_ICONINFORMATION);
		}

		int arrPanes[] = { 2, STATWNDCONFIG_WIDTH/2, STATWNDCONFIG_WIDTH };
		if(! m_StatusConfigBar.SetParts(sizeof(arrPanes) / sizeof(int),arrPanes)){
			MessageBox(_T("FAILED"), _T("ERROR2"), MB_OK | MB_ICONINFORMATION);
		}
		m_StatusConfigBar.SetSimple(FALSE);
		m_StatusConfigBar.SetUnicodeFormat(TRUE);

		//m_StatusConfigBar.SetText(0,_T(""));

		m_StatusConfigBar.SetIcon(1,(HICON)CMyRes::R().GetImg(IDI_ALEXA));
		m_StatusConfigBar.SetTipText(1,_T("Alexa Rank: N/A"));//???

		//m_StatusConfigBar.SetIcon(2,pr);
		m_StatusConfigBar.SetTipText(2,_T("PageRank是Google对此网页重要性的评估!(0/10)"));//???
		m_StatusConfigBar.SetMinHeight(18);
		return 0;
	}

};

#endif//__MY_NOTE_BHO_H__