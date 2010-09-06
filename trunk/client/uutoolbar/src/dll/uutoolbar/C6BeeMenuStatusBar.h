#ifndef __UUTOOLBAR_C6BEEMENUSTATUSBAR__
#define __UUTOOLBAR_C6BEEMENUSTATUSBAR__

#include "C6BeeAnimationPane.h"
#include <6BeeContextMenu.h>
#include <vector>
#include <map>
#include "../../dll/6beeres/resource.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "6bees_iwebbrowser2.h"
#include "../../dll/mousehook/mousehook.h"
using std::vector;

class C6BeeMenuStatusBar:
  public CMultiPaneStatusBarCtrlImpl<C6BeeMenuStatusBar>,
  public CAnimationInPaneImpl<C6BeeMenuStatusBar>,
  public CCoolContextMenu< C6BeeMenuStatusBar >
{
public:
  DECLARE_WND_SUPERCLASS(NULL, GetWndClassName())
  BEGIN_MSG_MAP(C6BeeMenuStatusBar)
    COMMAND_CODE_HANDLER(0, OnClickMenuItem)

    MESSAGE_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
    MESSAGE_HANDLER(WM_RBUTTONDOWN,OnRButtonDown)
    MESSAGE_RANGE_HANDLER(WM_HOOK,WM_HOOKKEYBOARD_SHOWSEARCH,OnHookMsg)

    CHAIN_MSG_MAP(CCoolContextMenu<C6BeeMenuStatusBar>)
    CHAIN_MSG_MAP(CAnimationInPaneImpl<C6BeeMenuStatusBar>)
    CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<C6BeeMenuStatusBar>)
  END_MSG_MAP()

  C6BeeMenuStatusBar();
  ~C6BeeMenuStatusBar();
  void CreatePanes();

  LRESULT OnClickMenuItem(WORD,WORD,HWND,BOOL&);
  LRESULT OnRButtonDown(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonDown(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnHookMsg(UINT,WPARAM,LPARAM,BOOL&);

  //////////////////////////////////////////////////////////////////////////////
  LRESULT OnInitMenuPopup(UINT,WPARAM,LPARAM,BOOL&);
  void AssociateImage(CMenuItemInfo& mii, coolMenuItemData * pMI);
  //////////////////////////////////////////////////////////////////////////
  BOOL UpdatePanesLayout();

  void opennewtab(const char* url,bool new_tab=true) const;
  void opennewtab(const wchar_t* url,bool new_tab=true) const;

private:
  HICON hi_setting;
  CMenu m_MyMenu;
  const wchar_t* getshortweburl(bool iswholeurl=true);

public:
  DWORD tid;
  _6bees_html::c6beewb2ptr* pwb_;

  _6bees_html::c6beewb2ptr* GetPWB2();
  wchar_t* GetHostname();
  wchar_t* GetWebURL();
  wchar_t* GetWebTitle();
  int  AddToImgList(HBITMAP h,COLORREF cmsk);
  bool RunJs(const wchar_t* jscode);
  bool InsertAdjacentHTML(const wchar_t* htmlcode,const wchar_t* pos);
  IHTMLElement* GetElementByIDName(const char* _name);

  // thread id -> C6BeeMenuStatusBar*
  static std::map<DWORD,C6BeeMenuStatusBar*> tid2menustatusbarptr_;

  // for use of plug-ins, coz DLL can not transfer map, so have to use array
  // Maximum support 2048 pointer to C6BeeMenuStatusBar
  static std::vector<C6BeeMenuStatusBar*> statusbarvec_;
};

#endif
