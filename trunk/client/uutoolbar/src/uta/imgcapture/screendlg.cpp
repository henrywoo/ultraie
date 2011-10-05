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

#include "screendlg.h"

#include "atlmisc.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#include <time.h>
#include "ImageRender.h"
#include "6bees_util.h"
#include "6bees_gdi.h"
#include <6bees_lang.h>
#include "util.h"

using namespace _6bees_util;

RECT RIGHT_RECT={0,INFO_OFFSET,0,INFO_OFFSET + INFO_HEIGHT};

//////////////////////////////////////////////////////////////////////////
CScreenDlg::CScreenDlg():
ipoint(),imrect(),ReadyforAdjust(FALSE),BDown(FALSE),m_SelectionFinished(FALSE),m_SelectingNow(FALSE),m_hCursor(0),
rect_w(0),rect_h(0),realleftx(0),realtopy(0),realrightx(0),realbottomy(0),m_rectSelect(),m_rectSelect_helper(),ms(ARROW)
{
  m_rectInfo=LEFT_RECT;
}

CScreenDlg::~CScreenDlg(void){
  ::DeleteObject(thinfont);
  ::DeleteObject(boldfont);
}

LRESULT CScreenDlg::OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  ShowWindow(SW_HIDE);
  CDC dcScreen;
  dcScreen.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
  m_size.cx = GetDeviceCaps(dcScreen.m_hDC, HORZRES);
  m_size.cy = GetDeviceCaps(dcScreen.m_hDC, VERTRES);
  m_dcMem.CreateCompatibleDC(dcScreen.m_hDC);
  m_bmpScreen.CreateCompatibleBitmap(dcScreen.m_hDC, m_size.cx,m_size.cy);
  HBITMAP pOldBmp = m_dcMem.SelectBitmap(m_bmpScreen.m_hBitmap);
  m_dcMem.BitBlt(0,0,m_size.cx, m_size.cy, dcScreen.m_hDC, 0, 0, SRCCOPY);
  RECT rect={0,0,m_size.cx, m_size.cy};
  MoveWindow(&rect);
  ShowWindow(SW_SHOW);
  RIGHT_RECT.left  = m_size.cx - INFO_WIDTH-INFO_OFFSET;
  RIGHT_RECT.right = m_size.cx-INFO_OFFSET;

  m_bmpInfoBK = (HBITMAP)c6beeres::R().GetImg(IDB_BITMAP_INFOBK,IMAGE_BITMAP,INFO_WIDTH,INFO_HEIGHT);

  S1= (HBITMAP)c6beeres::R().GetImg(IDB_S1,IMAGE_BITMAP,shadow_size,shadow_size);
  S2= (HBITMAP)c6beeres::R().GetImg(IDB_S2,IMAGE_BITMAP,shadow_size,shadow_size);
  S3= (HBITMAP)c6beeres::R().GetImg(IDB_S3,IMAGE_BITMAP,shadow_size,shadow_size);
  S4= (HBITMAP)c6beeres::R().GetImg(IDB_S4,IMAGE_BITMAP,shadow_size,shadow_size);
  S5= (HBITMAP)c6beeres::R().GetImg(IDB_S5,IMAGE_BITMAP,shadow_size,shadow_size);

  thinfont  = ::CreateFont(15,0,0,0,FW_THIN,false,FALSE,FALSE,DEFAULT_CHARSET,OUT_STRING_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));
  boldfont= ::CreateFont(15,0,0,0,FW_BOLD,false,FALSE,FALSE,DEFAULT_CHARSET,OUT_STRING_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));
  penLine1.CreatePen(PS_SOLID, 1, SELECT_RECTCOLOR);
  penLine2.CreatePen(PS_SOLID, 1, CROSS_LINECOLOR);
  bool border=false,shadow=false;
  COLORREF bcolor=0;
  nsplugin::GetImgCapParam(border,shadow,bcolor);
  pen_Border.CreatePen(PS_SOLID, border_size, bcolor);
  return 0;
}

LRESULT CScreenDlg::OnSetCursor(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  switch (ms){
  case ARROW:
    m_hCursor = BDown?LoadCursor(NULL,IDC_SIZEALL):(HCURSOR)c6beeres::R().GetImg(IDC_CURSOR_SEARCH_WINDOW,IMAGE_CURSOR,32,32);
    break;
  case N1:
    m_hCursor = LoadCursor(NULL,IDC_SIZENWSE);
    break;
  case N5:
    m_hCursor = LoadCursor(NULL,IDC_SIZENWSE);
    break;
  case N3:
    m_hCursor = LoadCursor(NULL,IDC_SIZENESW);
    break;
  case N7:
    m_hCursor = LoadCursor(NULL,IDC_SIZENESW);
    break;
  case N2:
    m_hCursor = LoadCursor(NULL,IDC_SIZENS);
    break;
  case N6:
    m_hCursor = LoadCursor(NULL,IDC_SIZENS);
    break;
  case N4:
    m_hCursor = LoadCursor(NULL,IDC_SIZEWE);
    break;
  case N8:
    m_hCursor = LoadCursor(NULL,IDC_SIZEWE);
    break;
  }
  if (m_hCursor){
    SetCursor(m_hCursor);
    DeleteObject(m_hCursor);
  }else{
    SetMsgHandled(false);
  }
  return TRUE;
}

LRESULT CScreenDlg::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  CPaintDC dc(m_hWnd);
  dc.StretchBlt(0,0,m_size.cx, m_size.cy, m_dcMem.m_hDC, 0,0,m_size.cx, m_size.cy,SRCCOPY);
  return 0;
}

LRESULT CScreenDlg::OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  m_ptMouse.x = GET_X_LPARAM(lParam);
  m_ptMouse.y = GET_Y_LPARAM(lParam);
  if(!m_SelectionFinished && m_SelectingNow){
    m_rectSelect.right  = m_ptMouse.x;
    m_rectSelect.bottom = m_ptMouse.y;
  }
  // move to adjust cap area
  if(ReadyforAdjust && m_SelectionFinished && !m_SelectingNow){
    if (BDown){
      if (ipoint.x == 0 && ipoint.y == 0){
        ipoint = m_ptMouse;
        imrect = m_rectSelect;
      }else{
        switch (ms){
      case ARROW:{
        int tmp1=imrect.left + m_ptMouse.x - ipoint.x;
        int tmp2=tmp1+rect_w;
        int tmp3=imrect.top + m_ptMouse.y - ipoint.y;
        int tmp4=tmp3+rect_h;
        // make sure the cap area is not beyond the range of area of the screen:-)
        if (tmp1>=0&&tmp3>=0    &&  tmp2<m_size.cx&&tmp4<m_size.cy){
          m_rectSelect.left      = tmp1;
          m_rectSelect.right    = tmp2;
          m_rectSelect.top      = tmp3;
          m_rectSelect.bottom= tmp4;
        }
        break;}
      case N1:
        m_rectSelect.left  = imrect.left + m_ptMouse.x - ipoint.x;
        m_rectSelect.top  = imrect.top + m_ptMouse.y - ipoint.y;
        break;
      case N2:
        m_rectSelect.top   = imrect.top + m_ptMouse.y - ipoint.y;
        break;
      case N3:
        m_rectSelect.right =  imrect.right + m_ptMouse.x - ipoint.x;
        m_rectSelect.top   = imrect.top + m_ptMouse.y - ipoint.y;
        break;
      case N4:
        m_rectSelect.right =  imrect.right + m_ptMouse.x - ipoint.x;
        break;
      case N5:
        m_rectSelect.right =  imrect.right + m_ptMouse.x - ipoint.x;
        m_rectSelect.bottom=   imrect.bottom + m_ptMouse.y - ipoint.y;
        break;
      case N6:
        m_rectSelect.bottom=  imrect.bottom + m_ptMouse.y - ipoint.y;
        break;
      case N7:
        m_rectSelect.left  = imrect.left + m_ptMouse.x - ipoint.x;
        m_rectSelect.bottom=  imrect.bottom + m_ptMouse.y - ipoint.y;
        break;
      case N8:
        m_rectSelect.left  = imrect.left + m_ptMouse.x - ipoint.x;
        break;
        }
      }
    }else{
      int w=abs(m_rectSelect.right-m_rectSelect.left);
      int h=abs(m_rectSelect.bottom-m_rectSelect.top);
      if (w<MINRECT || h<MINRECT){
        ms=N5;
      }else{
        RECT r1={m_rectSelect.left-5,m_rectSelect.top-5,m_rectSelect.left+5,m_rectSelect.top+5};
        RECT r2={m_rectSelect.left+5,m_rectSelect.top-5,m_rectSelect.right-5,m_rectSelect.top+5};
        RECT r3={m_rectSelect.right-5,m_rectSelect.top-5,m_rectSelect.right+5,m_rectSelect.top+5};
        RECT r4={m_rectSelect.right-5,m_rectSelect.top+5,m_rectSelect.right+5,m_rectSelect.bottom-5};
        RECT r5={m_rectSelect.right-5,m_rectSelect.bottom-5,m_rectSelect.right+5,m_rectSelect.bottom+5};
        RECT r6={m_rectSelect.left+5,m_rectSelect.bottom-5,m_rectSelect.right-5,m_rectSelect.bottom+5};
        RECT r7={m_rectSelect.left-5,m_rectSelect.bottom-5,m_rectSelect.left+5,m_rectSelect.bottom+5};
        RECT r8={m_rectSelect.left-5,m_rectSelect.top+5,m_rectSelect.left+5,m_rectSelect.bottom-5};
        if(PtInRect(&r1,m_ptMouse))ms=N1;
        else if(PtInRect(&r2,m_ptMouse)) ms=N2;
        else if(PtInRect(&r3,m_ptMouse)) ms=N3;
        else if(PtInRect(&r4,m_ptMouse)) ms=N4;
        else if(PtInRect(&r5,m_ptMouse)) ms=N5;
        else if(PtInRect(&r6,m_ptMouse)) ms=N6;
        else if(PtInRect(&r7,m_ptMouse)) ms=N7;
        else if(PtInRect(&r8,m_ptMouse)) ms=N8;
        else ms=ARROW;
      }
    }
  }
  DisplayInfo();
  return 0;
}

LRESULT CScreenDlg::OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  POINT point={GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};
  if(!m_SelectionFinished){
    ReadyforAdjust = FALSE;
    m_SelectingNow = TRUE;
    m_rectSelect.left = m_rectSelect.right = point.x;
    m_rectSelect.top = m_rectSelect.bottom = point.y;
    m_rectSelect_helper=m_rectSelect;
  }
  BDown = TRUE;
  return 0;
}


LRESULT CScreenDlg::OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  // Double click will fire this function.
  POINT point={GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};
  if(!m_SelectionFinished && m_SelectingNow && (m_rectSelect_helper.right!=point.x || m_rectSelect_helper.bottom!=point.y)){
    m_SelectingNow   = FALSE;
    m_SelectionFinished    = TRUE;
    if (!ReadyforAdjust){
      ReadyforAdjust = TRUE;
      ::RegisterHotKey(m_hWnd,HOTKEYID_COPYIMG,MOD_CONTROL,'C');
    }
  }else{
    m_SelectingNow = FALSE;
  }
  ipoint.x=ipoint.y=0;
  imrect = m_rectSelect;
  BDown = FALSE;
  return 0;
}

LRESULT CScreenDlg::OnLButtonDblClk(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  ipoint.x = ipoint.y = 0;
  if (ReadyforAdjust){
    SaveSelectBmp();
    ReadyforAdjust=FALSE;
    EndDialog(MSG_FINISH);
  }
  return 0;
}

LRESULT CScreenDlg::OnRButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  m_SelectionFinished = FALSE;
  ipoint.x = ipoint.y = 0;
  ms=ARROW;
  SendMessage(WM_SETCURSOR);
  if (ReadyforAdjust){
    DisplayInfo();
    ReadyforAdjust=FALSE;
    ::UnregisterHotKey(m_hWnd,HOTKEYID_COPYIMG);
  }else{
    EndDialog(0);
  }
  return 0;
}

LRESULT CScreenDlg::OnHotKey(UINT,WPARAM,LPARAM,BOOL&){
  m_SelectionFinished = TRUE;
  /*  m_SelectingNow   = FALSE;
  ReadyforAdjust = FALSE;
  ipoint.x = ipoint.y = 0;
  SendMessage(WM_SETCURSOR);
  */
  ::UnregisterHotKey(m_hWnd,HOTKEYID_COPYIMG);
  SaveSelectBmp();
  EndDialog(0);
  return 0;
}

void CScreenDlg::SaveSelectBmp(){
  if(!m_SelectionFinished)
    return;
  CDC dcSave;
  dcSave.CreateCompatibleDC(NULL);
  CBitmap bitmap;
  BITMAP bm;
  m_bmpScreen.GetBitmap(&bm);
  bool shadow=false,border=false;
  COLORREF bcolor=0;
  nsplugin::GetImgCapParam(border,shadow,bcolor);
  int shadow_offset=(shadow?shadow_size:0);
  int border_offset =(border?border_size:0);
  bm.bmWidth  = rect_w+shadow_offset+border_offset*2;//shadow picture's length is 6 pixel.
  bm.bmHeight = rect_h+shadow_offset+border_offset*2;
  bitmap.CreateBitmapIndirect(&bm);
  HBITMAP pOldBmp = dcSave.SelectBitmap(bitmap.m_hBitmap);
  dcSave.BitBlt(border_offset,border_offset,rect_w,rect_h,m_dcMem.m_hDC,realleftx,realtopy,SRCCOPY);

  using namespace Gdiplus;
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR           gdiplusToken;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  using _6bees_util::ultrapath;
  const wstring& upath = ultrapath::R().get(ultrapath::picdir);
  if(!_6bees_util::MakeSureDir(upath.c_str())){
    return;
  }
  Thumbpath.Format(_T("%sScreenCap_%lu_thumb.jpg"),upath.c_str(),time(0));
  RECT r={realleftx,realtopy,realleftx+rect_w,realtopy+rect_h};
  _6bees_gdi::GenerateThumb(Thumbpath,m_dcMem.m_hDC,r);

  // more fancy effects:-)
  if (border){
    dcSave.SelectPen(pen_Border.m_hPen);

    dcSave.MoveTo(0,0);
    dcSave.LineTo(0, rect_h+border_offset);
    dcSave.LineTo(rect_w+border_offset, rect_h+border_offset);
    dcSave.LineTo(rect_w+border_offset, 0);
    dcSave.LineTo(0, 0);
  }
  if (shadow){
    _6bees_gdi::_6BeeAddShadow(S1,S2,S3,S4,S5,dcSave,rect_w,rect_h,border_offset,shadow_size);
  }

  HBITMAP hBmp = (HBITMAP)bitmap.Detach();
  dcSave.SelectBitmap(pOldBmp);
  if(::OpenClipboard(m_hWnd)){
    ::EmptyClipboard();
    ::SetClipboardData(CF_BITMAP, hBmp);
    ::CloseClipboard();
  }
  GdiplusShutdown(gdiplusToken);
}

void CScreenDlg::DisplayInfo(){
  if(PtInRect(&LEFT_RECT,m_ptMouse)){  
    m_rectInfo = RIGHT_RECT;
  }else if(PtInRect(&RIGHT_RECT,m_ptMouse)){
    m_rectInfo = LEFT_RECT;
  }
  CClientDC dc(m_hWnd);
  COLORREF color = dc.GetPixel(m_ptMouse);
  CDC dcMem;
  dcMem.CreateCompatibleDC(dc.m_hDC);
  CBitmap bmp;
  bmp.CreateCompatibleBitmap(dc.m_hDC, m_size.cx, m_size.cy);
  HBITMAP pOldBmp = dcMem.SelectBitmap(bmp.m_hBitmap);
  // 0. get the background
  dcMem.BitBlt(0,0,m_size.cx, m_size.cy,m_dcMem.m_hDC, 0,0,SRCCOPY);

  // 1. draw info pane
  CBrush brushBK;  brushBK.CreatePatternBrush(m_bmpInfoBK.m_hBitmap);
  CPen   penBK;  penBK.CreatePen(PS_SOLID,1,INFO_EDGECOLOR);//PS_SOLID,PS_DASHDOT
  HPEN  pOldPen   =  dcMem.SelectPen(penBK.m_hPen);
  HBRUSH  pOldBrush =  dcMem.SelectBrush(brushBK.m_hBrush);

  dcMem.SetBrushOrg(m_rectInfo.left, m_rectInfo.top);
  dcMem.Rectangle(&m_rectInfo);// draw rectangle
  dcMem.SelectBrush(pOldBrush);
  dcMem.MoveTo(m_rectInfo.left, m_rectInfo.top+content_height);
  dcMem.LineTo(m_rectInfo.right, m_rectInfo.top+content_height);
  dcMem.SelectPen(pOldPen);

  // 2.draw the text
  dcMem.SetBkMode(TRANSPARENT);
  dcMem.SetTextColor(INFO_TEXT);
  dcMem.SelectFont(thinfont);
  static WCHAR str1[255]={}, str2[255]={};
  int yoffset=TEXT_OFFSET+m_rectInfo.left;
  static const int fontsize=14;
  if(m_SelectingNow || m_SelectionFinished){
    wsprintf(str1,tstart, m_rectSelect.left, m_rectSelect.top);
    wsprintf(str2,tend, m_rectSelect.right, m_rectSelect.bottom);
    dcMem.TextOut(yoffset, INFO_HEADER_H, str1);
    dcMem.TextOut(yoffset, fontsize + INFO_HEADER_H, str2);
    dcMem.SelectFont(boldfont);
    dcMem.TextOut(yoffset, fontsize*2+5 + INFO_HEADER_H, info_capture);
    dcMem.TextOut(yoffset, fontsize*3+5 + INFO_HEADER_H, info_hotkey);
    dcMem.SelectFont(thinfont);
    dcMem.TextOut(yoffset, fontsize*4+15 + INFO_HEADER_H, info_deselect);
    dcMem.TextOut(yoffset, fontsize*5+15 + INFO_HEADER_H, info_adjust);
  }else{
    wsprintf(str1,tmouse, m_ptMouse.x, m_ptMouse.y);
    wsprintf(str2,tcolor, GetRValue(color),  GetGValue(color),  GetBValue(color));
    dcMem.TextOut(yoffset, INFO_HEADER_H, str1);
    dcMem.TextOut(yoffset, fontsize  + INFO_HEADER_H, str2);
    dcMem.SelectFont(boldfont);
    dcMem.TextOut(yoffset, fontsize*4+5+ INFO_HEADER_H, info_select);
    dcMem.TextOut(yoffset, fontsize*5+5 + INFO_HEADER_H, info_exit);
    dcMem.SelectFont(thinfont);
  }

  // 3. draw the selected region
  if(m_SelectingNow || m_SelectionFinished){
    HPEN pOldPen1 = dcMem.SelectPen(penLine1.m_hPen);
    _6bees_gdi::_6BeeDrawRect(dcMem,m_rectSelect,true,0);

    int width1 =abs(m_rectSelect.right-m_rectSelect.left);
    int height1=abs(m_rectSelect.bottom-m_rectSelect.top);

    realleftx= min(m_rectSelect.left,m_rectSelect.right);
    realtopy = min(m_rectSelect.top,m_rectSelect.bottom);
    realrightx  = max(m_rectSelect.left,m_rectSelect.right);
    realbottomy = max(m_rectSelect.top,m_rectSelect.bottom);

    m_rectSelect.left = realleftx;
    m_rectSelect.top = realtopy;
    m_rectSelect.right = realrightx;
    m_rectSelect.bottom = realbottomy;

    rect_w = abs(m_rectSelect.right-m_rectSelect.left);
    rect_h = abs(m_rectSelect.bottom-m_rectSelect.top);

    if (width1>MINRECT || height1>MINRECT){// if the cap area is too small(smaller than 30x30), we wont draw small rects.
      //draw size of capture area
      static WCHAR str3[20]={};
      wsprintf(str3, tsize, rect_w, rect_h);
      dcMem.SetBkMode(OPAQUE);
      dcMem.SetBkColor(SELECT_RECTCOLOR);
      dcMem.SetTextColor(INFO_TEXT);
      //dcMem.SelectFont(boldfont);
      dcMem.TextOutW(m_rectSelect.left+5, m_rectSelect.top+5, str3);

      RECT r1={realleftx-3,realtopy-3,realleftx+3,realtopy+3};
      _6bees_gdi::_6BeeDrawRect(dcMem,r1,true,0);

      r1.left += width1/2;r1.right += width1/2;  _6bees_gdi::_6BeeDrawRect(dcMem,r1,true,0);
      r1.left += width1/2;r1.right += width1/2;  _6bees_gdi::_6BeeDrawRect(dcMem,r1,true,0);

      r1.top += height1/2;r1.bottom += height1/2;  _6bees_gdi::_6BeeDrawRect(dcMem,r1,true,0);
      r1.top += height1/2;r1.bottom += height1/2;  _6bees_gdi::_6BeeDrawRect(dcMem,r1,true,0);

      r1.left -= width1/2;r1.right -= width1/2;  _6bees_gdi::_6BeeDrawRect(dcMem,r1,true,0);
      r1.left -= width1/2;r1.right -= width1/2;  _6bees_gdi::_6BeeDrawRect(dcMem,r1,true,0);

      r1.top -= height1/2;r1.bottom -= height1/2;  _6bees_gdi::_6BeeDrawRect(dcMem,r1,true,0);
    }
    dcMem.SelectPen(pOldPen1);
  }

  // 4. display all
  dc.StretchBlt(0,0,m_size.cx, m_size.cy, dcMem.m_hDC, 0,0,m_size.cx, m_size.cy,SRCCOPY);

  // 5. draw magnifying rect window
  RECT mag={yoffset,m_rectInfo.top+content_height+TEXT_OFFSET,m_rectInfo.right-TEXT_OFFSET,m_rectInfo.bottom-TEXT_OFFSET};
  int mag_w= abs(mag.right-mag.left);
  int mag_h= abs(mag.bottom-mag.top);
  const static int mouse_area_size=10;
  dc.StretchBlt(mag.left,mag.top,mag_w, mag_h,m_dcMem.m_hDC, m_ptMouse.x-mouse_area_size,m_ptMouse.y-mouse_area_size,mouse_area_size*2,mouse_area_size*2,SRCCOPY);

  // draw corss
  HPEN pOldPen2 = dc.SelectPen(penLine2.m_hPen);
  dc.MoveTo(mag.left+mag_w/2, mag.top);
  dc.LineTo(mag.left+mag_w/2, mag.bottom);
  dc.MoveTo(mag.left,mag.top+mag_h/2);
  dc.LineTo(mag.right,mag.top+mag_h/2);
  dc.SelectPen(pOldPen2);

  // draw magnifier window border
  dc.MoveTo(mag.left, mag.top);
  dc.LineTo(mag.right, mag.top);
  dc.LineTo(mag.right,mag.bottom);
  dc.LineTo(mag.left,mag.bottom);
  dc.LineTo(mag.left,mag.top);
  dc.SelectPen(pOldPen);
}
