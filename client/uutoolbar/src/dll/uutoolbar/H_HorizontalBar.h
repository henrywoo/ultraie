#ifndef __UUTOOLBAR_HBAR__
#define __UUTOOLBAR_HBAR__

#include "resource.h"
#include "uutoolbar.h"
#include <shlobj.h>
#include <shlguid.h>
#include <Mshtml.h>    // IHTMLDocument2

#include "H_HBarReflectionWnd.h"

class ATL_NO_VTABLE CHorizontalBar:
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<CHorizontalBar, &CLSID_HorizontalBar>,
  public IObjectWithSiteImpl<CHorizontalBar>, //???
  public IDispatchImpl<IHorizontalBar, &IID_IHorizontalBar, &LIBID_UUToolBarLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
  public IDeskBand,
  public IInputObject
{
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_HORIZONTALBAR)

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  BEGIN_COM_MAP(CHorizontalBar)
    COM_INTERFACE_ENTRY(IHorizontalBar)
    COM_INTERFACE_ENTRY_IID(IID_IInputObject, IInputObject)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IObjectWithSite)
    COM_INTERFACE_ENTRY_IID(IID_IDeskBand, IDeskBand)
  END_COM_MAP()

  BEGIN_CATEGORY_MAP(CHorizontalBar)
    IMPLEMENTED_CATEGORY(CATID_CommBand)
  END_CATEGORY_MAP()

public:
  CHorizontalBar();

  // IObjectWithSite
  STDMETHOD(SetSite)( IUnknown *pUnkSite);
  STDMETHOD(GetSite)( REFIID riid,  LPVOID *ppvReturn);

  // IDeskBand(inherits IOleWindow and IDockingWindow)
  // 1. IDockingWindow
  STDMETHOD(CloseDW)( DWORD dwReserved);
  STDMETHOD(ResizeBorderDW)( LPCRECT prcBorder,  IUnknown* punkToolbarSite,  BOOL fReserved);
  STDMETHOD(ShowDW)( BOOL bShow);

  // 2. IOleWindow
  STDMETHOD(GetWindow)( HWND * phwnd);
  STDMETHOD(ContextSensitiveHelp)( BOOL fEnterMode);  
  STDMETHOD(GetBandInfo)( DWORD dwBandID,  DWORD dwViewMode,  DESKBANDINFO* pdbi);

  // IInputObject
  STDMETHOD(UIActivateIO)(BOOL fActivate, LPMSG lpMsg);
  STDMETHOD(HasFocusIO)();
  STDMETHOD(TranslateAcceleratorIO)(LPMSG lpMsg);
  void      FocusChange(BOOL);

  DWORD m_dwViewMode;
  DWORD m_dwBandID;

  HRESULT FinalConstruct(){return S_OK;}
  void FinalRelease(){}

  STDMETHOD(Invoke)(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);

private:
  HWND m_hwndParent;
  HWND m_hWnd;

  CComQIPtr< IInputObjectSite, &IID_IInputObjectSite > sp_inputobjsite_;
  CComQIPtr< IWebBrowser2, &IID_IWebBrowser2 > m_spFrameWB;

  CComPtr<IConnectionPoint> sp_connectionpoint_;
  DWORD m_dwCookie;

  IWebBrowser2 *pwb_;
  CH_HBarReflectionWnd m_ReflectionWnd;
  CComPtr<IHTMLWindow2> m_spWin2;

  BOOL iswebie;
};

OBJECT_ENTRY_AUTO(__uuidof(HorizontalBar), CHorizontalBar)

#endif
