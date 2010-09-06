#include "stdafx.h"
#include "horizontalbar.h"
#include <ExDispID.h>  // DWebBrowserEvents2
#include <6bees_util.h>
using namespace _6bees_util;

Chorizontalbar::Chorizontalbar():
m_dwCookie(0),
m_hwndParent(NULL),
iswebie(FALSE){
  m_ReflectionWnd.m_CH_HBarWnd.m_BandEditCtrl.SetBand(this);
}

// IObjectWithSite
STDMETHODIMP Chorizontalbar::SetSite(IUnknown *pSite){
  if( pSite==NULL ){
    m_ReflectionWnd.m_CH_HBarWnd.SetBrowser(NULL);
    sp_inputobjsite_.Release();
    m_spFrameWB.Release();
    if( sp_connectionpoint_ && m_dwCookie ){
      sp_connectionpoint_->Unadvise(m_dwCookie);
      sp_connectionpoint_.Release();
    }
  }else{
    m_hwndParent = NULL;
    CComQIPtr< IOleWindow, &IID_IOleWindow > spOleWindow(pSite);
    if(spOleWindow)
      spOleWindow->GetWindow(&m_hwndParent);//????
    if(!::IsWindow(m_hwndParent))
      return E_FAIL;

    RECT rc;
    ::GetClientRect(m_hwndParent,&rc);
    m_hWnd = m_ReflectionWnd.Create(m_hwndParent, rc,NULL,WS_CHILD);
    if(!m_ReflectionWnd.m_CH_HBarWnd.IsWindow()){return E_FAIL;}

    if(FAILED(pSite->QueryInterface(IID_IInputObjectSite, (LPVOID*)&sp_inputobjsite_))){
      return E_FAIL;
    }
    if( !sp_inputobjsite_ ) return E_FAIL;
    CComQIPtr < IServiceProvider, &IID_IServiceProvider> spSP(pSite);
    if( !spSP )  return E_FAIL;
    if (SUCCEEDED(spSP->QueryService( SID_SWebBrowserApp, &m_spFrameWB )) && m_spFrameWB!=NULL){
      CComBSTR fn;
      if(SUCCEEDED(m_spFrameWB->get_FullName(&fn))){
        std::wstring strfn = fn;
        std::wstring strfn_ = strfn.substr(strfn.size()-12,12);
        LOWERWSTR(strfn_);
        if(strfn_.compare(L"iexplore.exe")==0){
          //ie - iexplore.exe , shell - Explorer.EXE
          iswebie = TRUE;
          m_ReflectionWnd.m_CH_HBarWnd.SetBrowser(m_spFrameWB);
          CComQIPtr< IConnectionPointContainer,&IID_IConnectionPointContainer> spCPC( m_spFrameWB );
          if(spCPC){
            spCPC->FindConnectionPoint(DIID_DWebBrowserEvents2, &sp_connectionpoint_);
            if( sp_connectionpoint_ ){
              sp_connectionpoint_->Advise(reinterpret_cast<IDispatch *>(this),&m_dwCookie);
            }
          }
        }
      }
    }
  }
  return S_OK;
}

STDMETHODIMP Chorizontalbar::GetSite(REFIID riid, LPVOID *ppvReturn){
  *ppvReturn = NULL;
  if(sp_inputobjsite_)
    return sp_inputobjsite_->QueryInterface(riid, ppvReturn);
  return E_FAIL;
}

HRESULT Chorizontalbar::Invoke(
                               DISPID dispidMember,  // event id
                               REFIID riid,
                               LCID lcid,
                               WORD wFlags,
                               DISPPARAMS* pDispParams,// parameter
                               VARIANT* pvarResult,  // return
                               EXCEPINFO*  pExcepInfo,  // exception
                               UINT* puArgErr)
{
  switch (dispidMember){
  case DISPID_BEFORENAVIGATE2:
  case DISPID_NAVIGATECOMPLETE2:
    m_ReflectionWnd.m_CH_HBarWnd.SetInfo(0);
    m_ReflectionWnd.m_CH_HBarWnd.m_BandEditCtrl.SetWindowTextW(L"");
    m_ReflectionWnd.m_CH_HBarWnd.prev_search_words=L"";
    break;
  }
  return S_OK;
}

// IOleWindow
STDMETHODIMP Chorizontalbar::ContextSensitiveHelp(BOOL fEnterMode){
  return E_NOTIMPL;
}

STDMETHODIMP Chorizontalbar::GetWindow(HWND * phwnd){
  HRESULT hr = S_OK;
  if (NULL == phwnd){
    hr = E_INVALIDARG;
  }else{
    *phwnd = m_ReflectionWnd.m_CH_HBarWnd.m_hWnd;
  }
  return S_OK;
}

// IDockingWindow
STDMETHODIMP Chorizontalbar::ShowDW(BOOL bShow){
  m_ReflectionWnd.m_CH_HBarWnd.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
  return S_OK;
}

STDMETHODIMP Chorizontalbar::CloseDW(DWORD dwReserved){
  ShowDW(false);
  return S_OK;
}

STDMETHODIMP Chorizontalbar::ResizeBorderDW(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved){
  return E_NOTIMPL;
}

// IDeskBand
STDMETHODIMP Chorizontalbar::GetBandInfo(DWORD dwBandID, DWORD dwViewMode,  DESKBANDINFO* pdbi){
  static const int ktoolbarheight  = 24;
  if(NULL == pdbi)
    return E_INVALIDARG;
  m_dwBandID = dwBandID;
  m_dwViewMode = dwViewMode;
  if(pdbi->dwMask & DBIM_MINSIZE){
    pdbi->ptMinSize.x = ktoolbarheight;
    pdbi->ptMinSize.y = iswebie ? ktoolbarheight : 0;
  }
  if(pdbi->dwMask & DBIM_MAXSIZE){
    pdbi->ptMaxSize.x = -1;
    pdbi->ptMaxSize.y = iswebie ? ktoolbarheight : 0;
  }
  if(pdbi->dwMask & DBIM_INTEGRAL){
    pdbi->ptIntegral.x = -1;
    pdbi->ptIntegral.y = iswebie ? ktoolbarheight : 0;
  }
  if(pdbi->dwMask & DBIM_ACTUAL){
    pdbi->ptActual.x = -1;
    pdbi->ptActual.y = iswebie ? ktoolbarheight : 0;
  }
  if(pdbi->dwMask & DBIM_TITLE){
    wcscpy(pdbi->wszTitle,_T(""));
  }
  if(pdbi->dwMask & DBIM_MODEFLAGS){
    pdbi->dwModeFlags = DBIMF_VARIABLEHEIGHT;
  }
  if(pdbi->dwMask & DBIM_BKCOLOR){
    pdbi->dwMask &= ~DBIM_BKCOLOR;
  }
  return S_OK;
}


void Chorizontalbar::FocusChange(BOOL bHaveFocus){
  if (sp_inputobjsite_){
    IUnknown* pUnk = NULL;
    if (SUCCEEDED(QueryInterface(IID_IUnknown, (LPVOID*)&pUnk)) && pUnk != NULL){
      if(sp_inputobjsite_->OnFocusChangeIS(pUnk, bHaveFocus) != S_OK ){
        ::MessageBox(NULL,_T("ERROR"),_T("Horizontal Bar Focus Change"),NULL);
      }
      pUnk->Release();
      pUnk = NULL;
    }
  }
}

//// IInputObject
STDMETHODIMP Chorizontalbar::HasFocusIO(void){
  //::GetFocus() doesn't return CToolComboBox::m_hWnd, even if the control
  // has the focus (don't know why, maybe ::GetFocus() only returns the
  // handle to the EditField part of the ComboBox), used flag m_bHasFocus
  // instead.
  if (m_ReflectionWnd.m_CH_HBarWnd.m_BandEditCtrl.gotfocus || m_ReflectionWnd.m_CH_HBarWnd.m_hWnd == ::GetFocus()){
    return S_OK;
  }
  return S_FALSE;
}

STDMETHODIMP Chorizontalbar::TranslateAcceleratorIO(LPMSG lpMsg){
  // the only window that needs to translate messages is our edit box so forward them.
  if(m_ReflectionWnd.m_CH_HBarWnd.m_BandEditCtrl)
    return m_ReflectionWnd.m_CH_HBarWnd.m_BandEditCtrl.TranslateAcceleratorIO(lpMsg);
  else
    return S_FALSE;
}

STDMETHODIMP Chorizontalbar::UIActivateIO(BOOL fActivate, LPMSG lpMsg){
  if(fActivate){
    CBandEditCtrl& c = m_ReflectionWnd.m_CH_HBarWnd.m_BandEditCtrl;
    if(IsWindow(c.m_hWnd)){
      c.SetFocus();
      c.gotfocus = true;
    }
  }
  return S_OK;
}