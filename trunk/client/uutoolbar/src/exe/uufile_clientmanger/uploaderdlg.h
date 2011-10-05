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

#include "resource.h"
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
using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;
using namespace std;
using namespace _6bees_socket;
using namespace _6bees_pattern;

#define CP_CHINESE 936

#define ID_B_SUSPENDED 32780

class uploaderdlg:
  public CDialogImpl<uploaderdlg>,
  public CWinDataExchange<uploaderdlg>,
  public CDialogResize<uploaderdlg>,
  //public CTrayIconImpl<uploaderdlg>,
  public CCoolContextMenu<uploaderdlg>,
  public ftpclient::CNotification
  //public Idroptarget
{
private:
  CListWindowFileData m_wndFileList;
  HICON hIconSmall;
  HCURSOR  m_hCursor;
  CMenu    m_uploaderdlgmenu;
  FList m_uploadFiles;
  FList m_upload_finished_Files;
  ftpclient ftpClient[ATSMAXCOUNT];
  //For launch file & open folder. What ever how many files you open,we just process the first one!
  CFileProfile first_selected_file;
  list<int>& aSelectedItems;

  CImageList m_imglst;
  CBitmapButton m_addupload;
  CBitmapButton m_myuploads;
public:
  enum { IDD = IDD_UPLOADERDLG };

  BEGIN_MSG_MAP(uploaderdlg)
    MESSAGE_HANDLER(WM_SYSCOMMAND,OnSysCommand)
    MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
    MESSAGE_HANDLER(WM_COPYDATA,  OnCopyData)

    COMMAND_ID_HANDLER(IDC_ADDUPLOAD,OnIDC_ADDUPLOAD)
    COMMAND_ID_HANDLER(IDC_MYUPLOADS,OnFinished)
    //COMMAND_ID_HANDLER(IDC_UPLOAD,  OnUpload)
    //Menu 1
    //COMMAND_ID_HANDLER(ID_DISPLAY_UPLOADER,OnFileOpen)
    //COMMAND_ID_HANDLER(ID_A_ONLINESPACE,OnID_A_ONLINESPACE)
    //COMMAND_ID_HANDLER(ID_A_EXITUPLOADER,  OnExit)
    //Menu 2
    COMMAND_ID_HANDLER(ID_B_START,   OnID_B_START)
    COMMAND_ID_HANDLER(ID_B_OPENFOLDER,OnID_B_OPENFOLDER)
    COMMAND_ID_HANDLER(ID_B_LAUNCHFILE,OnID_B_LAUNCHFILE)
    COMMAND_ID_HANDLER(ID_B_PAUSE,  OnID_B_PAUSE)

    NOTIFY_HANDLER_EX(IDC_FILELIST,LCN_HYPERLINK, OnListHyperLink)
    NOTIFY_HANDLER_EX(IDC_FILELIST,LCN_RIGHTCLICK,OnRightClick)
    NOTIFY_HANDLER_EX(IDC_FILELIST,LCN_DBLCLICK,  OnDoubleClick)

    CHAIN_MSG_MAP(CDialogResize<uploaderdlg>)
    //CHAIN_MSG_MAP(CTrayIconImpl<uploaderdlg>)
    CHAIN_MSG_MAP(CCoolContextMenu<uploaderdlg>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DDX_MAP(uploaderdlg)
    DDX_CONTROL(IDC_FILELIST,m_wndFileList)
  END_DDX_MAP()

  BEGIN_DLGRESIZE_MAP(uploaderdlg)  
    DLGRESIZE_CONTROL(IDC_FILELIST,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_ADDUPLOAD,DLSZ_MOVE_X|DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_STATICTIP,DLSZ_MOVE_X|DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_MYUPLOADS,DLSZ_MOVE_X|DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()
public:
  static __int64 sentBytes[ATSMAXCOUNT];
  static bool is_prev_uploading;

  uploaderdlg():aSelectedItems(m_wndFileList.m_setSelectedItems){
    m_wndFileList.RegisterClass();
  }

  ~uploaderdlg(){
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
    HBITMAP hbmp=(HBITMAP)c6beeres::R().GetImg(ULTRA_IMGS,IMAGE_BITMAP,864);
    SetMyImageList(hbmp,m_ImageList,MASKCOLOR,16,16,54,GetSysColor(COLOR_3DFACE));
    CenterWindow();
    hIconSmall=(HICON)c6beeres::R().GetImg(IDI_6BEEUPLOADER,
      IMAGE_ICON,::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall,FALSE);

    DoDataExchange(false);

    ShowFList(m_uploadFiles);
    GetSystemSettings();
    InitializeCriticalSection(&csection);
    //InstallIcon(_T("UUFile文件上传工具"),hIconSmall,IDR_POPUP);
    //DestroyIcon(hIconSmall);
    //m_wndFileList.SetBackgroundImage((HBITMAP)c6beeres::R().GetImg(
    //IDB_BITMAP_INFOBK,IMAGE_BITMAP,INFO_WIDTH,INFO_HEIGHT));// wiil be release automatically

    HBITMAP hbmp2=(HBITMAP)c6beeres::R().GetImg(BUTTON_UPLOAD_CH,IMAGE_BITMAP,45*4,22);
    SetMyImageList(hbmp2,m_imglst,MASKPURPLE,45,22,4,GetSysColor(COLOR_3DFACE));
    m_addupload.SubclassWindow(GetDlgItem(IDC_ADDUPLOAD));
    m_addupload.SetToolTipText(_T("上传更多文件?"));
    m_addupload.SetImageList(m_imglst);
    m_addupload.SetImages(3,2,2,2);
    m_addupload.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_myuploads.SubclassWindow(GetDlgItem(IDC_MYUPLOADS));
    m_myuploads.SetToolTipText(_T("所有我上传的文件"));
    m_myuploads.SetImageList(m_imglst);
    m_myuploads.SetImages(1,0,0,0);
    m_myuploads.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    startupload();
    return TRUE;
  }

  LRESULT OnListHyperLink( LPNMHDR lpNMHDR ){
    CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );
    CString url=m_wndFileList.GetItemText(pListNotify->m_nItem,pListNotify->m_nSubItem);
    if (!url.IsEmpty()){
      _6beed_util::runIE(url);
    }
    return 0;
  }

  LRESULT OnRightClick( LPNMHDR lpNMHDR ){
    CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );
    m_wndFileList.GetFile( pListNotify->m_nItem,first_selected_file );
    if (m_uploaderdlgmenu.m_hMenu != NULL)
      return 0;
    SetFocus();
    CPoint pt=pListNotify->m_cp;
    ClientToScreen(&pt);
    if (!m_uploaderdlgmenu.LoadMenu(IDR_POPUP)){
      return 0;
    }
    CMenuHandle menuPopup = m_uploaderdlgmenu.GetSubMenu(1);
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
    m_uploaderdlgmenu.DestroyMenu();
    m_uploaderdlgmenu.m_hMenu = NULL;
    return 0;
  }

  LRESULT OnDoubleClick( LPNMHDR lpNMHDR ){
    startupload();
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
    //RemoveIcon();
    EndDialog(wID);
    return 0;
  }

  // Call Back by Upload thread!!!!
  virtual void OnSetLRemoteFileSize(int threadID,long lRemoteFileSize){
    sentBytes[threadID] += lRemoteFileSize;
  }
  virtual void OnBytesSent2(long lSentBytes,CString flocation,int threadID,
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
    static string ftp_ip;
    static short ftp_port=0;
    if(ftp_port==0){
      UUTOOLBAR::UserPrx _user = MyICE::R();
      if((_user._ptr==NULL)||
        !_user->GetFTPInfoForUpload(_6beed_share::u_getEmail(),ftp_ip,ftp_port))
      {
        return false;
      }
    }
    ip=ftp_ip;
    port=ftp_port;
    return true;
  }

  static string GetUploadedFileURL(const wstring& name){
    UUTOOLBAR::UserPrx _user = MyICE::R();
    if(_user._ptr!=NULL){
      //string fname=stringmaker() << f->m_filename.GetString();
      int fsize=0;
      if(_6beed_util::GetFileSizeW(name.c_str(),fsize)){
        string fname=stringmaker(CP_UTF8) << name;
        return _user->IsFileUploaded(fname,fsize);
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
    uploaderdlg* m_maindlg    = (uploaderdlg*)_ptps->t_pmaindlg;
    int t_ID = _ptps->t_ID;

    int t_nItem = t_pWndFD->GetnItem(t_pfp->m_absPath);
    USES_CONVERSION;

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

  static bool uploadfilewrap(CFileProfile* t_pfp,uploaderdlg* m_maindlg,int t_ID,
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
    if(GetMMF(mmf_descname,wdesc) &&
      GetMMF(mmf_tagname,wtag) &&
      GetMMF(mmf_thumbname,wthumb))
    {
      _6bees_str::strreplace(wdesc,"'","\\'");
      _6bees_str::strreplace(wtag,"'","\\'");
    }
    //Shutdown Screen capture
    HWND h=::FindWindow(NULL,_T("Image Capture"));
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

    uploaderdlg::sentBytes[t_ID]=0;
    int& status = t_pWndFD->m_aFiles[ t_nItem ].m_status;
    if(upload_success){
      USES_CONVERSION;
      string thumbpath = wthumb;
      string fetchurl;
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
          long fid=(long)MyICE::R()->PostUploadFile(filename, t_pfp->m_size_long,desc,tag,email,sid,fetchurl);
          if(fid>0){
            sfp_active.erase(t_pfp->m_absPath);
            UUTOOLBAR::ImgSeq thumbs;
            UUTOOLBAR::Image img;
            if(_6bees_shell::GenICEImg(thumbpath.c_str(),img)){
              thumbs.push_back(img);
              MyICE::R()->UploadImgSeq(thumbs,email,sid,UUTOOLBAR::MYTHUMB,fid);
            }

            t_pWndFD->m_aFiles[t_nItem].m_link = A2CW(fetchurl.c_str());
            t_pWndFD->InvalidateItem(t_nItem);

            string info = stringmaker(CP_ACP) << "Title:" << t_pfp->m_filename.GetString() << "\n";
            if(!wtag.empty()){
              info += stringmaker(CP_ACP) << "Tag:" << wtag << "\n";
            }
            if(!wdesc.empty()){
              info += stringmaker(CP_ACP) << "Description:" << wdesc;
            }
            _6bees_window::NotifyClientMng(CPYDATA_FILEUPLOAD_OK,
              _6bees_const::kClientMng_WndTitle,
              info.c_str());
          }else{
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

  LRESULT OnID_B_START(WORD,WORD wID,HWND,BOOL& ){
    startupload();
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
    _6beed_util::runIE(_6bees_const::kUltraIEURL);
    return 0;
  }
  LRESULT OnID_A_ONLINESPACE(WORD,WORD wID,HWND,BOOL& ){
    _6beed_util::runIE(_6bees_const::kUltraIEURL);
    return 0;
  }

  /*LRESULT OnUpload(WORD,WORD wID,HWND,BOOL& ){
    return 0;
  }*/

  LRESULT OnHide(WORD,WORD wID,HWND,BOOL& ){
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
    CCoolContextMenu<uploaderdlg>::OnInitMenuPopup(uMsg,wParam,lParam,bHandled);
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

  bool uu_StartClientMng(){
    if (!_6bees_window::NotifyClientMng(CPYDATA_LOGIN,kClientMng_WndTitle,NULL)){
      STARTUPINFO si = {sizeof(si)};
      PROCESS_INFORMATION pi;
      using _6bees_util::ultrapath;
      const wstring& clientmngpath = ultrapath::R().get(ultrapath::clientmng);
      if (CreateProcessW(NULL,(LPWSTR)clientmngpath.c_str(),NULL,NULL,false,0,NULL,NULL,&si,&pi)){//Wrap6BEE
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
      }else{
        ::MessageBox(NULL,clientmngpath.c_str(),_T("ERROR When Starting 6BeeClientMng Program."),MB_ICONINFORMATION);
        return false;
      }
    }
    return true;
  }

  void startupload(){
    // 1. tell if user has logged in, or start login UI
    if(!_6beed_share::u_getLoggedin()){
      uu_StartClientMng();
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
};

__int64 uploaderdlg::sentBytes[ATSMAXCOUNT]={};
bool uploaderdlg::is_prev_uploading=false;
