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

#ifndef __UUTOOLBAR_HBARLOGO__
#define __UUTOOLBAR_HBARLOGO__

#include <atlbase.h>
#include <atlwin.h>

#include <atlapp.h>
#include <atlctrls.h>

#include <gdiplus.h>

class CH_HBarLogo :
  public CWindowImpl< CH_HBarLogo,CButton >
{
public:
  BEGIN_MSG_MAP(CToolBarLogo)
    MESSAGE_HANDLER(WM_PAINT,OnPaint)
  END_MSG_MAP()

  LRESULT OnPaint(UINT,WPARAM,LPARAM,BOOL&);
  BOOL SetPNG(UINT);
  //BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR,Image * &pImg);

  CH_HBarLogo();
  ~CH_HBarLogo();

protected:
  Gdiplus::GdiplusStartupInput    m_gdiplusStartupInput;
  ULONG_PTR        m_gdiplusToken;
  BOOL                    m_gdiplus_started;

  Gdiplus::Image*   m_pngLogo;
  CBitmap m_bmp;
  HFONT myfont2;
};

#endif