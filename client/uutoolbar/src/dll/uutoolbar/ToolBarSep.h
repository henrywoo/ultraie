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

#ifndef __UUTOOLBAR_TOOLBARSEP__
#define __UUTOOLBAR_TOOLBARSEP__

//COM
#include <comdef.h>//_bstr_t

//ATL
#include <atlbase.h>

//WTL
#include <atlapp.h>
#include <atlctrls.h>//CComboBox
#include <commctrl.h>

//GDI+
#include <gdiplus.h>

class CToolBarCOM;

const int ksepwidth  = 6;
const int ksepheight = 24;
const int kminsearchboxlen = 50;
const int kmaxsearchboxlen = 600;

const int WM_SEP_TOOLBAR_MOUSEMOVE    = WM_USER+1666;


class CToolBarSep:
  public CWindowImpl< CToolBarSep,CButton >
{
public:
  //DECLARE_WND_SUPERCLASS(TEXT("CToolBarSep"), TEXT("BUTTON"))
  BEGIN_MSG_MAP(CToolBarSep)
    MESSAGE_HANDLER(WM_CREATE,OnCreate)
    MESSAGE_HANDLER(WM_SETCURSOR,OnSetCursor)
    MESSAGE_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONUP,OnLButtonUp)
    MESSAGE_HANDLER(WM_MOUSEMOVE,OnMouseMove)
    MESSAGE_HANDLER(WM_PAINT,OnPaint)
    //MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
  END_MSG_MAP()

  CToolBarSep(void);
  ~CToolBarSep(void);

  LRESULT OnPaint      (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnCreate   (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnSetCursor  (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonDown(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonUp  (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnMouseMove  (UINT,WPARAM,LPARAM,BOOL&);

  //BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR,Image * &pImg);
  Gdiplus::GdiplusStartupInput    m_gdiplusStartupInput;
  ULONG_PTR        m_gdiplusToken;
  Gdiplus::Image *m_pngLogo;
private:
  CToolBarCOM* m_CToolBarCOM;
  bool m_bMouseDown;
protected:
    CBitmap m_bmp;
};

#endif