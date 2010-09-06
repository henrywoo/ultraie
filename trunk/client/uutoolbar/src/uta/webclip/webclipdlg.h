#ifndef __PLUGINS_WEBCLIP_WEBCLIPDLG__
#define __PLUGINS_WEBCLIP_WEBCLIPDLG__

#include "resource.h"
#include <atlctrlx.h>
#include <atlimage.h>
#include <atlframe.h>

#include <copydatamsg.h>
#include <imgcap_setting.h>
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#include <6BeeResDialog.h>
#include <6BeeProgressBar.h>
#include "6bees_util.h"
#include "6bees_str.h"
#include <pcre++.h>
#include <6bees_lang.h>
using namespace _6bees_str;
using namespace _6bees_util;
using namespace pcrepp;

class webclipdlg:
  public C6BeeResDialogImpl<webclipdlg>,
  public CDialogResize<webclipdlg>
{
  ATL::CImage cimg;

  C6BeeProgressCtrl  m_Progress;
  CWindow m_Name;
  CWindow m_Thumbnail;
  CWindow m_Edit_Print;

  CWindow m_Tag;
  CWindow m_Desc;

  DWORD HTMLEditor_Process_ID;

  CImageList m_imglst;
  CBitmapButton m_upload;
  CBitmapButton m_ImagePreview;

  CWindow m_save;
  CWindow m_ext;
  CWindow m_name1;

  CImageList m_imglst2;
  CBitmapButton m_tag_sugg;
  CBitmapButton m_desc_sugg;

public:
  ATL::CString WndTitle;
  ATL::CString str_Name;
  ATL::CString FolderName;
  ATL::CString absHTMLPath;
  ATL::CString absThumbPath;
  ATL::CString waitthumbnail;

  int progress;

  static map<DWORD,webclipdlg*> tid2webclipdlgptrmap;
public:
  enum { IDD = IDD_IMG_CAPTURE };
  enum { TIMERID = 1 };

  BEGIN_MSG_MAP(webclipdlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDCANCEL,OnCancel)
    COMMAND_ID_HANDLER(IDC_IMGPREVIEW,OnEditPrint)
    COMMAND_ID_HANDLER(IDC_SAVE,OnUpload)

    COMMAND_HANDLER(IDC_TAG_SUGG,BN_CLICKED,OnTagSugg)
    COMMAND_HANDLER(IDC_DESC_SUGG,BN_CLICKED,OnDescSugg)

    MESSAGE_HANDLER(WM_TIMER, OnTimer)
    MESSAGE_HANDLER(WM_COPYDATA, OnCopyData)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_ACTIVATE,OnPaint)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)

    CHAIN_MSG_MAP(CDialogResize<webclipdlg>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(webclipdlg)
    DLGRESIZE_CONTROL(IDC_WEBCONTENTSDESC,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_UPLOAD_NOW,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_SAVE,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_STATIC_VER,DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_WEBCONTENTSTAG,DLSZ_SIZE_X)
  END_DLGRESIZE_MAP()

  webclipdlg():WndTitle(L""),str_Name(L""),absHTMLPath(L""),absThumbPath(L""),HTMLEditor_Process_ID(0){
    progress = 0;
    waitthumbnail = _6beed_util::Get6BEEPath();
    waitthumbnail.Append(_T("waitthumbnail.jpg"));
  }

  virtual ~webclipdlg(){
    m_imglst.Destroy();
    m_imglst2.Destroy();
    DWORD dwError = ERROR_SUCCESS;
    // try to terminate the process
    HANDLE IECAP_Process_Handle = OpenProcess(PROCESS_TERMINATE, FALSE, HTMLEditor_Process_ID);
    if (IECAP_Process_Handle){
      if (!TerminateProcess(IECAP_Process_Handle, (DWORD)-1))
        dwError = GetLastError();
      CloseHandle(IECAP_Process_Handle);// close process handle
    }
  }

  LRESULT OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    DlgResize_Init(true,true,WS_THICKFRAME | WS_CLIPCHILDREN);

    HICON m_hIcon = (HICON)c6beeres::R().GetImg(IDI_IMGCAPTURE);
    SetIcon(m_hIcon,TRUE);
    ::DestroyIcon(m_hIcon);

    m_Progress.SubclassWindow(GetDlgItem(IDC_PROGRESS));//initialization
    SetTimer(TIMERID,100);
    ::SetWindowTextW(m_hWnd,WndTitle);

    m_Name.Attach(GetDlgItem(IDC_WEBCONTENTSNAME2));
    m_Name.SetWindowText(str_Name);
    m_Thumbnail.Attach(GetDlgItem(IDC_WEBCONTENTS_THUMBNAIL));
    drawthumbnail(PathFileExists(absThumbPath)?absThumbPath:waitthumbnail);
    m_Edit_Print.Attach(GetDlgItem(IDC_IMGPREVIEW));
    m_Edit_Print.EnableWindow(FALSE);

    HBITMAP hbmp=(HBITMAP)c6beeres::R().GetImg(IDB_ULTRABUTTONS,IMAGE_BITMAP,95*17,26);
    SetMyImageList(hbmp,m_imglst,MASKPURPLE,95,26,17,GetSysColor(COLOR_3DFACE));
    m_upload.SubclassWindow(GetDlgItem(IDC_SAVE));
    m_upload.SetToolTipText(tipuploads);
    m_upload.SetImageList(m_imglst);
    m_upload.SetImages(5,-1,4,-1);
    m_upload.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_ImagePreview.SubclassWindow(GetDlgItem(IDC_IMGPREVIEW));
    m_ImagePreview.SetToolTipText(editprint);
    m_ImagePreview.SetImageList(m_imglst);
    m_ImagePreview.SetImages(11,-1,10,12);
    m_ImagePreview.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    HBITMAP hbmp2=(HBITMAP)c6beeres::R().GetImg(ULTRA_IMGS,IMAGE_BITMAP,54*16,16);
    SetMyImageList(hbmp2,m_imglst2,MASKGREY,16,16,54,GetSysColor(COLOR_3DFACE));
    m_tag_sugg.SubclassWindow(GetDlgItem(IDC_TAG_SUGG));
    m_tag_sugg.SetToolTipText(tipsuggest);
    m_tag_sugg.SetImageList(m_imglst2);
    m_tag_sugg.SetImages(12);

    m_Tag.Attach(GetDlgItem(IDC_WEBCONTENTSTAG));
    m_Desc.Attach(GetDlgItem(IDC_WEBCONTENTSDESC));

    // Hide some elements
    m_desc_sugg.SubclassWindow(GetDlgItem(IDC_DESC_SUGG));
    m_desc_sugg.ShowWindow(SW_HIDE);
    m_save.Attach(GetDlgItem(IDC_UPLOAD_NOW));
    m_save.ShowWindow(SW_HIDE);
    m_ext.Attach(GetDlgItem(IDC_EXT));
    m_ext.ShowWindow(SW_HIDE);
    m_name1.Attach(GetDlgItem(IDC_WEBCONTENTSNAME));
    m_name1.ShowWindow(SW_HIDE);
    return 0;
  }

  LRESULT OnCancel(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
    DeleteFile(absHTMLPath);
    DeleteFile(absThumbPath);
    RemoveDirectory(FolderName);
    EndDialog(0);
    return 0;
  }

  LRESULT OnEditPrint(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
    CallHTMLEdit(false);
    return 0;
  }
  LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    drawthumbnail();
    m_Progress.SetPos(progress);
    bHandled = false;
    return 0;
  }
  LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
    drawthumbnail();
    bHandled = false;
    return 0;
  }
  LRESULT OnTimer(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    if (wParam==TIMERID){
      m_Progress.Invalidate();
      m_Progress.SetPos(progress);
      if (progress==100){
        //m_ProgressMsg.SetWindowText(_T("Finished!"));
        if( PathFileExists(absThumbPath)){
          drawthumbnail(absThumbPath);
        }
        CallHTMLEdit(true);
        KillTimer(TIMERID);
      }
    }
    return 0;
  }
  LRESULT OnCopyData(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    PCOPYDATASTRUCT pMyCDS = (PCOPYDATASTRUCT) lParam;
    ULONG_PTR a= pMyCDS->dwData;
    if (a==CPYDATA_HTMLEDITOR_TO_PROGRESSDLG){//notify thumb generate successfully, now display it
      m_Edit_Print.EnableWindow(TRUE);
      drawthumbnail(absThumbPath);
      m_Progress.SetPos(100);
      ::SetForegroundWindow(m_hWnd);
    }
    return 0;
  }
  LRESULT OnTagSugg(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
    ATL::CString picTitle;
    m_Name.GetWindowText(picTitle);
    if (!picTitle.IsEmpty()){
      wstring sug=GetSugTxt(picTitle.GetString());
      m_Tag.SetWindowText(sug.c_str());
    }
    return 0;
  }
  LRESULT OnDescSugg(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
    //m_Desc.SetWindowText(Desc);
    return 0;
  }

private:
  void CallHTMLEdit(bool hide){
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::htmleditor);
    ATL::CString cmd;
    cmd.Format(_T("%s \"%s\" \"%s\" \"%s\" \"%s\""),
      upath.c_str(),absHTMLPath,WndTitle,absThumbPath,hide?_T("HIDE"):_T("SHOW"));
    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    if (CreateProcess(NULL,(LPWSTR)cmd.GetString(),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
      HTMLEditor_Process_ID = pi.dwProcessId;
    }else{
      ::MessageBox(NULL,cmd,kgentitle,MB_ICONINFORMATION);
    }
  }

  void drawthumbnail(LPCTSTR pic=NULL){
    if(pic){
      if (!cimg.IsNull()){
        cimg.Destroy();
      }
      if (FAILED(cimg.Load(pic))){
        return;
      }
    }
    HDC hdc=m_Thumbnail.GetDC();
    CRect r;
    m_Thumbnail.GetClientRect(&r);
    if (!cimg.IsNull()){
      int imgw = cimg.GetWidth();
      int imgh = cimg.GetHeight();
      POINT start={(r.Width()-imgw)/2,(r.Height()-imgh)/2};

      HGDIOBJ  hPrevBrush = SelectObject (hdc,GetStockObject(WHITE_BRUSH));
      Rectangle(hdc,r.left,r.top,r.right,r.bottom);
      DeleteObject(hPrevBrush);

      cimg.Draw(hdc,start.x,start.y,imgw,imgh,0,0,imgw,imgh);
    }
  }


  LRESULT OnUpload(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
    ATL::CString picTitle;
    m_Name.GetWindowText(picTitle);
    if (picTitle.IsEmpty()){
      MessageBox(kgiveaname,kgentitle,MB_OK);
      m_Name.SetFocus();
      return 0;
    }
    ATL::CString newHTMLname=FolderName+ GenFileName(picTitle.GetString()).c_str()+GenUniqStr().c_str()+_T(".html");
    if(::MoveFileW(absHTMLPath,newHTMLname)){
      ATL::CString Desc,Tag;
      m_Desc.GetWindowText(Desc);
      m_Tag.GetWindowText(Tag);
      _6beed_util::InvokeUploader(newHTMLname,Desc,Tag,absThumbPath);
      //OutputDebugString(Desc);
    }
    EndDialog(wID);
    return 0;
  }

};

map<DWORD,webclipdlg*> webclipdlg::tid2webclipdlgptrmap;

#endif