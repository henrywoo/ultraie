#include "stdafx.h"
#include "uufilecom.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/mousehook/mousehook.h"//HOOK IE's CreateProcess
#include "6bees_util.h"
#include "6bees_html.h"
#include "cppsqlite3.h"
#include "6bees_str.h"
#include "uulogging.h"
#include "6bees_window.h"
#include "6bees_net.h"
//#include "pluginmng.h"
//#include "globalvar.h"

#pragma comment(lib,"6beecommond.lib")

using namespace _6bees_util;
using namespace _6beed_util;
using namespace _6bees_str;

struct __declspec(uuid("79eac9e2-baf9-11ce-8c82-00aa004ba90b")) CLSID_HTTP_APP_HANDLER;
#define UUIDOFHTTPAPP __uuidof(CLSID_HTTP_APP_HANDLER)

map<DWORD,_6bees_html::c6beewb2ptr*> CToolBarCOM::tid_wbptr;

static const UINT IE7_TITLE_MAX_LENGTH=95;
static const UINT ksqlcmdlen=1024;

BOOL haszoom=TRUE;
BOOL hasanimate=TRUE;
bool ADSFILTER= false;

//1. Static variables initialization
bool CToolBarCOM::iswebie=false;
set<DWORD> CToolBarCOM::threadids;

DWORD CToolBarCOM::pid = GetCurrentProcessId();

bool CToolBarCOM::SystemBasedInit(){
  return true;
}

bool CToolBarCOM::SystemBasedTerm(){
  //Currently empty...
  return true;
}

bool CToolBarCOM::ProcessBasedInit(){
  if(!threadids.empty()){
    return false;
  }
  /// 0. Is it IE and version?
  TCHAR pszLoader[MAX_PATH];
  ::GetModuleFileName(NULL, pszLoader, MAX_PATH);
  wstring filename= PathFindFileNameW(pszLoader);
  LOWERWSTR(filename);
  if(iswebie = (filename==L"iexplore.exe")){
    _6beed_share::u_setieversion(_6beed_util::GetVersionBlk(pszLoader).wMajor);
  }

  /// 1. Logging
  UUDEBUG((LOG_ERROR,"Starting UUTOOLBAR"));
  return true;
}

bool CToolBarCOM::ProcessBasedTerm(){
  return true;
}

bool CToolBarCOM::ThreadBasedInit(IUnknown* pUnkSite){
  /// 0. some optional initialization work
  tid=GetCurrentThreadId();
  threadids.insert(tid);

  //OutputDebugStringA("Call Constructor!\n");
  if(FAILED(pUnkSite->QueryInterface(IID_IInputObjectSite, (LPVOID*)&sp_inputobjsite_))){
    return false;//E_FAIL;
  }
  //RegisterNamespace();
  CComQIPtr<IOleCommandTarget,&IID_IOleCommandTarget> spCmdTarget(pUnkSite);
  if (spCmdTarget.p!=NULL){
    CComQIPtr<IServiceProvider,&IID_IServiceProvider> spSP(spCmdTarget);
    if (spSP.p!=NULL){
      if(SUCCEEDED(spSP->QueryService(SID_SWebBrowserApp,IID_IWebBrowser2,(LPVOID*)&spwb_))){
        tid_wbptr[GetCurrentThreadId()] = &spwb_;
        m_ReflectionWnd.pw = &spwb_;
        m_ReflectionWnd.m_6BeeStatusWnd.setPWebBrowser(&spwb_);
        CToolBarWnd& tbwnd = m_ReflectionWnd.GetToolBar();
        //if (::IsWindow(tbwnd.m_hWnd)){
          tbwnd.SetBrowser(&spwb_);
          tbwnd.GetToolBarCombo().SetBand(this);
        //}
      }
    }
  }

  /// Connection point
  CComQIPtr<IConnectionPointContainer,&IID_IConnectionPointContainer> spCPC(spwb_.GetPtr());
  if(spCPC.p){
    spCPC->FindConnectionPoint(DIID_DWebBrowserEvents2, &sp_connectionpoint_);
    if(sp_connectionpoint_.p){
      /// transfer event handler.
      /// Then IDispatch's Invoke will be called once container has event
      sp_connectionpoint_->Advise(reinterpret_cast<IDispatch *>(this),&m_dwCookie);
    }
  }

  CComQIPtr<IOleWindow,&IID_IOleWindow> spOleWindow(pUnkSite);
  if (spOleWindow.p){
    spOleWindow->GetWindow(&m_hWnd);
    if(::IsWindow(m_hWnd)){
      CreateRefWnd();
      spwb_.ShowStatusBar(true);
    }else
      return false;
  }
  return true;
}

bool CToolBarCOM::ThreadBasedTerm(){
  threadids.erase(tid);
  
  //UnRegisterNamespace();
  
  if( sp_connectionpoint_ && m_dwCookie ){
    sp_connectionpoint_->Unadvise(m_dwCookie);
  }
  return true;
}

/// Every IE thread will call the constructor and destructor
/// Can we do initialization in setSite??
CToolBarCOM::CToolBarCOM():
showit_(false),
m_hWnd(NULL),
tid(0)
{}

CToolBarCOM::~CToolBarCOM(){}

BOOL CToolBarCOM::CreateRefWnd(){
  RECT rect;
  ::GetClientRect(m_hWnd, &rect);
  m_ReflectionWnd.Create(m_hWnd,rect,NULL,WS_CHILD);
  return ::IsWindow(m_ReflectionWnd.GetToolBar().m_hWnd);
}

/// IDeskBand
STDMETHODIMP CToolBarCOM::GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi){
  static const int ktoolbarheight  = 24;
  if (pdbi){
    if (pdbi->dwMask & DBIM_MINSIZE){
      pdbi->ptMinSize.x = 200;//leave 200 space for the button behind
      pdbi->ptMinSize.y = ktoolbarheight;
    }
    if (pdbi->dwMask & DBIM_MAXSIZE){
      pdbi->ptMaxSize.x = -1;
      pdbi->ptMaxSize.y = ktoolbarheight;
    }
    if (pdbi->dwMask & DBIM_INTEGRAL){
      pdbi->ptIntegral.x = 1;
      pdbi->ptIntegral.y = 1;
    }
    if (pdbi->dwMask & DBIM_ACTUAL){
      CRect rc;
      int nWidth(0);
//#ifdef HASTOOLBARWND
      int nCount = this->m_ReflectionWnd.GetToolBar().GetButtonCount();
      for(int i = 0; i < nCount; i++){
        this->m_ReflectionWnd.GetToolBar().GetItemRect(i, &rc);
        nWidth += rc.right - rc.left;
      }
//#endif
      //The total length of the toolbar!
      nWidth += 200;
      pdbi->ptActual.x = nWidth;
      pdbi->ptActual.y = 24;
    }
    if (pdbi->dwMask & DBIM_TITLE){
      wcscpy_s(pdbi->wszTitle, L"");
    }
    if (pdbi->dwMask & DBIM_BKCOLOR){
      pdbi->dwMask &= ~DBIM_BKCOLOR;
    }
    if (pdbi->dwMask & DBIM_MODEFLAGS){
      pdbi->dwModeFlags = DBIMF_NORMAL|DBIMF_VARIABLEHEIGHT|DBIMF_USECHEVRON;
      //|DBIMF_BREAK;///// Add Chevron
    }
  }
  return S_OK;
}

STDMETHODIMP CToolBarCOM::GetWindow(HWND* phwnd){
  if (phwnd==NULL){
    return E_INVALIDARG;
  }else{
    *phwnd = m_ReflectionWnd.GetToolBar().m_hWnd;
    return S_OK;
  }
}

STDMETHODIMP CToolBarCOM::ContextSensitiveHelp(BOOL fEnterMode){
  return S_OK;
}

STDMETHODIMP CToolBarCOM::CloseDW(unsigned long dwReserved){
  ShowDW(false);
  return S_OK;
}

STDMETHODIMP CToolBarCOM::ResizeBorderDW(const RECT* prcBorder, IUnknown* punkToolbarSite, BOOL fReserved){
  return E_NOTIMPL;
}

STDMETHODIMP CToolBarCOM::ShowDW(BOOL fShow){
  showit_ = fShow;
  m_ReflectionWnd.GetToolBar().ShowWindow(showit_ ? SW_SHOW : SW_HIDE);
  return S_OK;
}

STDMETHODIMP CToolBarCOM::SetSite(IUnknown* pUnkSite){
  if(pUnkSite){
    SystemBasedInit();
    ProcessBasedInit();
    ThreadBasedInit(pUnkSite);
  }else{
    ThreadBasedTerm();
    ProcessBasedTerm();
    SystemBasedTerm();
  }
  return S_OK;
}

STDMETHODIMP CToolBarCOM::GetSite(REFIID riid,void **ppvSite){
  *ppvSite = NULL;
  if(sp_inputobjsite_.p){
    //::MessageBox(NULL,_T("Havefocus..."),_T("CToolBarCOM::FocusChange"),NULL);
    return sp_inputobjsite_->QueryInterface(riid, ppvSite);
  }
  return E_FAIL;
}

void  CToolBarCOM::FocusChange(BOOL bHaveFocus){
  //::MessageBox(NULL,_T("Havefocus。。。"),_T("CToolBarCOM::FocusChange"),NULL);
  if (sp_inputobjsite_.p){
    IUnknown* pUnk = NULL;
    if (SUCCEEDED(QueryInterface(IID_IUnknown, (LPVOID*)&pUnk)) && pUnk != NULL){
      if ( sp_inputobjsite_->OnFocusChangeIS(pUnk, bHaveFocus) != S_OK ){
        //::MessageBox(NULL,_T("ERROR..."),_T("CToolBarCOM::FocusChange"),NULL);
      }
      pUnk->Release();
      pUnk = NULL;
    }
  }
}

/// IInputObject
STDMETHODIMP CToolBarCOM::HasFocusIO(void){
  /*
  * ::GetFocus() doesn't return CToolComboBox::m_hWnd, even if the control
  * has the focus (don't know why, maybe ::GetFocus() only returns the
  * handle to the EditField part of the ComboBox), used flag m_bHasFocus
  * instead.
  */
  if (m_ReflectionWnd.GetToolBar().GetToolBarCombo().gotfocus){
    return S_OK;
  }
  if (m_ReflectionWnd.GetToolBar().m_hWnd == ::GetFocus()){
    return S_OK;
  }
  return S_FALSE;
}

STDMETHODIMP CToolBarCOM::TranslateAcceleratorIO(LPMSG lpMsg){
  /// the only window that needs to translate messages is our edit box so forward them.
  if(m_ReflectionWnd.GetToolBar().GetToolBarCombo())
    return m_ReflectionWnd.GetToolBar().GetToolBarCombo().TranslateAcceleratorIO(lpMsg);
  else
    return S_FALSE;
}

STDMETHODIMP CToolBarCOM::UIActivateIO(BOOL fActivate, LPMSG lpMsg){
  /// Internet浏览器会在面板被激活和失去焦点时调用UIActivateIO方法通知面板对象，
  /// 这时面板应该调用Focus方法获得焦点，同时调用Select方法激活界面上的子控件。
  /// 方法的fActivate表示是激活还是失活的状态
  /// if our deskband is being activated then set focus to the edit box.
  if(fActivate){
    //m_ReflectionWnd.GetToolBar().SetFocus();
    m_ReflectionWnd.GetToolBar().GetToolBarCombo().SetFocus();
    m_ReflectionWnd.GetToolBar().GetToolBarCombo().gotfocus = true;
    //::SendMessage(m_ReflectionWnd.GetToolBar().GetToolBarCombo().m_hWnd,WM_SETFOCUS,0,0);
  }

  //else{
  // m_ReflectionWnd.GetToolBar().GetToolBarCombo().gotfocus = false;
  // if(m_ReflectionWnd.GetToolBar().GetToolBarCombo().gotfocus){
  //   ::MessageBox(NULL,_T("problem2"),_T("CToolBarCOM::FocusChange"),NULL);
  // }
  //}
  return S_OK;
}

/***************** COM Events ********************/
HRESULT CToolBarCOM::Invoke(DISPID dispidMember,REFIID riid,LCID lcid,WORD wFlags,
                            DISPPARAMS* pDispParams,VARIANT* pvarResult,EXCEPINFO* pExcepInfo,UINT* puArgErr)
{
  if(!pDispParams || !iswebie)return E_INVALIDARG;
  switch (dispidMember)
  {
  case DISPID_PROGRESSCHANGE:
    if (pDispParams->rgvarg[0].vt==VT_I4 ||
      (pDispParams->cArgs>1 && pDispParams->rgvarg[1].vt==VT_I4) )
    {
      long progress = pDispParams->rgvarg[1].lVal;
      long progressMax = pDispParams->rgvarg[0].lVal;
      m_ReflectionWnd.ProgressChange(progress,progressMax);
    }
    break;
  case DISPID_DOCUMENTCOMPLETE:
    {
      //// @todo  - handle check zoom animation gracefully
      //m_ReflectionWnd.check_zoom_animate();
      m_ReflectionWnd.CreateMinuStatusWindow();
      std::wstring url=spwb_.GetURL();
      std::wstring title=spwb_.GetTitle();
      if(url.size()<11 || last_url==url){//min http://6.cn
        return S_OK;
      }else{
        IHTMLDocument2Ptr& doc2 = spwb_.GetHTMLDoc2();
        if (doc2!=NULL){
          _6bees_net::apr_uri_t t;
          string tmpurl = stringmaker()<<url;
          _6bees_net::_6beeparseuri(tmpurl.c_str(),t);
          wstring wscheme=wstringmaker()<<t.scheme;
          wstring faviconURL;
          _6bees_html::getFavicon(doc2,wscheme.c_str(),faviconURL);
          if (!faviconURL.empty()){
            nsplugin::PluginMng::R().OnDocComp(url,title,faviconURL);
            last_url = url;
          }
        }
      }
    }break;
  case DISPID_TITLECHANGE:
    if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR){
      m_ReflectionWnd.CreateMinuStatusWindow();
    }
    break;
  case DISPID_BEFORENAVIGATE2:
    {
      std::wstring url=spwb_.GetURL();
      nsplugin::PluginMng::R().OnBeforeNav2(url);
      m_ReflectionWnd.SetProgress(FALSE);
      m_ReflectionWnd.CreateMinuStatusWindow();
    }break;
  case DISPID_ONQUIT:{
    std::wstring url=spwb_.GetURL();
    nsplugin::PluginMng::R().OnQuit(url);
    }break;
  case DISPID_NAVIGATECOMPLETE2:
    {
      nsplugin::PluginMng::R().OnNavComp2(spwb_.GetURL().c_str());
    }break;
  default:
    break;
  }
  return S_OK;
}
