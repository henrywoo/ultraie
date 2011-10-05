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

#pragma once

#include <atltime.h>
#include "ListCtrl.h"
#include "6bees_ftpclient.h"
#include "TrayiconImpl.h"
#include <6BeeContextMenu.h>
#include "util.h"
#include <6bees_const.h>
#include <6BeeResDialog.h>
#include "6bees_shell.h"
#include "6bees_window.h"
#include "uulogging.h"
#include <imgcap_setting.h>
#include "constdata.h"
#include "6bees_str.h"
#include "../../dll/6beeres/resource.h"

using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;
using namespace std;
using namespace _6bees_socket;
using namespace _6bees_pattern;
using namespace _6bees_const;

#define CP_CHINESE 936
#define CPYDATA_UPLOAD_2_UPLOAD 16

#define ACCOUNT L"Account"
#define MD5PASSWORDINREG L"Pswdmima"
#define STARPWD  L"************"
#define STARPWDA  "4e7f25a06c7dde7efa0f5d7f8d1f11a9"
#define TIMER_LOGIN 100
// data for inter-thread transfer
bool isloggedin = false;
class CMainDlg;
struct loginfo{
  CMainDlg* mdlg;
  std::string name;
  std::string passwd;
};
static loginfo info={};
wstring servertxt;
std::string uuname;
std::string ppword;
std::wstring pwdfromreg;//password stored in registry

UINT WINAPI _ThreadLogin(LPVOID lp);

//TODO
/*
void LoginPHPWind(const CComPtr<IWebBrowser2>& spWebBrowser_,const string& email,const string& pass){
  string szPostData = "pwuser=" + email + "&pwpwd=" + pass +
    "&step=2&lgt=Email&jumpurl=http://www.6bee.net&cktime=31536000";
  CComVariant vURL(L"http://www.6bee.net/login.php");
  size_t nSize = szPostData.size();
  SAFEARRAY *psa = SafeArrayCreateVector(VT_UI1, 0, (ULONG)nSize);
  LPSTR pPostData;
  SafeArrayAccessData(psa, (LPVOID*) &pPostData);
  memcpy(pPostData, szPostData.c_str(), nSize);
  SafeArrayUnaccessData(psa);
  CComVariant vPostData;
  vPostData.vt = VT_ARRAY|VT_UI1;
  vPostData.parray = psa;
  CComVariant vHeaders(L"Content-Type: application/x-www-form-urlencoded\r\n");
  spWebBrowser_->Navigate2(&vURL,NULL,NULL,&vPostData,&vHeaders);
}*/

void SetRegVal(const wchar_t* k,const wchar_t* s){
  CRegKey crk;
  long rk2=crk.Open(HKEY_LOCAL_MACHINE,_T("Software\\6BeeNetworks\\6beeuploader"),KEY_WRITE);
  if (rk2==ERROR_SUCCESS){
    rk2=crk.SetStringValue(k,s);
  }
}

wstring GetRegVal(const wchar_t* k){
  CRegKey crk;
  long rk2=crk.Open(HKEY_LOCAL_MACHINE,_T("Software\\6BeeNetworks\\6beeuploader"),KEY_READ);
  if (rk2==ERROR_SUCCESS){
    wchar_t pszValue[100];
    ULONG len=100;
    rk2=crk.QueryStringValue(k,pszValue,&len);
    if (rk2==ERROR_SUCCESS){
      return wstring(pszValue);
    }
  }
  return wstring();
}

CAppModule _Module;

class CMainDlg:
  //public CDialogImpl<CMainDlg>,
  public CAxDialogImpl<CMainDlg>,
  public CWinDataExchange<CMainDlg>,
  public CDialogResize<CMainDlg>,
  public CTrayIconImpl<CMainDlg>,
  public CCoolContextMenu<CMainDlg>,
  public ftpclient::CNotification
  //public Idroptarget
{
private:
  CListWindowFileData m_wndFileList;
  CWindow m_wndFinished;
  CWindow m_wndUpload;
  CWindow m_CEmail;
  CWindow m_CPassWd;

  CWindow email_;
  CWindow paswd_;
  CWindow servertxt_;

  CButton rememberchkb_;

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
  CBitmapButton m_login;
  CBitmapButton m_register;
  CBitmapButton m_NoteImg;

public:
  bool isremember;

  enum { IDD = IDD_MAINDLG };

  BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_SYSCOMMAND,OnSysCommand)
    MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
    MESSAGE_HANDLER(WM_COPYDATA,  OnCopyData)

    COMMAND_ID_HANDLER(IDC_CHECKREMEMBER,OnChangeRemember)

    COMMAND_ID_HANDLER(IDC_ADDUPLOAD,OnIDC_ADDUPLOAD)
    COMMAND_ID_HANDLER(IDC_MYUPLOADS,OnFinished)
    COMMAND_ID_HANDLER(IDC_LOGIN,  OnLogin)
    COMMAND_ID_HANDLER(IDC_REGISTER,  OnRegister)
    //Menu 1
    COMMAND_ID_HANDLER(ID_DISPLAY_UPLOADER,OnFileOpen)
    COMMAND_ID_HANDLER(ID_A_ONLINESPACE,OnID_A_ONLINESPACE)
    COMMAND_ID_HANDLER(ID_A_EXITUPLOADER,  OnExit)
    //Menu 2
    COMMAND_ID_HANDLER(ID_B_START,   OnID_B_START)
    COMMAND_ID_HANDLER(ID_B_OPENFOLDER,OnID_B_OPENFOLDER)
    COMMAND_ID_HANDLER(ID_B_LAUNCHFILE,OnID_B_LAUNCHFILE)
    COMMAND_ID_HANDLER(ID_B_PAUSE,  OnID_B_PAUSE)

    NOTIFY_HANDLER_EX(IDC_FILELIST,LCN_HYPERLINK, OnListHyperLink)
    NOTIFY_HANDLER_EX(IDC_FILELIST,LCN_RIGHTCLICK,OnRightClick)
    NOTIFY_HANDLER_EX(IDC_FILELIST,LCN_DBLCLICK,  OnDoubleClick)

    MESSAGE_HANDLER(WM_TIMER,      OnTimer)

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
    DLGRESIZE_CONTROL(IDC_STATICTIP,DLSZ_MOVE_X|DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_MYUPLOADS,DLSZ_MOVE_X|DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()

public:
  static __int64 sentBytes[ATSMAXCOUNT];
  static bool is_prev_uploading;

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
        MessageBox(fileProfile.m_absPath,_T("你已经选择了该文件!"));
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
    DlgResize_Init(true,false,WS_THICKFRAME|WS_CLIPCHILDREN);

    HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ULTRAIE_UPLOADER), 
      IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTSIZE|LR_SHARED);
    SetIcon(hIcon, TRUE);

    hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ULTRAIE_UPLOADER),
      IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTSIZE|LR_SHARED);
    SetIcon(hIconSmall,FALSE);

    HBITMAP hbmp=(HBITMAP)c6beeres::R().GetImg(ULTRA_IMGS,IMAGE_BITMAP,16*54);
    SetMyImageList(hbmp,m_ImageList,MASKCOLOR,16,16,54,GetSysColor(COLOR_3DFACE));

    CenterWindow();

    DoDataExchange(false);

    ShowFList(m_uploadFiles);
    m_wndFinished.Attach(GetDlgItem(IDC_FINISHED));
    m_wndUpload.Attach(GetDlgItem(IDC_UPLOAD));
    //m_wndUpload.EnableWindow(FALSE);
    GetSystemSettings();
    InitializeCriticalSection(&csection);
    InstallIcon(_T("6bee文件上传工具"),hIconSmall,IDR_POPUP);
    //DestroyIcon(hIconSmall);
    SetDefaultItem(IDC_FINISHED);
    m_wndFileList.SetBackgroundImage((HBITMAP)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_BG), 
      IMAGE_BITMAP, INFO_WIDTH, INFO_HEIGHT, LR_DEFAULTSIZE|LR_SHARED));// wiil be release automatically

    CAxWindow wndIE = GetDlgItem(IDC_LOGINWND);
    /*CComPtr<IWebBrowser2> pWB2;
    HRESULT hr = wndIE.QueryControl(&pWB2);
    if (pWB2){
      pWB2->put_Silent(VARIANT_TRUE);
      pWB2->put_RegisterAsBrowser(VARIANT_FALSE);
      pWB2->put_RegisterAsDropTarget(VARIANT_FALSE);
      pWB2->put_AddressBar(VARIANT_FALSE);
      pWB2->put_Offline(VARIANT_FALSE);
      //CComVariant v;
      //pWB2->Navigate(CComBSTR("http://www.6bee.net/win32client/ads.php"),&v,&v,&v,&v);
      //LoginPHPWind(pWB2,"wufuheng@gmail.com","wfh168178");
    }*/

    HBITMAP hbmp2=(HBITMAP)c6beeres::R().GetImg(BUTTON_UPLOAD_CH,IMAGE_BITMAP,45*8,22);
    //HBITMAP hbmp2=(HBITMAP)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_BUTTONS), 
    //  IMAGE_BITMAP, 45*8, 22, LR_DEFAULTCOLOR|LR_DEFAULTSIZE|LR_SHARED);
    SetMyImageList(hbmp2,m_imglst,MASKPURPLE,45,22,8,GetSysColor(COLOR_3DFACE));
    m_addupload.SubclassWindow(GetDlgItem(IDC_ADDUPLOAD));
    m_addupload.SetToolTipText(_T("上传更多文件?"));
    m_addupload.SetImageList(m_imglst);
    m_addupload.SetImages(7,6,6,6);
    m_addupload.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_myuploads.SubclassWindow(GetDlgItem(IDC_MYUPLOADS));
    m_myuploads.SetToolTipText(_T("所有我上传的文件"));
    m_myuploads.SetImageList(m_imglst);
    m_myuploads.SetImages(5,4,4,4);
    m_myuploads.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_login.SubclassWindow(GetDlgItem(IDC_LOGIN));
    m_login.SetToolTipText(_T("登陆到6bee"));
    m_login.SetImageList(m_imglst);
    m_login.SetImages(1,0,0,0);
    m_login.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_register.SubclassWindow(GetDlgItem(IDC_REGISTER));
    m_register.SetToolTipText(_T("注册一个6bee账号"));
    m_register.SetImageList(m_imglst);
    m_register.SetImages(3,2,2,2);
    m_register.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_CEmail.Attach(GetDlgItem(IDC_EMAILBOX));
    m_CEmail.SetFocus();
    m_CEmail.SetWindowText(L"");//TODO
    m_CPassWd.Attach(GetDlgItem(IDC_PASSWORDBOX));
    email_.Attach(GetDlgItem(IDC_EMAIL));
    paswd_.Attach(GetDlgItem(IDC_PASSWORD));

    servertxt_.Attach(GetDlgItem(IDC_TXTFROMSERVER));
    servertxt_.SetWindowText(L"");

    rememberchkb_.Attach(GetDlgItem(IDC_CHECKREMEMBER));
    isremember = (GetRegVal(L"Remember") == L"y");
    rememberchkb_.SetCheck(isremember?1:0);

    if (isremember){
      m_CPassWd.SetWindowText(STARPWD);
      pwdfromreg = GetRegVal(MD5PASSWORDINREG);
    }

    m_NoteImg.SubclassWindow(GetDlgItem(IDC_NOTEIMG));
    m_NoteImg.SetImageList(m_ImageList);
    m_NoteImg.SetImages(pkey);

    servertxt_.ShowWindow(SW_HIDE);
    //m_NoteImg.ShowWindow(SW_HIDE);

    m_CEmail.SetWindowText(GetRegVal(ACCOUNT).c_str());

    //StartUpload();
    return TRUE;
  }

  LRESULT OnTimer(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    const static int maxiconsindex= 1;
    static bool b = false;
    if (wParam==TIMER_LOGIN){
      b = !b;
      /// image button flash
      m_NoteImg.SetImages(b?pglobe:pglobelink);
      m_NoteImg.Invalidate();
    }
    return 0;
  }

  void UpdateNoteImg(int status){
    switch(status){
      case 1:
        m_NoteImg.SetImages(pfav);break;
      case 0:
         m_NoteImg.SetImages(pkey);break;
      case -1:
         m_NoteImg.SetImages(pbad);break;
    }
    m_NoteImg.Invalidate();
    m_NoteImg.UpdateWindow();
  }

  LRESULT OnListHyperLink( LPNMHDR lpNMHDR ){
    CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );
    CString url=m_wndFileList.GetItemText(pListNotify->m_nItem,pListNotify->m_nSubItem);
    if (!url.IsEmpty()){
      runIE(url);
    }
    return 0;
  }

  LRESULT OnRightClick( LPNMHDR lpNMHDR ){
    CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );
    m_wndFileList.GetFile( pListNotify->m_nItem,first_selected_file );
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
      if(index <msize ){
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

  LRESULT OnChangeRemember(WORD,WORD wID,HWND,BOOL& ){
    isremember=!isremember;
    rememberchkb_.SetCheck(isremember?1:0);
    SetRegVal(L"Remember",isremember?L"y":L"-");
    return 1;
  }
  

  LRESULT OnDoubleClick( LPNMHDR lpNMHDR ){
    StartUpload();
    return 0;
  }

  LRESULT OnIDC_ADDUPLOAD(WORD,WORD wID,HWND,BOOL& ){
    try{
      int mssize=0;
      ATL::CString strFolderPath,strFileName;
      PopDlgtoSelectFiles(strFolderPath,strFileName,mssize,false);
      ShowFList(m_uploadFiles,false);
    }catch (...){}
    return 0;
  }

  LRESULT OnExit(WORD,WORD wID,HWND,BOOL& ){
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
    EndDialog(wID);
    return 0;
  }

  // Call Back by Upload thread!!!!
  virtual void OnSetLRemoteFileSize(int threadID,long lRemoteFileSize){
    sentBytes[threadID] += lRemoteFileSize;
  }
  virtual void OnBytesSent2(long lSentBytes,const ATL::CString& flocation,int threadID,
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
      m_wndFileList.m_aFiles[ nItem ].m_progress = 100;
      m_wndFileList.m_aFiles[ nItem ].m_timeremaining = _T("");
      //m_wndFileList.m_aFiles[ nItem ].m_link = _6bees_const::kUltraIEURL;
      m_wndFileList.SetItemStatus(nItem,S_FINISHED);
      m_upload_finished_Files.Add(m_wndFileList.m_aFiles[nItem]);
      m_wndFileList.Invalidate();
    }
  }

  static bool getftpinfo(string& ip,short& port){
    ip="66.235.160.14";
    port=21;
    return true;
  }

  static string GetUploadedFileURL(const wstring& name){
    int fsize=0;
    if(GetFileSizeW(name.c_str(),fsize)){
      string fname=stringmaker(CP_UTF8) << name;
      char result[4096];
      urlencode(result,4096,fname.c_str());
      string url=stringmaker() << _6bees_const::kUltraIEURL << "win32client/getuploadedfileurl.php?fname="
        << result
        <<"&fsize="<<fsize;
      int clen=0;
      char *data = _6bees_net::GetDataByHTTP(url.c_str(),clen);
      if (data!=NULL){
        string adata = stringmaker()  << data;
        delete [] data;
        if(_6bees_str::startwith<string>(adata,"http://")){
          return adata;
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
    string realpass;
    if(pwdfromreg.empty() || ppword!=STARPWDA){
      realpass = ppword;
    }else{
      realpass = stringmaker() << pwdfromreg;
    }
    CLogonInfo logonInfo(A2CW(ftp_ip.c_str()),ftp_port,A2CW(uuname.c_str()),A2CW(realpass.c_str()));
    if(ftpClient->Login(logonInfo)){
      string fetchurl = GetUploadedFileURL(t_pfp->m_filename.GetString());
      if (!fetchurl.empty()){
        const wchar_t* link = A2CW(fetchurl.c_str());
        t_pfp->m_link = link;
        t_pfp->m_status = S_FINISHED;
        t_pWndFD->InvalidateItem(t_nItem);
        t_pWndFD->SetItemStatus(t_nItem,S_FINISHED);
        t_pWndFD->m_aFiles[t_nItem].m_link = link;
        t_pWndFD->m_aFiles[t_nItem].m_progress = 100;
        sfp_active.erase(t_pfp->m_absPath);
      }else{
        if(!uploadfilewrap(t_pfp,m_maindlg,t_ID,ftpClient,t_pWndFD,true)){
          return 0;
        }
      }
      bool isDirectUpload=t_pfp->m_isDirectUpload;
      while( !sfp_waiting_list.empty() ){
        if(sfp_active.size() >= ATSMAXCOUNT){break;}
        if (is_prev_uploading || isDirectUpload==sfp_waiting_list.front()->m_isDirectUpload){

          CFileProfile* tmp = sfp_waiting_list.front();
          string fetchurl2 = GetUploadedFileURL(tmp->m_filename.GetString());
          if (!fetchurl2.empty()){// this file has been uploaded before
            const wchar_t* link2 = A2CW(fetchurl2.c_str());
            int nitem = t_pWndFD->GetnItem(tmp->m_absPath);
            t_pWndFD->InvalidateItem(nitem);
            t_pWndFD->SetItemStatus(nitem,S_FINISHED);
            t_pWndFD->m_aFiles[nitem].m_link = link2;
            t_pWndFD->m_aFiles[nitem].m_progress = 100;
            sfp_waiting_list.pop_front();
            sfp_active.erase(tmp->m_absPath);
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
    //timer_notifier tn(CPYDATA_FILEUPLOAD_START,CPYDATA_UPLOADBUSY_STOP);
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

    bool upload_success = ftpClient->UploadFile(
      t_pfp->m_absPath.GetString(),
      t_pfp->m_filename.GetString());//blocking operation

    CMainDlg::sentBytes[t_ID]=0;
    int& status = t_pWndFD->m_aFiles[ t_nItem ].m_status;
    if(upload_success){
      USES_CONVERSION;
      string thumbpath = wthumb;
      //string fetchurl;
      string filename = CW2A(t_pfp->m_filename,CP_UTF8);
      string desc;
      string tag;
      //http://www.6bee.net/win32client/postupload.php?e=wufuheng@gmail.com&p=6373cdd728d45ee48dfd584acca5616e&fname=1247715470test.txt&fsize=4
      char result[4096];
      urlencode(result,4096,filename.c_str());
      //unsigned int b64len=(unsigned int)filename.size();
      //string b64url=_6bees_util::base64_encode((unsigned char const*)filename.c_str(),b64len);
      if(!filename.empty()){
        string realpass;
        if(pwdfromreg.empty() || ppword!=STARPWDA){
          realpass = ppword;
        }else{
          realpass = stringmaker() << pwdfromreg;
        }
        DWORD crc32=0;
        if(_6bees_util::FileCrc32Assembly(t_pfp->m_absPath,crc32)!=ERROR_CRC){
          string url=stringmaker() << _6bees_const::kUltraIEURL << "win32client/postupload.php?fname="
            << result <<"&fsize="<<t_pfp->m_size_long << "&e=" << uuname << "&p=" << realpass <<"&c="<<crc32;
          int clen=0;
          char *data = _6bees_net::GetDataByHTTP(url.c_str(),clen);
          if (data!=NULL){
            string adata = stringmaker()  << _6bees_const::kUltraIEURL << data;
            delete [] data;
            t_pWndFD->m_aFiles[t_nItem].m_link = A2CW(adata.c_str());
            t_pWndFD->InvalidateItem(t_nItem);
          }
        }
        sfp_active.erase(t_pfp->m_absPath);
      }else{
        //@todo
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

  LRESULT OnID_B_START(WORD,WORD wID,HWND,BOOL& ){
    StartUpload();
    return 0;
  }

  LRESULT OnID_B_OPENFOLDER(WORD,WORD wID,HWND,BOOL& ){
    if(aSelectedItems.size()<1)return 0;
    ATL::CString tmp=first_selected_file.m_absPath;
    int i=tmp.ReverseFind(L'\\');
    tmp=tmp.Left(i);
    ShellExecuteW(NULL,NULL,tmp,NULL,NULL,SW_SHOW);
    return 0;
  }

  LRESULT OnID_B_LAUNCHFILE(WORD,WORD wID,HWND,BOOL& ){
    if(aSelectedItems.size()<1)return 0;
    ATL::CString tmp=first_selected_file.m_absPath;
    ShellExecuteW(NULL,NULL,tmp,NULL,NULL,SW_SHOW);
    return 0;
  }

  LRESULT OnID_B_PAUSE(WORD,WORD wID,HWND,BOOL& ){
    pauseupload();
    return 0;
  }

  LRESULT OnFileOpen(WORD,WORD,HWND,BOOL& ){
    if (!IsWindowVisible())
      ShowWindow(SW_SHOW);
    if (IsIconic())
      ShowWindow(SW_RESTORE);
    else
      BringWindowToTop();
    ::SetForegroundWindow(m_hWnd);
    return 0;
  }

  LRESULT OnFinished(WORD,WORD wID,HWND,BOOL& ){
    runIE(_6bees_const::kUltraIEURL);
    return 0;
  }

  LRESULT OnLogin(WORD,WORD wID,HWND,BOOL& ){
    //post data to http://www.6bee.net/login.php using a 0 size window,保持和网站登陆登出状态同步
    static const short maxlen = 40;
    wchar_t username[maxlen]={},password[maxlen]={};
    m_CEmail.GetWindowText(username,maxlen);
    m_CPassWd.GetWindowText(password,maxlen);
    info.mdlg = this;
    info.name = stringmaker() << username;
    info.passwd = GetMD5((stringmaker()<<password).c_str());
    ::_beginthreadex(NULL,0,_ThreadLogin,(LPVOID)&info,0,NULL);
    SetTimer(TIMER_LOGIN,500);
    return 0;
  }

  LRESULT OnRegister(WORD,WORD wID,HWND,BOOL& ){
    runIE(kUltraIEURL_REGISTER);
    return 0;
  }

  LRESULT OnID_A_ONLINESPACE(WORD,WORD wID,HWND,BOOL& ){
    runIE(_6bees_const::kUltraIEURL);
    return 0;
  }

  LRESULT OnUpload(WORD,WORD wID,HWND,BOOL& ){
    return 0;
  }

  LRESULT OnHide(WORD,WORD wID,HWND,BOOL& ){
    ShowWindow(SW_HIDE);
    return 0;
  }

  LRESULT OnCommand(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    bHandled = FALSE;
    if (!HIWORD(wParam)){
      long lSite = LOWORD(wParam);
      if (lSite == WM_DESTROY && uMsg == WM_COMMAND){
        //InstallIcon(_T("6bee.net - UpDownLoader!\n2007@All Copyrights Reserved!"),hIconSmall,IDR_POPUP);
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
            if(!GetFileSizeW(vf[i].c_str(),mssize)){continue;}
            CFileProfile fileProfile(0,vf[i].c_str(),mssize);
            AddUploadFile(fileProfile,false);
          }
        }break;
#if 0
      case CPYDATA_UPLOAD_2_UPLOAD_DIRECT:
        {
          char* tmp= (char*)(p->lpData);
          wstring filepath=wstringmaker(CP_ACP)<<tmp;
          int fsize=0;
          if(!GetFileSizeW(filepath.c_str(),fsize)){return 0;}
          CFileProfile fileProfile(0,filepath.c_str(),fsize,true);
          AddImediateUploadFile(fileProfile,false);
          StartUpload();
        }break;
#endif
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
case ID_DISPLAY_UPLOADER:
  pMI->iImage = 5;
  break;
case ID_A_EXITUPLOADER:
  pMI->iImage = 8;
  break;
case ID_A_ONLINESPACE:
  pMI->iImage = 20;
  break;
case ID_B_START:
  pMI->iImage = 24;
  break;
case ID_B_PAUSE:
  pMI->iImage = 23;
  break;
case ID_B_OPENFOLDER:
  pMI->iImage = 25;
  break;
case ID_B_LAUNCHFILE:
  pMI->iImage = 26;
  break;
default:
  pMI->iImage = -1;
  break;
    }
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

  void StartUpload(){
    // 1. tell if user has logged in, or start login UI
    if (!isloggedin){
      ::MessageBox(NULL,_T("请用您的电子邮件和密码登陆6bee"),_T("请先登录"),MB_ICONINFORMATION);
      return;
    }
    // 2. to upload
    int a= (int)aSelectedItems.size();
    int msize = m_wndFileList.m_aFiles.GetSize();
    list<int>::const_iterator begin=aSelectedItems.begin();
    list<int>::const_iterator end  =aSelectedItems.end();
    for (list<int>::const_iterator i =begin; i != end; ++i){
      int nItem = (int)*i;
      if( nItem >= msize ){break;}

      CString filelocation(m_wndFileList.m_aFiles[nItem].m_absPath);
      if (filelocation.IsEmpty()){return;}
      int status = m_wndFileList.m_aFiles[nItem].m_status;
      if( status==S_INITAL || status==S_WAITING || status==S_SUSPENDED){
        if(status!=S_SUSPENDED){
          m_wndFileList.UpdateItemProgress( nItem,3,0 );//set progress as 0
        }
        int atscount=(int)sfp_active.size();
        if (atscount >= ATSMAXCOUNT ){
          m_wndFileList.SetItemStatus( nItem,S_WAITING );//set status as waiting
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
            if( status != S_WAITING ){
              WaitingFileArray500[nItem] = m_wndFileList.m_aFiles[nItem];
              sfp_waiting_list.push_back(&WaitingFileArray500[nItem]);
            }
          }
        }else{
          active_fp[atscount] = m_wndFileList.m_aFiles[nItem];//assign operator
          m_wndFileList.SetItemStatus( nItem,S_ACTIVE );
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
        m_wndFileList.InvalidateItem( nItem );
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
              if ( active_fp[j].m_absPath == WaitingFileArray500[nItem].m_absPath ){
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
                    sfp_active.erase( active_fp[j].m_absPath );
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
    C6BeeFileDialog fileDlg(true,_T("请选择文件上传"),_T(""),
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
            if(!GetFileSizeW(strFileName.GetString(),mssize)){return false;}
            CFileProfile fileProfile(0,strFileName,mssize);
            AddUploadFile(fileProfile,isFirst);
            nStart=nEnd+1;
          }
        }
      }else{
        if(!GetFileSizeW(chBuffer,mssize)){
          return false;
        }
        CFileProfile fileProfile(0,chBuffer,mssize);
        AddUploadFile(fileProfile,isFirst);
      }
    }
    return true;
  }

  bool LoginSuccess(){
    if (true){
      m_CEmail.ShowWindow(SW_HIDE);
      m_CPassWd.ShowWindow(SW_HIDE);
      email_.ShowWindow(SW_HIDE);
      paswd_.ShowWindow(SW_HIDE);
      m_login.ShowWindow(SW_HIDE);
      m_register.ShowWindow(SW_HIDE);
      rememberchkb_.ShowWindow(SW_HIDE);

      servertxt_.SetWindowText(servertxt.c_str());
      servertxt_.ShowWindow(SW_SHOW);
      KillTimer(TIMER_LOGIN);
      UpdateNoteImg(1);
    }else{
      m_CEmail.ShowWindow(SW_SHOW);
      m_CPassWd.ShowWindow(SW_SHOW);
      email_.ShowWindow(SW_SHOW);
      paswd_.ShowWindow(SW_SHOW);
      m_login.ShowWindow(SW_SHOW);
      m_register.ShowWindow(SW_SHOW);
      rememberchkb_.ShowWindow(SW_SHOW);

      servertxt_.ShowWindow(SW_HIDE);
      UpdateNoteImg(0);
    }
    return true;
  }

};

__int64 CMainDlg::sentBytes[ATSMAXCOUNT]={};
bool CMainDlg::is_prev_uploading=false;


UINT WINAPI _ThreadLogin(LPVOID lp){
  try{
    loginfo* li = (loginfo*) lp;
    CMainDlg* pMainDlg = li->mdlg;
    if (pMainDlg==NULL){
      UUDEBUG((LOG_ERROR,"pMainDlg is NULL in ThreadLogin"));
      return 0;
    }
    uuname = li->name;
    ppword = li->passwd;

    string loginurl = stringmaker() << _6bees_const::kUltraIEURL  <<"win32client/login.php?e=" << uuname << "&p=";
    string realpass;
    if(pwdfromreg.empty() || ppword!=STARPWDA){
      realpass = ppword;
    }else{
      realpass = stringmaker() << pwdfromreg;
    }
    loginurl.append(realpass);
    
    int clen=0;
    char *data = _6bees_net::GetDataByHTTP(loginurl.c_str(),clen);
    if (data && !isloggedin){
      string aurl = stringmaker()  << data;
      delete [] data;
      isloggedin = (aurl.at(0) == '1');
      if(isloggedin){
        string meat = aurl.substr(1);
        vector<string> vs = _6bees_util::Split(meat,":");
        string username = vs[0];
        string spaceused = vs[1];
        string uploadnum = vs[2];
        string gname = vs[3];
        wstring status = ((vs.size()>4) && atoi(vs[4].c_str()) < 0) ? L"未激活" : L"激活";
        string sinfo;
        if (vs.size()>5){
          sinfo = vs[5];
        }
        wchar_t szFileSize[30]={};
        StrFormatByteSize((DWORD)atol(spaceused.c_str()),szFileSize,30);
        servertxt = wstringmaker() << L"用户名:" << username << L" (等级:" << gname <<L" 状态:"<< status
          <<L" 流量:" << szFileSize << L" 文件:"<< uploadnum <<L"个"<< sinfo << L")" ;
        pMainDlg->LoginSuccess();
        wstring emailw = wstringmaker() << uuname;
        SetRegVal(ACCOUNT,emailw.c_str());
        if (pMainDlg->isremember){
          wstring wp=wstringmaker() << realpass;
          SetRegVal(MD5PASSWORDINREG,wp.c_str());
        }else{
          SetRegVal(MD5PASSWORDINREG,L"");
        }
      }else{
        KillTimer(pMainDlg->m_hWnd,TIMER_LOGIN);
        ::MessageBox(NULL,_T("请确保您的电子邮件和密码正确"),_T("登陆失败"),MB_ICONINFORMATION);
      }
    }
    
  }catch (...){}
  return 0;
}