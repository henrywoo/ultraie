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

#include "ToolBarCOM.h"

#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/mousehook/mousehook.h"//HOOK IE's CreateProcess
#include "6bees_util.h"
#include "6bees_html.h"
#include "cppsqlite3.h"
#include "6bees_str.h"
#include "uulogging.h"
#include "6bees_window.h"
#include "6bees_net.h"
#include "pluginmng.h"
#include "globalvar.h"

#pragma comment(lib,"6beecommond.lib")

using namespace _6bees_util;

///About Class Constructor and variables' initialization:
///There is one and only one thread for IE's GUI will call setSite(...) and tiggers
///the Toolbar creation.But for every URL the web browser will access, the constructor
///will be called.So there are many many calls to:
///Constructor()
///FinalConstructor()
///FinalDestructor()
///Destrucotr()
///
///So:
///(1.) For CToolBarCOM, initialize at:
///SetSite(...)
///(2.) For Other Window Class like CToolBarWnd, C6BeeStatusWnd, initialize at:
///OnCreate(...)

using namespace _6beed_util;
using namespace _6bees_str;

///http: Asychronous Pluggable Protocol Handler
///If you look under HKEY_CLASSES_ROOT/PROTOCOLS/Handler you'll see a very familiar
///list of protocol schemes, eg http:, file:, local:, mailto:. Under each of these
///keys is a STRING value with the name of CLSID. For HKEY_CLASSES_ROOT/PROTOCOLS/Handler/http/CLSID
///this is {79eac9e2-baf9-11ce-8c82-00aa004ba90b}
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
      if(sp_inputobjsite_->OnFocusChangeIS(pUnk, bHaveFocus) != S_OK ){
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
  /*
  if (dispidMember == DISPID_ONSTATUSBAR){
  //::MessageBox(NULL,_T("catch DISPID_ONSTATUSBAR"),_T("Info"),NULL);
  }
  if (dispidMember == DISPID_NEWWINDOW2){//在原有窗口链接右击，然后新开一个窗口或者TAB
  //::MessageBox(NULL,_T("catch DISPID_NEWWINDOW2"),_T("Info"),NULL);
  }
  if (dispidMember == DISPID_NEWWINDOW3){//在原有窗口链接右击，然后新开一个窗口或者TAB
  //::MessageBox(NULL,_T("catch DISPID_NEWWINDOW3"),_T("Info"),NULL);
  }
  if (dispidMember == DISPID_ONVISIBLE){//新开的窗口可见了 触发
  //OutputDebugStringA("catch DISPID_ONVISIBLE");
  }
  if (dispidMember == DISPID_ONTOOLBAR){
  //OutputDebugStringA("catch DISPID_ONTOOLBAR");
  }
  if (dispidMember == DISPID_ONMENUBAR){
  //OutputDebugStringA("catch DISPID_ONMENUBAR");
  }
  if (dispidMember == DISPID_ONFULLSCREEN){
  //OutputDebugStringA("catch DISPID_ONFULLSCREEN");
  }
  if (dispidMember == DISPID_ONTHEATERMODE){
  //OutputDebugStringA("catch DISPID_ONTHEATERMODE");
  }
  if (dispidMember == DISPID_ONADDRESSBAR){
  //OutputDebugStringA("catch DISPID_ONTHEATERMODE");
  }
  if (dispidMember == DISPID_WINDOWSETRESIZABLE){
  //OutputDebugStringA("catch DISPID_WINDOWSETRESIZABLE");
  }
  if (dispidMember == DISPID_WINDOWCLOSING){
  //OutputDebugStringA("catch DISPID_WINDOWCLOSING");
  }
  if (dispidMember == DISPID_FILEDOWNLOAD){//OK
  //OutputDebugStringA("catch DISPID_FILEDOWNLOAD");
  }
  if (dispidMember == DISPID_WINDOWSETLEFT){
  //OutputDebugStringA("catch DISPID_WINDOWSETLEFT");
  }
  if (dispidMember == DISPID_NAVIGATEERROR){//OK
  //OutputDebugStringA("catch DISPID_NAVIGATEERROR");
  }
  if (dispidMember == DISPID_PRIVACYIMPACTEDSTATECHANGE){
  //OutputDebugStringA("catch DISPID_PRIVACYIMPACTEDSTATECHANGE");
  }
  if (dispidMember == DISPID_STATUSTEXTCHANGE){//OK
  //OutputDebugStringA("catch DISPID_STATUSTEXTCHANGE");
  }
  if (dispidMember == DISPID_QUIT){
  //OutputDebugStringA("catch DISPID_QUIT");
  }
  if (dispidMember == DISPID_COMMANDSTATECHANGE){//OKKKKKKKKKK!
  //OutputDebugStringA("catch DISPID_COMMANDSTATECHANGE");
  }
  if (dispidMember == DISPID_DOWNLOADBEGIN){
  //OK 要開始很多次
  //if (m_6BeeStatusWnd.m_Progress.ipaddress.Compare(_T(""))==0)
  //{
  ///  m_6BeeStatusWnd.m_Progress.ipaddress = _T("123.234.134.223");
  //}
  //::MessageBox(NULL,_T("catch DISPID_DOWNLOADBEGIN"),_T("Info"),NULL);
  //
  }
  if (dispidMember == DISPID_NEWWINDOW){
  //OutputDebugStringA("catch DISPID_NEWWINDOW");
  }
  if (dispidMember == DISPID_WINDOWMOVE){
  ::MessageBox(NULL,_T("catch DISPID_WINDOWMOVE"),_T("Info"),NULL);
  //
  }
  if (dispidMember == DISPID_WINDOWRESIZE){
  ::MessageBox(NULL,_T("catch DISPID_WINDOWRESIZE"),_T("Info"),NULL);
  //
  }
  if (dispidMember == DISPID_WINDOWACTIVATE){
  ::MessageBox(NULL,_T("catch DISPID_WINDOWACTIVATE"),_T("Info"),NULL);
  //
  }
  if (dispidMember == DISPID_PROPERTYCHANGE){
  //
  }
  if (dispidMember == DISPID_TITLEICONCHANGE){//OK
  //::MessageBox(NULL,_T("catch DISPID_TITLEICONCHANGE"),_T("Info"),NULL);
  }
  if (dispidMember == DISPID_FRAMEBEFORENAVIGATE){
  ::MessageBox(NULL,_T("catch DISPID_FRAMEBEFORENAVIGATE"),_T("Info"),NULL);
  //
  }
  if (dispidMember == DISPID_FRAMENAVIGATECOMPLETE){
  ::MessageBox(NULL,_T("catch DISPID_FRAMENAVIGATECOMPLETE"),_T("Info"),NULL);
  //
  }
  if (dispidMember == DISPID_FRAMENEWWINDOW){
  ::MessageBox(NULL,_T("catch DISPID_FRAMENEWWINDOW"),_T("Info"),NULL);
  //
  }
  if (dispidMember == DISPID_WINDOWREGISTERED){
  ::MessageBox(NULL,_T("catch DISPID_WINDOWREGISTERED"),_T("Info"),NULL);
  //
  }
  if (dispidMember == DISPID_WINDOWREVOKED){
  ::MessageBox(NULL,_T("catch DISPID_WINDOWREVOKED"),_T("Info"),NULL);
  //
  }
  if (dispidMember == DISPID_RESETFIRSTBOOTMODE){
  //
  }
  if (dispidMember == DISPID_RESETSAFEMODE){
  //
  }
  if (dispidMember == DISPID_REFRESHOFFLINEDESKTOP){
  //
  }
  if (dispidMember == DISPID_ADDFAVORITE){
  //::MessageBox(NULL,_T("catch DISPID_ADDFAVORITE"),_T("Info"),NULL);
  //
  }
  if (dispidMember == DISPID_DOWNLOADCOMPLETE){/// Download complete (to catch refresh and F5)
  //::MessageBox(NULL,_T("catch DISPID_DOWNLOADCOMPLETE"),_T("Info"),NULL);
  }
  */

  //C6BeeMenuStatusBar _menubarptr = C6BeeMenuStatusBar::tid2menustatusbarptr_[tid];
  //_menubarptr->ToolBarCOMInvoke(dispidMember,&spwb_);
  switch (dispidMember)
  {
    //case DISPID_STATUSTEXTCHANGE:
    //ATLASSERT(pDispParams->cArgs == 1);
    //OutputDebugString(pDispParams->rgvarg[0].bstrVal);
    //break;
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

/*

HRESULT CToolBarCOM::RegisterNamespace(){
HRESULT hr;
if(SUCCEEDED(hr = CoInternetGetSession(0, &spInternetSession, 0)) ){
hr = spInternetSession->RegisterNameSpace(m_spCFProtocol,UUIDOFHTTPAPP,_T("http"), 1, NULL, 0);
}
return hr;
}

HRESULT CToolBarCOM::UnRegisterNamespace(){
return spInternetSession->UnregisterNameSpace(m_spCFProtocol, _T("http"));
}

/// Very strange problem: inside Start, we cannot get IWebBrowser2.
HRESULT STDMETHODCALLTYPE CToolBarCOM::Start(LPCWSTR szUrl,IInternetProtocolSink *pOIProtSink,
                                             IInternetBindInfo *pOIBindInfo,DWORD grfPI,HANDLE_PTR dwReserved)
{
  if (szUrl!=NULL && wcslen(szUrl)>0){
    nsplugin::PluginMng::R().APPURLFilter(szUrl);
  }

  return m_spProtocol->Start(szUrl, pOIProtSink, pOIBindInfo, grfPI, dwReserved);
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::Continue(PROTOCOLDATA *pProtocolData){
  return m_spProtocol->Continue(pProtocolData);
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::Abort(HRESULT hrReason,DWORD dwOptions){
  return m_spProtocol->Abort(hrReason, dwOptions);
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::Terminate(DWORD dwOptions){
  return m_spProtocol->Terminate(dwOptions);
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::Suspend(void)
{
  return m_spProtocol->Suspend();
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::Resume(void)
{
  return m_spProtocol->Resume();
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::Read(void *pv,ULONG cb,ULONG *pcbRead){
  return m_spProtocol->Read(pv, cb, pcbRead);
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::Seek(LARGE_INTEGER dlibMove,
                                            DWORD dwOrigin,
                                            ULARGE_INTEGER *plibNewPosition)
{
  return m_spProtocol->Seek(dlibMove, dwOrigin, plibNewPosition);
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::LockRequest(DWORD dwOptions){
  return m_spProtocol->LockRequest(dwOptions);
  //return S_OK;
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::UnlockRequest(void){
  return m_spProtocol->UnlockRequest();
  //return S_OK;
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::ParseUrl(LPCWSTR pwzUrl,
                                                PARSEACTION ParseAction,
                                                DWORD dwParseFlags,
                                                LPWSTR pwzResult,
                                                DWORD cchResult,
                                                DWORD *pcchResult,
                                                DWORD dwReserved)
{
  return INET_E_DEFAULT_ACTION;
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::CombineUrl(LPCWSTR pwzBaseUrl,
                                                  LPCWSTR pwzRelativeUrl,
                                                  DWORD dwCombineFlags,
                                                  LPWSTR pwzResult,
                                                  DWORD cchResult,
                                                  DWORD *pcchResult,
                                                  DWORD dwReserved)
{
  return INET_E_DEFAULT_ACTION;
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::CompareUrl(LPCWSTR pwzUrl1,
                                                  LPCWSTR pwzUrl2,
                                                  DWORD dwCompareFlags)
{
  return INET_E_DEFAULT_ACTION;
}

HRESULT STDMETHODCALLTYPE CToolBarCOM::QueryInfo(LPCWSTR szUrl,
                                                 QUERYOPTION OueryOption,
                                                 DWORD dwQueryFlags,
                                                 LPVOID pBuffer,
                                                 DWORD cbBuffer,
                                                 DWORD *pcbBuf,
                                                 DWORD dwReserved)
{
  return INET_E_DEFAULT_ACTION;
}

HRESULT CToolBarCOM::FinalConstruct(){
  CComObject<CComClassFactory>* pCOMCF=NULL;
  HRESULT hr = CComObject<CComClassFactory>::CreateInstance(&pCOMCF);
  if (SUCCEEDED(hr)){
    pCOMCF->SetVoid(&CreateProtocolInstance);
    m_spCFProtocol = pCOMCF;
  }
  return m_spProtocol.CoCreateInstance(__uuidof(CLSID_HTTP_APP_HANDLER));
}

void CToolBarCOM::FinalRelease(){}

HRESULT WINAPI CToolBarCOM::CreateProtocolInstance(void* pv, REFIID riid, LPVOID* ppv){
  CComObject<CToolBarCOM>* pCToolBarAPP=NULL;
  HRESULT hr = CComObject<CToolBarCOM>::CreateInstance(&pCToolBarAPP);
  if (SUCCEEDED(hr)){
    pCToolBarAPP->AddRef();
    hr = pCToolBarAPP->QueryInterface(riid, ppv);
    pCToolBarAPP->Release();
  }
  return hr;
}

STDMETHODIMP CToolBarCOM::InterfaceSupportsErrorInfo(REFIID riid){
  static const IID* arr[] = {&IID_IToolBarCOM};
  for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++){
    if (InlineIsEqualGUID(*arr[i],riid))
      return S_OK;
  }
  return S_FALSE;
}

*/