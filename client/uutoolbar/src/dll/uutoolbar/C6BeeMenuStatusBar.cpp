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


#include "C6BeeMenuStatusBar.h"
#include "6BeeFunctionHook.h"
#include "6bees_util.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#include "6bees_util.h"
#include <6bees_str.h>
#include "6bees_window.h"
#include "IceSingleton.h"
#include <imgcap_setting.h>

#include "pluginmng.h"
#include "globalvar.h"
#include "C6BeeStatusWnd.h"
#include "ToolBarCom.h"
#include <6bees_lang.h>

using namespace _6beed_util;
using namespace _6bees_util;
using namespace nsplugin;

#pragma comment(lib,"6beecommond.lib")

#ifdef __CHINESE__
#define tsetting L"¡˘÷ª√€∑‰…Ë÷√."
#else
#define tsetting L"Setting for UltraIE status bar."
#endif

map<DWORD,C6BeeMenuStatusBar*> C6BeeMenuStatusBar::tid2menustatusbarptr_;
vector<C6BeeMenuStatusBar*> C6BeeMenuStatusBar::statusbarvec_;

UINT WINAPI CheckVersion_Thread(PVOID p){
  try{
    const wchar_t* uutbpath = ultrapath::R().get(ultrapath::uutoolbardll).c_str();
    versionblk verblk = GetVersionBlk(uutbpath);
    string version = _6bees_str::stringmaker()
      << verblk.wMajor << "."
      << verblk.wMinor << "."
      << verblk.wRelease << "."
      << verblk.wBuild;
    // For version 1.0.1, we only provide manually update.
    UUTOOLBAR::UserPrx _user = MyICE::R();
    if(_user==0){
      ::MessageBox(NULL,tip_ntwkfail,kgentitle,MB_OK|MB_ICONINFORMATION);
      return 1;
    }
    string download_url = _user->IsLatestVersion(version);
    if(download_url.empty()){
      ::MessageBox(NULL,knonewversiontxt,kgentitle,MB_OK|MB_ICONINFORMATION);
    }else{
      if(::MessageBox(NULL,knewversiontxt,kgentitle,MB_YESNO|MB_ICONQUESTION)==IDYES){
        wstring dl_url = _6bees_str::wstringmaker() << download_url;
        runIE(dl_url.c_str());
      }
    }
  }catch (...){}
  return 0;
}

///////////////////////////////////////////////////////////////////
C6BeeMenuStatusBar::C6BeeMenuStatusBar():
m_MyMenu(NULL),hi_setting(NULL),
tid(GetCurrentThreadId()){}

C6BeeMenuStatusBar::~C6BeeMenuStatusBar(){
  DestroyIcon(hi_setting);
  m_ImageList.Destroy();
  if (IsWindow()){
    DestroyWindow();
  }
}

_6bees_html::c6beewb2ptr* C6BeeMenuStatusBar::GetPWB2(){
  return pwb_;
}

void C6BeeMenuStatusBar::CreatePanes(){
  // 1. icon
  HBITMAP hbmp=(HBITMAP)c6beeres::R().GetImg(
    IDB_STATUSBARIMG,IMAGE_BITMAP,16*7);
  SetMyImageList(hbmp,m_ImageList,MASKGREY,16,16,7,
    GetSysColor(COLOR_3DFACE));

  // 2. conjure up an array of starting points of all 
  // panes - process based
  /*
  if (PanesVec.empty()&&PanestuffVec.empty()){
    int panenum = _6beed_util::CConfig::R().GetPaneNum();
    panestruct* pstruct = 
      _6beed_util::CConfig::R().GetPaneStructArr();
    for (int i=0;i<panenum;++i){
      PanesVec.push_back(pstruct[i].startingpnt);
      PanestuffVec.push_back(pstruct[i].pt);
    }
    PanesVec.push_back(_6beed_util::CConfig::R().GetSBWidth());
  }*/

  // 3. create panes from the array
  int enabledplugin_num = PluginMng::R().GetEnabledPluginNum();
  SetParts_v2(enabledplugin_num+2,PluginMng::R().GetPartsWidthPtr());

  // 4. add icon and text to panes
  int paneid=1;
  // (1) Setting
  SetTipText(paneid,tsetting);
  if(hi_setting==0)
    hi_setting=ImageList_GetIcon(m_ImageList,6,ILD_NORMAL);
  SetIcon(paneid,hi_setting);

  // (2) Plugins
  for (int i=0;i<PluginMng::R().GetAllPluginNum();i++){
    uuplugin* uup=PluginMng::R().GetPlugin(i);
    if (uup->enabled_){
      paneid++;
      SetTipText(paneid,uup->tip_.c_str());
      if(!uup->icons_.empty()){
        SetIcon(paneid,
          uup->active_icon_ ? uup->active_icon_ : uup->icons_.at(0));
      }
    }
  }
  SetMinHeight(20);

  // 6. add animation to panes
  //AnimCreate(3,_T("c:\\Program Files\\6BeeNetworks\\6BeeToolBar\\Monkey.avi"));
}

// The caller will be responsible to release memory
wchar_t* C6BeeMenuStatusBar::GetHostname(){
  ATL::CString weburl = pwb_->GetURL().c_str();
  int cut=weburl.Find(_T("/"),8);
  if (cut>0){
    weburl.Truncate(cut);
  }
  int len = weburl.GetLength();
  if (len>1024*10){
    return NULL;
  }
  wchar_t* hostname = new wchar_t[len+1];
  wmemset(hostname,L'\0',len+1);
  wcscpy_s(hostname,len+1,weburl.GetString());
  return hostname;
}

// The caller will be responsible to release memory
wchar_t* C6BeeMenuStatusBar::GetWebURL(){
  std::wstring _weburl = pwb_->GetURL();

  int len = (int)_weburl.size();
  wchar_t* wurl = new wchar_t[len+1];
  wmemset(wurl,L'\0',len+1);
  wcscpy_s(wurl,len+1,_weburl.c_str());
  return wurl;
}

// The caller will be responsible to release memory
wchar_t* C6BeeMenuStatusBar::GetWebTitle(){
  std::wstring _webtitle = pwb_->GetTitle();

  int len = (int)_webtitle.size();
  wchar_t* wtitle = new wchar_t[len+1];
  wmemset(wtitle,L'\0',len+1);
  wcscpy_s(wtitle,len+1,_webtitle.c_str());
  return wtitle;
}

// RunJs(L"alert('hello world.')");
bool C6BeeMenuStatusBar::RunJs(const wchar_t* jscode){
  IHTMLDocument2Ptr& pHtmlDoc2 = pwb_->GetHTMLDoc2();
  if (pHtmlDoc2){
    IHTMLWindow2Ptr pHtmlw2(NULL);
    if (FAILED(pHtmlDoc2->get_parentWindow(&pHtmlw2))){
      return false;
    }
    CComVariant vEmpty;
    pHtmlw2->execScript(CComBSTR(jscode), CComBSTR("javascript"), &vEmpty);
  }
  return true;
}

// When using the IHTMLElement::insertAdjacentHTML method to insert script, 
// you must include the DEFER attribute in the script element
bool C6BeeMenuStatusBar::InsertAdjacentHTML(const wchar_t* htmlcode,
  const wchar_t* pos)
{
  IHTMLDocument2Ptr& pHtmlDoc2 = pwb_->GetHTMLDoc2();
  if (pHtmlDoc2){
    IHTMLElementPtr lpElm=NULL;
    if(SUCCEEDED(pHtmlDoc2->get_body(&lpElm))){
      lpElm->insertAdjacentHTML(CComBSTR(pos),CComBSTR(htmlcode));
    }
  }
  return true;
}

IHTMLElement* C6BeeMenuStatusBar::GetElementByIDName(const char* _name){
  return pwb_->GetElementByIDName(_name);
}

LRESULT C6BeeMenuStatusBar::OnClickMenuItem(WORD,WORD id,HWND,
  BOOL& handled)
{
  if(!_6beed_share::u_getLoggedin()){
    _6beed_util::Login_v2();
    return 0;
  }
  //1. Setting menu for enable/disable plugins
  if (id>=1314 && id<1414){
    int lenchange = 0;
    int plugins_num = PluginMng::R().GetAllPluginNum();
    vector<uuplugin*>& plugin_vector = PluginMng::R().GetPluginVec();

    for (int i=0;i<plugins_num;i++){
      uuplugin* uup=plugin_vector.at(i);
      if (uup->menuwID_==id){
        if(uup->disenable_plugin()){//logic
          uup->enabled_ = !uup->enabled_;
          lenchange = uup->enabled_ ? (lenchange+uup->len_) :
            (lenchange-uup->len_);
        }
      }
    }

    PluginMng::R().fillpartswidth();
    int enabledplugin_num = PluginMng::R().GetEnabledPluginNum();
    //////////////////////////////////////////////////////////////////
    //notify parent window to change size
    RECT rct_;
    GetWindowRect(&rct_);
    int statusbar_height = rct_.bottom - rct_.top;
    RECT SBRect = {-1,0,PluginMng::R().GetTotalWidth(),
      statusbar_height};
    uutoolbar::TidStatusWndMap& tsmap = 
      uutoolbar::GlobalVar::R().GetTidStatusWndMap();
    for(uutoolbar::TidStatusWndMap::const_iterator i=tsmap.begin();i!=tsmap.end();i++){
        C6BeeStatusWnd* _statuswnd = const_cast<C6BeeStatusWnd*>(i->second);
        C6BeeMenuStatusBar& _statusbar = _statuswnd->m_StatusBar;
        if (!::IsWindow(_statusbar.m_hWnd)){
          continue;
        }
        _statusbar.MoveWindow(&SBRect);
        _statusbar.SetParts_v2(enabledplugin_num+2,
          PluginMng::R().GetPartsWidthPtr());
        int _paneid = 1;
        for (int i=0;i<plugins_num;i++){
          uuplugin* uup = plugin_vector.at(i);
          if (uup->enabled_){
            _paneid++;
            _statusbar.SetTipText(_paneid,uup->tip_.c_str());
            _statusbar.SetText(_paneid,
              uup->tid2panetext_[_statusbar.tid].c_str());
            if(!uup->icons_.empty()){
              HICON hi = NULL;
              if (!uup->tid2activeicon_.empty()){
                hi = uup->tid2activeicon_[_statusbar.tid];
              }else if(uup->active_icon_){
                hi = uup->active_icon_;
              }else if(!uup->icons_.empty()){
                hi = uup->icons_.at(0);
              }
              _statusbar.SetIcon(_paneid,hi);
            }
          }
        }

        RECT r0,r1;
        _statuswnd->GetWindowRect(&r0);
        ::GetWindowRect(_statuswnd->GetParent(),&r1);
        RECT r3;
        statusbar_height = (r0.bottom-r0.top);
        switch(_6beed_share::u_getieversion()){
          case 7:
          case 8:
            r3.left  = r0.left - r1.left - lenchange;
            r3.top  = r0.top - r1.top;
            break;
          case 6:
            r3.left  = r0.left - r1.left - lenchange -4;
            if (_6beed_util::IsXPTheme()){
              r3.top  = r0.top - r1.top - statusbar_height - 7;
            }else{
              r3.top  = r0.top - r1.top - statusbar_height - 3;
            }break;
        }
        r3.right= r3.left + PluginMng::R().GetTotalWidth();
        r3.bottom = r3.top + statusbar_height;
        _statuswnd->MoveWindow(&r3,true);//move wnd but not control!
    }
  }else{
    // 2. Others
    int plugins_num = PluginMng::R().GetAllPluginNum();
    vector<uuplugin*>& plugin_vector = PluginMng::R().GetPluginVec();

    for (int i=0;i<plugins_num;i++){
      uuplugin* uup=plugin_vector.at(i);
      if (uup->enabled_){
        uup->OnClickMenuItem(id);
      }
    }

    switch (id){
    case ID_SETTING_SETTING:
      pwb_->NavToURL(_T("ultra://GeneralSetting"));
      break;
    case ID_SETTING_ABOUT:
      {
        const wchar_t* uutbpath = ultrapath::R().get(ultrapath::uutoolbardll).c_str();
        versionblk verblk = GetVersionBlk(uutbpath);
        wstring url = _6bees_str::wstringmaker()<< _6bees_const::kUltraIEURL << 
          L"?ver=" << verblk.wMajor << 
          L"." << verblk.wMinor << 
          L"." << verblk.wRelease << 
          L"." << verblk.wBuild;
        pwb_->NavToURL(url.c_str());
      }
      break;
    case ID_SETTING_CHECKUPDATES:
      {
        ::_beginthreadex(NULL,0,CheckVersion_Thread,(LPVOID)NULL,0,0);
      }
      break;
    /*case ID_SETTING_UNINSTALL:
      if(::MessageBoxA(NULL,"Are you sure you want to unintall ultraie?","Uninstall UltraIE",MB_YESNO)==IDYES){
        ShellExecuteA(NULL,"open","MsiExec.exe","/x {56EEAC08-FAA6-471A-B1F1-BEF0984DAB87} /qr","",SW_SHOW);
      }
      break;*/
    }
  }
  handled = true;
  return 1;
}

LRESULT C6BeeMenuStatusBar::OnRButtonDown(UINT uMsg,WPARAM wParam,
  LPARAM lParam,BOOL& bHandled)
{
  if(!_6beed_share::u_getLoggedin()){
    _6beed_util::Login_v2();
    return 0;
  }
  /*
  SetFocus();
  CPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  //_6beed_util::paneT pt = point_in_range(point.x);
  m_MyMenu = c6beeres::R().GetMenu(IDR_SBar);
  CMenuHandle menuPopup;
  switch (pt){
  case SETTING:{
    menuPopup = m_MyMenu.GetSubMenu(0);
    for (int i=0;i<PluginMng::R().GetAllPluginNum();i++){
      uuplugin* uup=PluginMng::R().GetPlugin(i);
      const wchar_t* text = uup->caption_.c_str();
      int img_index = uup->enabled_ ? 0 : 1;
      int menuwID = uup->menuwID_;

      wchar_t* tmpstr = new wchar_t[MAX_MENU_ITEM_TEXT_LENGTH+1];
      wcscpy_s(tmpstr, MAX_MENU_ITEM_TEXT_LENGTH+1, text);
      CMenuItemInfo mii;
      mii.cbSize = sizeof(MENUITEMINFO);
      mii.fMask = MIIM_CHECKMARKS|MIIM_DATA|MIIM_ID|
        MIIM_STATE|MIIM_SUBMENU| MIIM_TYPE;
      mii.cch = (UINT)wcslen(tmpstr)*2;
      mii.fType = MFT_OWNERDRAW;
      coolMenuItemData* _pMI = new coolMenuItemData;
      _pMI->fType  = mii.fType;
      _pMI->fState = mii.fState;
      _pMI->iImage = img_index;
      _pMI->lpstrText = tmpstr;
      mii.dwItemData = (ULONG_PTR)_pMI;
      mii.dwTypeData = tmpstr;
      mii.wID = menuwID;
      menuPopup.InsertMenuItem(0,true,&mii);
    }
    }break;
  default:
    break;
  }
  if (!menuPopup.IsNull()){
    ClientToScreen(&point);
    menuPopup.TrackPopupMenu(
      TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_LEFTBUTTON|TPM_HORPOSANIMATION,
      point.x ,point.y, m_hWnd);
  }
  menuPopup.DestroyMenu();
  m_MyMenu.DestroyMenu();*/
  return 0;
}

LRESULT C6BeeMenuStatusBar::OnLButtonDown(UINT uMsg,WPARAM wParam,
  LPARAM lParam,BOOL& bHandled)
{
  if(!_6beed_share::u_getLoggedin()){
    _6beed_util::Login_v2();
    return 0;
  }
  CPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  int paneid=0;
  uuplugin* uup = PluginMng::R().GetPluginByLocation(point.x,paneid);
  if(uup){
    if (uup==(uuplugin*)0x0001){
      m_MyMenu = c6beeres::R().GetMenu(IDR_SBar);
      CMenuHandle menuPopup = m_MyMenu.GetSubMenu(0);
      for (int i=0;i<PluginMng::R().GetAllPluginNum();i++){
        uuplugin* uup=PluginMng::R().GetPlugin(i);
        const wchar_t* text = uup->caption_.c_str();
        int img_index = uup->enabled_ ? 0 : 1;
        int menuwID = uup->menuwID_;

        wchar_t* tmpstr = new wchar_t[MAX_MENU_ITEM_TEXT_LENGTH+1];
        wcscpy_s(tmpstr, MAX_MENU_ITEM_TEXT_LENGTH+1, text);
        CMenuItemInfo mii;
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_CHECKMARKS|MIIM_DATA|MIIM_ID|
          MIIM_STATE|MIIM_SUBMENU| MIIM_TYPE;
        mii.cch = (UINT)wcslen(tmpstr)*2;
        mii.fType = MFT_OWNERDRAW;
        coolMenuItemData* _pMI = new coolMenuItemData;
        _pMI->fType  = mii.fType;
        _pMI->fState = mii.fState;
        _pMI->iImage = img_index;
        _pMI->lpstrText = tmpstr;
        mii.dwItemData = (ULONG_PTR)_pMI;
        mii.dwTypeData = tmpstr;
        mii.wID = menuwID;
        menuPopup.InsertMenuItem(0,true,&mii);
      }
      if (!menuPopup.IsNull()){
        ClientToScreen(&point);
        menuPopup.TrackPopupMenu(
          TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_LEFTBUTTON|TPM_HORPOSANIMATION,
          point.x ,point.y, m_hWnd);
      }
      menuPopup.DestroyMenu();
      m_MyMenu.DestroyMenu();
    }else{
      ClientToScreen(&point);
      uup->OnLButtonDown(paneid,point.x,point.y);
    }
  }
  return true;
}

LRESULT C6BeeMenuStatusBar::OnHookMsg(UINT uMsg,WPARAM wParam,
  LPARAM lParam,BOOL& bHandled)
{
  int plugins_num = PluginMng::R().GetAllPluginNum();
  vector<uuplugin*>& plugin_vector = PluginMng::R().GetPluginVec();
  for (int i=0;i<plugins_num;i++){
    uuplugin* uup=plugin_vector.at(i);
    if (uup->enabled_){
      uup->OnHookMsg(uMsg,wParam,lParam,bHandled);
    }
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////
LRESULT C6BeeMenuStatusBar::OnInitMenuPopup(UINT uMsg,WPARAM wParam,
  LPARAM lParam,BOOL& bHandled)
{
  CCoolContextMenu<C6BeeMenuStatusBar>::OnInitMenuPopup(uMsg, wParam, lParam, bHandled);
  HICON m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
  ::SetCursor(m_hCursor);
  return 0;
}


int C6BeeMenuStatusBar::AddToImgList(HBITMAP h,COLORREF cmsk){
  return m_ImageList.Add(h,cmsk);
}

void C6BeeMenuStatusBar::AssociateImage(CMenuItemInfo& mii,
                                        coolMenuItemData* pMI)
{
  int plugins_num = PluginMng::R().GetAllPluginNum();
  vector<uuplugin*>& plugin_vector = PluginMng::R().GetPluginVec();

  for (int i=0;i<plugins_num;i++){
    uuplugin* uup=plugin_vector.at(i);
    if (uup->enabled_){
      uup->AssociateImage(mii.wID,pMI->iImage);
    }
  }
  if (pMI->iImage>-1){
    return;
  }
  switch (mii.wID){
      case ID_SETTING_SETTING:
        pMI->iImage = 5;
        break;
      case ID_SETTING_ABOUT:
        pMI->iImage = 4;
        break;
      case ID_SETTING_CHECKUPDATES:
        pMI->iImage = 2;
        break;
      case ID_SETTING_UNINSTALL:
        pMI->iImage = 3;
        break;
      default:
        pMI->iImage = -1;
        break;
  }
}
//////////////////////////////////////////////////////////////////////////
BOOL C6BeeMenuStatusBar::UpdatePanesLayout(void)
{
  BOOL ret = CMultiPaneStatusBarCtrlImpl<C6BeeMenuStatusBar>::UpdatePanesLayout();
  CAnimationInPaneImpl<C6BeeMenuStatusBar>::UpdatePanesLayout();
  return ret;
}

// helper function
void C6BeeMenuStatusBar::opennewtab(const char * url,bool b) const{
  pwb_->NavToURL(url,true);
}

void C6BeeMenuStatusBar::opennewtab(const wchar_t* url,bool b) const{
  pwb_->NavToURL(url,true);
}

const wchar_t* C6BeeMenuStatusBar::getshortweburl(bool iswholeurl){
  static ATL::CString weburl;
  weburl = pwb_->GetURL().c_str();
  if(iswholeurl){
    return weburl.GetString();
  }
  int cut=weburl.Find(_T("/"),8);
  if (cut<0){return _T("");}
  weburl.Truncate(cut);
  return weburl.GetString();
}