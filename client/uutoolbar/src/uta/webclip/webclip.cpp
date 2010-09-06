#include "webclip.h"
#include "resource.h"
#include "webclipdlg.h"
#include "6bees_util.h"
#include "6bees_html.h"
#include "6bees_window.h"
#include "6bees_const.h"
#include <6bees_lang.h>
#include <set>

extern HMODULE hinst;

namespace nsplugin{

  using namespace std;

  //////////////////////////////////////////////////////////////////////////
  enum{SAVEWHOLEPAGE=0,SAVESELECTED,SAVEWEBELEHTML};
  static const wchar_t* WebClipWndTitle[3] = { kentire,kcapselected,kwebele};

  LRESULT SaveEntirePageToHTML();
  UINT WINAPI thread_savewebhtml(LPVOID);
  UINT WINAPI thread_ripoff_wholepage(LPVOID);
  void SaveWebElementHTML(_6bees_html::c6beewb2ptr* _pwb,
    IHTMLElement* elementHit);
  void SaveWebHTML(const wchar_t* content,
    const wchar_t* wURL,
    const wchar_t* wstrPath,
    const wchar_t* wstrTitle);

  struct webclipstruct2{
    DWORD tid;
    UINT Type;
    ATL::CString HTMLPageName;
    ATL::CString CacheHTMLFilename;
    ATL::CString FolderName;
    ATL::CString absThumbPath;
    void set(DWORD _tid,
      UINT a,
      const ATL::CString& b,
      const ATL::CString& c,
      const ATL::CString& d,
      const ATL::CString& ThumbPath_)
    {
      tid = _tid;
      Type = a;
      HTMLPageName = b;
      CacheHTMLFilename = c;
      FolderName = d;
      absThumbPath = ThumbPath_;
    }
  } _webclipstruct2;

  struct webclipstruct{
    DWORD tid;
    wstring webpage;
    wstring wstrURL;
    wstring hostname;
    ATL::CString mstr;
    void set(DWORD _tid,
      const wstring& a,
      const wstring& b,
      const wstring& c,
      const ATL::CString& d)
    {
      tid = _tid;
      webpage = a;
      wstrURL = b;
      hostname= c;
      mstr = d;
    }
  } _webclipstruct;
  //////////////////////////////////////////////////////////////////////////

  bool webclip::OnNavigateComplete2(int paneid,const wchar_t* url){
    return true;
  }

  int webclip::InsertandGetMenuImgIndex(){
    static set<DWORD> threadids;
    static int first_img_index = 0;
    DWORD tid = GetCurrentThreadId();
    if (threadids.find(tid)==threadids.end()){
      C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
      HBITMAP hbitmap=(HBITMAP)LoadImage(hinst,
        MAKEINTRESOURCE(IDB_WEBCLIPMENU),
        IMAGE_BITMAP,3*16,16,LR_DEFAULTSIZE|LR_SHARED);
      threadids.insert(tid);
      first_img_index = pSB->AddToImgList(hbitmap,RGB(193,193,193));
    }
    return first_img_index;
  }

  void webclip::AssociateImage(int wID,int& _img){
    int first_img_index = InsertandGetMenuImgIndex();
    switch(wID){
      case ID_WEBCLIP_SELECTED:
        _img = first_img_index;
        break;
      case ID_WEBCLIP_WEBELEMENT:
        _img = first_img_index + 1;
        break;
      case ID_WEBCLIP_ENTIREPAGE:
        _img = first_img_index + 2;
        break;
    }
  }

  bool webclip::OnClickMenuItem(int id){
    switch(id){
      case ID_WEBCLIP_ENTIREPAGE:
        {
          SaveEntirePageToHTML();
        }break;
      case ID_WEBCLIP_WEBELEMENT:
        {
          C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
          HCURSOR mycursor = (HCURSOR)c6beeres::R().GetImg(
            IDC_CURSOR_SEARCH_WINDOW,IMAGE_CURSOR,32,32);
          if(mycursor==NULL){
            return false;
          }
          InstallMouseHook(pSB->m_hWnd,mycursor,CAPWEBELEHTML);
        }break;
      case ID_WEBCLIP_SELECTED:
        {
          C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
          _6bees_html::c6beewb2ptr* _pwb = pSB->GetPWB2();
          IHTMLSelectionObjectPtr pSel(0);
          IHTMLTxtRangePtr pTxtRange(0);
          IHTMLDocument2Ptr pHtmlDoc2(_pwb->GetHTMLDoc2());
          if (pHtmlDoc2.GetInterfacePtr()==NULL){
            MessageBox(NULL,knothtml,kgentitle,NULL);
            return 0;
          }
          if (FAILED(pHtmlDoc2->get_selection(&pSel))){return 0;}
          if (FAILED(pSel->createRange((IDispatch **)&pTxtRange))) return 0;
          pTxtRange->select();
          CComBSTR bstrTxt;
          pTxtRange->get_htmlText(&bstrTxt);//get the selected contents
          ATL::CString csText(bstrTxt);
          if (csText.IsEmpty()){
            MessageBox(NULL,kemptycontents,kgentitle,NULL);
            return 0;
          }
          wstring strURL = _pwb->GetURL();
          wstring strTitle = _pwb->GetTitle();
          CComBSTR bstrHostName,bstrMimetype;
          pHtmlDoc2->get_mimeType(&bstrMimetype);
          if(wcscmp(_T("HTML Document"),bstrMimetype)!=0){
            MessageBox(NULL,knothtml,kgentitle,NULL);
            return 0;
          }

          CComPtr<IHTMLLocation> pLocation;
          pHtmlDoc2->get_location(&pLocation);
          pLocation->get_hostname(&bstrHostName);
          ATL::CString HostName;
          HostName.Format(_T("http://%s"),bstrHostName);

          ATL::CString webcontent;
          webcontent.Format(_6bees_const::kHtml_Template,strURL.c_str(),strTitle.c_str(),csText);
          SaveWebHTML(webcontent,strURL.c_str(),HostName,strTitle.c_str());
        }break;
    }
    return true;
  }

  webclip::webclip(){
    enabled_ = true;
    len_ = 28;
    caption_= kcap_wc;
    tip_ = ktip_wc;
    menuwID_ = 1321;
  }

  webclip::~webclip(){
    for (vector<HICON>::const_iterator i=icons_.begin();i!=icons_.end();++i){
      DestroyIcon(*i);
    }
  }

  bool webclip::ProcessBasedInit(){
    return true;
  }

  bool webclip::ProcessBasedTerm(){
    return true;
  }

  bool webclip::ThreadBasedInit(){
    return true;
  }

  bool webclip::ThreadBasedTerm(){
    return true;
  }

  bool webclip::init(){
    return true;
  }

  bool webclip::StartPlugin(){
    return true;
  }

  bool webclip::StopPlugin(){
    return true;
  }

  bool webclip::OnLButtonDown(int paneid,int xpos,int ypos){
    menu_ = ::LoadMenuW(hinst,MAKEINTRESOURCE(IDR_WEBCLIP));
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

  bool webclip::OnRButtonDown(int){
    return true;
  }

  bool webclip::disenable_plugin(){
    return true;
  }

  void webclip::OnHookMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    static RECT caprect={};
    static HWND g_hwndFoundWindow=NULL;
    static IHTMLElement* elementHit=NULL;
    switch (uMsg){
      case WM_HKMOUSEMOVE_WEBELEHTML:
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
      case WM_HOOKMOUSEUNINSTALL:
        {
          C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
          _6bees_html::c6beewb2ptr* _pwb = pSB->GetPWB2();
          _6bees_window::RefreshWindow(GetDesktopWindow());
          _6bees_window::RefreshWindow(_pwb->GetCurrentTab());
          UnInstallMouseHook();

          if (lParam==LP_WELLDONE){
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
              case CAPWEBELEHTML:
                {
                  SaveWebElementHTML(_pwb,elementHit);
                }break;
            }
          }else if(lParam==LP_GIVEUP){}
        }
        break;
    }
  }

  void SaveWebElementHTML(_6bees_html::c6beewb2ptr* _pwb,IHTMLElement* elementHit){
    CComBSTR tmp;
    if(elementHit && SUCCEEDED(elementHit->get_innerHTML(&tmp))){
      ATL::CString innerhtml = tmp;
      if (innerhtml.IsEmpty()){
        MessageBox(NULL,kemptycontents,kgentitle,NULL);
        return;
      }
      IHTMLDocument2Ptr pHtmlDoc2(_pwb->GetHTMLDoc2());
      CComPtr<IHTMLLocation> pLocation;
      pHtmlDoc2->get_location(&pLocation);
      ATL::CString cur_url(_pwb->GetURL().c_str());
      if (cur_url.Left(7).CompareNoCase(_T("http://"))!=0){
        MessageBox(NULL,knothtml,kgentitle,NULL);
        return;
      }
      CComBSTR bstrHostName;
      pLocation->get_hostname(&bstrHostName);
      ATL::CString csStr;
      csStr.Format(_T("http://%s"),bstrHostName.m_str);

      wstring strURL = _pwb->GetURL();
      wstring strTitle = _pwb->GetTitle();
      ATL::CString webcontent;
      webcontent.Format(_6bees_const::kHtml_Template,strURL.c_str(),strTitle.c_str(),innerhtml);
      SaveWebHTML(webcontent,cur_url,csStr,strTitle.c_str());
    }
  }

  // subroutine for implement saving some html code to file and show etc
  void SaveWebHTML(const wchar_t* content,
    const wchar_t* wURL,
    const wchar_t* wstrPath,
    const wchar_t* wstrTitle)
  {
    static wstring webpage,wstrURL,hostname;
    static ATL::CString MagicStr,FolderName,Title,AbsPicFilename_thumb;
    webpage = content;
    wstrURL  = wURL;
    hostname= wstrPath;
    Title = wstrTitle;
    MagicStr= GenUniqStr().c_str();
    DWORD tid = GetCurrentThreadId();

    // for debug, temporarily comment
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::webcontentsdir);
    ATL::CString CacheHTMLFilename = upath.c_str() + MagicStr+_T(".html");
    FolderName = upath.c_str()+MagicStr+_T("\\");
    if(!_6bees_util::MakeSureDir(FolderName.GetString())){
      return;
    }

    AbsPicFilename_thumb.Format(_T("%s%s_thumb%s"),FolderName,MagicStr,L".jpg");
    _webclipstruct2.set(tid,SAVESELECTED,Title,CacheHTMLFilename,FolderName,AbsPicFilename_thumb);
    ::_beginthreadex(NULL,0,thread_savewebhtml,(LPVOID)&_webclipstruct2,0,NULL);

    Sleep(500);
    _webclipstruct.set(tid,webpage,wstrURL,hostname,MagicStr);
    ::_beginthreadex(NULL,0,thread_ripoff_wholepage,(LPVOID)&_webclipstruct,0,NULL);
  }

  //Helper function
  UINT WINAPI thread_savewebhtml(LPVOID p){
    webclipstruct2 * pst = static_cast<webclipstruct2*>(p);
    UINT Type = pst->Type;
    if(Type>SAVEWEBELEHTML) return 0;
    ATL::CString HTMLPageName = pst->HTMLPageName;
    ATL::CString CacheHTMLFilename = pst->CacheHTMLFilename;
    ATL::CString FolderName = pst->FolderName;
    ATL::CString absThumbPath = pst->absThumbPath;
    webclipdlg cpwnd;
    cpwnd.WndTitle = WebClipWndTitle[Type];
    cpwnd.FolderName = FolderName;
    cpwnd.absHTMLPath=CacheHTMLFilename;
    cpwnd.str_Name = HTMLPageName;
    cpwnd.absThumbPath = absThumbPath;
    webclipdlg::tid2webclipdlgptrmap[pst->tid] = &cpwnd;
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::resdll);
    cpwnd.DoModal(upath.c_str());
    return 0;
  }

  LRESULT SaveEntirePageToHTML(){
    C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
    _6bees_html::c6beewb2ptr* _pwb = pSB->GetPWB2();

    IHTMLDocument2Ptr pHtmlDoc2(_pwb->GetHTMLDoc2());
    IHTMLElementPtr body,lpCOMHTML;
    if (SUCCEEDED(pHtmlDoc2->get_body(&body))){
      if (SUCCEEDED(body->get_parentElement(&lpCOMHTML))){
        BSTR lpHTML;
        if (SUCCEEDED(lpCOMHTML->get_outerHTML(&lpHTML))){
          USES_CONVERSION;
          wstring webpage(OLE2T(lpHTML));

          BSTR bstrHostName,bstrMimetype;// todo - get charset to tell if we need to convert to UTF8
          CComPtr<IHTMLLocation> pLocation;
          pHtmlDoc2->get_location(&pLocation);
          pHtmlDoc2->get_mimeType(&bstrMimetype);

          ATL::CString strURL(_pwb->GetURL().c_str());
          ATL::CString strTitle = _pwb->GetTitle().c_str();
          ATL::CString strMimetype = OLE2T(bstrMimetype);

          pLocation->get_hostname(&bstrHostName);
          ATL::CString csStr = OLE2T(bstrHostName);
          csStr = _T("http://") + csStr;
          const wchar_t *strPath = csStr.GetString();
          wstring  wstrURL = strURL.GetString();
          wstring hostname = strPath;
          ATL::CString MagicStr=GenUniqStr().c_str();

          using _6bees_util::ultrapath;
          const wstring& upath = ultrapath::R().get(ultrapath::webcontentsdir);
          ATL::CString CacheHTMLFilename = upath.c_str() + MagicStr+_T(".html");
          ATL::CString FolderName = upath.c_str()+MagicStr+_T("\\");
          ATL::CString AbsPicFilename_thumb;
          AbsPicFilename_thumb.Format(_T("%s%s_thumb%s"),FolderName,MagicStr,L".jpg");
          if(!_6bees_util::MakeSureDir(FolderName.GetString())){
            return 0;
          }

          /*
          _6bees_html::EntirePageCapture(
            pSB->m_hWnd,
            pSB->GetPWB2(),
            NULL,
            AbsPicFilename_thumb,
            true,false,false,0);
            */

          DWORD tid1 = ::GetCurrentThreadId();
          _webclipstruct2.set(tid1,SAVEWHOLEPAGE,strTitle,CacheHTMLFilename,FolderName,AbsPicFilename_thumb);
          ::_beginthreadex(NULL,0,thread_savewebhtml,(LPVOID)&_webclipstruct2,0,NULL);
          Sleep(100);
          _webclipstruct.set(tid1,webpage,wstrURL,hostname,MagicStr);
          ::_beginthreadex(NULL,0,thread_ripoff_wholepage,(LPVOID)&_webclipstruct,0,NULL);
        }
        SysFreeString(lpHTML);
      }
    }
    return 1;
  }

  UINT WINAPI thread_ripoff_wholepage(LPVOID p){
    webclipstruct* pst = (webclipstruct*)p;
    DWORD tid = pst->tid;
    webclipdlg* _wcdlg = webclipdlg::tid2webclipdlgptrmap[tid];
    if (_wcdlg!=NULL){
      string webpage = stringmaker() << pst->webpage;
      string wstrURL = stringmaker() << pst->wstrURL;
      string hostname = stringmaker() << pst->hostname;
      string mstr = stringmaker() << pst->mstr.GetString();
      _6bees_html::process_wholepage(_wcdlg->progress,webpage,wstrURL,hostname,mstr);
    }
    return 0;
  }

};