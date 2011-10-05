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

#include "stdafx.h"
#include "HTMLEdit.h"
#include "MainFrm.h"
#include <imgcap_setting.h>
#include <copydatamsg.h>

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  ON_WM_CREATE()
END_MESSAGE_MAP()

MYMODE mode = HIDE_WINDOW_TO_THUMB;

const wchar_t* LocalHTML=NULL;
const wchar_t* WndTitle=NULL;
const wchar_t* absThumbPath=NULL;

void NotifyWnd(){
  COPYDATASTRUCT cds={CPYDATA_HTMLEDITOR_TO_PROGRESSDLG,NULL,NULL  };
  HWND hwndTo = ::FindWindow(NULL,WndTitle);
  if (hwndTo){
    ::SendMessage(hwndTo,WM_COPYDATA,(WPARAM)0,(LPARAM)(LPVOID)&cds);
  }
}

static UINT indicators[] ={
  ID_SEPARATOR,// status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
};

CMainFrame::CMainFrame(){}
CMainFrame::~CMainFrame(){}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct){
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;
  if (!m_wndFormatBar.CreateEx(this) || !m_wndFormatBar.LoadToolBar(IDR_FORMATBAR)){
    TRACE0("Failed to create formatbar\n");
    return -1;    // fail to create
  }
  if (!m_wndToolBar.CreateEx(this) ||  !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)){
    TRACE0("Failed to create toolbar\n");
    return -1;      // fail to create
  }
  if (!m_wndReBar.Create(this) ||  !m_wndReBar.AddBar(&m_wndFormatBar) || !m_wndReBar.AddBar(&m_wndToolBar)){
    TRACE0("Failed to create rebar\n");
    return -1;      // fail to create
  }
  if (!m_wndStatusBar.Create(this) ||  !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT))){
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }
  m_wndFormatBar.SetBarStyle(m_wndFormatBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_ALIGN_LEFT);
  m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_ALIGN_LEFT |CBRS_SIZE_FIXED );
  return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs){
  // HTMLEDIT.exe "C:\\xxx.html" "WNDTITE" "C:\\YYY.jpg" [HIDE|SHOW]- generate thumb and then close window
  if (__argc!=5){::PostQuitMessage(0);}
  LocalHTML    = __targv[1];
  WndTitle     = __targv[2];
  absThumbPath = __targv[3];
  if (wcscmp(__targv[4],_T("HIDE"))==0){
    mode = HIDE_WINDOW_TO_THUMB;
    cs.cx=Thumb_Width*4;
    cs.cy=Thumb_Height*4;
    cs.x =-Thumb_Width*10;
    cs.y =-Thumb_Height*10;
    cs.dwExStyle = WS_EX_TOOLWINDOW|SWP_HIDEWINDOW;
  }else if (wcscmp(__targv[4],_T("SHOW"))==0){
    mode = SHOW_WINDOW_TO_EDIT;
  }else{
    ::PostQuitMessage(0);
  }
  if( !CFrameWnd::PreCreateWindow(cs) )
    return FALSE;
  return TRUE;
}
// CMainFrame message handlers