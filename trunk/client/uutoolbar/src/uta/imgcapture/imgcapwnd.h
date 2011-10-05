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

#ifndef __STUB_SKELETON_IMAGECAPWND__
#define __STUB_SKELETON_IMAGECAPWND__

#include "../../dll/6beeres/resource.h"
#include "../../dll/6beecommond/6beecommond.h"
#include <imgcap_setting.h>
#include "6bees_util.h"
#include "uulogging.h"
#include "6bees_str.h"

#undef _CSTRING_NS
#define _CSTRING_NS  ATL

#include <atldlgs.h>  //CFileDialog
#include <atlimage.h> //ATL::CImage
#include <atlframe.h> //DlgResize_Init

#include <shellapi.h>
#include <shlobj.h>   //SHGetSpecialFolderPath

#include <6BeeResDialog.h>
#include <pcre++.h>
#include <6bees_lang.h>

using namespace _6bees_util;
using namespace pcrepp;
using namespace _6bees_str;

class CImagecapwnd :
  public C6BeeResDialogImpl<CImagecapwnd>,
  public CDialogResize<CImagecapwnd>
{
private:
  ATL::CImage cimg;

  CWindow m_Name;
  CWindow m_Thumbnail;
  CComboBox m_ComBox;

  CWindow m_Tag;
  CWindow m_Desc;

  CImageList m_imglst;
  CBitmapButton m_upload;
  CBitmapButton m_save;
  CBitmapButton m_ImagePreview;

  CImageList m_imglst2;
  CBitmapButton m_tag_sugg;
  CBitmapButton m_desc_sugg;

  CWindow m_progress;
  CWindow m_name2;
public:
  ATL::CString Tag;
  ATL::CString Desc;

  ATL::CString WndTitle;
  ATL::CString str_Name;
  ATL::CString FolderName;
  ATL::CString absPath;
  ATL::CString absThumbPath;
  ATL::CString waitthumbnail;

public:
  enum {IDD = IDD_IMG_CAPTURE};

  BEGIN_MSG_MAP(CImagecapwnd)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDCANCEL,OnCancel)
    COMMAND_ID_HANDLER(IDC_IMGPREVIEW,OnImagePreview)
    COMMAND_ID_HANDLER(IDC_UPLOAD_NOW,OnUpload)
    COMMAND_ID_HANDLER(IDC_SAVE,OnSave)

    COMMAND_HANDLER(IDC_TAG_SUGG,BN_CLICKED,OnTagSugg)
    COMMAND_HANDLER(IDC_DESC_SUGG,BN_CLICKED,OnDescSugg)

    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_ACTIVATE,OnPaint)

    CHAIN_MSG_MAP(CDialogResize<CImagecapwnd>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CImagecapwnd)  
    DLGRESIZE_CONTROL(IDC_WEBCONTENTSDESC,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_UPLOAD_NOW,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_SAVE,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_STATIC_VER,DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_WEBCONTENTSTAG,DLSZ_SIZE_X)
  END_DLGRESIZE_MAP()

  CImagecapwnd():WndTitle(_T("")),str_Name(_T("")),absPath(_T("")){}
  virtual ~CImagecapwnd(){
    m_imglst.Destroy();
    m_imglst2.Destroy();
  }

  LRESULT OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    DlgResize_Init(true,true,WS_THICKFRAME | WS_CLIPCHILDREN);

    HICON m_hIcon = (HICON)c6beeres::R().GetImg(IDI_IMGCAPTURE);
    SetIcon(m_hIcon,TRUE);
    ::DestroyIcon(m_hIcon);

    ::SetWindowTextW(m_hWnd,WndTitle);

    m_Name.Attach(GetDlgItem(IDC_WEBCONTENTSNAME));
    m_Name.SetWindowText(str_Name);

    m_Thumbnail.Attach(GetDlgItem(IDC_WEBCONTENTS_THUMBNAIL));

    if (absThumbPath.IsEmpty()){
      if(!absPath.IsEmpty())
        drawthumbnail(absPath);
      else{
        ATL::CString waitthumbnail = _6beed_util::Get6BEEPath();
        waitthumbnail.Append(_T("waitthumbnail.jpg"));
        drawthumbnail(waitthumbnail);
        m_ImagePreview.EnableWindow(FALSE);
      }
    }else{
      drawthumbnail(absThumbPath);
    }

    m_ComBox.Attach(GetDlgItem(IDC_EXT));
    m_ComBox.InsertString(0,_T("png"));
    m_ComBox.InsertString(1,_T("jpg"));
    m_ComBox.InsertString(2,_T("gif"));
    m_ComBox.InsertString(3,_T("bmp"));
    m_ComBox.SetCurSel(0);

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
    m_ImagePreview.SetToolTipText(_T("Image Preview"));
    m_ImagePreview.SetImageList(m_imglst);
    m_ImagePreview.SetImages(9,-1,8,-1);
    m_ImagePreview.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    HBITMAP hbmp2=(HBITMAP)c6beeres::R().GetImg(ULTRA_IMGS,IMAGE_BITMAP,54*16,16);
    SetMyImageList(hbmp2,m_imglst2,MASKGREY,16,16,54,GetSysColor(COLOR_3DFACE));
    m_tag_sugg.SubclassWindow(GetDlgItem(IDC_TAG_SUGG));
    m_tag_sugg.SetToolTipText(tipsuggest);
    m_tag_sugg.SetImageList(m_imglst2);
    m_tag_sugg.SetImages(12);

    m_desc_sugg.SubclassWindow(GetDlgItem(IDC_DESC_SUGG));
    m_desc_sugg.SetToolTipText(_T("suggect description?"));
    m_desc_sugg.SetImageList(m_imglst2);
    m_desc_sugg.SetImages(29);

    m_Tag.Attach(GetDlgItem(IDC_WEBCONTENTSTAG));
    m_Desc.Attach(GetDlgItem(IDC_WEBCONTENTSDESC));

    // Hide Some element
    m_progress.Attach(GetDlgItem(IDC_PROGRESS));
    m_progress.ShowWindow(SW_HIDE);
    m_name2.Attach(GetDlgItem(IDC_WEBCONTENTSNAME2));
    m_name2.ShowWindow(SW_HIDE);
    return 0;
  }

  LRESULT OnCancel(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
    DeleteFile(absPath);
    DeleteFile(absThumbPath);
    EndDialog(0);
    return 0;
  }

  LRESULT OnImagePreview(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
    ShellExecuteW(NULL,NULL,absPath,NULL,NULL,SW_SHOW);
    return 0;
  }

  LRESULT OnSave(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
    if(absPath.IsEmpty())
      return 0;
    ATL::CString title;
    m_Name.GetWindowText(title);
    GUID guidFileType;
    int cs=m_ComBox.GetCurSel();
    ATL::CString ext;
    GetExt(cs,ext,guidFileType);

    std::wstring validTitle= _6bees_str::GenFileName(title);
    ATL::CString savext = ksavefile + ext + _T(")");
    CFileDialog fileDlg(FALSE,ext,validTitle.c_str(),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,savext);

    TCHAR DesktopPath[MAX_PATH];
    SHGetSpecialFolderPath(0,DesktopPath,CSIDL_DESKTOPDIRECTORY,FALSE/*Create if doesn't exists?*/);
    fileDlg.m_ofn.lpstrInitialDir=DesktopPath;
    if(IDOK==fileDlg.DoModal()){
      ATL::CString absPath_ext = absPath.Right(3);
      if (absPath_ext.CompareNoCase(ext)==0){
        CopyFile(absPath,fileDlg.m_szFileName,false);
      }else{
        CImage mmage;
        if (FAILED(mmage.Load(absPath))) {
          MessageBox(_T("Load File Failed!"));
        }
        if (FAILED(mmage.Save(fileDlg.m_szFileName,guidFileType))) { 
          MessageBox(_T("Save File Failed!"));
        }
      }
    }else{
      DeleteFile(absPath);
      DeleteFile(absThumbPath);
    }
    //EndDialog(wID);
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
    m_Desc.SetWindowText(Desc);
    return 0;
  }

  LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    drawthumbnail();
    bHandled = false;
    return 0;
  }

private:
  // implementation
  void drawthumbnail(LPCTSTR pic=NULL){
    if(pic){
      if (FAILED(cimg.Load(pic))){
        return;
      }
    }
    HDC hdc=m_Thumbnail.GetDC();
    CRect r;
    m_Thumbnail.GetClientRect(&r);
    int imgw = cimg.GetWidth();
    int imgh = cimg.GetHeight();
    POINT start={(r.Width()-imgw)/2,(r.Height()-imgh)/2};

    HGDIOBJ  hPrevBrush = SelectObject (hdc,GetStockObject(WHITE_BRUSH));
    Rectangle(hdc,r.left,r.top,r.right,r.bottom);
    DeleteObject(hPrevBrush);

    cimg.Draw(hdc,start.x,start.y,imgw,imgh,0,0,imgw,imgh);
  }

  LRESULT OnUpload(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
    ATL::CString picTitle;
    m_Name.GetWindowText(picTitle);
    if (picTitle.IsEmpty()){
      MessageBox(_T("Please name the picture's title!"),_T("Reminder..."),MB_OK);
      m_Name.SetFocus();
      return 0;
    }
    ATL::CString ext;
    GUID guidFileType;
    int cs=m_ComBox.GetCurSel();
    GetExt(cs,ext,guidFileType);

    CImage mmage;
    ATL::CString bmpfilelocation = absPath;
    wstring tmp = GenFileName(picTitle.GetString());
    ATL::CString targetfilelocation=FolderName + tmp.c_str()  + _T(".") + ext;
    if (FAILED(mmage.Load(bmpfilelocation))) {
      wstring errormsg=L"Load File Failed: ";
      errormsg += bmpfilelocation;
      UUDEBUG((LOG_ERROR,L"Load File Failed!"));
      return 0;
    }
    if (FAILED(mmage.Save(targetfilelocation, guidFileType))) {
      ATL::CString info;
      info.Format(_T("Cannot save the picture as:\n%s\n\nPlease give the picture another name."),targetfilelocation);
      ::MessageBox(NULL,info,_T("Save file failed - Please check if the name is correct!"),MB_ICONINFORMATION);
      m_Name.SetFocus();
      return 0;
    }
    m_Desc.GetWindowText(Desc);
    m_Tag.GetWindowText(Tag);
    _6beed_util::InvokeUploader(targetfilelocation,Desc,Tag,absThumbPath);
    EndDialog(wID);
    return 0;
  }
};

#endif
