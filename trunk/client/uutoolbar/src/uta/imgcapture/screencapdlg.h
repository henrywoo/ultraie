#ifndef __UUPLUGIN_IMGCAPTURE_SCREENCAPDLG__
#define __UUPLUGIN_IMGCAPTURE_SCREENCAPDLG__

#include "screendlg.h"
#include "atlimage.h"
#include <atlwfile.h>
#include <time.h>
#include "../../dll/6beecommond/6beecommond.h"
#include <6BeeResDialog.h>
#include "../../dll/6beeres/resource.h"
#include <atlctrlx.h>
#include "6bees_util.h"//SetMyImageList
#include "6bees_str.h"
using namespace _6bees_str;
#include <copydatamsg.h>
#include <6bees_lang.h>

class screencapdlg:
  public C6BeeResDialogImpl<screencapdlg>,
  public CDialogResize<screencapdlg>
{
private:
  HBITMAP m_hBitmap;
  CComboBox m_ComBox;
  HICON hIcon;
  HICON hIconSmall;
  CWindow m_Title;
  ATL::CString picTitle;
  ATL::CString PicturePath;
  ATL::CString PicturePath_thumb;
  ATL::CString Tag;
  ATL::CString Desc;

  ATL::CString ext;
  GUID guidFileType;

  CImageList m_imglst;
  CBitmapButton m_upload;
  CBitmapButton m_save;
  CBitmapButton m_ImagePreview;

  CImageList m_imglst2;
  CBitmapButton m_tag_sugg;
  CBitmapButton m_desc_sugg;

  CWindow m_Tag;
  CWindow m_Desc;

  CWindow pSaveButton;
  CWindow imgName;
  CWindow pWnd;

  CWindow m_progress;
  CWindow m_name2;
public:
  enum { IDD = IDD_IMG_CAPTURE };

  BEGIN_MSG_MAP(screencapdlg)
    MESSAGE_HANDLER(WM_INITDIALOG,   OnInitDialog)
    MESSAGE_HANDLER(WM_TIMER,        OnTimer)
    MESSAGE_HANDLER(WM_COPYDATA,        OnCopy)
    COMMAND_ID_HANDLER(IDC_UPLOAD_NOW,    OnUpload)
    COMMAND_ID_HANDLER(IDC_IMGPREVIEW,    OnPreview)
    COMMAND_ID_HANDLER(IDCANCEL,     OnCancel)
    COMMAND_ID_HANDLER(IDC_SAVE,     OnSave)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_ACTIVATE,OnPaint)
    CHAIN_MSG_MAP(CDialogResize<screencapdlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(screencapdlg)  
    DLGRESIZE_CONTROL(IDC_WEBCONTENTSDESC,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_UPLOAD_NOW,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_SAVE,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_STATIC_VER,DLSZ_SIZE_Y)
  END_DLGRESIZE_MAP()

  screencapdlg::screencapdlg(){
    m_hBitmap=NULL;
    hIcon=NULL;
    hIconSmall=NULL;

    picTitle=L"";
    PicturePath=L"";
    PicturePath_thumb=L"";
    Tag=L"";
    Desc=L"";
    ext=L"";
  }

  LRESULT OnInitDialog(UINT,WPARAM,LPARAM,BOOL&){
    DlgResize_Init(true,true,WS_THICKFRAME | WS_CLIPCHILDREN);
    CenterWindow();
    m_ComBox.Attach(GetDlgItem(IDC_EXT));
    m_ComBox.InsertString(0,_T("png"));
    m_ComBox.InsertString(1,_T("jpg"));
    m_ComBox.InsertString(2,_T("gif"));
    m_ComBox.InsertString(3,_T("bmp"));
    m_ComBox.SetCurSel(0);

    m_Title.Attach(GetDlgItem(IDC_WEBCONTENTSNAME));
    ext=_T("");
    guidFileType = Gdiplus::ImageFormatBMP;

    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::picdir);
    if(!_6bees_util::MakeSureDir(upath.c_str())){
      return 0;
    }
    PicturePath.Format(_T("%s%lu.bmp"),upath.c_str(),time(0));
    picTitle = _T("");
    //////////////////////////////////////////////////////////////////////////
    HBITMAP hbmp=(HBITMAP)c6beeres::R().GetImg(IDB_ULTRABUTTONS,IMAGE_BITMAP,95*17,26);
    SetMyImageList(hbmp,m_imglst,MASKPURPLE,95,26,17,GetSysColor(COLOR_3DFACE));
    m_upload.SubclassWindow(GetDlgItem(IDC_UPLOAD_NOW));
    m_upload.SetToolTipText(tipuploads);
    m_upload.SetImageList(m_imglst);
    m_upload.SetImages(5,-1,4,-1);
    m_upload.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_save.SubclassWindow(GetDlgItem(IDC_SAVE));
    m_save.SetToolTipText(_T("Save as a local file"));
    m_save.SetImageList(m_imglst);
    m_save.SetImages(3,-1,2,-1);
    m_save.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_ImagePreview.SubclassWindow(GetDlgItem(IDC_IMGPREVIEW));
    m_ImagePreview.SetToolTipText(_T("Save as a local file"));
    m_ImagePreview.SetImageList(m_imglst);
    m_ImagePreview.SetImages(11,-1,10,-1);
    m_ImagePreview.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    pSaveButton.Attach(GetDlgItem(IDC_SAVE));
    imgName.Attach(GetDlgItem(IDC_WEBCONTENTSNAME));
    pWnd.Attach(GetDlgItem(IDC_WEBCONTENTS_THUMBNAIL));

    // Hide Some element
    m_progress.Attach(GetDlgItem(IDC_PROGRESS));
    m_progress.ShowWindow(SW_HIDE);
    m_name2.Attach(GetDlgItem(IDC_WEBCONTENTSNAME2));
    m_name2.ShowWindow(SW_HIDE);
    m_tag_sugg.SubclassWindow(GetDlgItem(IDC_TAG_SUGG));
    m_tag_sugg.ShowWindow(SW_HIDE);
    m_desc_sugg.SubclassWindow(GetDlgItem(IDC_DESC_SUGG));
    m_desc_sugg.ShowWindow(SW_HIDE);

    m_Tag.Attach(GetDlgItem(IDC_WEBCONTENTSTAG));
    m_Desc.Attach(GetDlgItem(IDC_WEBCONTENTSDESC));
    //////////////////////////////////////////////////////////////////////////
    ShowWindow(SW_HIDE);
    Sleep(50);
    INT_PTR nReturn;
    CScreenDlg dlg;
    if((nReturn = dlg.DoModal()) == MSG_FINISH){
      PicturePath_thumb = dlg.Thumbpath;
      ShowWindow(SW_SHOW);
      if(::OpenClipboard(m_hWnd)){
        m_hBitmap = (HBITMAP)::GetClipboardData(CF_BITMAP);
        CreateTempBMPFile();
        ShowCapturedBmp();
        ::CloseClipboard();
      }
    }else if(nReturn == 0){
      ::CloseClipboard();
      EndDialog(0);
    }
    return TRUE;
  }

  LRESULT OnPreview(WORD,WORD,HWND,BOOL&){
    ShellExecuteW(NULL,NULL,PicturePath,NULL,NULL,SW_SHOW);
    return 0;
  }

  LRESULT OnUpload(WORD,WORD wID,HWND,BOOL&){
    m_Title.GetWindowText(picTitle);
    if (picTitle.IsEmpty()){
      MessageBox(_T("Please name the picture's title!"),_T("Reminder..."),MB_OK);
      m_Title.SetFocus();
      return 0;
    }
    GetExt(m_ComBox.GetCurSel(),ext,guidFileType);

    ATL::CString targetfilelocation;
    unsigned long t=(unsigned long)time(0);
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::picdir);
    if(!_6bees_util::MakeSureDir(upath.c_str())){
      return 0;
    }
    targetfilelocation.Format(_T("%sScreenCapture_%lu."),upath.c_str(),t);
    targetfilelocation.Append(ext);
    ATL::CString newthumbFilepath;
    newthumbFilepath.Format(_T("%sScreenCapture_%lu_thumb.jpg"),upath.c_str(),t);
    CFile::Rename(PicturePath_thumb,newthumbFilepath);
    CImage mmage;
    if (FAILED(mmage.Load(PicturePath))||FAILED(mmage.Save(targetfilelocation, guidFileType))){
      MessageBox(_T("Save File Failed!"));
    }
    HWND hwndTo = ::FindWindow(0,_6bees_const::kUploader_WndTitle);
    m_Desc.GetWindowText(Desc);
    m_Tag.GetWindowText(Tag);
    //////////////////////////////////////////////////////////////////////////
    std::string mmfname_desc,mmfname_tag,mmfname_thumb;
    string fname = stringmaker() << PathFindFileNameW(targetfilelocation);
    GenMMFName(fname,mmfname_desc,mmfname_tag,mmfname_thumb);
    string desc_ = stringmaker()<<Desc.GetString();
    string tag_ = stringmaker()<<Tag.GetString();
    string newthumbFilepath_ = stringmaker()<<newthumbFilepath.GetString();
    _6beed_util::SetMMFA(mmfname_desc.c_str() , desc_.c_str());
    _6beed_util::SetMMFA(mmfname_tag.c_str()  , tag_.c_str());
    _6beed_util::SetMMFA(mmfname_thumb.c_str(), newthumbFilepath_.c_str());
    //////////////////////////////////////////////////////////////////////////
    if(!hwndTo){
      using _6bees_util::ultrapath;
      const wstring& upath = ultrapath::R().get(ultrapath::uploader);
      ATL::CString UploaderCmd;
      UploaderCmd.Format(_T("%s -m directupload -f \"%s\""),upath.c_str(),targetfilelocation);
      STARTUPINFO si = {sizeof(si)};
      PROCESS_INFORMATION pi;
      if (!CreateProcess(NULL,(LPWSTR)UploaderCmd.GetString(),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
        MessageBox(_T("ERROR"),_T("Cannot start uploader."),MB_OK);
      }
    }else{
      string afilepath=stringmaker(CP_ACP) << targetfilelocation.GetString();
      _6beed_util::SendMsgToUploader(hwndTo,afilepath.c_str(),CPYDATA_UPLOAD_2_UPLOAD_DIRECT);
    }
    //EndDialog(wID);
    ShowWindow(SW_HIDE);
    return 0;
  }

  LRESULT OnCancel(WORD, WORD wID, HWND, BOOL& ){
    DeleteFile(PicturePath);
    DeleteFile(PicturePath_thumb);
    EndDialog(wID);
    return 0;
  }

  void ShowCapturedBmp(){
    ATL::CString name;
    name.Format(_T("UltraIE_%d"),time(0));
    imgName.SetWindowText(name);
    if(m_hBitmap!=NULL){
      RECT rect;
      pWnd.GetWindowRect(&rect);
      CBitmap pBitmap;
      pBitmap.Attach(m_hBitmap);
      BITMAP bm;
      pBitmap.GetBitmap(&bm);
      HDC pDC = pWnd.GetDC();
      CDC dcMem;
      dcMem.CreateCompatibleDC(pDC);
      HBITMAP pOldBmp = dcMem.SelectBitmap(pBitmap.m_hBitmap);
      SetStretchBltMode(pDC,HALFTONE);//This is very important, or the thumb will be rather ulgly!!!
      StretchBlt(pDC,1,1,rect.right-rect.left-2, rect.bottom-rect.top-2,dcMem.m_hDC,0,0,bm.bmWidth, bm.bmHeight,SRCCOPY);
      dcMem.SelectBitmap(pOldBmp);
      pSaveButton.EnableWindow(TRUE);
      SetForegroundWindow(m_hWnd);
    }else{
      pSaveButton.EnableWindow(FALSE);
    }
  }

  LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    ShowCapturedBmp();
    bHandled = false;
    return 0;
  }

  LRESULT OnTimer(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    if (m_hBitmap!=NULL){
      ShowCapturedBmp();
    }
    return 0;
  }

  LRESULT OnSave(WORD,WORD wID,HWND,BOOL&){
    if(m_hBitmap==NULL)
      return 0;    
    CWindow pTitle;
    pTitle.Attach(GetDlgItem(IDC_WEBCONTENTSNAME));
    ATL::CString title;
    pTitle.GetWindowText(title);
    GetExt(m_ComBox.GetCurSel(),ext,guidFileType);

    ATL::CString savext = _T("Save Files(*.") + ext + _T(")");
    CFileDialog fileDlg(FALSE, ext.GetString(), title, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,savext.GetString());
    TCHAR DesktopPath[ MAX_PATH ];
    SHGetSpecialFolderPath(0,DesktopPath,CSIDL_DESKTOPDIRECTORY,FALSE/*Create if doesn't exists?*/);
    fileDlg.m_ofn.lpstrInitialDir=DesktopPath;
    if(IDOK==fileDlg.DoModal()){
      CImage mmage;
      ATL::CString bmpfilelocation = PicturePath;
      if (FAILED(mmage.Load(bmpfilelocation))) {
        MessageBox(_T("Load File Failed!"));
      }
      if (FAILED(mmage.Save(fileDlg.m_szFileName,guidFileType))) { 
        MessageBox(_T("Save File Failed!"));
      }
      EndDialog(wID);
    }
    return 0;
  }

  BOOL CreateTempBMPFile(){
    PBITMAPINFO pbmp;
    if(!CreateBitmapInfoStruct(m_hBitmap, pbmp)){
      return FALSE;
    }
    if(!CreateBMPFile(PicturePath, pbmp, m_hBitmap, GetDC()))
      MessageBox(_T("Save File Error!"),_T("Error"));
    return TRUE;
  }

  LRESULT OnCopy(UINT,WPARAM,LPARAM,BOOL&){
    EndDialog(0);
    return 0;
  }
};

#endif