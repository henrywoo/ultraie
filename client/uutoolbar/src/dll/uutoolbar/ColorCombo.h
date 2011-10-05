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

#if !defined(AFX_COLORCOMBO_H__20020314_DDA3_617F_0D88_0080AD509054__INCLUDED_)
#define AFX_COLORCOMBO_H__20020314_DDA3_617F_0D88_0080AD509054__INCLUDED_

#pragma once

#ifndef __cplusplus
#error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLCTRLS_H__
#error ColorCombo.h requires atlctrls.h to be included first
#endif


/////////////////////////////////////////////////////////////////////////////
// 
#include "RegIterator.h"
#include <string>

template< class T, class TBase = CComboBox, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE C6BeeComBoImpl : 
	public CWindowImpl< T, TBase, TWinTraits >
{
private:
	//////////////////////////////////////////////////////////////////////////
	CBitmap bmp_left;
	CBitmap bmp_center;
	CBitmap bmp_right;
	//////////////////////////////////////////////////////////////////////////
public:
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

	C6BeeComBoImpl(){
		bmp_left  = NULL;
		bmp_center= NULL;
		bmp_right = NULL;
	}

	VOID LoadSkin(UINT left = IDB_COMBOX_CENTER,UINT right=IDB_COMBOX_RIGHT){
		bmp_left.LoadBitmap(IDB_COMBOX_CENTER);
		bmp_right.LoadBitmap(IDB_COMBOX_RIGHT);
	}

	// Message map and handlers
	BEGIN_MSG_MAP(C6BeeComBoImpl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		REFLECTED_COMMAND_CODE_HANDLER(CBN_EDITUPDATE,OnEditChange)
		REFLECTED_COMMAND_CODE_HANDLER(CBN_SELENDOK,  OnSelChange)
	END_MSG_MAP()

	LRESULT OnEditChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// Get the text in the edit box
		TCHAR str[200];
		GetWindowText(str, 200);
		int nLength = GetWindowTextLength();


		// Currently selected range
		DWORD dwCurSel = GetEditSel();
		WORD dStart = LOWORD(dwCurSel);
		WORD dEnd = HIWORD(dwCurSel);

		// Search for, and select in, and string in the combo box that is prefixed
		// by the text in the edit box
		if (SelectString(-1, _bstr_t(str)) == CB_ERR)
		{
			SetWindowText(str);		// No text selected, so restore what was there before
			if (dwCurSel != CB_ERR)
				SetEditSel(dStart, dEnd);	// restore cursor postion
		}
		// Set the text selection as the additional text that we have added
		if (dEnd < nLength && dwCurSel != CB_ERR)
			SetEditSel(dStart, dEnd);
		else
			SetEditSel(nLength, -1);

		return 0;
	}

	LRESULT OnSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		int iItem = GetCurSel();
		if(iItem>=0)
		{
			TCHAR cValue[1024];             
			GetLBText(iItem, cValue);
			AddtoHistory(_bstr_t(cValue));
			//选择之后就开始搜索。。。
			::PostMessage(GetParent(), WM_COMBO_TOOLBAR_SEARCH, 0, 0);
		}
		return 0;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		LRESULT lRes = DefWindowProc();
		if(bmp_left==NULL || bmp_right==NULL){
			LoadSkin(IDB_COMBOX_CENTER,IDB_COMBOX_RIGHT);
		}
		ATLASSERT(::IsWindow(m_hWnd));
		SetItemHeight(-1, 18);
		return lRes;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
		RECT rectcombo;
		this->GetClientRect(&rectcombo);

		CPaintDC dc(m_hWnd);
		CDC dcMem;
		dcMem.CreateCompatibleDC(dc);
		dc.SaveDC();
		dcMem.SaveDC();

		//left
		dcMem.SelectBitmap(bmp_left);
		SIZE bmp_left_size;
		bmp_left.GetSize(bmp_left_size);
		dc.StretchBlt(0,0,rectcombo.right-20,TBBUTTON_HEIGHT ,dcMem,0,0,bmp_left_size.cx,bmp_left_size.cy ,SRCCOPY);
		dcMem.RestoreDC(-1);

		//right
		dcMem.SelectBitmap(bmp_right);
		dc.BitBlt(rectcombo.right-20,0,20,TBBUTTON_HEIGHT ,dcMem,0,0,SRCCOPY);
		dcMem.RestoreDC(-1);
		dc.RestoreDC(-1);

		return 0;
	}

	LRESULT AddtoHistory(_bstr_t bstrURL)
	{
		CRegKey keyAppID;

		if(std::basic_string<TCHAR>(bstrURL).find_first_not_of(_T(" "))!=std::string::npos)
		{
			int h = GetCount();
			int pos = FindStringExact(-1,bstrURL);
			if(pos == CB_ERR) 
			{
				if(h<50)
				{
					InsertString(0,bstrURL);
				}else{
					DeleteString(h-1);
					InsertString(0,bstrURL);
				}
			}else{
				DeleteString(pos);
				InsertString(0,bstrURL);
			}

			{
				CRegKey keyAppID;
				keyAppID.Attach(Get6BeeAppRegistryKey());
				keyAppID.RecurseDeleteKey(_T("History"));
			}

			{
				CRegKey keyAppID;
				keyAppID.Create(Get6BeeAppRegistryKey(),_T("History"));

				for(int i=0;i < GetCount();i++)
				{
					TCHAR cValue[1024];
					GetLBText(i, cValue);
					keyAppID.SetDWORDValue(cValue,1);
				}
			}

			SetCurSel(0);
		}

		return 0;
	}

	bool InsertHistory()
	{
		ResetContent();
		for (RegIterator it; it; it++)
			InsertString(-1, it);
		return true;
	}

};

class C6BeeComBoCtrl : public C6BeeComBoImpl<C6BeeComBoCtrl, CComboBox, CWinTraitsOR<CBS_DROPDOWN> >
{
public:
	DECLARE_WND_SUPERCLASS(NULL, GetWndClassName())  
};

#endif // !defined(AFX_COLORCOMBO_H__20020314_DDA3_617F_0D88_0080AD509054__INCLUDED_)
