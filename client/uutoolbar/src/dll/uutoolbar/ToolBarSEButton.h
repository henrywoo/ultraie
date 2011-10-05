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

#ifndef __UUTOOLBAR_TOOLBARSEBUTTON__
#define __UUTOOLBAR_TOOLBARSEBUTTON__

#include "ToolBarCombo.h"
#include "6bees_iwebbrowser2.h"
#include "6bees_const.h"

//constants of message id
const int WM_TOOLBAR_SEBUTTON_SEARCH  = WM_USER+3;

class CToolBarSEButton:
  public CWindowImpl< CToolBarSEButton,CButton >
{
public:
  DECLARE_WND_SUPERCLASS(TEXT("CToolBarSEButton"), TEXT("BUTTON"))

  BEGIN_MSG_MAP(CToolBarSEButton)
    MESSAGE_HANDLER(WM_CREATE,OnCreate)
    MESSAGE_HANDLER(WM_SETCURSOR,  OnSetCursor  )
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseOut   )
    MESSAGE_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
    MESSAGE_HANDLER(WM_TOOLBAR_SEBUTTON_SEARCH,OnLButtonDown)
    MESSAGE_HANDLER(WM_PAINT,OnPaint)
    ALT_MSG_MAP(2)
    MESSAGE_HANDLER(_6bees_const::kWM_SE_GG,   OnSetGGURL)
    MESSAGE_HANDLER(_6bees_const::kWM_SE_BAIDU,OnSetBaiduURL)
    MESSAGE_HANDLER(_6bees_const::kWM_SE_YAHOO,OnSetYahooURL)
    MESSAGE_HANDLER(_6bees_const::kWM_SE_SETTING,OnSearchSetting)
  END_MSG_MAP()

  LRESULT OnSetCursor  (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnMouseOut   (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonDown(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnPaint      (UINT,WPARAM,LPARAM,BOOL&);

  LRESULT OnSetGGURL   (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnSetBaiduURL   (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnSetYahooURL   (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnSearchSetting(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnCreate   (UINT,WPARAM,LPARAM,BOOL&);

  void DrawItem(LPDRAWITEMSTRUCT lpdis);
  void SetBrowser(_6bees_html::c6beewb2ptr*);

  CToolBarSEButton(void);
  ~CToolBarSEButton(void);

  CToolBarCombo* p_CToolBarCombo;

protected:
  CBitmap m_bmp;
  CBitmap m_bmp_bkg;
private:
  _6bees_html::c6beewb2ptr* pwb_;
  short m_count;
  _bstr_t searchURL;
  bool BG2312SearchEngine;
};

#endif