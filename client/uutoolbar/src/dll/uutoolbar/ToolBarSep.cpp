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

#include "ToolBarSep.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#include "6bees_util.h"
using namespace _6bees_util;
#include <sstream>

CToolBarSep::CToolBarSep():m_CToolBarCOM(NULL),m_bMouseDown(false){}

CToolBarSep::~CToolBarSep(){
  if(!m_bmp.IsNull())
    m_bmp.DeleteObject();
  if (IsWindow()){
    DestroyWindow();
  }
}

LRESULT CToolBarSep::OnCreate(UINT,WPARAM,LPARAM,BOOL&){
  m_bmp = (HBITMAP)c6beeres::R().GetImg(IDB_TBSEP,IMAGE_BITMAP,ksepwidth,ksepheight);
  return 1;
}

LRESULT CToolBarSep::OnSetCursor(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  static HCURSOR hcur = LoadCursor(NULL,/*IDC_SIZEALL*/IDC_SIZEWE);
  if (hcur!=NULL){
    SetCursor(hcur);
    return 1;
  }else{
    SetMsgHandled(false);
    return 0;
  }
}

LRESULT CToolBarSep::OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  ::SetCapture(this->m_hWnd);
  m_bMouseDown = true;
  return 0;
}

LRESULT CToolBarSep::OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  //::MessageBox(NULL,_T("Simon Woo Release!!"),_T("HELLO"),0);
  ::ReleaseCapture();
  m_bMouseDown = false;
  {
    //find the location of mouse and record it
    POINT p;
    ::GetCursorPos(&p);
    HWND TBLOGOWnd = ::FindWindowEx(GetParent(),NULL,_T("TOOLBARSELOGO"),NULL);
    RECT r;
    ::GetWindowRect(TBLOGOWnd,&r);
    int slength=p.x-r.left;
    if(slength<kminsearchboxlen) slength=kminsearchboxlen;
    if(slength>kmaxsearchboxlen) slength=kmaxsearchboxlen;
    _6beed_share::u_setSearchboxlen(slength);
  }
  return 0;
}

LRESULT CToolBarSep::OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  if (m_bMouseDown){
    ::SendMessage(GetParent().m_hWnd,WM_SEP_TOOLBAR_MOUSEMOVE,0,lParam);
  }
  return 0;
}

LRESULT CToolBarSep::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  CPaintDC hdc(m_hWnd);
  CDC dcMem;
  dcMem.CreateCompatibleDC(hdc);
  hdc.SaveDC();
  dcMem.SaveDC();
  dcMem.SelectBitmap(m_bmp);
  hdc.BitBlt(0,0,ksepwidth,ksepheight,dcMem,0,0,SRCCOPY);
  dcMem.RestoreDC(-1);
  hdc.RestoreDC(-1);
  return 0;
}

