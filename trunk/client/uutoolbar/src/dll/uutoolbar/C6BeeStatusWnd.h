#ifndef __UUTOOLBAR_C6BEEMENUSTATUSWND__
#define __UUTOOLBAR_C6BEEMENUSTATUSWND__

#include "6beeprogressbar.h"
#include "../../dll/mousehook/mousehook.h"
#include "6bees_iwebbrowser2.h"
#include "pluginmng.h"

extern int status_height;

class C6BeeMenuStatusBar;

#define WM_DELETEPANE WM_USER+1111
#define WM_ADDPANE WM_USER+1112

class C6BeeStatusWnd:
  public CWindowImpl<C6BeeStatusWnd>
{  
public:
  DECLARE_WND_SUPERCLASS(_T("C6BeeStatusWnd"), GetWndClassName())

  C6BeeStatusWnd();
  ~C6BeeStatusWnd();
public:
  BEGIN_MSG_MAP(C6BeeStatusWnd)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_SETCURSOR,  OnSetCursor)
    MESSAGE_HANDLER(WM_PARENTNOTIFY,OnParentNotify)
    MESSAGE_HANDLER(WM_DELETEPANE,OnDeletePane)
    MESSAGE_HANDLER(WM_ADDPANE,OnAddPane)
  END_MSG_MAP()

public:
  C6BeeMenuStatusBar m_StatusBar;
  C6BeeProgressCtrl  m_Progress;
  RECT SBRect;

public:
  LRESULT OnParentNotify(UINT,WPARAM,LPARAM,BOOL&); 
  LRESULT OnSetCursor(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonDown(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnCreate(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnMouseMessage(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnDeletePane(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    int length=(int) lParam;

    RECT r0={},r1={};
    GetWindowRect(&r0);
    ::GetWindowRect(GetParent(),&r1);
    RECT r3;
    r3.left  = r0.left-r1.left+length;
    r3.top  = r0.top-r1.top;
    r3.right= r3.left+nsplugin::PluginMng::R().GetTotalWidth();
    r3.bottom = r3.top+status_height;
    MoveWindow(&r3,true);
    return 0;
  }

  LRESULT OnAddPane(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    int length=(int) lParam;

    RECT r0={},r1={};
    GetWindowRect(&r0);
    ::GetWindowRect(GetParent(),&r1);
    RECT r3;
    r3.left  = r0.left-r1.left-length;
    r3.top  = r0.top-r1.top;
    r3.right= r3.left+nsplugin::PluginMng::R().GetTotalWidth();
    r3.bottom = r3.top+status_height;
    MoveWindow(&r3,true);//move wnd but not control!
    return 0;
  }

  void setPWebBrowser(_6bees_html::c6beewb2ptr* p){
    m_StatusBar.pwb_ = p;
  }

private:
  bool canshowhandcursor;
};

#endif//__MY_NOTE_BHO_H__
