#pragma once

#include <atltime.h>
#include "ListCtrl.h"
#include "6bees_ftpclient.h"
#include "TrayiconImpl.h"
#include <6BeeContextMenu.h>
#include "util.h"
#include <copydatamsg.h>
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#include <6BeeResDialog.h>
#include <Ice/Ice.h>
#include "IceSingleton.h"
#include "6bees_shell.h"
#include "6bees_window.h"
#include "uulogging.h"
#include <imgcap_setting.h>
#include "constdata.h"
#include "6bees_str.h"
#include <6bees_lang.h>
using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;
using namespace std;
using namespace _6bees_socket;
using namespace _6bees_pattern;
using namespace _6beed_share;

#define CP_CHINESE 936

class CMainDlg;

struct loginfo{
  CMainDlg* mdlg;
  std::string name;
  std::string passwd;
};
static loginfo info={};
UINT WINAPI ThreadLogin(LPVOID);
UINT WINAPI ThreadLogout(LPVOID lp);
static const COLORREF BGCOLOR = RGB(237,255,200);
static const COLORREF TXTCOLOR = RGB(7,5,20);
static WINDOWPLACEMENT wp={};

enum etimer{ TIMER_LOGIN=1,TIMER_LOGOUT,TIMER_BUSYUPLOADING,TIMER_BUSYDOWNLOADING };

/// system tray icon image list - start with s
enum systrayicon{sloggedin=0,sconnect1,sconnect2,suploading,sdownloading,soffline,siconnum};
enum smode{MLOGIN=0,MUPLOAD};

string GetModiFileURL(const string& mod_filename);

class CMainDlg:
  public CDialogImpl<CMainDlg>,
  public CWinDataExchange<CMainDlg>,
  public CDialogResize<CMainDlg>,
  public CTrayIconImpl<CMainDlg>,
  public CCoolContextMenu<CMainDlg>,
  public ftpclient::CNotification
{
private:
  CListWindowFileData m_wndFileList;
  CWindow m_wndFinished;
  CWindow m_wndUpload;
  HICON hIconSmall;
  HCURSOR  m_hCursor;
  CMenu    m_CMainDlgmenu;
  FList m_uploadFiles;
  FList m_upload_finished_Files;
  ftpclient ftpClient[ATSMAXCOUNT];
  //For launch file & open folder. What ever how many files you open,we just process the first one!
  CFileProfile first_selected_file;
  list<int>& aSelectedItems;

  CImageList m_imglst;
  CBitmapButton m_addupload;
  CBitmapButton m_myuploads;

  // Add for login mode
  CHyperLink m_ForgetPass;
  CWindow m_CUserName;
  CWindow m_CPassWd;
  CWindow m_bgimg;
  CWindow m_YourEmail;
  CWindow m_YourPass;
  CBitmapButton m_CLogIn;
  CBitmapButton m_CRegister;
  CBitmapButton m_NoteImg;

  /// login system tray icon
  CImageList ultraiestatusicons_;

public:
  enum { IDD = IDD_MAINDLG };

  BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_SYSCOMMAND,OnSysCommand)
    MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
    MESSAGE_HANDLER(WM_COPYDATA,  OnCopyData)
    MESSAGE_HANDLER(WM_TIMER,      OnTimer)
    MSG_WM_DROPFILES(OnDropFiles)
    COMMAND_ID_HANDLER(IDC_ADDUPLOAD,OnIDC_ADDUPLOAD)
    COMMAND_ID_HANDLER(IDC_MYUPLOADS,OnFinished)
    COMMAND_ID_HANDLER(IDC_UPLOAD,  OnUpload)
    COMMAND_HANDLER(IDC_LOGIN_,BN_CLICKED,   OnSignin)
    COMMAND_HANDLER(IDC_REGISTER_,BN_CLICKED,   OnRegister)
    //Menu 1
    COMMAND_ID_HANDLER(ID_D_DISPLAY,OnFileOpen)
    COMMAND_ID_HANDLER(ID_D_ONLINESPACE,OnID_D_ONLINESPACE)
    COMMAND_ID_HANDLER(ID_D_EXITUPLOADER,  OnExit)
    COMMAND_ID_HANDLER(ID_D_EXIT,  OnExit)
    COMMAND_ID_HANDLER(ID_D_LOGINOUT,  OnLogInOut)
    COMMAND_ID_HANDLER(ID_D_AUTOLOGIN,   OnAutoLogIn)
    //Menu 2
    COMMAND_ID_HANDLER(ID_B_START,   OnID_B_START)
    COMMAND_ID_HANDLER(ID_B_OPENFOLDER,OnID_B_OPENFOLDER)
    COMMAND_ID_HANDLER(ID_B_LAUNCHFILE,OnID_B_LAUNCHFILE)
    COMMAND_ID_HANDLER(ID_B_PAUSE,  OnID_B_PAUSE)
    NOTIFY_HANDLER_EX(IDC_FILELIST,LCN_HYPERLINK, OnListHyperLink)
    NOTIFY_HANDLER_EX(IDC_FILELIST,LCN_RIGHTCLICK,OnRightClick)
    NOTIFY_HANDLER_EX(IDC_FILELIST,LCN_DBLCLICK,  OnDoubleClick)
    CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
    CHAIN_MSG_MAP(CTrayIconImpl<CMainDlg>)
    CHAIN_MSG_MAP(CCoolContextMenu<CMainDlg>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DDX_MAP(CMainDlg)
    DDX_CONTROL(IDC_FILELIST,m_wndFileList)
  END_DDX_MAP()

  BEGIN_DLGRESIZE_MAP(CMainDlg)  
    DLGRESIZE_CONTROL(IDC_FILELIST,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_ADDUPLOAD,DLSZ_MOVE_X|DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_MYUPLOADS,DLSZ_MOVE_X|DLSZ_MOVE_Y)
    //DLGRESIZE_CONTROL(IDC_STATICTIP,DLSZ_MOVE_X|DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()

public:
  CWindow m_Note;
  CWindow m_usertip;

  static __int64 sentBytes[ATSMAXCOUNT];
  static bool is_prev_uploading;

  void OnDropFiles(HDROP hDrop){
    wchar_t   szFilePathName[_MAX_PATH+1] = {0};
    UINT  nNumOfFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
    int mssize=0;
    for (UINT nIndex=0 ; nIndex<nNumOfFiles; ++nIndex){
      DragQueryFile(hDrop, nIndex, szFilePathName, _MAX_PATH);
      if(!PathIsDirectory(szFilePathName) && PathFileExists(szFilePathName) && _6beed_util::GetFileSizeW(szFilePathName,mssize)){
        CFileProfile fileProfile(0,szFilePathName,mssize);
        AddUploadFile(fileProfile,false);
      }
    }
    ShowFList(m_uploadFiles,false);
    DragFinish(hDrop);
  }

  CMainDlg():aSelectedItems(m_wndFileList.m_setSelectedItems){
    m_wndFileList.RegisterClass();
  }

  ~CMainDlg(){
    m_ImageList.Destroy();
  }

  bool IsEmpty(){
    return m_uploadFiles.IsEmpty();
  }

  bool AddUploadFile(const CFileProfile& fileProfile,bool first=true){
    for (int i=0;i<m_uploadFiles.m_nSize;++i){
      if (m_uploadFiles[i]==fileProfile){
        MessageBox(fileProfile.m_absPath,kselected,MB_ICONINFORMATION|MB_OK);
        return false;
      }
    }
    if(!first)m_wndFileList.AddFile(fileProfile);
    return m_uploadFiles.Add(fileProfile);
  }

  bool AddImediateUploadFile(const CFileProfile& fileProfile,bool first=true){
    if(AddUploadFile(fileProfile,first)){
      if(!sfp_active.empty()){
        is_prev_uploading=true;
        pauseupload(true);
      }
      int nItem = m_uploadFiles.GetSize()-1;
      if (aSelectedItems.empty() || nItem!=aSelectedItems.front()){
        aSelectedItems.push_front(nItem);
      }
    }
    return true;
  } 

  LRESULT OnInitDialog(UINT,WPARAM,LPARAM,BOOL&){
    const wchar_t* uutbpath = ultrapath::R().get(ultrapath::uutoolbardll).c_str();
    _6beed_util::CheckVersion(uutbpath);
    DragAcceptFiles(TRUE);
    DlgResize_Init(true,false,WS_THICKFRAME|WS_CLIPCHILDREN);
    HBITMAP hbmp=(HBITMAP)c6beeres::R().GetImg(ULTRA_IMGS,IMAGE_BITMAP,16*54);
    SetMyImageList(hbmp,m_ImageList,MASKCOLOR,16,16,54,GetSysColor(COLOR_3DFACE));
    CenterWindow();

    HBITMAP hbmp3=(HBITMAP)c6beeres::R().GetImg(IDB_ULTRAIESTATUS,IMAGE_BITMAP,16*siconnum);
    SetMyImageList(hbmp3,ultraiestatusicons_,MASKGREY,16,16,siconnum,GetSysColor(COLOR_3DFACE));

    hIconSmall = ultraiestatusicons_.GetIcon(soffline);
    //HICON h1 = SetIcon(hIconSmall,FALSE);
    //HICON h2 = SetIcon(hIconSmall,TRUE);
    DoDataExchange(false);
    ShowFList(m_uploadFiles);
    m_wndFinished.Attach(GetDlgItem(IDC_FINISHED));
    m_wndUpload.Attach(GetDlgItem(IDC_UPLOAD));
    //m_wndUpload.EnableWindow(FALSE);
    GetSystemSettings();
    InitializeCriticalSection(&csection);
    InstallIcon(ktitle,hIconSmall,IDR_POPUP);
    DestroyIcon(hIconSmall);
    SetDefaultItem(IDC_FINISHED);
    //m_wndFileList.SetBackgroundImage((HBITMAP)c6beeres::R().GetImg(
    //IDB_BITMAP_INFOBK,IMAGE_BITMAP,INFO_WIDTH,INFO_HEIGHT));// wiil be release automatically

    HBITMAP hbmp2=(HBITMAP)c6beeres::R().GetImg(IDB_ULTRABUTTONS,IMAGE_BITMAP,95*17,26);
    SetMyImageList(hbmp2,m_imglst,MASKPURPLE,95,26,17,GetSysColor(COLOR_3DFACE));
    m_addupload.SubclassWindow(GetDlgItem(IDC_ADDUPLOAD));
    m_addupload.SetToolTipText(kaddtip);
    m_addupload.SetImageList(m_imglst);
    m_addupload.SetImages(7,-1,6,-1);
    m_addupload.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_myuploads.SubclassWindow(GetDlgItem(IDC_MYUPLOADS));
    m_myuploads.SetToolTipText(kallfilestip);
    m_myuploads.SetImageList(m_imglst);
    m_myuploads.SetImages(1,-1,0,-1);
    m_myuploads.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    USES_CONVERSION;
    const wchar_t* uu = A2CW(u_getEmail());
    bool registereduser=(wcslen(uu)!=0);
    const wchar_t* pp = A2CW(u_getPassword());
    m_ForgetPass.SubclassWindow(GetDlgItem(IDC_FORGETPWD));// URL link
    m_ForgetPass.SetHyperLink(_6bees_const::kUltraIEURL_FORGETPASS);
    m_Note.Attach(GetDlgItem(IDC_NOTE));
    m_CUserName.Attach(GetDlgItem(IDC_USERNAME_));
    m_CUserName.SetFocus();
    m_CUserName.SetWindowText(uu);
    m_CPassWd.Attach(GetDlgItem(IDC_PASSWD_));

    m_CLogIn.SubclassWindow(GetDlgItem(IDC_LOGIN_));
    m_CLogIn.SetToolTipText(klogin);
    m_CLogIn.SetImageList(m_imglst);
    m_CLogIn.SetImages(16,-1,15,-1);
    m_CLogIn.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_CRegister.SubclassWindow(GetDlgItem(IDC_REGISTER_));
    m_CRegister.SetToolTipText(kregister);
    m_CRegister.SetImageList(m_imglst);
    m_CRegister.SetImages(14,-1,13,-1);
    m_CRegister.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_NoteImg.SubclassWindow(GetDlgItem(IDC_NOTEIMG));
    m_NoteImg.SetImageList(m_ImageList);
    m_NoteImg.SetImages(pinfo);

    m_usertip.Attach(GetDlgItem(IDC_STATICTIP));

    m_bgimg.Attach(GetDlgItem(IDC_BGIMG));
    m_YourEmail.Attach(GetDlgItem(IDC_YOUREMAIL));
    m_YourPass.Attach(GetDlgItem(IDC_YOURPASS));
    UpdateMode(MLOGIN);

    GetWindowPlacement(&wp); //For window restore
    if(registereduser){
      if(u_getAutologin()){
        wchar_t notice[100]={};
        swprintf_s(notice,100,klogintip,uu);
        m_Note.SetWindowText(notice);
        m_CPassWd.SetWindowText(L"**********");
        info.mdlg = this;
        info.name = u_getEmail();
        info.passwd = u_getPassword();
        ::_beginthreadex(NULL,0,ThreadLogin,(LPVOID)&info,0,NULL);
      }
    }else{
      m_Note.SetWindowText(ktip1);
    }
    startupload();
    return TRUE;
  }

  // login mode or upload mode!
  void UpdateMode(smode mode){
    static int cx = ::GetSystemMetrics( SM_CXFULLSCREEN);
    static int cy = ::GetSystemMetrics( SM_CYFULLSCREEN);

    switch( mode){
case MLOGIN:
  m_wndFileList.ShowWindow(SW_HIDE);
  m_addupload.ShowWindow(SW_HIDE);
  m_myuploads.ShowWindow(SW_HIDE);
  m_usertip.ShowWindow(SW_HIDE);
  ::SetWindowPos(m_hWnd,HWND_TOP,(cx-342)/2,(cy-230)/2,342,230,SWP_FRAMECHANGED);

  m_ForgetPass.ShowWindow(SW_SHOW);
  m_CUserName.ShowWindow(SW_SHOW);
  m_CPassWd.ShowWindow(SW_SHOW);
  m_CLogIn.ShowWindow(SW_SHOW);
  m_CRegister.ShowWindow(SW_SHOW);
  m_NoteImg.ShowWindow(SW_SHOW);
  m_Note.ShowWindow(SW_SHOW);
  m_bgimg.ShowWindow(SW_SHOW);
  m_YourEmail.ShowWindow(SW_SHOW);
  m_YourPass.ShowWindow(SW_SHOW);
  break;
case MUPLOAD:
  {
    static const int cwidth = 580;
    static const int cheight = 300;
    m_wndFileList.ShowWindow(SW_SHOW);
    m_addupload.ShowWindow(SW_SHOW);
    m_myuploads.ShowWindow(SW_SHOW);
    m_usertip.ShowWindow(SW_SHOW);
    ::SetWindowPos(m_hWnd,HWND_TOP,(cx-cwidth)/2,(cy-cheight)/2,cwidth,cheight,SWP_FRAMECHANGED);

    m_ForgetPass.ShowWindow(SW_HIDE);
    m_CUserName.ShowWindow(SW_HIDE);
    m_CPassWd.ShowWindow(SW_HIDE);
    m_CLogIn.ShowWindow(SW_HIDE);
    m_CRegister.ShowWindow(SW_HIDE);
    m_NoteImg.ShowWindow(SW_HIDE);
    m_Note.ShowWindow(SW_HIDE);
    m_bgimg.ShowWindow(SW_HIDE);
    m_YourEmail.ShowWindow(SW_HIDE);
    m_YourPass.ShowWindow(SW_HIDE);
  }
  break;
    }
  }

  LRESULT OnLogInOut(WORD,WORD wID,HWND,BOOL&){
    if(u_getLoggedin()){
      ExitMe();
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

  LRESULT OnRegister(WORD,WORD,HWND,BOOL&){
    _6beed_util::runIE(_6bees_const::kUltraIEURL_REGISTER);
    return 0;
  }

  LRESULT OnSignin(WORD,WORD wID,HWND,BOOL&){
    static const short UPmaxlen = 40;
    static const short UPminlen = 4;
    wchar_t username[UPmaxlen]={},password[UPmaxlen]={};
    m_CUserName.GetWindowText(username,UPmaxlen);
    m_CPassWd.GetWindowText(password,UPmaxlen);
    wstring tmp(username);
    if (tmp.find(L'@')==wstring::npos){
      MessageBoxA(NULL,kunerrortxt,kunerrortitle,MB_OK|MB_ICONINFORMATION);
      return 0;
    }
    if(wcslen(password)<=UPminlen){
      MessageBoxA(NULL,kpwerrortxt,kpwerrortitle,MB_OK|MB_ICONINFORMATION);
      return 0;
    }
    UpdateNoteImg(true);
    m_Note.SetWindowText(tip_logining);
    SetTimer(TIMER_LOGIN,300);

    info.mdlg = this;
    info.name = stringmaker() << username;
    info.passwd = GetMD5((stringmaker()<<password).c_str());
    ::_beginthreadex(NULL,0,ThreadLogin,(LPVOID)&info,0,NULL);
    return 0;
  }

  LRESULT OnListHyperLink( LPNMHDR lpNMHDR){
    CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR);
    CString url=m_wndFileList.GetItemText(pListNotify->m_nItem,pListNotify->m_nSubItem);
    if (!url.IsEmpty()){
      _6beed_util::runIE(url);
    }
    return 0;
  }

  LRESULT OnRightClick( LPNMHDR lpNMHDR){
    CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR);
    m_wndFileList.GetFile( pListNotify->m_nItem,first_selected_file);
    if (m_CMainDlgmenu.m_hMenu != NULL)
      return 0;
    SetFocus();
    CPoint pt=pListNotify->m_cp;
    ClientToScreen(&pt);
    if (!m_CMainDlgmenu.LoadMenu(IDR_POPUP)){
      return 0;
    }
    CMenuHandle menuPopup = m_CMainDlgmenu.GetSubMenu(1);
    bool ifshowstart=true;
    bool ifshowdelete=true;
    int msize = m_wndFileList.m_aFiles.GetSize();
    for(list<int>::const_iterator pr=aSelectedItems.begin();pr!=aSelectedItems.end();pr++){
      int index = (int)(*pr);
      if(index <msize){
        CFileProfile fp(m_wndFileList.m_aFiles[index]);
        switch (fp.m_status){
case S_ACTIVE:
  ifshowstart = false;
  break;
case S_SUSPENDED:
  ifshowdelete = false;
  break;
default:break;
        }
      }
    }
    if(!ifshowstart)
      menuPopup.EnableMenuItem(ID_B_START,MF_GRAYED);
    if(!ifshowdelete)
      menuPopup.EnableMenuItem(ID_B_PAUSE,MF_GRAYED);
    if (menuPopup.m_hMenu == NULL){
      return 0;
    }
    if (!menuPopup.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON,pt.x,pt.y,m_hWnd)){
      return 0;
    }
    menuPopup.DestroyMenu();
    m_CMainDlgmenu.DestroyMenu();
    m_CMainDlgmenu.m_hMenu = NULL;
    return 0;
  }

  LRESULT OnDoubleClick( LPNMHDR lpNMHDR){
    startupload();
    return 0;
  }

  LRESULT OnIDC_ADDUPLOAD(WORD,WORD wID,HWND,BOOL&){
    try{
      int mssize=0;
      ATL::CString strFolderPath,strFileName;
      PopDlgtoSelectFiles(strFolderPath,strFileName,mssize,false);
      ShowFList(m_uploadFiles,false);
    }catch (...){}
    return 0;
  }

  LRESULT OnExit(WORD,WORD wID,HWND,BOOL&){
    ExitMe();
    return 0;
  }

  void ExitMe(){
    //forbid uploading and close ftp connection at the same time...
    for(int i=0;i<ATSMAXCOUNT;i++){
      ftpClient[i].SetTerminatethisthread(true);
    }
    for (int i=0;i<50;i++){
      Sleep(100);
      if(sfp_active.empty()){
        break;
      }
    }
    DeleteCriticalSection(&csection);
    RemoveIcon();

    const static DWORD kTimeout = 6000;
    HANDLE h = (HANDLE)::_beginthreadex(NULL,0,ThreadLogout,(LPVOID)this,0,NULL);
    DWORD dw=WaitForSingleObject(h,kTimeout);
    switch (dw){
case WAIT_OBJECT_0:
case WAIT_TIMEOUT:
default:{EndDialog(0);break;}
    }
  }

  // Call Back by Upload thread!!!!
  virtual void OnSetLRemoteFileSize(int threadID,long lRemoteFileSize){
    sentBytes[threadID] += lRemoteFileSize;
  }

  virtual void OnBytesSent2(long lSentBytes,const CString& flocation,int threadID,
    long curspeed,long timepassed,long timeremaining)
  {
    int nItem=m_wndFileList.GetnItem(flocation);
    if (sentBytes[threadID]>0){
      m_wndFileList.SetItemStatus(nItem,S_ACTIVE);
    }
    sentBytes[threadID] += lSentBytes;
    if(ftpClient[threadID].suspendit==NO_SUSPEND)//update uploading progress
      m_wndFileList.UpdateItemProgress(flocation,sentBytes[threadID],
      nItem,curspeed,timepassed,timeremaining);
    if(sentBytes[threadID]==m_wndFileList.m_aFiles[nItem].m_size_long){
      m_wndFileList.m_aFiles[nItem].m_progress = 100;
      m_wndFileList.m_aFiles[nItem].m_timeremaining = _T("");
      //m_wndFileList.m_aFiles[nItem].m_link = _6bees_const::kUltraIEURL;
      m_wndFileList.SetItemStatus(nItem,S_FINISHED);
      m_upload_finished_Files.Add(m_wndFileList.m_aFiles[nItem]);
      m_wndFileList.Invalidate();
    }
  }

  static bool getftpinfo(string& ip,short& port){
    static string ftp_ip;
    static short ftp_port=0;
    if(ftp_port==0){
      UUTOOLBAR::UserPrx _user = MyICE::R();
      if((_user._ptr==NULL)||!_user->GetFTPInfoForUpload(_6beed_share::u_getEmail(),ftp_ip,ftp_port)){
        return false;
      }
    }
    ip=ftp_ip;
    port=ftp_port;
    return true;
  }

  static string GetUploadedFileURL(const wstring& absname){
    UUTOOLBAR::UserPrx _user = MyICE::R();
    if(_user._ptr!=NULL){
      int fsize=0;
      if(_6beed_util::GetFileSizeW(absname.c_str(),fsize)){
        DWORD crc32=0;
        if(_6bees_util::FileCrc32Assembly(absname.c_str(),crc32)!=ERROR_CRC){
          string crc32str = stringmaker() << crc32;
          string mod_filename = _user->IsFileUploaded(crc32str,fsize);
          return GetModiFileURL(mod_filename);
        }
      }
    }
    return string("");
  }

  static UINT WINAPI _ThreadUploadProc(LPVOID pParam){
    tpacket* _ptps = (tpacket*)pParam;

    CFileProfile* t_pfp    = _ptps->t_pfp;
    CListWindowFileData* t_pWndFD    = _ptps->t_pWndFD;
    FList* m_uploadFiles   = _ptps->t_up_flist;
    _6bees_timer::CTimer* t_Timer   = _ptps->t_timer;
    ftpclient* ftpClient  = _ptps->t_ftpclient;
    CMainDlg* m_maindlg    = (CMainDlg*)_ptps->t_pmaindlg;
    int t_ID = _ptps->t_ID;

    int t_nItem = t_pWndFD->GetnItem(t_pfp->m_absPath);

    USES_CONVERSION;
    string fetchurl = GetUploadedFileURL(t_pfp->m_absPath.GetString());
    if (!fetchurl.empty()){
      const wchar_t* link = A2CW(fetchurl.c_str());
      t_pfp->m_link = link;
      t_pfp->m_status = S_FINISHED;
      t_pWndFD->InvalidateItem(t_nItem);
      t_pWndFD->SetItemStatus(t_nItem,S_FINISHED);
      t_pWndFD->m_aFiles[t_nItem].m_link = link;
      t_pWndFD->m_aFiles[t_nItem].m_progress = 100;
      sfp_active.erase(t_pfp->m_absPath);
      return 0;
    }

    ftpClient->AttachObserver(m_maindlg);
    ftpClient->SetTimer(t_Timer);
    ftpClient->SetThreadID(t_ID);

    static string ftp_ip;
    static short ftp_port=0;
    if(ftp_port==0){
      if(!getftpinfo(ftp_ip,ftp_port)){
        t_pWndFD->m_aFiles[t_nItem].m_status = S_SUSPENDED;
        t_pWndFD->SetItemStatus(t_nItem,S_SUSPENDED);
        sfp_active.erase(t_pfp->m_absPath);
        return 0;
      }
    }
    CLogonInfo logonInfo(A2CW(ftp_ip.c_str()),ftp_port,A2CW(_6beed_share::u_getEmail()),A2CW(_6beed_share::u_getPassword()));
    if(ftpClient->Login(logonInfo)){
      if(!uploadfilewrap(t_pfp,m_maindlg,t_ID,ftpClient,t_pWndFD,true)){
        return 0;
      }
      bool isDirectUpload=t_pfp->m_isDirectUpload;
      while( !sfp_waiting_list.empty()){
        if(sfp_active.size() >= ATSMAXCOUNT){break;}
        if (is_prev_uploading || isDirectUpload==sfp_waiting_list.front()->m_isDirectUpload){
          CFileProfile* tmp = sfp_waiting_list.front();
          string fetchurl2 = GetUploadedFileURL(tmp->m_absPath.GetString());
          if (!fetchurl2.empty()){// this file has been uploaded before
            const wchar_t* link2 = A2CW(fetchurl2.c_str());
            int nitem = t_pWndFD->GetnItem(tmp->m_absPath);
            t_pWndFD->InvalidateItem(nitem);
            t_pWndFD->SetItemStatus(nitem,S_FINISHED);
            t_pWndFD->m_aFiles[nitem].m_link = link2;
            t_pWndFD->m_aFiles[nitem].m_progress = 100;
            sfp_waiting_list.pop_front();
          }else{
            if(!uploadfilewrap(tmp,m_maindlg,t_ID,ftpClient,t_pWndFD,false)){
              return 0;
            }
          }
        }else{
          break;
        }
      }
      ftpClient->Logout();
    }else{
      int t_nItem = t_pWndFD->GetnItem(t_pfp->m_absPath);
      t_pWndFD->m_aFiles[ t_nItem ].m_status = S_SUSPENDED;
      t_pWndFD->SetItemStatus(t_nItem,S_SUSPENDED);
      sfp_active.erase(active_fp[t_ID].m_absPath);
      UUDEBUG((LOG_ERROR,"Upload loging failed."));
    }
    ftpClient->DetachObserver(m_maindlg);
    return 0;
  }

  static bool uploadfilewrap(CFileProfile* t_pfp,CMainDlg* m_maindlg,int t_ID,
    ftpclient* ftpClient,CListWindowFileData* t_pWndFD, bool isFirst)
  {
    timer_notifier tn(CPYDATA_FILEUPLOAD_START,CPYDATA_UPLOADBUSY_STOP);
    if (!isFirst){
      sfp_active.insert(t_pfp->m_absPath);
      EnterCriticalSection(&csection);
      active_fp[t_ID] = (*t_pfp); //update active_fp - thread safe
      LeaveCriticalSection(&csection);
      sfp_waiting_list.pop_front();//thread safe
      m_maindlg->m_wndFileList.SetFtpClientID(t_pfp->m_absPath,t_ID);
    }
    ftpClient->SetFL(t_pfp->m_absPath);
    ftpClient->Setufilesize(t_pfp->m_size_long);

    int t_nItem = t_pWndFD->GetnItem(t_pfp->m_absPath);
    t_pWndFD->SetItemStatus(t_nItem,S_ACTIVE);

    string wdesc,wtag,wthumb,mmf_descname,mmf_tagname,mmf_thumbname;
    string fname_ = stringmaker(CP_ACP) << PathFindFileNameW(t_pfp->m_filename);
    GenMMFName(fname_,mmf_descname,mmf_tagname,mmf_thumbname);
    if(GetMMF(mmf_descname,wdesc) && GetMMF(mmf_tagname,wtag) && GetMMF(mmf_thumbname,wthumb)){
      _6bees_str::strreplace(wdesc,"'","\\'");
      _6bees_str::strreplace(wtag,"'","\\'");
    }
    //Shutdown Screen capture
    HWND h=::FindWindow(NULL,kcap_ic);
    if (h){
      COPYDATASTRUCT cds;
      cds.dwData = 0;
      cds.lpData   = NULL;
      cds.cbData  = NULL;
      ::SendMessage(h,WM_COPYDATA,(WPARAM)0,(LPARAM)&cds);
    }

    bool upload_success = ftpClient->UploadFile(
      t_pfp->m_absPath.GetString(),
      t_pfp->m_filename.GetString());//blocking operation

    CMainDlg::sentBytes[t_ID]=0;
    int& status = t_pWndFD->m_aFiles[ t_nItem ].m_status;
    if(upload_success){
      USES_CONVERSION;
      string thumbpath = wthumb;
      string fetchurl,modifname;
      string filename = CW2A(t_pfp->m_filename,CP_UTF8);
      UUTOOLBAR::UserPrx _user = MyICE::R();
      if(_user._ptr!=NULL){
        bool ischineseos_ = _6beed_util::isChineseOS();
        string desc = stringmaker() << (wstringmaker(ischineseos_?CP_CHINESE:CP_UTF8) << wdesc);
        string tag  = stringmaker() << (wstringmaker(ischineseos_?CP_CHINESE:CP_UTF8) << wtag);
        try{
          // here we need utf-8 encoding
          const char* email = _6beed_share::u_getEmail();
          const char* sid = _6beed_share::u_getSID();
          DWORD crc32 = 0;
          _6bees_util::FileCrc32Assembly(t_pfp->m_absPath,crc32);
          string crc32str = stringmaker() << crc32;
          long fid=(long)MyICE::R()->PostUploadFile(
            filename,t_pfp->m_size_long,crc32str,desc,tag,email,sid,modifname);
          sfp_active.erase(t_pfp->m_absPath);
          if(fid>0){

            // 1.Upload thumbs
            if (thumbpath.empty()){
              if(t_pfp->m_extension.CompareNoCase(L"jpg") || 
                t_pfp->m_extension.CompareNoCase(L"png") || 
                t_pfp->m_extension.CompareNoCase(L"bmp") ||
                t_pfp->m_extension.CompareNoCase(L"gif"))
              {
                wstring tmp = wstringmaker(CP_ACP) << t_pfp->m_absPath.GetString();
                wstring newname;
                CreateThumbnail(tmp,newname,THUMBIMGW/3,THUMBIMGH/3);
                thumbpath = stringmaker(CP_ACP) << newname;
              }
            }
            if (!thumbpath.empty()){
              UUTOOLBAR::ImgSeq thumbs;
              UUTOOLBAR::Image img;
              if(_6bees_shell::GenICEImg(thumbpath.c_str(),img)){
                thumbs.push_back(img);
                MyICE::R()->UploadImgSeq(thumbs,email,sid,UUTOOLBAR::MYTHUMB,fid);
              }
              ::DeleteFileA(thumbpath.c_str());
            }

            fetchurl = GetModiFileURL(modifname);
            t_pWndFD->m_aFiles[t_nItem].m_link = A2CW(fetchurl.c_str());
            t_pWndFD->InvalidateItem(t_nItem);

            string info = stringmaker(CP_ACP) << kktitle  << ": "<< t_pfp->m_filename.GetString() << "\n";
            if(!wtag.empty()){
              info += stringmaker(CP_ACP) << kktag << ": "<< wtag << "\n";
            }
            if(!wdesc.empty()){
              info += stringmaker(CP_ACP) << kkdesc << ": "<< wdesc;
            }
            // TODO
            _6bees_window::NotifyClientMng(CPYDATA_FILEUPLOAD_OK,
              _6bees_const::kUploader_WndTitle,info.c_str());
          }else{
            t_pWndFD->m_aFiles[t_nItem].m_link = L"上传失败,请重试";
            t_pWndFD->InvalidateItem(t_nItem);
            /// @todo if one upload failed
          }
        }catch (const Ice::Exception& e){
          const char* s = e.what();
          UUDEBUG((LOG_ERROR,s));
        }
      }
    }else{
      UUDEBUG((LOG_DBG,"Upload failed."));
      switch (ftpClient->suspendit){
case SUSPEND_SUCCESS:
  UUDEBUG((LOG_DBG,"FTPClient close by command manually."));
  UUDEBUG((LOG_DBG,t_pfp->m_absPath.GetString()));
  sfp_active.erase(t_pfp->m_absPath);
  ATLASSERT(status==S_ACTIVE);
  status = S_SUSPENDED;
  t_pWndFD->Invalidate();
  ftpClient->suspendit=SUSPEND_RECOVER;
  return false;
case NO_SUSPEND:
  UUDEBUG((LOG_DBG,"FTPClient close by natural reson."));
  UUDEBUG((LOG_DBG,t_pfp->m_absPath.GetString()));
  sfp_active.erase(t_pfp->m_absPath);
  ATLASSERT(status==S_ACTIVE);
  status = S_SUSPENDED;
  t_pWndFD->Invalidate();
  ftpClient->suspendit=SUSPEND_RECOVER;
  ftpClient->Logout();
  return false;
default:break;
      }
    }
    return true;
  }

  LRESULT OnID_B_START(WORD,WORD wID,HWND,BOOL&){
    startupload();
    return 0;
  }

  LRESULT OnID_B_OPENFOLDER(WORD,WORD wID,HWND,BOOL&){
    if(aSelectedItems.size()<1)return 0;
    ATL::CString tmp=first_selected_file.m_absPath;
    int i=tmp.ReverseFind(L'\\');
    tmp=tmp.Left(i);
    ShellExecuteW(NULL,NULL,tmp,NULL,NULL,SW_SHOW);
    return 0;
  }

  LRESULT OnID_B_LAUNCHFILE(WORD,WORD wID,HWND,BOOL&){
    if(aSelectedItems.size()<1)return 0;
    ATL::CString tmp=first_selected_file.m_absPath;
    ShellExecuteW(NULL,NULL,tmp,NULL,NULL,SW_SHOW);
    return 0;
  }

  LRESULT OnID_B_PAUSE(WORD,WORD wID,HWND,BOOL&){
    pauseupload();
    return 0;
  }

  LRESULT OnFileOpen(WORD,WORD,HWND,BOOL&){
    if (!IsWindowVisible())
      ShowWindow(SW_SHOW);
    if (IsIconic())
      ShowWindow(SW_RESTORE);
    else
      BringWindowToTop();
    ::SetForegroundWindow(m_hWnd);
    return 0;
  }

  LRESULT OnFinished(WORD,WORD wID,HWND,BOOL&){
    _6beed_util::runIE(_6bees_const::kUltraIEURL);
    return 0;
  }
  LRESULT OnID_D_ONLINESPACE(WORD,WORD wID,HWND,BOOL&){
    _6beed_util::runIE(_6bees_const::kUltraIEURL);
    return 0;
  }

  LRESULT OnUpload(WORD,WORD wID,HWND,BOOL&){
    return 0;
  }

  LRESULT OnHide(WORD,WORD wID,HWND,BOOL&){
    ShowWindow(SW_HIDE);
    return 0;
  }

  LRESULT OnCommand(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    bHandled = FALSE;
    if (!HIWORD(wParam)){
      long lSite = LOWORD(wParam);
      if (lSite == WM_DESTROY && uMsg == WM_COMMAND){
        //InstallIcon(_T("6Bee.Com - UpDownLoader!\n2007@All Copyrights Reserved!"),hIconSmall,IDR_POPUP);
        ShowWindow(SW_HIDE);
        bHandled = TRUE;
      }
    }
    return 0;
  }

  LRESULT OnSysCommand(UINT uMsg,WPARAM wParam,LPARAM,BOOL& bHandled){
    bHandled = FALSE;
    if(wParam == SC_MINIMIZE || wParam == SC_CLOSE) {
      ShowWindow(SW_HIDE);
      bHandled = TRUE;
    }
    if(wParam == SC_MAXIMIZE) {
      bHandled = FALSE;
    }
    return true;
  }

  LRESULT OnCopyData(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    COPYDATASTRUCT* p = (COPYDATASTRUCT*)lParam;
    int m = (int)p->dwData;
    switch (m){
case CPYDATA_UPLOAD_2_UPLOAD:
  {
    char* tmp= (char*)(p->lpData);
    wstring f=wstringmaker(CP_ACP)<<tmp;
    vector<wstring> vf=Split(f,_T("\t"));
    for(UINT i=0;i<vf.size();i++){
      int mssize(0);
      if(!_6beed_util::GetFileSizeW(vf[i].c_str(),mssize)){continue;}
      CFileProfile fileProfile(0,vf[i].c_str(),mssize);
      AddUploadFile(fileProfile,false);
    }
  }break;
case CPYDATA_UPLOAD_2_UPLOAD_DIRECT:
  {
    char* tmp= (char*)(p->lpData);
    wstring filepath=wstringmaker(CP_ACP)<<tmp;
    int fsize=0;
    if(!_6beed_util::GetFileSizeW(filepath.c_str(),fsize)){return 0;}
    CFileProfile fileProfile(0,filepath.c_str(),fsize,true);
    AddImediateUploadFile(fileProfile,false);
    startupload();
  }break;
case CPYDATA_LOGIN:
  if (u_getAutologin()){
    info.mdlg = this;
    info.name = u_getEmail();
    info.passwd = u_getPassword();
    ::_beginthreadex(NULL,0,ThreadLogin,(LPVOID)&info,0,NULL);
  }else{
    UpdateMode(MLOGIN);
  }
  break;
case CPYDATA_LOGOUT:
  // Do not update database, the source side(Toolbar) should update it.???
  //::_beginthreadex(NULL,0,ThreadLogout,(LPVOID)this,0,NULL);
  ExitMe();
  break;
case CPYDATA_FILEUPLOAD_OK:
  {
    wstring m = wstringmaker(CP_ACP)<<(char*)(p->lpData);
    m.resize(255);
    PopMsg(m.c_str(),tipuploadsuc);
    return true;
  }break;
default:
  break;
    }
    if (!IsWindowVisible())
      ShowWindow(SW_SHOW);
    if (IsIconic())
      ShowWindow(SW_RESTORE);
    else
      BringWindowToTop();
    ::SetForegroundWindow(m_hWnd);
    ShowFList(m_uploadFiles,false);
    return true;
  }

  // add popup menu code start
  LRESULT OnInitMenuPopup(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    int nPos;
    UINT uId;
    CCoolContextMenu<CMainDlg>::OnInitMenuPopup(uMsg,wParam,lParam,bHandled);
    m_hCursor = ::LoadCursor(NULL,IDC_ARROW);
    ATLASSERT(m_hCursor);
    ::SetCursor(m_hCursor);
    CMenuHandle menuPopup = (HMENU)wParam;
    for (nPos = 0; nPos < menuPopup.GetMenuItemCount(); nPos++){ 
      uId = GetMenuItemID(menuPopup,nPos);
      switch (uId){ 
case ID_B_SUSPENDED:
  EnableMenuItem(menuPopup,uId,MF_BYCOMMAND|MF_GRAYED);
  break;
default:
  break;
      }
    }
    return 0;
  }

  void AssociateImage(CMenuItemInfo& mii,coolMenuItemData * pMI){
    switch (mii.wID){
case ID_B_START:
  pMI->iImage = pup;
  break;
case ID_B_PAUSE:
  pMI->iImage = pgreen;
  break;
case ID_B_OPENFOLDER:
  pMI->iImage = pfolder;
  break;
case ID_B_LAUNCHFILE:
  pMI->iImage = papp;
  break;
case ID_D_DISPLAY:
  pMI->iImage = pupload;
  break;
case ID_D_EXITUPLOADER:
  pMI->iImage = pexit;
  break;
case ID_D_ONLINESPACE:
  pMI->iImage = pglobe;
  break;
case ID_D_LOGINOUT:
  pMI->iImage = pkey;
  break;
case ID_D_EXIT:
  pMI->iImage = pexit;
  break;
case ID_D_AUTOLOGIN:
  pMI->iImage = _6beed_share::u_getAutologin()?pgood:pbad;
  break;
default:
  pMI->iImage = -1;
  break;
    }
  }

  void DynamicChangeMenuItem(CMenuItemInfo& mii, coolMenuItemData * pMI, TCHAR* szString,bool& ignore){
    if (u_getLoggedin()){
      if (mii.wID==ID_D_LOGINOUT){
        int b = (int)wcslen(texit)+1;//wcslen-must plus 1, because it is memory copy, need to copy the trailing null
        if (b>MAX_MENU_ITEM_TEXT_LENGTH){
          return;
        }
        ::wmemcpy(szString,texit,b);
        pMI->iImage = pexit;
      }
      if (mii.wID==ID_D_EXIT){
        ignore = true;
      }
    }else{
      if(mii.wID!=ID_D_LOGINOUT && 
        mii.wID!=ID_D_ONLINESPACE && 
        mii.wID!=ID_D_EXITUPLOADER && 
        mii.wID!=ID_D_EXIT && 
        mii.wID!=ID_D_AUTOLOGIN)
      {
        mii.fState = MFS_GRAYED;
      }

    }
  }

  virtual void PrepareMenu(HMENU hMenu){
    //CMenuHandle menu(hMenu);
    //menu.EnableMenuItem(ID_D_LOGINOUT, MF_GRAYED);
  }

  virtual void SetMyMenu(CMenu& m){
    //m.Attach(c6beeres::R().GetMenu(IDR_CLIENTMNGMENU));
  }
  // add popup menu code finish

  void ShowFList(const FList& m,bool first=true){
    if(first){
      m_wndFileList.m_aFiles.RemoveAll();
      for (int i=0;i<m.GetSize();i++){
        m_wndFileList.AddFile(m[i]);
      }
    }
    m_wndFileList.Invalidate();
  }

  void startupload(){
    // 1. tell if user has logged in, or start login UI
#if 0
    if(!_6beed_share::u_getLoggedin()){
      _6beed_util::StartClientMng();
      return;
    }
#endif
    // 2. to upload
    int a= (int)aSelectedItems.size();
    int msize = m_wndFileList.m_aFiles.GetSize();
    list<int>::const_iterator begin=aSelectedItems.begin();
    list<int>::const_iterator end  =aSelectedItems.end();
    for (list<int>::const_iterator i =begin; i != end; ++i){
      int nItem = (int)*i;
      if( nItem >= msize){break;}

      CString filelocation(m_wndFileList.m_aFiles[nItem].m_absPath);
      if (filelocation.IsEmpty()){return;}
      int status = m_wndFileList.m_aFiles[nItem].m_status;
      if( status==S_INITAL || status==S_WAITING || status==S_SUSPENDED){
        if(status!=S_SUSPENDED){
          m_wndFileList.UpdateItemProgress( nItem,colProgess,0);//set progress as 0
        }
        int atscount=(int)sfp_active.size();
        if (atscount >= ATSMAXCOUNT){
          m_wndFileList.SetItemStatus( nItem,S_WAITING);//set status as waiting
          bool has_item_inside_list = false;
          //////////////////////////////////////////////////////////////////////////
          for(list<CFileProfile*>::const_iterator i=sfp_waiting_list.begin();
            i!=sfp_waiting_list.end();i++){
              if((*i)->m_absPath.Compare(WaitingFileArray500[nItem].m_absPath)==0){
                has_item_inside_list=true;
                break;
              }
          }
          //////////////////////////////////////////////////////////////////////////
          if(!has_item_inside_list){
            if( status != S_WAITING){
              WaitingFileArray500[nItem] = m_wndFileList.m_aFiles[nItem];
              sfp_waiting_list.push_back(&WaitingFileArray500[nItem]);
            }
          }
        }else{
          active_fp[atscount] = m_wndFileList.m_aFiles[nItem];//assign operator
          m_wndFileList.SetItemStatus(nItem,S_ACTIVE);
          threadpacket[atscount].t_pWndFD    = & m_wndFileList;
          threadpacket[atscount].t_pfp   = & active_fp[atscount];
          threadpacket[atscount].t_up_flist   = & m_uploadFiles;
          threadpacket[atscount].t_timer  = & m_Timer[atscount];
          threadpacket[atscount].t_ftpclient  = & ftpClient[atscount];
          threadpacket[atscount].t_pmaindlg   = this;
          threadpacket[atscount].t_ID    = atscount;
          ftpClient[atscount].suspendit = NO_SUSPEND;
          ftpClient[atscount].SetTerminatethisthread(false);
          m_wndFileList.m_aFiles[nItem].m_tID = atscount;

          sfp_active.insert(active_fp[atscount].m_absPath);

          if (active_fp[atscount].m_status==S_WAITING){
            sfp_waiting_list.remove(&WaitingFileArray500[nItem]);
          }
          if (!::_beginthreadex(NULL,0,_ThreadUploadProc,(LPVOID)&threadpacket[atscount],0,NULL)){
            sfp_active.erase(active_fp[atscount].m_absPath);
          }
        }
        m_wndFileList.InvalidateItem( nItem);
      }
    }
  }

  static UINT WINAPI _ThreadPauseUpload(LPVOID pParam){
    ppacket* mypp = static_cast<ppacket*>(pParam);
    ftpclient* ftpClient = mypp->_6bees_socket;
    __int64* t_psbytes = mypp->psbytes;
    ATL::CString t_fpath(mypp->fpath);
    CListWindowFileData* t_cfd = mypp->cfd;
    int status = mypp->status;
    while(TRUE){
      if(ftpClient->suspendit==SUSPEND_RECOVER){
        if(ftpClient->ftpalive==false){
          ftpClient->SetTerminatethisthread(false);
          ftpClient->ftpalive = true;
          ftpClient->m_fTransferInProgress = false;
          ftpClient->m_fAbortTransfer = false;
          *t_psbytes = 0;
        }
        int nItem = t_cfd->GetnItem(t_fpath);
        t_cfd->SetItemStatus(nItem,status);
        t_cfd->InvalidateItem(nItem);
        break;
      }else{
        Sleep(100);
      }
    }
    return 0;
  }

  bool pauseupload(bool isImmediatePause=false){
    if (isImmediatePause && !sfp_active.empty()){
      int MynItem = m_wndFileList.GetnItem(*sfp_active.begin());
      if (aSelectedItems.empty() || MynItem != aSelectedItems.front()){
        aSelectedItems.push_front(MynItem);
      }
    }
    list<int>::const_iterator begin=aSelectedItems.begin();
    list<int>::const_iterator end  =aSelectedItems.end();
    for (list<int>::const_iterator i=begin;i!=end; ++i){
      int nItem = (int)*i;
      if(!m_wndFileList.GetFile(nItem,WaitingFileArray500[nItem])){
        continue;
      }
      int status = isImmediatePause?S_WAITING:S_SUSPENDED;
      switch (WaitingFileArray500[nItem].m_status){
case S_FINISHED:
case S_WAITING:
case S_INITAL:
  continue;
case S_ACTIVE:
  {
    short ftpid=m_wndFileList.m_aFiles[nItem].m_tID ; // get ftp index
    if(ftpid<0) return false;
    ftpClient[ftpid].SetTerminatethisthread(true); // terminate the thread
    for (int j=0;j<ATSMAXCOUNT;j++){
      if(active_fp[j].m_absPath == WaitingFileArray500[nItem].m_absPath){
        ftpClient[ftpid].suspendit = SUSPEND_SUCCESS;
        static ppacket pp;
        pp.cfd = &m_wndFileList;
        pp.fpath = WaitingFileArray500[nItem].m_absPath;
        pp._6bees_socket = &ftpClient[ftpid];
        pp.psbytes= &sentBytes[ftpid];
        pp.status= status;
        HANDLE m_hThread=
          (HANDLE)::_beginthreadex(NULL,0,_ThreadPauseUpload,(LPVOID)&pp,0,NULL);
        static const int time_out = 5000;
        DWORD dw=WaitForSingleObject(m_hThread,time_out);
        switch (dw){
case WAIT_OBJECT_0:
  sfp_active.erase( active_fp[j].m_absPath);
  if (isImmediatePause){
    sfp_waiting_list.push_front(&WaitingFileArray500[nItem]);
  }
  sentBytes[ftpid]=0;
  break;
case WAIT_TIMEOUT:
  UUDEBUG((LOG_DBG,"WAIT_TIMEOUT"));
  return false;
case WAIT_FAILED:
  UUDEBUG((LOG_DBG,"WAIT_FAILED"));
  return false;
default:
  break;
        }
        break;
      }
    }
  }break;
default:break;
      }
      nItem = m_wndFileList.GetnItem(WaitingFileArray500[nItem].m_absPath);
      m_wndFileList.SetItemStatus(nItem,status);
      m_wndFileList.InvalidateItem(nItem);
    }
    return true;
  }

  bool PopDlgtoSelectFiles(ATL::CString& strFolderPath,ATL::CString& strFileName,int& mssize,bool isFirst=true){
    wchar_t chBuffer[MAXFILE+1]={};
    C6BeeFileDialog fileDlg(true,_T("Files"),_T(""),
      OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,_T("All Files|*.*|"));
    fileDlg.m_ofn.lpstrFile= chBuffer;
    fileDlg.m_ofn.nMaxFile = MAXFILE;
    INT_PTR iReturn = fileDlg.DoModal();
    if(iReturn==IDCANCEL){
      if(CommDlgExtendedError()==FNERR_BUFFERTOOSMALL){//cache memory not enough
        return false;
      }
    }else if(iReturn==IDOK){
      strFolderPath = chBuffer;
      int nStart=strFolderPath.GetLength()+1;
      int nEnd  =nStart;
      DWORD gfa =  GetFileAttributes(strFolderPath);
      if(gfa == FILE_ATTRIBUTE_DIRECTORY){
        for(int nIndex=nStart;nIndex<MAXFILE+1;nIndex++){
          if (chBuffer[nIndex]==0){
            nEnd=nIndex;
            if( nStart == nEnd){
              break;
            }
            strFileName.Format(_T("%s\\%ls"),strFolderPath,&chBuffer[nStart]);
            if(!_6beed_util::GetFileSizeW(strFileName.GetString(),mssize)){return false;}
            CFileProfile fileProfile(0,strFileName,mssize);
            AddUploadFile(fileProfile,isFirst);
            nStart=nEnd+1;
          }
        }
      }else{
        if(!_6beed_util::GetFileSizeW(chBuffer,mssize)){
          return false;
        }
        CFileProfile fileProfile(0,chBuffer,mssize);
        AddUploadFile(fileProfile,isFirst);
      }
    }
    return true;
  }

  void UpdateNoteImg(bool loggedin=true){
    m_NoteImg.SetImages(loggedin?pinfo:pbad);
    m_NoteImg.Invalidate();
    m_NoteImg.UpdateWindow();
  }

  void PostLogin(std::string& username,std::string& passwordtostore,std::string& sid){
    ShowWindow(SW_HIDE);
    std::wstring unamew_ = wstringmaker() << GetEmailShort(username.c_str());
    ATL::CString bonjour;
    bonjour.Format(kbonjour1,unamew_.c_str());
    SetTooltipText(bonjour);
    unamew_ = kbonjour2 + unamew_;
    PopMsg(kbonjour3,unamew_.c_str());
    m_Note.SetWindowText(kbonjour4);
    HICON hIconSmall=ultraiestatusicons_.GetIcon(sloggedin);
    ChangeIcon(hIconSmall);
    DestroyIcon(hIconSmall);
    SetIcon(ultraiestatusicons_.GetIcon(suploading),FALSE);
    _6beed_share::u_setLoggedin(true);
    _6beed_share::u_setSID(sid.c_str());
    _6beed_share::u_setPassword(passwordtostore.c_str());
    _6beed_share::u_setEmail(username.c_str());
    _6beed_util::Notify6BeeTB(CPYDATA_LOGIN);
    UpdateMode(MUPLOAD);
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
    }
    else if(wParam==TIMER_BUSYUPLOADING){
      b = !b;
      /// system tray flash
      HICON hi = ultraiestatusicons_.GetIcon(b?suploading:sloggedin);
      ChangeIcon(hi);
      DestroyIcon(hi);
    }
    else if(wParam==TIMER_BUSYDOWNLOADING){
      b = !b;
      /// system tray flash
      HICON hi = ultraiestatusicons_.GetIcon(b?sdownloading:sloggedin);
      ChangeIcon(hi);
      DestroyIcon(hi);
    }
    return 0;
  }

  void OnLogoutSuccess(){
    //SetTooltipText(_T("You have logged out UltraIE!"));
    _6beed_share::u_setSID("");
    _6beed_share::u_setLoggedin(false);
    _6beed_util::Notify6BeeTB(CPYDATA_LOGOUT);
    UpdateTimer(TIMER_LOGOUT,true);
  }

};

__int64 CMainDlg::sentBytes[ATSMAXCOUNT]={};
bool CMainDlg::is_prev_uploading=false;

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
        Ice::Long spaceused, ULnum;
        wstring userinfo;
        if(_user->GetUserInfo(uuname,spaceused,ULnum)){
          TCHAR sztmp[30]={};
          StrFormatByteSize(spaceused,sztmp,30);
          userinfo  = wstringmaker()<< L"用户: " << uuname
            << L" 上传文件:" << (DWORD)ULnum << L"个 上传流量:" << sztmp;
          pMainDlg->m_usertip.SetWindowText(userinfo.c_str());
        }
        pMainDlg->PostLogin(uuname,ppword,sid);
      }else{
        ErrorFunc(pMainDlg,tip_loginfail);
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


string GetModiFileURL(const string& mod_filename){
  if (mod_filename.empty()){
    return string();
  }
  bool ishtml = (_6bees_str::GetFileExt(mod_filename) == "html");
  static const unsigned int MAXLEN=1024;
  char ansistr[MAXLEN*2]={};
  if (mod_filename.find('#')!=std::string::npos
    ||mod_filename.find('&')!=std::string::npos
    ||mod_filename.find('?')!=std::string::npos)
  {
    // contain special chars - TODO str_replace(array("'",'%'),array('%27',"'"),urlencode($u));
    if(!_6beed_util::urlencode(ansistr,MAXLEN*2,mod_filename.c_str())){
      return string("");
    }
    string url(ansistr);
    _6bees_str::strreplace(url,"'","%27");
    _6bees_str::strreplace(url,"%","'");
    url = stringmaker() << kUltraIEURLa << "uu_" << url << ".htm";
    return url;
  }else{
    //string b64 = base64_encode((unsigned char const*)mod_filename.c_str(),(unsigned int)mod_filename.size());
    if(!_6beed_util::urlencode(ansistr,MAXLEN*2,mod_filename.c_str())){
      return string("");
    }
    string b64(ansistr); 
    string url = stringmaker() << kUltraIEURLa << (ishtml ? "h_" : "u_") << b64 << ".htm";
    return url;
  }
}