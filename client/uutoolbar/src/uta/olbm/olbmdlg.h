#ifndef __PLUGINS_OLBMDLG__
#define __PLUGINS_OLBMDLG__

#include "resource.h"

#include "../../dll/6beecommond/6beecommond.h"
#include <6BeeResDialog.h>
#include <imgcap_setting.h>
#include <copydatamsg.h>
#include "../../dll/6beeres/resource.h"

#include "IceSingleton.h"
#include "6bees_net.h"
#include "6bees_util.h"
#include "6bees_html.h"
#include "6bees_window.h"
#include "6bees_const.h"
#include <copydatamsg.h>

#include <atlframe.h>
#include <atlimage.h>
#include <time.h>
#include "6bees_str.h"
#include <pcre++.h>
#include <6bees_lang.h>

using namespace _6bees_util;
using namespace _6bees_str;
using namespace std;
using namespace pcrepp;

struct OLBMThunk{
  ATL::CString _str_Name;
  ATL::CString _str_URL;
  ATL::CString _str_Tag;
  ATL::CString _str_Desc;
  ATL::CString _str_faviconURL;
  wstring absThumbPath;
} myolbm;

UINT WINAPI thread_SaveOLBM(LPVOID p);

class olbmdlg:
  public C6BeeResDialogImpl<olbmdlg>,
  public CDialogResize<olbmdlg>
{
public:
  ATL::CImage cimg;

  CWindow m_Name;
  CWindow m_Thumbnail;
  CWindow m_Url;
  CWindow m_Tag;
  CWindow m_Desc;

  wstring absThumbPath;
  wstring str_Name;
  wstring str_URL;
  ATL::CString faviconURL;

  string str_Tag;
  ATL::CString Desc;

  CImageList m_imglst;
  CBitmapButton m_save;

  CImageList m_imglst2;
  CBitmapButton m_tag_sugg;
  CBitmapButton m_desc_sugg;

public:

  enum { IDD = IDD_ONLINEBOOKMARK };

  BEGIN_MSG_MAP(olbmdlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDCANCEL,OnCancel)
    COMMAND_ID_HANDLER(IDC_OLBM_SAVE,OnSave)

    COMMAND_HANDLER(IDC_TAG_SUGG,BN_CLICKED,OnTagSugg)
    COMMAND_HANDLER(IDC_DESC_SUGG,BN_CLICKED,OnDescSugg)

    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_ACTIVATE,OnPaint)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    CHAIN_MSG_MAP(CDialogResize<olbmdlg>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(olbmdlg)
    DLGRESIZE_CONTROL(IDC_OLBM_DESC,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    //DLGRESIZE_CONTROL(IDC_WEBCONTENTSTAG,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_OLBM_TAG,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_OLBM_NAME,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_OLBM_URL,DLSZ_SIZE_X)
  END_DLGRESIZE_MAP()

  olbmdlg():str_Name(),str_URL(),absThumbPath(),Desc(),str_Tag(),faviconURL(){}
  virtual ~olbmdlg(){}

  LRESULT OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    SetIcon((HICON)c6beeres::R().GetImg(IDI_FAVEDIT),TRUE);//TO be changed

    m_Name.Attach(GetDlgItem(IDC_OLBM_NAME));
    m_Name.SetWindowText(str_Name.c_str());

    m_Url.Attach(GetDlgItem(IDC_OLBM_URL));
    m_Url.SetWindowText(str_URL.c_str());

    USES_CONVERSION;
    m_Tag.Attach(GetDlgItem(IDC_OLBM_TAG));
    m_Tag.SetWindowText(A2CW(str_Tag.c_str()));

    m_Desc.Attach(GetDlgItem(IDC_OLBM_DESC));
    //m_Desc.SetWindowText(Desc);

    m_Thumbnail.Attach(GetDlgItem(IDC_OLBM_THUMBNAIL));
    if (absThumbPath.empty()){
      ATL::CString waitthumbnail = _6beed_util::Get6BEEPath();
      waitthumbnail.Append(_T("waitthumbnail.jpg"));
      drawthumbnail(waitthumbnail);
    }else{
      drawthumbnail(absThumbPath.c_str());
    }

    HBITMAP hbmp=(HBITMAP)c6beeres::R().GetImg(IDB_ULTRABUTTONS,IMAGE_BITMAP,95*17,26);
    SetMyImageList(hbmp,m_imglst,MASKPURPLE,95,26,17,GetSysColor(COLOR_3DFACE));
    m_save.SubclassWindow(GetDlgItem(IDC_OLBM_SAVE));
    m_save.SetToolTipText(_T("Save to online bookmark."));
    m_save.SetImageList(m_imglst);
    m_save.SetImages(3,-1,2,-1);
    m_save.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

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

    DlgResize_Init(true,true,WS_THICKFRAME | WS_CLIPCHILDREN);

    return 0;
  }
  LRESULT OnCancel(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
    DeleteFile(absThumbPath.c_str());
    EndDialog(0);
    return 0;
  }

  LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    drawthumbnail();
    bHandled = false;
    return 0;
  }
  LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
    drawthumbnail();
    bHandled = false;
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

  LRESULT OnSave(WORD,WORD,HWND,BOOL&){
    static const int MAX_VARCHAR=255;
    ATL::CString _str_Name,_str_URL,_str_Tag,_str_Desc;
    m_Name.GetWindowText(_str_Name);
    if (_str_Name.IsEmpty()){
      MessageBox(_T("Please name the picture's title!"),_T("Reminder..."),MB_OK);
      m_Name .SetFocus();
      return 0;
    }
    m_Url.GetWindowText(_str_URL);
    if (_str_URL.GetLength()>MAX_VARCHAR){
      ::MessageBoxA(NULL,"URL is too long to save.(URL length couldn't greate than 255.)",
        "Information from UltraIE:",MB_OK|MB_ICONINFORMATION);
      return 0;
    }
    m_Tag.GetWindowText(_str_Tag);
    m_Desc.GetWindowText(_str_Desc);
    _str_Name.Replace(_T("'"),_T("\\'"));
    _str_Name.Replace(_T(":"),_T(" "));
    _str_Tag.Replace(_T("'"),_T("\\'"));
    _str_Desc.Replace(_T("'"),_T("\\'"));

    myolbm._str_Name =_str_Name;
    myolbm._str_URL=_str_URL;
    myolbm._str_Tag=_str_Tag;
    myolbm._str_Desc=_str_Desc;
    myolbm.absThumbPath=absThumbPath;
    myolbm._str_faviconURL= faviconURL;
    ::_beginthreadex(NULL,0,thread_SaveOLBM,(LPVOID)&myolbm,0,NULL);    
    EndDialog(0);
    return 0;
  }
};

UINT WINAPI thread_SaveOLBM(LPVOID p){
  try{
    timer_notifier tn(CPYDATA_SAVEOLBM_START,CPYDATA_UPLOADBUSY_STOP);
    OLBMThunk* polbm= (OLBMThunk*)p;

    UUTOOLBAR::UserPrx _user = MyICE::R();
    if(_user==0){
      ::MessageBoxA(NULL,"ERROR: NetWork failure. Please check your network setting.",
        "Information from UltraIE:",MB_OK | MB_ICONINFORMATION);
    }else{
      USES_CONVERSION;
      string name(CW2A(polbm->_str_Name,CP_UTF8));
      string url(CW2A(polbm->_str_URL,CP_UTF8));
      string desc(CW2A(polbm->_str_Desc,CP_UTF8));
      string tag(CW2A(polbm->_str_Tag,CP_UTF8));
      string email(_6beed_share::u_getEmail());
      string sid(_6beed_share::u_getSID());

      ::Ice::Long bookmarkid;
      if((bookmarkid=_user->SaveOnlineBookmark(url,name,desc,tag,email,sid)) ==0 ){
        ::MessageBoxA(NULL,"You have saved this page before.","Information from UltraIE:",MB_OK | MB_ICONINFORMATION);
        return 0;
      }

      // Upload thumb and get thumb info
      ATL::CString remotefile;
      string uname(email.substr(0,email.find_first_of("@")));
      if (!uname.empty()){
        remotefile.Format(_T("%s_%lu.jpg"),A2CW(uname.c_str()),time(0));
        _6beed_util::QuietUploadFile(polbm->absThumbPath.c_str(),
          remotefile.GetString(),
          (wstringmaker()<<(long)bookmarkid).c_str(),
          _6bees_const::UPLOAD_THUMB_MODE);
      }
      ATL::CString faviconURL(polbm->_str_faviconURL);
      string realicon;
      if (!faviconURL.IsEmpty() && _6bees_html::getValidFavicon(W2CA(faviconURL),realicon)>0){
        using _6bees_util::ultrapath;
        const wstring& upath = ultrapath::R().get(ultrapath::picdir);
        if(_6bees_util::MakeSureDir(upath.c_str())){
          ATL::CString iconlocalpath=upath.c_str();
          iconlocalpath.Append(_T("favicon.ico"));
          if(_6bees_net::DownloadURLToFile(realicon.c_str(),W2CA(iconlocalpath.GetString()))){
            remotefile.Format(_T("%s_%lu.ico"),A2CW(uname.c_str()),time(0));
            _6beed_util::QuietUploadFile(iconlocalpath.GetString(),
              remotefile.GetString(),
              (wstringmaker()<<(long)bookmarkid).c_str(),
              _6bees_const::UPLOAD_ICON_MODE);
          }
        }
      }
      wstring tmp1 = wstringmaker()
        << L"Title:" << polbm->_str_Name.GetString() << L"\n"
        << L"URL:" << polbm->_str_URL.GetString() << L"\n";
      wstring tmp2 =  (polbm->_str_Tag.IsEmpty()) ? tmp1 :
        (wstringmaker(tmp1) << L"Tag:" << polbm->_str_Tag.GetString() << L"\n");
      wstring tmp3 =  (polbm->_str_Desc.IsEmpty()) ? tmp2 :
        (wstringmaker(tmp2) << L"Description:" << polbm->_str_Desc.GetString());

      _6bees_window::NotifyClientMng(CPYDATA_SAVEOLBM_OK,
        _6bees_const::kUploader_WndTitle,
        (stringmaker()<<tmp3).c_str());
    }
  }catch (Ice::TimeoutException& e){
    MessageBoxA(NULL,e.what(),"Timeout Error",NULL);
  }catch (...){}
  return 0;
}



#endif