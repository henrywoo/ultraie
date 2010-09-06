#include "imgcap.h"
#include "imgcapwnd.h"
#include "resource.h"
#include "6bees_html.h"
#include "6bees_const.h"
#include "6bees_window.h"
#include <set>
#include "../../dll/mousehook/mousehook.h"
#include "ImageRender.h"
#include "cppsqlite3.h"
#include "screencapdlg.h"
#include "util.h"

extern HMODULE hinst;

#ifdef __CHINESE__
#define ktip_ic L"Í¼Ïñ²¶×½"
#define kcap_ic L"Í¼Ïñ²¶×½"
#else
#define ktip_ic L"Image Capture"
#define kcap_ic L"Image Capture"
#endif

namespace nsplugin{
  using namespace std;

  void openimagecapwnd(const ATL::CString& Name,
    const ATL::CString& absPath,
    const ATL::CString& FolderName,
    const ATL::CString& Desc,
    const ATL::CString& wndTitle,
    const wchar_t* absthumbPath)
  {
    CImagecapwnd cpwnd;
    cpwnd.str_Name = Name;
    cpwnd.FolderName = FolderName;
    cpwnd.absPath = absPath;
    cpwnd.absThumbPath = absthumbPath;
    cpwnd.WndTitle = wndTitle;
    cpwnd.Tag = L"";
    cpwnd.Desc = Desc;
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::resdll);
    cpwnd.DoModal(upath.c_str());
  }

  bool imgcapture::OnNavigateComplete2(int paneid,const wchar_t* url){
    return true;
  }

  int imgcapture::InsertandGetMenuImgIndex(){
    static set<DWORD> threadids;
    static int first_img_index = 0;
    DWORD tid = GetCurrentThreadId();
    if (threadids.find(tid)==threadids.end()){
      C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
      HBITMAP hbitmap=(HBITMAP)LoadImage(hinst,
        MAKEINTRESOURCE(IDB_IMGCAP_MENU),
        IMAGE_BITMAP,5*16,16,LR_DEFAULTSIZE|LR_SHARED);
      threadids.insert(tid);
      first_img_index = pSB->AddToImgList(hbitmap,RGB(193,193,193));
    }
    return first_img_index;
  }

  void imgcapture::AssociateImage(int wID,int& _img){
    int first_img_index = InsertandGetMenuImgIndex();
    switch(wID){
      case ID_IMGCAP_ENTIREPAGE:
        _img = first_img_index;
        break;
      case ID_IMGCAP_WEBELE:
        _img = first_img_index + 1;
        break;
      case ID_IMGCAP_SCREEN:
        _img = first_img_index + 2;
        break;
      case ID_IMGCAP_SETTING:
        _img = first_img_index + 4;
        break;
    }
  }

  bool imgcapture::OnClickMenuItem(int id){
    switch(id){
      case ID_IMGCAP_ENTIREPAGE:
        {
        C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
        wchar_t* _wtitle = pSB->GetWebTitle();
        ATL::CString title(_wtitle);
        delete [] _wtitle;

        using _6bees_util::ultrapath;
        const wstring& upath = ultrapath::R().get(ultrapath::picdir);
        if(!_6bees_util::MakeSureDir(upath.c_str())){
          return false;
        }
        ATL::CString FolderName(upath.c_str());
        ATL::CString imgpath = FolderName+ GenUniqStr().c_str() +L".png";
        ATL::CString imgpath_thumb= FolderName+GenUniqStr().c_str()+_T("_thumb.png");
        bool border=false,shadow=false;
        COLORREF color=0;
        GetImgCapParam(border,shadow,color);
        _6bees_html::EntirePageCapture(
          pSB->m_hWnd,
          pSB->GetPWB2(),
          imgpath,
          imgpath_thumb,
          true,border,shadow,color);

        IHTMLElement* pelement = pSB->GetPWB2()->GetElementByTagName("body");
        CComBSTR bt;
        pelement->get_innerText(&bt);
        ATL::CString bodytext = bt;

        openimagecapwnd(title,
          imgpath,
          FolderName,
          bodytext.Left(200),
          _6bees_const::kCapImg_EntirePage_WndTitle,
          imgpath_thumb);
      }break;
      case ID_IMGCAP_WEBELE:
        {
          C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
          HCURSOR mycursor = (HCURSOR)c6beeres::R().GetImg(
            IDC_CURSOR_SEARCH_WINDOW,IMAGE_CURSOR,32,32);
          if(mycursor==NULL){
            return false;
          }
          InstallMouseHook(pSB->m_hWnd,mycursor,CAPWEBELEIMG);
          HPEN mypen = CreatePen (PS_SOLID,1,SELECT_RECTCOLOR);
        }break;
      case ID_IMGCAP_SCREEN:
        {
          using _6bees_util::ultrapath;
          const wstring& upath = ultrapath::R().get(ultrapath::resdll);
          screencapdlg _screencapdlg;
          _screencapdlg.DoModal(upath.c_str());
        }
        break;
      case ID_IMGCAP_SETTING:
        {
          C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
          pSB->GetPWB2()->NavToURL(L"ultra://imgcapsetting");
        }break;
      default:
        break;
    }
    return true;
  }

  void imgcapture::OnHookMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    static RECT caprect={};
    static HWND g_hfound=NULL;
    static IHTMLElement* elementHit=NULL;
    switch (uMsg){
    case WM_HKMOUSEMOVE_WEBELEIMG:
      {
        C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
        _6bees_html::HighlightWebElement(
          elementHit,
          pSB->GetPWB2(),
          caprect,
          uMsg,
          wParam,
          lParam,
          bHandled);
      }
      break;
    /*case WM_HOOKMOUSEMOVE_4:
      {
        C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();

        POINT mouse_point={(int)wParam,(int)lParam};
        HWND hfound = WindowFromPoint (mouse_point);

        if (_6bees_window::CheckWindowValidity(NULL,hfound,g_hfound)){
          if (g_hfound){
            _6bees_window::RefreshWindow(g_hfound);
          }
          g_hfound = hfound;
          HFONT EDITTXTFONT= ::CreateFont(16,0,0,0,FW_THIN,false,FALSE,FALSE,DEFAULT_CHARSET,
            OUT_STRING_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));
          HPEN g_hRectanglePen = CreatePen (PS_SOLID,2,SELECT_RECTCOLOR);
          _6bees_window::HighlightFoundWindow(pSB->m_hWnd, g_hfound,g_hRectanglePen,EDITTXTFONT);
        }
      }
      break;*/
    case WM_HOOKMOUSEUNINSTALL:
      {
        C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
        _6bees_html::c6beewb2ptr* _pwb = pSB->GetPWB2();
        _6bees_window::RefreshWindow(GetDesktopWindow());
        _6bees_window::RefreshWindow(_pwb->GetCurrentTab());
        if(UnInstallMouseHook()){
          wchar_t* _wtitle = pSB->GetWebTitle();
          ATL::CString title(_wtitle);
          delete [] _wtitle;

          using _6bees_util::ultrapath;
          const wstring& upath = ultrapath::R().get(ultrapath::picdir);
          if(!_6bees_util::MakeSureDir(upath.c_str())){
            return;
          }
          ATL::CString FolderName(upath.c_str());
          ATL::CString imgpath = FolderName+GenUniqStr().c_str()+L".png";
          ATL::CString imgpath_thumb= FolderName+GenUniqStr().c_str()+_T("_thumb.png");

          switch (wParam){
            case CAPWEBELEIMG:
              {
                if(lParam==LP_WELLDONE){
                  bool border=false,shadow=false;
                  COLORREF bcolor=0;
                  GetImgCapParam(border,shadow,bcolor);
                  _6bees_html::GeneratePicFromWebElement(_pwb,caprect,imgpath,imgpath_thumb,border,shadow,bcolor);
                  openimagecapwnd(title,
                    imgpath,
                    FolderName,
                    L"",
                    _6bees_const::kCapImg_WebEleme_WndTitle,
                    imgpath_thumb);
                }
                //else (lParam==LP_GIVEUP)
              }
              break;
          }
        } 
      }
      break;
    }
  }

  imgcapture::imgcapture(){
    enabled_ = true;
    len_ = 28;
    caption_= kcap_ic;
    tip_ = ktip_ic;
    menuwID_ = 1320;
  }

  imgcapture::~imgcapture(){
    for (vector<HICON>::const_iterator i=icons_.begin();i!=icons_.end();++i){
      DestroyIcon(*i);
    }
  }

  bool imgcapture::ProcessBasedInit(){
    return true;
  }

  bool imgcapture::ProcessBasedTerm(){
    return true;
  }

  bool imgcapture::ThreadBasedInit(){
    return true;
  }

  bool imgcapture::ThreadBasedTerm(){
    return true;
  }

  bool imgcapture::init(){
    return true;
  }
  
  bool imgcapture::StartPlugin(){
    return true;
  }
  
  bool imgcapture::StopPlugin(){
    return true;
  }
  
  bool imgcapture::OnLButtonDown(int paneid,int xpos,int ypos){
    menu_ = ::LoadMenuW(hinst,MAKEINTRESOURCE(IDR_IMGCAPMENU));
    HMENU menuPopup=::GetSubMenu(menu_,0);
    if (menuPopup){
      C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
      TrackPopupMenu(menuPopup,
        TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_LEFTBUTTON|TPM_HORPOSANIMATION,
        xpos,ypos,0,pSB->m_hWnd,NULL);
      DestroyMenu(menuPopup);
      DestroyMenu(menu_);
    }
    return true;
  }
  
  bool imgcapture::OnRButtonDown(int){
    return true;
  }
  
  bool imgcapture::disenable_plugin(){
    return true;
  }

#ifdef __CHINESE__
#define _imgshadow L"Í¼ÏñÒõÓ°Ð§¹û"
#define _imgborder L"Í¼Ïñ±ßÏßÐ§¹û"
#define _imgbordercolor L"Í¼Ïñ±ßÏßÑÕÉ«"
#else
#define _imgshadow "Image has shadow"
#define _imgborder "Image has border"
#define _imgbordercolor "Image border color"
#endif
  void ImgCaptureSetting(string& dbcontent,
    wstring& fname,
    char*& flag)
  {
    flag = "<!-- ImgCapSetting -->";
    bool imghasborder=false,imghasshadow=false;
    COLORREF color = RGB(0,0,0);
    GetImgCapParam(imghasborder,imghasshadow,color);
    string checkborder = imghasborder ? "checked" : "unchecked";
    string checkshadow = imghasshadow ? "checked" : "unchecked";
    string hexcolor = _6bees_gdi::DectoHex(color);
    dbcontent = stringmaker() 
      << "<tr><td>\n"
      << _imgshadow
      << ": <td align=right><INPUT TYPE=checkbox " 
      << checkshadow 
      << " ID=chk1 name=chkbox1><tr><td>"
      << _imgborder <<": <td align=right><INPUT TYPE=checkbox "
      << checkborder << " ID=chk2 name=chkbox2><tr><td>"
      <<_imgbordercolor<<":\n<td align=right>\
                <input class='o5582but' type='button' value='...' onclick=\"showColorGrid2('bcolor','box');\">\n\
                <input type='text' value='"
      << hexcolor << "' name='bordercolor' ID='bcolor'><span id='box' style='width:12px;height:10px;background-color="
      << hexcolor << "'></span>";

    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      fname = wstringmaker() << upath << L"imgcapture\\imgcapsetting.html";
    }
  }

  void imgcapture::StartUltraProtocol(const wchar_t* _url,
    std::string& dbcontent,
    std::wstring& fname,
    char*& flag,BYTE* pbuffer,ULONG buffersize)
  {
    ATL::CStringW url(_url);
    url.MakeLower();

    static const wchar_t historyurl[] = L"ultra://imgcapsetting";
    static const wchar_t actionurl[] = L"ultra://action";
    static int len_historyurl = sizeof(historyurl) / sizeof(historyurl[0]) - 1;
    static int len_actionurl  = sizeof(actionurl) / sizeof(actionurl[0]) - 1;

    if(url.Left(len_historyurl) == historyurl){
      ImgCaptureSetting(dbcontent,fname,flag);
    }else if (url.Left(len_actionurl) == actionurl){
      string cmd;
      wstring redirectpage;
      if(_6bees_html::ParseAction(L"imgcapsetting",fname,cmd,redirectpage)){
        CppSQLite3DB& DBRef=CppSQLite3DB::instance(GetDBPath().c_str());
        DBRef.execDML(cmd.c_str());
        static wchar_t* redirectflag = L"reimgcapsetting";
        if (redirectpage == redirectflag){
          ImgCaptureSetting(dbcontent,fname,flag);
        }else{
          //fname = redirectpage;
        }
      }
    }
  }

};