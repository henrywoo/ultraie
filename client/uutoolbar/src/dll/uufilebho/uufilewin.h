#ifndef __UUTOOLBAR_TOOLBARWND__
#define __UUTOOLBAR_TOOLBARWND__

#include <set>
#include "../../dll/6beeres/resource.h"

#include "ToolBar6BeeMenu.h"

#include "../../dll/mousehook/mousehook.h"

#include "cppsqlite3.h"
#include "6bees_iwebbrowser2.h"
#include "6bees_const.h"

_COM_SMARTPTR_TYPEDEF(IHTMLElement, __uuidof(IHTMLElement));

const unsigned int TBBUTTON_CLASSIC_PADDING_X = 6;
const unsigned int TBBUTTON_CLASSIC_PADDING_Y = 8;

class CToolBarWnd:
  public CWindowImpl<CToolBarWnd, CToolBarCtrl>
{
public:
  DECLARE_WND_SUPERCLASS(_6bees_const::kWND_NAME_TBWND,CToolBarWnd::GetWndClassName())

  BEGIN_MSG_MAP(CToolBarWnd)
    MESSAGE_HANDLER(WM_SETTINGCHANGE, OnWM_SETTINGCHANGE)

    CHAIN_MSG_MAP_ALT_MEMBER(m_CToolBarCombo,    1)
    CHAIN_MSG_MAP_ALT_MEMBER(m_CToolBarSEButton, 2)

    CHAIN_MSG_MAP_MEMBER(pPopup)
    MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)
    MESSAGE_HANDLER(WM_CREATE,OnCreate)
    MESSAGE_HANDLER(WM_SEP_TOOLBAR_MOUSEMOVE,OnSepMove)
    MESSAGE_HANDLER(kWM_TBCOMBOSEARCH_WEB, OnToolBarSearch)
    MESSAGE_HANDLER(WM_HOOKKEYBOARD_SHOWSEARCH, OnShowHBar)

    // Clicking Toolbar button will tiger this message
    MESSAGE_HANDLER(WM_COMMAND, OnCommand)
    // todo: handle the rebar chevron messages, does not require reflection (refer to Toolband_src demo)
    //ALT_MSG_MAP(1) // Parent window messages
    //NOTIFY_CODE_HANDLER(TBN_DROPDOWN, OnToolbarDropDown)
    //REFLECT_NOTIFICATIONS()//return ToolBarLogo's ownerdraw message, must put at the last place-- also reflect all
  END_MSG_MAP()

  LRESULT OnCreate   (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnSepMove  (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnCommand  (UINT,WPARAM,LPARAM,BOOL&);  
  LRESULT OnCopyData  (UINT,WPARAM,LPARAM,BOOL&);  
  LRESULT OnToolBarSearch(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnShowHBar(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnInitMenuPopup(UINT,WPARAM,LPARAM,BOOL&);

  /// @notice - this message can make logo button small, so filter it
  LRESULT OnWM_SETTINGCHANGE(UINT,WPARAM,LPARAM,BOOL& b){b=true;return S_OK;}

  LRESULT OnSetFocus(WORD,WORD,HWND,BOOL&);
  //LRESULT OnUploadFolder(WORD,WORD,HWND,BOOL&);
  LRESULT OnUploadFiles();
  //LRESULT OnToolbarDropDown(int idCtrl,LPNMHDR pnmh,BOOL& bHandled);

  CToolBarWnd();
  virtual ~CToolBarWnd();

  void SetBrowser(_6bees_html::c6beewb2ptr *);
  CToolBarCombo& GetToolBarCombo();

  DWORD sepmovetime;

  //For chevron
  CReBarCtrl        m_ctlRebar;
  CContainedWindow  m_RebarContainer;
  HWND FindRebar(HWND hwndStart);
  BOOL SetBandRebar();

  //void ShowFlv(LPWSTR,BOOL);
  void RefreshTButtons(bool);

private:
  _6bees_html::c6beewb2ptr* pwb_;

  int searchboxlen;
  enum{ButtonNumber=10};
  TBBUTTON Buttons[ButtonNumber];
  int bcount;
  int m_ncy;

  CImageList m_6beeImageList;
  HIMAGELIST m_hTBLogoImageList;

  void Logout();

  RECT wndRect_Logo;
  RECT wndRect_SELogo;

  C6BeeMenu pPopup;
  CToolBarCombo    m_CToolBarCombo;
  CToolBarSELogo   m_CToolBarSELogo;
  CToolBarSEButton m_CToolBarSEButton;
  CToolBarSep      m_CToolBarSep;

  HPEN    g_hRectanglePen;// NOTE: take care of resource leak
  HCURSOR g_hCursorSearchWindow;

  int HTMLElementSourceID;
  HWND g_hwndFoundWindow;

  ATL::CString CacheHTMLFilename;/// @todo (wufuheng@gmail.com): Is it really necessary?
  RECT CaptureRect;// for image capture
  IHTMLElementPtr elementHit;//move mouse and get the IHTMLElementPtr and then we can get innerhtml!
  HFONT boldfont;
  void CheckVersion(bool passive=true);

  void setlogobutton();
  void setspaceforcombo();
};




#endif
