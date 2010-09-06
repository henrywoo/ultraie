#ifndef __UUTOOLBAR_TOOLBAR_REFLECTIONWND__
#define __UUTOOLBAR_TOOLBAR_REFLECTIONWND__

#include <hash_map>
using namespace stdext;

#include "ToolBarWnd.h"
#include "C6BeeStatusWnd.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "6bees_iwebbrowser2.h"
#include "6bees_const.h"

struct IEFrameHook{  
  virtual void IEFrameSizeChanging(int) = 0;
  virtual void ShowHideStatusBar(bool) = 0;
  virtual void Setoffset_var() = 0;
};

class ReflectionWnd:
  public CWindowImpl<ReflectionWnd>,
  public IEFrameHook
{
public:
  DECLARE_WND_SUPERCLASS( _6bees_const::kWND_NAME_REFLECTION,ReflectionWnd::GetWndClassName() )
  BEGIN_MSG_MAP(ReflectionWnd)
    MESSAGE_HANDLER(WM_COPYDATA, OnCopyData)
    MESSAGE_HANDLER(WM_CREATE,OnCreate)
    CHAIN_MSG_MAP_MEMBER(m_CToolBarWnd)
    //REFLECT_NOTIFICATIONS()//???
  END_MSG_MAP()

  LRESULT OnCreate(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnCopyData(UINT,WPARAM,LPARAM,BOOL&);

  ReflectionWnd(void);
  virtual ~ReflectionWnd(void);

  inline CToolBarWnd& GetToolBar(){ return m_CToolBarWnd;};
private:
  CToolBarWnd m_CToolBarWnd;
  DWORD tid_;
public:
  _6bees_html::c6beewb2ptr* pw;

  C6BeeStatusWnd m_6BeeStatusWnd;

  HWND RawIEtabWnd,RawStatusWnd;
  RECT tabwndrect;
  RECT RawStatusWndRect;
  int offset;

  VOID    CreateMinuStatusWindow();
  HRESULT CreateMinuStatusWindow_V6();
  HRESULT CreateMinuStatusWindow_V7();
  UINT themechange_count_v6;

  void IEFrameSizeChanging(int cy);
  void ShowHideStatusBar(bool);
  void Setoffset_var();
  void ProgressChange(long,long);
  void SetProgress(BOOL);
};

#endif