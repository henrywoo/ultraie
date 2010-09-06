#include "ToolBarWnd.h"

#include <memory.h>
#include <map>
#include <math.h>
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#include <imgcap_setting.h>
#include "ToolBarCOM.h"//iswebie

#include <6bees_util.h>
#include <6bees_lang.h>
#include "IceSingleton.h"
#include "6bees_shell.h"
#include "ImageRender.h"
#include "6bees_window.h"
#include "6bees_const.h"
#include <copydatamsg.h>

#pragma comment(lib,"6beecommond.lib")
#pragma comment(lib,"6beemousehook.lib")

using namespace _6bees_util;

using namespace std;

const int IDM_SEARCHBOX = 19791;
const int searchlogolength = 30;
const int searchbuttonlength = 21;
const int TBPadding = 0;

HFONT EDITTXTFONT= ::CreateFont(16,0,0,0,FW_THIN,false,FALSE,FALSE,DEFAULT_CHARSET,
                                OUT_STRING_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));

const static BYTE BSTYLE_BTN = BTNS_BUTTON |BTNS_AUTOSIZE;
const static BYTE BSTYLE_DROPDOWN =  BTNS_BUTTON|BTNS_DROPDOWN;
const static BYTE BSTYLE_TEXT = BTNS_BUTTON | BTNS_SHOWTEXT |BTNS_AUTOSIZE;
const static BYTE BSTYLE_DROPDOWN_TEXT =  BTNS_BUTTON | BTNS_SHOWTEXT|BTNS_AUTOSIZE|BTNS_DROPDOWN;

extern bool ADSFILTER,MEDIADETECTION;

#define EDIT_CONTROL 2
#define COMBO_HEIGHT 130

CToolBarWnd::CToolBarWnd():
pwb_(NULL),
sepmovetime(0),
g_hRectanglePen(0),
g_hCursorSearchWindow(0),
HTMLElementSourceID(0),
g_hwndFoundWindow(0),
CaptureRect(),
boldfont(0),elementHit(0),wndRect_Logo(),m_6beeImageList(0),searchboxlen(0)
{}

CToolBarWnd::~CToolBarWnd(){
  m_6beeImageList.Destroy();
  pPopup.m_ImageList.Destroy();
  if(boldfont)
    ::DeleteObject(boldfont);
  if(m_RebarContainer.IsWindow())
    m_RebarContainer.UnsubclassWindow();// Don't forget to UnSubclass the rebar otherwise IE could crash
  SetBrowser(NULL);
  if (IsWindow()){
    UnInstallMouseHook();
    DestroyWindow();
  }
}

LRESULT CToolBarWnd::OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  bool isloggedin = _6beed_share::u_getLoggedin();
  BYTE _style = (_6beed_share::u_getLoggedin()? (BSTYLE_DROPDOWN_TEXT):(BSTYLE_TEXT));
  searchboxlen = _6beed_share::u_getSearchboxlen();
  pPopup.GetSystemSettings();
  HBITMAP hbmp=(HBITMAP)c6beeres::R().GetImg(ULTRA_IMGS,IMAGE_BITMAP,864);
  SetMyImageList(hbmp,m_6beeImageList,MASKGREY,16,16,54,GetSysColor(COLOR_3DFACE));
  pPopup.m_ImageList = m_6beeImageList;
  m_CToolBarSEButton.p_CToolBarCombo = &m_CToolBarCombo;

  boldfont = ::CreateFont(14,0,0,0,FW_BOLD,false,FALSE,FALSE,DEFAULT_CHARSET,OUT_STRING_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));

  // 1. Draw TB
  SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_MIXEDBUTTONS);
  SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
  SendMessage(m_hWnd, TB_SETMAXTEXTROWS, 1, 0L);
  SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
  SetImageList(m_6beeImageList.m_hImageList,0);

  m_hTBLogoImageList = ImageList_Create(48,48,ILC_COLOR32,1,1);
  HICON hi_ultralogo = (HICON)c6beeres::R().GetImg(IDI_ULTRAICON);
  int iUltra  = ImageList_AddIcon(m_hTBLogoImageList,hi_ultralogo);
  DestroyIcon(hi_ultralogo);// and also need to release imagelist resource
  SetImageList(m_hTBLogoImageList,1);

  SecureZeroMemory((void*)Buttons,(sizeof(Buttons)));

  bcount=0;
  Buttons[bcount].iString = (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0,(LPARAM)_T(""));
  Buttons[bcount].iBitmap = MAKELONG(iUltra,1);
  Buttons[bcount].fsState = TBSTATE_ENABLED;
  Buttons[bcount].fsStyle = BSTYLE_TEXT;
  Buttons[bcount].idCommand = IDM_GOHOME;

  ++bcount;// SEP
  Buttons[bcount].fsStyle = TBSTYLE_SEP;

  ++bcount;// SEP
  Buttons[bcount].fsStyle = TBSTYLE_SEP;
  Buttons[bcount].idCommand = IDM_SEARCHBOX;

  ++bcount;// SEP
  Buttons[bcount].fsStyle = TBSTYLE_SEP;

  ++bcount;
  Buttons[bcount].iString = (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0,
    (LPARAM)(_6beed_share::u_getLoggedin()?tlogout:tlogin));
  Buttons[bcount].iBitmap = isloggedin ? _6bees_const::pexit : _6bees_const::pkey;
  Buttons[bcount].fsState = TBSTATE_ENABLED;
  Buttons[bcount].fsStyle = BSTYLE_TEXT;
  Buttons[bcount].idCommand = IDM_LOGIN;

  //MUST: Set TB_SETTBPadding before TB_ADDBUTTONS
  long TBTBPadding =MAKELONG(TBBUTTON_CLASSIC_PADDING_X,TBBUTTON_CLASSIC_PADDING_Y);
  ::SendMessage(m_hWnd, TB_SETPADDING, 0, TBTBPadding);

  ++bcount;
  ::SendMessage(m_hWnd, TB_ADDBUTTONS, bcount, (LPARAM)&Buttons);

  setlogobutton();
  setspaceforcombo();

  // 2. Draw Search Box
  RECT rc;
  GetItemRect(EDIT_CONTROL, &rc);

  RECT rect = rc;
  rect.right = rc.left+searchlogolength;
  m_CToolBarSELogo.Create(m_hWnd,rect,NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW);
  m_CToolBarSELogo.GetSystemSettings();

  RECT rect2=rc;
  rect2.left += searchlogolength;
  rect2.right = rect2.left + searchboxlen;
  rect2.bottom = COMBO_HEIGHT;
  m_CToolBarCombo.Create(m_hWnd,rect2,NULL,
    WS_CHILD|WS_VISIBLE|CBS_DROPDOWN|WS_VSCROLL|
    CBS_SORT|CBS_AUTOHSCROLL|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
  m_CToolBarCombo.SetFont(EDITTXTFONT);
  m_CToolBarCombo.InsertHistory();
  RegisterDragDrop(m_CToolBarCombo.m_hWnd,(LPDROPTARGET)&m_CToolBarCombo);

  RECT rect3=rc;
  rect3.left = rect2.right;
  rect3.right=rect3.left+searchbuttonlength;
  m_CToolBarSEButton.Create(m_hWnd,rect3,NULL,
    WS_CHILD|WS_VISIBLE|BS_OWNERDRAW);

  RECT rect4=rc;
  rect4.left = rc.right - ksepwidth - TBPadding;
  rect4.right= rect4.left + ksepwidth;
  m_CToolBarSep.Create(m_hWnd,rect4,NULL,
    WS_CHILD|WS_VISIBLE|BS_OWNERDRAW);
  return 0;
}

void CToolBarWnd::setlogobutton(){
  unsigned short TBLogoWidth = (_6beed_util::IsXPTheme() ? 48+10 : 48+5);
  TBBUTTONINFO tbi2;
  tbi2.cbSize = sizeof(TBBUTTONINFO);
  tbi2.dwMask = TBIF_SIZE | TBIF_STYLE;
  tbi2.fsStyle = Buttons[0].fsStyle;
  tbi2.cx = TBLogoWidth;
  if (::IsWindow(m_hWnd)){
    ::SendMessage(m_hWnd, TB_SETBUTTONINFO, IDM_GOHOME, (LPARAM)&tbi2);
  }
}

void CToolBarWnd::setspaceforcombo(){
  TBBUTTONINFO tbi;
  tbi.cbSize = sizeof(TBBUTTONINFO);
  tbi.dwMask = TBIF_SIZE | TBIF_STYLE;
  tbi.fsStyle = TBSTYLE_SEP;
  tbi.cx = searchboxlen+searchlogolength+ksepwidth+searchbuttonlength+TBPadding;
  if (::IsWindow(m_hWnd)){
    ::SendMessage(m_hWnd, TB_SETBUTTONINFO, IDM_SEARCHBOX, (LPARAM)&tbi);
  }
}

void    CToolBarWnd::RefreshTButtons(bool comein){
  ///
  /// (icon) (|)(|)(|) (login|logout)
  ///   0     1  2  3        4
  static const int k=4;// important - k should be the order number of the button
#if 0
  TBBUTTONINFO AllButtonInfo[ButtonNumber]={};
  SecureZeroMemory((void*)AllButtonInfo,(sizeof(AllButtonInfo)));
  for(int j=0;j<ButtonNumber;++j){
    AllButtonInfo[j].cbSize = sizeof(TBBUTTONINFO);
  }
  int bicount=k;
  if (comein){
    AllButtonInfo[bicount].dwMask = TBIF_BYINDEX|TBIF_TEXT|TBIF_IMAGE;
    AllButtonInfo[bicount].pszText = _T("Logout");
    AllButtonInfo[bicount].lParam  = bicount;
    AllButtonInfo[bicount].iImage  = 11;

    ++bicount;
    for(int i=k;i<bicount;i++){// The first three buttons shouldn't be changed at all
      ::SendMessage(m_hWnd, TB_SETBUTTONINFO, AllButtonInfo[i].lParam,(LPARAM)&AllButtonInfo[i]);
    }
  }else{
    AllButtonInfo[bicount].dwMask = TBIF_BYINDEX|TBIF_TEXT|TBIF_IMAGE;
    AllButtonInfo[bicount].pszText = _T("Login");
    AllButtonInfo[bicount].lParam  = bicount;
    AllButtonInfo[bicount].iImage  = 9;
    ++bicount;
    for(int i=k;i<bicount;i++){
      ::SendMessage(m_hWnd, TB_SETBUTTONINFO, AllButtonInfo[i].lParam,(LPARAM)&AllButtonInfo[i]);
    }
  }
#else
  setlogobutton();
  setspaceforcombo();
  TBBUTTONINFO AllButtonInfo={};
  AllButtonInfo.cbSize = sizeof(TBBUTTONINFO);
  if (comein){
    AllButtonInfo.dwMask = TBIF_BYINDEX|TBIF_TEXT|TBIF_IMAGE;
    AllButtonInfo.pszText = tlogout;
    AllButtonInfo.iImage  = _6bees_const::pexit;
    ::SendMessage(m_hWnd,TB_SETBUTTONINFO,k,(LPARAM)&AllButtonInfo);
  }else{
    AllButtonInfo.dwMask = TBIF_BYINDEX|TBIF_TEXT|TBIF_IMAGE;
    AllButtonInfo.pszText = tlogin;
    AllButtonInfo.iImage  = _6bees_const::pkey;
    ::SendMessage(m_hWnd,TB_SETBUTTONINFO,k,(LPARAM)&AllButtonInfo);
  }
#endif
}

LRESULT CToolBarWnd::OnSepMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  if(sepmovetime - ::GetTickCount() < 100){return 1;}
  sepmovetime = ::GetTickCount();

  // 1. Move Combox related windows
  int xPos = GET_X_LPARAM(lParam);
  RECT TBWndRect;
  ::GetWindowRect(m_hWnd,&TBWndRect);
  RECT seprect_;
  ::GetWindowRect(m_CToolBarSep.m_hWnd, &seprect_);
  RECT selogoRect_;
  ::GetWindowRect(m_CToolBarSELogo.m_hWnd, &selogoRect_);
  int tmpwidth = seprect_.left + xPos - selogoRect_.right;
  int lowerbound = kminsearchboxlen + searchbuttonlength;
  int higherbound= kmaxsearchboxlen + searchbuttonlength;

  // 1.1 new location of seperator
  RECT seprect = {0,0,0,ksepheight};
  if(( tmpwidth >= lowerbound )&&( tmpwidth < higherbound )){
    seprect.left  = seprect_.left + xPos;
  }else if( tmpwidth >= higherbound ){
    seprect.left  = selogoRect_.right + higherbound;
  }else{
    seprect.left  = selogoRect_.right + lowerbound;
  }
  seprect.right = seprect.left + ksepwidth;
  // 1.2 new location of combobox
  RECT Comborect = {selogoRect_.right,0,seprect.left-searchbuttonlength,COMBO_HEIGHT};
  int Combolength = Comborect.right - Comborect.left;
  // 1.3 new location of Search/Magnifier Button
  RECT SEButtonect = {Comborect.right,0,Comborect.right+searchbuttonlength,_6bees_const::ktbbuttonheight};
  HDWP hWinPosInfo = ::BeginDeferWindowPos(3);

  // 1.4 move the windows all once time
  // NOTE: the left,top parameter is relative value
  ::DeferWindowPos(hWinPosInfo,m_CToolBarCombo.m_hWnd,HWND_TOP,
    Comborect.left-TBWndRect.left,Comborect.top,Combolength,COMBO_HEIGHT ,SWP_NOZORDER|SWP_NOACTIVATE);
  ::DeferWindowPos(hWinPosInfo,m_CToolBarSEButton.m_hWnd,HWND_TOP,
    SEButtonect.left-TBWndRect.left,SEButtonect.top,searchbuttonlength,_6bees_const::ktbbuttonheight,SWP_NOZORDER|SWP_NOACTIVATE);
  ::DeferWindowPos(hWinPosInfo,m_CToolBarSep.m_hWnd,HWND_TOP,
    seprect.left-TBWndRect.left,seprect.top,ksepwidth,ksepheight,SWP_NOZORDER|SWP_NOACTIVATE);
  ::EndDeferWindowPos(hWinPosInfo);

  // 2. Move ToolBar Buttons
  TBBUTTONINFO tbi;
  tbi.cbSize = sizeof(TBBUTTONINFO);
  tbi.dwMask = TBIF_SIZE | TBIF_STYLE;
  tbi.fsStyle = TBSTYLE_SEP;
  int cxsize = seprect.right - selogoRect_.left + TBPadding;
  tbi.cx = cxsize;
  SendMessage(m_hWnd,TB_SETBUTTONINFO, IDM_SEARCHBOX, (LPARAM)&tbi);

  if(wParam==1){
    int slength = seprect.right-selogoRect_.right-searchbuttonlength;
    searchboxlen = slength;
    _6beed_share::u_setSearchboxlen(searchboxlen);
  }
  return 0;
}

void    CToolBarWnd::SetBrowser(_6bees_html::c6beewb2ptr* _p){
  pwb_ = _p;
  m_CToolBarSEButton.SetBrowser(_p);
}

//show chevron
HWND    CToolBarWnd::FindRebar(HWND hwndStart){
  HWND hwndFound=NULL;
  if (((hwndFound = FindWindowEx(hwndStart, NULL, REBARCLASSNAME, NULL)) != NULL))
    return hwndFound;

  HWND hwndEnum=NULL;
  while ((hwndEnum = FindWindowEx(hwndStart, hwndEnum, NULL, NULL)) != NULL){
    if ((hwndFound = FindRebar(hwndEnum)) != NULL)
      return hwndFound;
  }
  return NULL;
}

BOOL    CToolBarWnd::SetBandRebar(){
  HWND hWnd=pwb_->GetCurrentTab();
  if (hWnd == NULL)
    return FALSE;
  m_ctlRebar.m_hWnd = FindRebar(hWnd);
  if (m_ctlRebar.m_hWnd == NULL)
    return FALSE;
  m_RebarContainer.SubclassWindow(m_ctlRebar);
  return TRUE;
}

LRESULT CToolBarWnd::OnSetFocus(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
  ::MessageBox(m_hWnd,_T("ToolBar getfocus!!"),_T("Search"),NULL);return 0;
}

LRESULT CToolBarWnd::OnInitMenuPopup(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  pPopup.OnInitMenuPopup(uMsg, wParam, lParam, bHandled);
  return true;
}

CToolBarCombo& CToolBarWnd::GetToolBarCombo() {
  return m_CToolBarCombo;
}

LRESULT CToolBarWnd::OnUploadFiles(){
  HWND hwndTo;
  hwndTo = ::FindWindow(0,_6bees_const::kUploader_WndTitle);
  if(!hwndTo){
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::uploader);
    ATL::CString UpdownloaderPath = upath.c_str();
    UpdownloaderPath += _T(" -m upload");

    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    if (!CreateProcess(NULL,(LPWSTR)UpdownloaderPath.GetString(),NULL,
      NULL,false,0,NULL,NULL,&si,&pi))
    {
      ::MessageBox(NULL,UpdownloaderPath,_T("ERROR"),MB_ICONINFORMATION);
    }
  }else{
    COPYDATASTRUCT cds;
    cds.dwData = '1';
    cds.lpData = NULL;
    cds.cbData = 0;
    ::SendMessage(hwndTo,WM_COPYDATA,(WPARAM)0,(LPARAM)&cds);
  }
  return 0;
}

LRESULT CToolBarWnd::OnCommand(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  if (HIWORD(wParam)){return 1;}
  long lSite = LOWORD(wParam);
  if(!_6beed_share::u_getLoggedin()){
    _6beed_util::Login_v2();
    return 0;
  }
  switch (lSite){
  case IDM_GOHOME:
    pwb_->PostNav("/admin/index.php");
    break;
  case IDM_VIDEO:
    {
      STARTUPINFO si = {sizeof(si)};
      PROCESS_INFORMATION pi;
      using _6bees_util::ultrapath;
      const wstring& upath = ultrapath::R().get(ultrapath::capvideo);
      if (!CreateProcess(NULL,(LPWSTR)upath.c_str(),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
        ::MessageBox(NULL,_T("Cannot find CapVideo.exe!"),_T("ERROR"),MB_ICONINFORMATION);
      }
    }
    break;
  case IDM_VOICE:
    {
      STARTUPINFO si = {sizeof(si)};
      PROCESS_INFORMATION pi;
      using _6bees_util::ultrapath;
      const wstring& upath = ultrapath::R().get(ultrapath::recorder);
      if (!CreateProcess(NULL,(LPWSTR)upath.c_str(),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
        ::MessageBox(NULL,_T("Cannot find Audio Recorder!"),_T("ERROR"),MB_ICONINFORMATION);
      }
    }
    break;
  case IDM_LOGIN:
    Logout();
    break;
  case IDM_UPLOAD:
    OnUploadFiles();
    break;
  default:
    break;
  }
  return -1;
}

void    CToolBarWnd::Logout(){
  _6beed_share::u_setLoggedin(false);
  RefreshTButtons(false);
  //_6bees_window::NotifyClientMng(CPYDATA_LOGOUT,_6bees_const::kClientMng_WndTitle,NULL);
  _6bees_window::NotifyClientMng(CPYDATA_LOGOUT,_6bees_const::kUploader_WndTitle,NULL);
  _6beed_util::Notify6BeeTB(CPYDATA_LOGOUT,m_hWnd);
}

//transfer search message to SEBUTTON^_^
LRESULT CToolBarWnd::OnToolBarSearch(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  ::SendMessage(m_CToolBarSEButton.m_hWnd,WM_TOOLBAR_SEBUTTON_SEARCH,0,0);
  return true;
}

LRESULT CToolBarWnd::OnShowHBar(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  if (_6beed_share::u_getieversion()<8){
    pwb_->ShowHBar(L"{92268371-AE67-4B85-BF60-157E57009ECC}");
  }
  return 0;
}