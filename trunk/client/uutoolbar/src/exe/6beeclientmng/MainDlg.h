#ifndef __CLIENTMNG_MAINDLG__
#define __CLIENTMNG_MAINDLG__

#include "resource.h"
#include "TrayiconImpl.h"
#include <6beecontextmenu.h>
#include <atlctrlx.h>
#include <copydatamsg.h>
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#include <ice/ice.h>
#include "6bees_window.h"
#include "icesingleton.h"
#include "cppsqlite3.h"
#include "6bees_util.h"
#include "uulogging.h"
#include "6bees_const.h"
#include "6bees_str.h"

using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;
#include "../../dll/mousehook/mousehook.h"
#include <imgcap_setting.h>
#include <vector>
#include "../../dll/6beeclientmng_hook/6BeeClientMng_H.h"



using std::vector;
using namespace _6beed_share;
using namespace _6bees_const;
using namespace _6bees_util;

extern CAppModule _Module;
class CMainDlg;

struct loginfo{
  CMainDlg* mdlg;
  std::string name;
  std::string passwd;
};
static loginfo info={};
static WINDOWPLACEMENT wp={};
UINT WINAPI ThreadLogin(LPVOID);
UINT WINAPI ThreadLogout(LPVOID lp);

static const COLORREF BGCOLOR = RGB(237,255,200);
static const COLORREF TXTCOLOR = RGB(7,5,20);

enum etimer{ TIMER_LOGIN=1,TIMER_LOGOUT,TIMER_BUSYUPLOADING,TIMER_BUSYDOWNLOADING };

/// system tray icon image list - start with s
enum systrayicon{sloggedin=0,sconnect1,sconnect2,suploading,sdownloading,soffline,siconnum};

const wchar_t* const tip_offline = L"UltraIE.Com - You are Not Singed In!";
const wchar_t* const tip_ntwkfail= L"NetWork failure. Please check your network setting.";
const wchar_t* const tip_logining= L"Logining, please wait...";

class CMainDlg :
  public CDialogImpl<CMainDlg>,
  public CUpdateUI<CMainDlg>,
  public CMessageFilter,
  public CIdleHandler,
  public CTrayIconImpl<CMainDlg>,
  public CCoolContextMenu<CMainDlg>
{
private:
  //CAnimateCtrl m_animation;/// @todo 
  CHyperLink m_ForgetPass;
  CWindow m_CUserName;
  CWindow m_CPassWd;
  //CButton m_CAutoLogIn;
  //bool isAutoLogin;
  //bool isloggedin;
  WINDOWPLACEMENT m_wp;//For hide window

  CImageList m_imglst;
  CBitmapButton m_CLogIn;
  CBitmapButton m_CRegister;
  CBitmapButton m_NoteImg;

  /// login system tray icon
  CImageList ultraiestatusicons_;
public:
  CWindow m_Note;

  enum { IDD = IDD_MAINDLG };
  BEGIN_UPDATE_UI_MAP(CMainDlg)
  END_UPDATE_UI_MAP()

  BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_DESTROY,    OnDestroy)
    MESSAGE_HANDLER(WM_COPYDATA,   OnCopyData)
    MESSAGE_HANDLER(WM_TIMER,      OnTimer)

    COMMAND_HANDLER(IDC_LOGIN_,BN_CLICKED,   OnSignin)
    COMMAND_HANDLER(IDC_REGISTER_,BN_CLICKED,OnRegister)

    COMMAND_ID_HANDLER(ID_A_MY6BEESPACE, OnMy6BeeSpace)
    COMMAND_ID_HANDLER(ID_A_UPLOADFILES, OnUploadFiles)
    COMMAND_ID_HANDLER(ID_A_LOGINOUT,    OnLogInOut)
    COMMAND_ID_HANDLER(ID_A_AUTOLOGIN,   OnAutoLogIn)
    COMMAND_ID_HANDLER(ID_A_SIGNIN,      OnSignin)
    COMMAND_ID_HANDLER(ID_A_EXIT,        OnEXIT)

    CHAIN_MSG_MAP(CTrayIconImpl<CMainDlg>)
    CHAIN_MSG_MAP(CCoolContextMenu<CMainDlg>)
  END_MSG_MAP()

  CMainDlg(){}

  virtual ~CMainDlg(){
    m_imglst.Destroy();
    m_ImageList.Destroy();
    if(!UnInstallKeyboardHook()){
      UUDEBUG((LOG_ERROR,"Failed to uninstall keyboard hook."));
    }
    if (u_getieversion()==7 && _6beed_share::u_getieonewindow()){
      if(!UnInstallIECreatProcessHook()){
        UUDEBUG((LOG_ERROR,"Failed to uninstall IE CreatProcess Hook."));
      }
    }
  }

  LRESULT OnEXIT(WORD,WORD wID,HWND,BOOL&){
    CloseDialog(wID);
    return 0;
  }

  LRESULT OnDestroy(UINT,WPARAM,LPARAM,BOOL&){
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);
    return 0;
  }

  void CloseDialog(int nVal){
    const static DWORD kTimeout = 6000;
    HANDLE h = (HANDLE)::_beginthreadex(NULL,0,ThreadLogout,(LPVOID)this,0,NULL);
    DWORD dw=WaitForSingleObject(h,kTimeout);
    switch (dw){
        case WAIT_OBJECT_0:
        case WAIT_TIMEOUT:
        default:
          {
            DestroyWindow();
            ::PostQuitMessage(nVal);
            break;
          }
    }
  }

  virtual BOOL PreTranslateMessage(MSG* pMsg){
    return CWindow::IsDialogMessage(pMsg);
  }

  virtual BOOL OnIdle(){
    return FALSE;
  }

  LRESULT OnInitDialog(UINT,WPARAM,LPARAM,BOOL&){
    HBITMAP hbmp=(HBITMAP)c6beeres::R().GetImg(ULTRA_IMGS,IMAGE_BITMAP,864);
    SetMyImageList(hbmp,m_ImageList,MASKGREY,16,16,54,GetSysColor(COLOR_3DFACE));

    HBITMAP hbmp2=(HBITMAP)c6beeres::R().GetImg(BUTTON_ClientMng,IMAGE_BITMAP,380,26);
    SetMyImageList(hbmp2,m_imglst,MASKPURPLE,95,26,4,GetSysColor(COLOR_3DFACE));

    HBITMAP hbmp3=(HBITMAP)c6beeres::R().GetImg(IDB_ULTRAIESTATUS,IMAGE_BITMAP,16*siconnum);
    SetMyImageList(hbmp3,ultraiestatusicons_,MASKGREY,16,16,siconnum,GetSysColor(COLOR_3DFACE));

    // One process IE is only for IE7
    if (u_getieversion()==7 && _6beed_share::u_getieonewindow()){
      InstallIECreatProcessHook();
    }
    InstallKeyboardHook();

    HICON hi=(HICON)c6beeres::R().GetImg(IDI_LOGINWND,
      IMAGE_ICON,::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hi,FALSE);
    ::DestroyIcon(hi);

    USES_CONVERSION;
    const wchar_t* uu = A2CW(u_getEmail());
    bool registereduser=(wcslen(uu)!=0);
    const wchar_t* pp = A2CW(u_getPassword());

    hi=(HICON)c6beeres::R().GetImg(
      u_getLoggedin()?IDI_6BEELOGO:IDI_6BEEDISABLE,
      IMAGE_ICON,::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON));
    ATL::CString bonjour(tip_offline);
    if(u_getLoggedin())
      bonjour.Format(_T("Welcome %s:\nYou are now Singed In UltraIE.Com!"),uu);
    InstallIcon(bonjour, hi, IDR_CLIENTMNGMENU);
    SetDefaultItem(ID_A_MY6BEESPACE);
    GetSystemSettings();

    m_ForgetPass.SubclassWindow(GetDlgItem(IDC_FORGETPWD));// URL link
    m_ForgetPass.SetHyperLink(_6bees_const::kUltraIEURL_FORGETPASS);
    m_Note.Attach(GetDlgItem(IDC_NOTE));
    m_CUserName.Attach(GetDlgItem(IDC_USERNAME_));
    m_CUserName.SetFocus();
    m_CUserName.SetWindowText(uu);
    m_CPassWd.Attach(GetDlgItem(IDC_PASSWD_));

    m_CLogIn.SubclassWindow(GetDlgItem(IDC_LOGIN_));
    m_CLogIn.SetToolTipText(_T("Login to UltraIE.com"));
    m_CLogIn.SetImageList(m_imglst);
    m_CLogIn.SetImages(1,0,0,0);
    m_CLogIn.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_CRegister.SubclassWindow(GetDlgItem(IDC_REGISTER_));
    m_CRegister.SetToolTipText(_T("Register a new account of UltraIE.com"));
    m_CRegister.SetImageList(m_imglst);
    m_CRegister.SetImages(3,2,2,2);
    m_CRegister.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_NoteImg.SubclassWindow(GetDlgItem(IDC_NOTEIMG));
    m_NoteImg.SetImageList(m_ImageList);
    m_NoteImg.SetImages(pinfo);

    //m_CAutoLogIn.Attach(GetDlgItem(IDC_AUTOLOGIN_));
    //m_CAutoLogIn.SetCheck(isAutoLogin?BST_CHECKED:BST_UNCHECKED);

    GetWindowPlacement(&wp); //For window restore
    if (u_getLoggedin()||u_getAutologin()){
      RECT r={-1000,-1000,1,1};
      SetWindowPos(HWND_TOPMOST,&r,SWP_HIDEWINDOW);
      ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);//´Ótask bar/atl+tabÖÐÈ¥µô.
    }

    CMessageLoop* pLoop = _Module.GetMessageLoop();
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);
    UIAddChildWindowContainer(m_hWnd);

    if(registereduser){
      if(u_getAutologin()){
        wchar_t notice[100]={};
        swprintf_s(notice,100,_T("Logging with %s, please waiting..."),uu);
        m_Note.SetWindowText(notice);
        info.mdlg = this;
        info.name = u_getEmail();
        info.passwd = u_getPassword();
        _beginthreadex(NULL,0,ThreadLogin,(LPVOID)&info,0,NULL);
      }
    }else{
      //runIE(UltraIEURL_REGISTER);// a little bit annoying??
      m_Note.SetWindowText(_T("Please register and get an UltraIE account."));
    }
    //UIAddChildWindowContainer(m_hWnd);
    return TRUE;
  }

  void UpdateNoteImg(bool loggedin=true){
    m_NoteImg.SetImages(loggedin?pinfo:pbad);
    m_NoteImg.Invalidate();
    m_NoteImg.UpdateWindow();
  }

  /// kill or set a timer
  /// @param etimer timer channel
  /// @param kill true - kill a timer; false - set a timer
  void UpdateTimer(etimer t,bool kill=true){
    static const int kduration = 500;
    if (kill){
      KillTimer(t);
      Sleep(kduration+100);
      if (t==TIMER_LOGIN){
        /// log in can be failed
        bool loggedin = _6beed_share::u_getLoggedin();
        HICON hi = ultraiestatusicons_.GetIcon(loggedin?sloggedin:soffline);
        ChangeIcon(hi);
        DestroyIcon(hi);
        UpdateNoteImg(loggedin);
      }else if(t==TIMER_LOGOUT){
        HICON hi = ultraiestatusicons_.GetIcon(soffline);
        ChangeIcon(hi);
        DestroyIcon(hi);
        UpdateNoteImg(false);
      }else{
        /// it should be (t==TIMER_BUSYUPLOADING||t==TIMER_BUSYDOWNLOADING).
        HICON hi = ultraiestatusicons_.GetIcon(sloggedin);
        ChangeIcon(hi);
        DestroyIcon(hi);
      }
    }else{
      SetTimer(t,kduration);
    }
  }

  LRESULT OnTimer(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    const static int maxiconsindex= 1;
    static bool b = false;
    if (wParam==TIMER_LOGIN){
      b = !b;
      /// system tray flash
      HICON hi = ultraiestatusicons_.GetIcon(b?sconnect1:sconnect2);
      ChangeIcon(hi);
      DestroyIcon(hi);
      /// image button flash
      m_NoteImg.SetImages(b?pglobe:pglobelink);
      m_NoteImg.Invalidate();
    }else if(wParam==TIMER_BUSYUPLOADING){
      b = !b;
      /// system tray flash
      HICON hi = ultraiestatusicons_.GetIcon(b?suploading:sloggedin);
      ChangeIcon(hi);
      DestroyIcon(hi);
    }else if(wParam==TIMER_BUSYDOWNLOADING){
      b = !b;
      /// system tray flash
      HICON hi = ultraiestatusicons_.GetIcon(b?sdownloading:sloggedin);
      ChangeIcon(hi);
      DestroyIcon(hi);
    }
    return 0;
  }

  LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM , BOOL& bHandled){
    bHandled = FALSE;
    if(wParam == SC_MINIMIZE || wParam == SC_CLOSE) {
      ShowWindow(SW_HIDE);
      bHandled = TRUE;
    }
    return true;
  }

  LRESULT OnMy6BeeSpace(WORD,WORD,HWND,BOOL&){
    _6beed_util::runIE(kUltraIEURL);
    return 0;
  }

  //LRESULT OnScreenCapture(WORD,WORD,HWND,BOOL&){
    //_6beed_util::CallExeW(_6beed_util::Get6BEEPath(_6bees_const::kScreenCapName));
    //return 0;
  //}

  LRESULT OnUploadFiles(WORD,WORD,HWND,BOOL&){
    using _6bees_util::ultrapath;
    wstring cmd(ultrapath::R().get(ultrapath::uploader));
    cmd.append(L" -m upload");
    _6beed_util::CallExeW(cmd.c_str());
    return 0;
  }

  /*LRESULT OnRecordVoice(WORD,WORD,HWND,BOOL&){
    _6beed_util::CallExeW(_6beed_util::Get6BEEPath(_6bees_const::kAudioRecorderName));
    return 0;
  }

  LRESULT OnShootVideo(WORD,WORD,HWND,BOOL&){
    _6beed_util::CallExeW(_6beed_util::Get6BEEPath(_6bees_const::kCapVideoName));
    return 0;
  }*/

  LRESULT OnLogInOut(WORD,WORD wID,HWND,BOOL&){
    if(u_getLoggedin()){
      ::_beginthreadex(NULL,0,ThreadLogout,(LPVOID)this,0,NULL);
    }else{
      COPYDATASTRUCT cds;
      cds.dwData = CPYDATA_LOGIN;
      cds.lpData = NULL;
      cds.cbData = NULL;
      ::SendMessage(m_hWnd,WM_COPYDATA,(WPARAM)0,(LPARAM)(LPVOID)&cds);
    }
    return 0;
  }

  LRESULT OnAutoLogIn(WORD,WORD wID,HWND,BOOL&){
    u_setAutologin(!u_getAutologin());
    return 0;
  }

  LRESULT OnSignin(WORD,WORD wID,HWND,BOOL&){
    static const short maxlen = 40;
    wchar_t username[maxlen]={},password[maxlen]={};
    m_CUserName.GetWindowText(username,maxlen);
    m_CPassWd.GetWindowText(password,maxlen);

    m_Note.SetWindowText(tip_logining);
    SetTimer(TIMER_LOGIN,500);

    info.mdlg = this;
    info.name = stringmaker() << username;
    info.passwd = GetMD5((stringmaker()<<password).c_str());
    ::_beginthreadex(NULL,0,ThreadLogin,(LPVOID)&info,0,NULL);
    return 0;
  }

  LRESULT OnRegister(WORD,WORD,HWND,BOOL&){
    _6beed_util::runIE(_6bees_const::kUltraIEURL_REGISTER);
    return 0;
  }

  LRESULT OnCopyData(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    PCOPYDATASTRUCT pMyCDS = (PCOPYDATASTRUCT) lParam;
    ULONG_PTR a= pMyCDS->dwData;
    switch (a){
      case CPYDATA_LOGOUT:
        // Do not update database, the source side(Toolbar) should update it.???
        ::_beginthreadex(NULL,0,ThreadLogout,(LPVOID)this,0,NULL);
        break;
      case CPYDATA_LOGIN:
        if (u_getAutologin()){
          _beginthreadex(NULL,0,ThreadLogin,(LPVOID)&info,0,NULL);
        }else{
          ModifyStyleEx(WS_EX_TOOLWINDOW,WS_EX_APPWINDOW);
          ::SetWindowPlacement(m_hWnd,&wp);
          ::SetForegroundWindow(m_hWnd);
        }
        break;
      case CPYDATA_DOWNLOADIEFAV_OK:
        PopMsg(L"You have Synchronized down all your UltraIE online"
          L" bookmarks to your IE favorites successfully!",
          L"OnlineBookmarks downloaded successfully!");
        break;
      case CPYDATA_UPLOADIEFAV_OK:
        PopMsg(L"You have Synchronized up all your IE favorites to"
          L" UltraIE online bookmarks successfully!",
          L"IE Favorites uploaded successfully!");
        break;
      case CPYDATA_FILEUPLOAD_OK:
        {
        wstring m = wstringmaker(CP_ACP)<<(char*)(pMyCDS->lpData);
        m.resize(255);
        PopMsg(m.c_str(),L"File Upload Successfully!");
        }break;
      case CPYDATA_SAVEOLBM_OK:
        {
        wstring f=wstringmaker()<<(char*)(pMyCDS->lpData);
        PopMsg(f.c_str(),L"Saving this page to online bookmark successfully.");
        }break;
      case CPYDATA_SAVEOLBM_START:
      case CPYDATA_FILEUPLOAD_START:
      case CPYDATA_UPLOADIEFAV_START:
        UpdateTimer(TIMER_BUSYUPLOADING,false);
        break;
      case CPYDATA_DOWNLOADIEFAV_START:
        UpdateTimer(TIMER_BUSYDOWNLOADING,false);
        break;
      case CPYDATA_UPLOADBUSY_STOP:
        UpdateTimer(TIMER_BUSYUPLOADING,true);
        break;
      case CPYDATA_DATATRANSFER_TERM:
      case CPYDATA_DOWNLOADBUSY_STOP:
        UpdateTimer(TIMER_BUSYDOWNLOADING,true);
        break;
      default:
        return 0;
    }
    return 0;
  }

  // Cool Menu related function
  LRESULT OnInitMenuPopup(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    CCoolContextMenu<CMainDlg>::OnInitMenuPopup(uMsg, wParam, lParam, bHandled);
    return 0;
  }

  void AssociateImage(CMenuItemInfo& mii, coolMenuItemData * pMI){
    switch (mii.wID){
      case ID_A_MY6BEESPACE:
        pMI->iImage = phome;
        break;
      /*case ID_A_SCREENCAPTURE:
        pMI->iImage = 10;
        break;*/
      case ID_A_UPLOADFILES:
        pMI->iImage = pupload;
        break;
        /*
      case ID_A_RECORDVOICE:
        pMI->iImage = 7;
        break;
      case ID_A_SHOOTAVIDEO:
        pMI->iImage = 6;
        break;*/
      case ID_A_LOGINOUT:
        pMI->iImage = pkey;
        break;
      case ID_A_AUTOLOGIN:
        pMI->iImage = _6beed_share::u_getAutologin()?pgood:pbad;
        break;
      case ID_A_EXIT:
        pMI->iImage = pexit;
        break;
      default:
        pMI->iImage = -1;
        break;
    }
  }

  void DynamicChangeMenuItem(CMenuItemInfo& mii, coolMenuItemData * pMI, TCHAR* szString,bool& ignore){
    if (u_getLoggedin()){
      if (mii.wID==ID_A_LOGINOUT){
        const wchar_t* const a=L"Logout";
        int b = (int)wcslen(a)+1;//wcslen-must plus 1, because it is memory copy, need to copy the trailing null
        if (b>MAX_MENU_ITEM_TEXT_LENGTH){
          return;
        }
        ::wmemcpy(szString,a,b);
        pMI->iImage = pleave;
      }
      /*
      if (mii.wID==ID_A_EXIT){
      ignore = true;
      }
      */
    }else{
      if(mii.wID!=ID_A_LOGINOUT && 
        mii.wID!=ID_A_MY6BEESPACE && 
        mii.wID!=ID_A_EXIT && 
        mii.wID!=ID_A_AUTOLOGIN)
      {
        mii.fState = MFS_GRAYED;
      }
    }
  }

  virtual void PrepareMenu(HMENU hMenu){
    CMenuHandle menu(hMenu);
    menu.EnableMenuItem(ID_A_SINGOUT, MF_GRAYED);
  }

  virtual void SetMyMenu(CMenu& m){
    m.Attach(c6beeres::R().GetMenu(IDR_CLIENTMNGMENU));
  }

  void OnLogoutSuccess(){
    //SetTooltipText(_T("You have logged out UltraIE!"));
    _6beed_share::u_setSID("");
    _6beed_share::u_setLoggedin(false);
  _6beed_util::Notify6BeeTB(CPYDATA_LOGOUT);
    UpdateTimer(TIMER_LOGOUT,true);
  }

  void PostLogin(std::string& username,std::string& passwordtostore,std::string& sid){
    ShowWindow(SW_HIDE);
    std::wstring unamew_ = wstringmaker() << GetEmailShort(username.c_str());
    ATL::CString bonjour;
    bonjour.Format(L"Welcome %s:\nYou are now singed in UltraIE!",unamew_.c_str());
    SetTooltipText(bonjour);
    unamew_ = L"Welcome, " + unamew_;
    PopMsg(L"You have now singed in UltraIE!",unamew_.c_str());
    m_Note.SetWindowText(_T("Please login with your email and password."));
    HICON hIconSmall=ultraiestatusicons_.GetIcon(sloggedin);
    ChangeIcon(hIconSmall);
    DestroyIcon(hIconSmall);
    _6beed_share::u_setLoggedin(true);
    _6beed_share::u_setSID(sid.c_str());
    _6beed_share::u_setPassword(passwordtostore.c_str());
    _6beed_share::u_setEmail(username.c_str());
  _6beed_util::Notify6BeeTB(CPYDATA_LOGIN);
  }
};

void ErrorFunc(CMainDlg* pMainDlg,const wchar_t* errormsg){
  pMainDlg->PopMsg(errormsg,L"ERROR");
  pMainDlg->UpdateNoteImg(false);
  pMainDlg->m_Note.SetWindowText(errormsg);
  pMainDlg->SetTooltipText(tip_offline);
  UUDEBUG((LOG_ERROR,errormsg));
}

UINT WINAPI ThreadLogin(LPVOID lp){
  try{
    loginfo* li = (loginfo*) lp;
    CMainDlg* pMainDlg = li->mdlg;
    if (pMainDlg==NULL){
      UUDEBUG((LOG_ERROR,"pMainDlg is NULL in ThreadLogin"));
      return 0;
    }
    std::string uuname = li->name;
    std::string ppword = li->passwd;

    pMainDlg->SetTooltipText(tip_logining);
    UUTOOLBAR::UserPrx _user = MyICE::R();
    if(_user!=0){
      std::string sid;
      if(_user->Login(uuname,ppword,sid)==::UUTOOLBAR::LOGINOK){
        pMainDlg->PostLogin(uuname,ppword,sid);
      }else{
        pMainDlg->ModifyStyleEx(WS_EX_TOOLWINDOW,WS_EX_APPWINDOW);
        ::SetWindowPlacement(pMainDlg->m_hWnd,&wp);
        ::SetForegroundWindow(pMainDlg->m_hWnd);
        ErrorFunc(pMainDlg,tip_ntwkfail);
      }
    }else{
      ErrorFunc(pMainDlg,tip_ntwkfail);
    }
    pMainDlg->UpdateTimer(TIMER_LOGIN,true);
  }catch (...){}
  return 0;
}



UINT WINAPI ThreadLogout(LPVOID lp){
  try{
    CMainDlg* pMainDlg = (CMainDlg*)lp;
    
    /// sid should before OnLogoutSuccess()
    std::string sid(_6beed_share::u_getSID());

    pMainDlg->OnLogoutSuccess();
    pMainDlg->UpdateNoteImg();
    /// @todo Can we hide the icon and then log out???
    UUTOOLBAR::UserPrx _user = MyICE::R();
    if(_user==0){
      ErrorFunc(pMainDlg,tip_ntwkfail);
      return 0;
    }
    if(!_user->Logout(sid)){
#ifdef DEBUG
      ErrorFunc(pMainDlg,L"ERROR: Logout failed.");
#endif
    }
  }catch (...){}
  return 0;
}

#endif
