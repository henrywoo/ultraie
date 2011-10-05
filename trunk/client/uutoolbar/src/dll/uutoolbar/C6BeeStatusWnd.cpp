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

#include "C6BeeStatusWnd.h"

#include "6bees_util.h"
#include "6BeeFunctionHook.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#include "pluginmng.h"

#pragma comment(lib,"6beecommond.lib")

using namespace _6beed_util;

extern int status_height;

C6BeeStatusWnd::C6BeeStatusWnd():
canshowhandcursor(true){
  SBRect.left = -1;
  SBRect.top = 0;
  SBRect.right = nsplugin::PluginMng::R().GetTotalWidth();
  SBRect.bottom = 20;
}

C6BeeStatusWnd::~C6BeeStatusWnd(){
  if(IsWindow())
    DestroyWindow();
}

LRESULT C6BeeStatusWnd::OnParentNotify(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  if(WM_RBUTTONDOWN==LOWORD(wParam)){
    canshowhandcursor = false;
  }
  bHandled = false;
  return 0;
}


LRESULT C6BeeStatusWnd::OnSetCursor(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  static HCURSOR hcur = LoadCursor(NULL,IDC_HAND);
  POINT pos={};
  GetCursorPos(&pos);
  RECT rect={};
  this->GetWindowRect(&rect);
  int xpos= pos.x - rect.left;
  //if (hcur!=NULL &&  willpopmenu(m_StatusBar.point_in_range(xpos)) && !m_StatusBar.popped){
  if (hcur!=NULL &&  nsplugin::PluginMng::R().IsPointOverIcon(xpos) && !m_StatusBar.popped){
    SetCursor(hcur);
    return true;
  }else{
    SetMsgHandled(false);
    return false;
  }
}

LRESULT C6BeeStatusWnd::OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){

  // 1. create status bar window
  C6BeeMenuStatusBar::tid2menustatusbarptr_[GetCurrentThreadId()] = &m_StatusBar;
  C6BeeMenuStatusBar::statusbarvec_.push_back(&m_StatusBar);

  m_StatusBar.Create(m_hWnd, SBRect, NULL,
    WS_CHILD|WS_VISIBLE|SB_BOTTOM|SBT_TOOLTIPS|BS_OWNERDRAW);
  m_StatusBar.GetSystemSettings();
  m_StatusBar.CreatePanes();

  RECT SBRect2;
  m_StatusBar.GetRect(1,&SBRect2);
  SBRect2.left   += 20;//add a icon in the left- Setting
  if(status_height==20){
    SBRect2.right  -= 2;
    SBRect2.top    += 1;
    SBRect2.bottom -= 1;
  }else{
    SBRect2.right  -= 5;
    SBRect2.top    += 1;
    SBRect2.bottom -= 1;
  }

  // 2. create progress window - over status bar window
  m_Progress.Create(m_hWnd, SBRect2, _T(""), WS_CHILD|WS_VISIBLE/*|PBS_SMOOTH*/);

  //RECT rc;
  //::GetClientRect(m_StatusBar.m_hWnd, &rc);
  return 0;
}

LRESULT C6BeeStatusWnd::OnMouseMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){ 
  bHandled = FALSE;
  return 1; 
}
