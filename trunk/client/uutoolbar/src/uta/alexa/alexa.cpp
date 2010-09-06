#include "alexa.hpp"
#include <atlbase.h>//CRegKey
#include <atlstr.h>
#include "../../dll/6beecommond/6beecommond.h"
#include <set>
#include "util.h"
#include "6bees_util.h"
#include "6bees_window.h"
#include "resource.h"

using std::set;

extern HMODULE hinst;

#ifdef __CHINESE__
#define kcap_alexa L"Alexa全球排名"
#define ktip_alexa L"Alexa全球排名"
#else
#define kcap_alexa L"Alexa"
#define ktip_alexa L"Alexa Global Rank"
#endif

namespace nsplugin{
  std::map<std::wstring,int> alexa::url2alexa;
  alexainfo alexa::alexa_info;

  UINT WINAPI alexa::thread_getalexa(LPVOID p){
    alexainfo* pas = static_cast<alexainfo*>(p);
    int alexarank = 0;
    std::wstring hostname(pas->hostname.c_str());
    if(url2alexa.find(hostname)==url2alexa.end()){
      alexarank = GetAlexaRank(pas->hostname.c_str());
      url2alexa[hostname] = alexarank;
    }else{
      alexarank = url2alexa[hostname];
    }

    ATL::CString stralexa;
    stralexa.Format(L"%d",alexarank);
    pas->alexaptr->tid2panetext_[pas->tid] = stralexa;
    if(IsWindow(pas->psb->m_hWnd))
      pas->psb->SetText(pas->paneid,stralexa);
    return 0;
  }

  bool alexa::OnNavigateComplete2(int paneid,const wchar_t* url){
    C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
    if (pSB==NULL){
      return false;
    }
    ATL::CString myurl = url;
    if(myurl.Find(_T("http://"))!=0){
      return false;
    }
    int cut=myurl.Find(_T("/"),8);
    if (cut<0){return false;}
    myurl.Truncate(cut);

    alexa_info.paneid = paneid;
    alexa_info.hostname = myurl;
    alexa_info.psb = pSB;
    alexa_info.tid = GetCurrentThreadId();
    alexa_info.alexaptr = this;
    ::_beginthreadex(NULL,0,thread_getalexa,
      (LPVOID)&alexa::alexa_info,0,NULL);
    return true;
  }

  void alexa::OpenAlexaUrl(const wchar_t* alexalink){
    C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
    wchar_t* hostname=pSB->GetHostname();
    std::wstring url(alexalink);
    url.append(hostname);
    pSB->opennewtab(url.c_str(),true);
    delete [] hostname;
  }

  bool alexa::OnClickMenuItem(int id){
    switch(id){
      case ID_DLLALEXA_SITEOVERVIEW:
        OpenAlexaUrl(L"http://www.alexa.com/data/details/main/");
        break;
      case ID_DLLALEXA_TRAFFICDETAILS:
        OpenAlexaUrl(L"http://www.alexa.com/data/details/traffic_details/");
        break;
      case ID_DLLALEXA_RELATEDLINKS:
        OpenAlexaUrl(L"http://www.alexa.com/data/details/related_links/");
        break;
    }
    return true;
  }

  int alexa::InsertandGetMenuImgIndex(){
    static set<DWORD> threadids;
    static int first_img_index = 0;
    DWORD tid = GetCurrentThreadId();
    if (threadids.find(tid)==threadids.end()){
      C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
      HBITMAP hbitmap=(HBITMAP)LoadImage(hinst,
        MAKEINTRESOURCE(IDB_ALEXAMENUICON),
        IMAGE_BITMAP,3*16,16,LR_DEFAULTSIZE|LR_SHARED);
      threadids.insert(tid);
      first_img_index = pSB->AddToImgList(hbitmap,RGB(193,193,193));
    }
    return first_img_index;
  }

  void alexa::AssociateImage(int wID,int& _img){    
    int first_img_index = InsertandGetMenuImgIndex();
    switch(wID){
case ID_DLLALEXA_TRAFFICDETAILS:
  _img = first_img_index;
  break;
case ID_DLLALEXA_SITEOVERVIEW:
  _img = first_img_index + 1;
  break;
case ID_DLLALEXA_RELATEDLINKS:
  _img = first_img_index + 2;
  break;
    }
  }

  /*
  bool alexa::PluginIsEnabled(HMODULE hm_){
    TCHAR pszLoader[MAX_PATH]={};
    if(GetModuleFileName(hm_,pszLoader,MAX_PATH)){
      CRegKey crk;
      long rk=crk.Open(HKEY_LOCAL_MACHINE, kplugindisabled, KEY_READ|KEY_WRITE);
      if (rk==ERROR_SUCCESS){
        DWORD i=0;
        if (crk.QueryDWORDValue(PathFindFileName(pszLoader),i)==ERROR_SUCCESS){
          return false;
        }
      }
    }
    return true;
  }

  void alexa::Write2Reg(HMODULE hm_){
    TCHAR pszLoader[MAX_PATH]={};
    GetModuleFileName(hm_,pszLoader,MAX_PATH);
    CRegKey crk;
    long rk=crk.Open(HKEY_LOCAL_MACHINE, kplugindisabled, KEY_READ|KEY_WRITE);
    if (rk==ERROR_SUCCESS){
      if(enabled_){
        crk.DeleteValue(PathFindFileName(pszLoader));
      }else{
        crk.SetDWORDValue(PathFindFileName(pszLoader),1);
      }
    }
  }*/

  alexa::alexa(){
    len_ = 78;
    caption_=kcap_alexa;
    tip_ = ktip_alexa;
    menuwID_ = 1316;
    enabled_ = true;
  }

  alexa::~alexa(){
    //Write2Reg(hinst);
    for (vector<HICON>::const_iterator i=icons_.begin();i!=icons_.end();++i){
      DestroyIcon(*i);
    }
  }

  bool alexa::ProcessBasedInit(){
    return true;
  }

  bool alexa::ProcessBasedTerm(){
    return true;
  }

  bool alexa::ThreadBasedInit(){
    return true;
  }

  bool alexa::ThreadBasedTerm(){
    return true;
  }

  bool alexa::init(){
    return true;
  }

  bool alexa::StartPlugin(){
    return true;
  }

  bool alexa::StopPlugin(){
    return true;
  }

  bool alexa::OnLButtonDown(int paneid,int xpos,int ypos){
    menu_ = ::LoadMenuW(hinst,MAKEINTRESOURCE(IDR_ALEXAMENU));
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

  bool alexa::OnRButtonDown(int){
    return true;
  }

  bool alexa::disenable_plugin(){
    return true;
  }

};
