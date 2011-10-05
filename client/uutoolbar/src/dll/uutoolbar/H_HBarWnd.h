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

#ifndef __UUTOOLBAR_HBARWND__
#define __UUTOOLBAR_HBARWND__

#include "H_HBarCombo.h"
#include <string>
#include <map>

#include <atlctrlw.h>
#include <atlstr.h>

using namespace std;

const int WM_HBAR_SEARCH_NEXT = WM_USER+1013;
const int WM_HBAR_SEARCH_PREV = WM_USER+1014;
const int kWM_HBarCOMBOSEARCH = WM_USER+1020;

_COM_SMARTPTR_TYPEDEF(IHTMLDocument2, __uuidof(IHTMLDocument2));
_COM_SMARTPTR_TYPEDEF(IHTMLTxtRange, __uuidof(IHTMLTxtRange));
_COM_SMARTPTR_TYPEDEF(IHTMLElement, __uuidof(IHTMLElement));

enum styleaction {ADDSTYLE,DELSTYLE};

class CH_HBarWnd:
  public CCommandBarCtrlImpl<CH_HBarWnd>
{
public:
  DECLARE_WND_SUPERCLASS( _T("6Bee Search Bar"),CH_HBarWnd::GetWndClassName() )

  BEGIN_MSG_MAP(CH_HBarWnd)
    CHAIN_MSG_MAP_ALT_MEMBER(m_BandEditCtrl,1)
    MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
    MESSAGE_HANDLER(WM_CREATE,OnCreate)
    MESSAGE_HANDLER(WM_SHOWWINDOW,OnWM_SHOWWINDOW)
    MESSAGE_HANDLER(WM_COMMAND,   OnCommand)

    MESSAGE_HANDLER(kWM_HBarCOMBOSEARCH, OnHBarSearch)
    MESSAGE_HANDLER(WM_HBAR_SEARCH_NEXT, OnHBarSearchNext)
    MESSAGE_HANDLER(WM_HBAR_SEARCH_PREV, OnHBarSearchPrev)
  END_MSG_MAP()

  LRESULT OnCtlColorEdit(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
  LRESULT OnCreate   (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnWM_SHOWWINDOW(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnCommand  (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnHBarSearch(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnHBarSearchNext(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnHBarSearchPrev(UINT,WPARAM,LPARAM,BOOL&);

  UINT highLight(IHTMLDocument2Ptr&, const wstring&, styleaction, const wstring&, bool, bool);
  IHTMLDocument2Ptr GetHtmlDocument();
  void SetInfo(int count,const wchar_t* str=NULL);
  CH_HBarWnd();
  virtual ~CH_HBarWnd();
  void SetBrowser(IWebBrowser2*);
  HBRUSH m_hBrush;
  CBandEditCtrl m_BandEditCtrl;
  CButton m_case;
  BOOL newpage;
  wstring prev_search_words;

private:
  static const int kbtnnum=7;
  TBBUTTON Buttons[kbtnnum];

  IWebBrowser2 * pwb_;
  HIMAGELIST m_hImageList;
  wstring casestr_;
  wstring HLstyle;
  bool isHighLight;
  UINT totalfound;

  IHTMLTxtRangePtr bodyRange;
  IHTMLTxtRangePtr currentRange;
  IHTMLTxtRangePtr currentRange2;
  IHTMLElementPtr  body;

  COLORREF bkcolor;
  HICON hi_HL;
  HFONT font_case;
  HFONT font_edit;
};

#endif
