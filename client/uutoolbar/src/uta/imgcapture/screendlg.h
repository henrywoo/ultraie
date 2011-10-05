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

#ifndef __UUPLUGIN_IMGCAPTURE_SCREENDLG__
#define __UUPLUGIN_IMGCAPTURE_SCREENDLG__

#include <windows.h>
#include "resource.h"
#include <shlobj.h>
#include <atlstr.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlgdi.h>
#include <imgcap_setting.h>

enum MOUSESTATUS{ARROW,N1,N2,N3,N4,N5,N6,N7,N8};
#define MSG_FINISH 5678

class CScreenDlg:public CDialogImpl<CScreenDlg>
{
public:
  enum { IDD = IDD_DIALOG_SCREEN };

  BEGIN_MSG_MAP(CScreenDlg)
    MESSAGE_HANDLER(WM_INITDIALOG,   OnInitDialog)
    MESSAGE_HANDLER(WM_SETCURSOR,    OnSetCursor)
    MESSAGE_HANDLER(WM_MOUSEMOVE,    OnMouseMove)
    MESSAGE_HANDLER(WM_LBUTTONUP,    OnLButtonUp)
    MESSAGE_HANDLER(WM_LBUTTONDOWN,  OnLButtonDown)  
    MESSAGE_HANDLER(WM_LBUTTONDBLCLK,OnLButtonDblClk)
    MESSAGE_HANDLER(WM_RBUTTONUP,    OnRButtonUp)
    MESSAGE_HANDLER(WM_PAINT,        OnPaint)
    MESSAGE_HANDLER(WM_HOTKEY,OnHotKey)
  END_MSG_MAP()

  CScreenDlg(void);
  ~CScreenDlg(void);

  LRESULT OnInitDialog(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnMouseMove(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnSetCursor(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnPaint(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonUp(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonDown(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonDblClk(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnRButtonUp(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnHotKey(UINT,WPARAM,LPARAM,BOOL&);

  void DisplayInfo();
  void SaveSelectBmp();
public:
  ATL::CString Thumbpath;
private:
  SIZE  m_size;// screen size
  CDC    m_dcMem;
  CBitmap  m_bmpScreen; // the screen bitmap
  CBitmap m_bmpInfoBK;
  POINT  m_ptMouse;   //mouse position

  BOOL m_SelectingNow;  // is we are selecting the cap area now?
  BOOL m_SelectionFinished;    // is selecting cap area finished?
  BOOL ReadyforAdjust;// is ready to adjust the cap area?
  BOOL BDown;              // is mouse left button down?

  RECT    m_rectSelect;
  RECT    m_rectSelect_helper;
  RECT  m_rectInfo;

  HCURSOR  m_hCursor;

  int     rect_w;
  int     rect_h;

  int realleftx;
  int realtopy ;
  int realrightx ;
  int realbottomy;

  POINT   ipoint;
  RECT   imrect;
  HFONT thinfont;
  HFONT boldfont;

  CPen penLine1;
  CPen penLine2;

  CPen pen_Border;

  MOUSESTATUS ms;

  // For shadow
  HBITMAP S1;
  HBITMAP S2;
  HBITMAP S3;
  HBITMAP S4;
  HBITMAP S5;
};

#endif