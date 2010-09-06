#ifndef __UUTOOLBAR_TOOLBARCOM__
#define __UUTOOLBAR_TOOLBARCOM__

//COM
#include <comdef.h>//_bstr_t,...
//ATL
#include <atlbase.h>

#include <shlguid.h>
#include <shlobj.h>
#include <exdispid.h>
#include <comdefsp.h>

#include "ReflectionWnd.h"
#include "uutoolbar.h"
#include "C6BeeStatusWnd.h"
#include "6bees_iwebbrowser2.h"

using namespace std;

class ATL_NO_VTABLE CToolBarCOM :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<CToolBarCOM, &CLSID_ToolBarCOM>,
  public IObjectWithSiteImpl<CToolBarCOM>,
  public IDeskBand,
  public IInputObject,
  public IDispatchImpl<IToolBarCOM, &IID_IToolBarCOM, &LIBID_UUToolBarLib, 1, 0>/*,
  public IInternetProtocol,
  public IInternetProtocolInfo,
  public ISupportErrorInfo*/
{
public:
  CToolBarCOM();
  ~CToolBarCOM();

  DECLARE_REGISTRY_RESOURCEID(IDR_TOOLBARCOM)

  BEGIN_CATEGORY_MAP(CToolBarCOM)
  //  IMPLEMENTED_CATEGORY(CATID_InfoBand)
  //  IMPLEMENTED_CATEGORY(CATID_CommBand)
  //  IMPLEMENTED_CATEGORY(CATID_DeskBand)
  END_CATEGORY_MAP()

  BEGIN_COM_MAP(CToolBarCOM)
    COM_INTERFACE_ENTRY(IToolBarCOM)
    COM_INTERFACE_ENTRY_IID(IID_IInputObject, IInputObject)
    //COM_INTERFACE_ENTRY(IInputObject)
    //{&IID_IInputObject, offsetofclass(IInputObject, _ComMapClass), _ATL_SIMPLEMAPENTRY},//VC6

    /*
    COM_INTERFACE_ENTRY(IInternetProtocol)
    COM_INTERFACE_ENTRY(IInternetProtocolRoot)
    COM_INTERFACE_ENTRY(IInternetProtocolInfo)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    */

    COM_INTERFACE_ENTRY(IOleWindow)
    COM_INTERFACE_ENTRY_IID(IID_IDockingWindow, IDockingWindow)
    COM_INTERFACE_ENTRY(IObjectWithSite)
    COM_INTERFACE_ENTRY_IID(IID_IDeskBand, IDeskBand)

    COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()

  // IDeskBand
  STDMETHOD(GetBandInfo)(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi);

  // IOleWindow
  STDMETHOD(GetWindow)(HWND* phwnd);
  STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);

  // IDockingWindow
  STDMETHOD(CloseDW)(unsigned long dwReserved);
  STDMETHOD(ResizeBorderDW)(const RECT* prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);
  STDMETHOD(ShowDW)(BOOL fShow);

  // IObjectWithSiteImpl
  STDMETHOD(SetSite)(IUnknown* pUnkSite);
  STDMETHOD(GetSite)(REFIID riid, void **ppvSite);

  // IInputObject
  STDMETHOD(HasFocusIO)(void);
  STDMETHOD(TranslateAcceleratorIO)(LPMSG lpMsg);
  STDMETHOD(UIActivateIO)(BOOL fActivate, LPMSG lpMsg);

  //IDispatchImpl
  STDMETHOD(Invoke)(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);

  void FocusChange(BOOL bHaveFocus);
  BOOL CreateRefWnd();

  /*
  //HTTP APP
  HRESULT RegisterNamespace();
  HRESULT UnRegisterNamespace();

  // IInternetProtocolRoot
  HRESULT STDMETHODCALLTYPE Start(LPCWSTR,IInternetProtocolSink *,IInternetBindInfo *,DWORD,HANDLE_PTR);
  HRESULT STDMETHODCALLTYPE Continue(PROTOCOLDATA *pProtocolData);
  HRESULT STDMETHODCALLTYPE Abort(HRESULT hrReason,DWORD dwOptions);
  HRESULT STDMETHODCALLTYPE Terminate(DWORD dwOptions);
  HRESULT STDMETHODCALLTYPE Suspend( void);
  HRESULT STDMETHODCALLTYPE Resume( void);

  // IInternetProtocol based on IInternetProtocolRoot
  HRESULT STDMETHODCALLTYPE Read(void *pv,ULONG cb,ULONG *pcbRead);
  HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER dlibMove,DWORD dwOrigin,ULARGE_INTEGER *plibNewPosition);
  HRESULT STDMETHODCALLTYPE LockRequest(DWORD dwOptions);
  HRESULT STDMETHODCALLTYPE UnlockRequest( void);

  // IInternetProtocolInfo
  HRESULT STDMETHODCALLTYPE ParseUrl( 
    LPCWSTR pwzUrl,
    PARSEACTION ParseAction,
    DWORD dwParseFlags,
    LPWSTR pwzResult,
    DWORD cchResult,
    DWORD *pcchResult,
    DWORD dwReserved);
  HRESULT STDMETHODCALLTYPE CombineUrl( 
    LPCWSTR pwzBaseUrl,
    LPCWSTR pwzRelativeUrl,
    DWORD dwCombineFlags,
    LPWSTR pwzResult,
    DWORD cchResult,
    DWORD *pcchResult,
    DWORD dwReserved);
  HRESULT STDMETHODCALLTYPE CompareUrl( 
    LPCWSTR pwzUrl1,
    LPCWSTR pwzUrl2,
    DWORD dwCompareFlags);
  HRESULT STDMETHODCALLTYPE QueryInfo( 
    LPCWSTR pwzUrl,
    QUERYOPTION OueryOption,
    DWORD dwQueryFlags,
    LPVOID pBuffer,
    DWORD cbBuffer,
    DWORD *pcbBuf,
    DWORD dwReserved);
  HRESULT FinalConstruct();
  void FinalRelease();
  static HRESULT WINAPI CreateProtocolInstance(
    void* pv,
    REFIID riid,
    LPVOID* ppv);
  // ISupportsErrorInfo
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
  */

  static bool iswebie;

private:
  // smart pointer of web browser
  _6bees_html::c6beewb2ptr spwb_;

  BOOL showit_;
  HWND m_hWnd;
  CComPtr<IInputObjectSite> sp_inputobjsite_;
  CComPtr<IConnectionPoint> sp_connectionpoint_;
  DWORD m_dwCookie;
  wstring last_url;
  DWORD tid;
  ReflectionWnd m_ReflectionWnd;

  static DWORD pid;
  // All thread ids' set.
  // If threadids.size()==1 means this is the last IE tab.Then we can call ProcessBasedTerm().
  // If threadids.size()==0 means this is the first IE tab.Then we can call ProcessBasedInit().
  static set<DWORD> threadids;
  // Or you can call a global init
  static bool ProcessBasedInit();
  static bool ProcessBasedTerm();
  static bool SystemBasedInit();// For the first IE Process
  static bool SystemBasedTerm();// For the last IE process

  // For every new thread(new tab), this function will be called
  bool ThreadBasedInit(IUnknown*);
  bool ThreadBasedTerm();

public:
  static map<DWORD,_6bees_html::c6beewb2ptr*> tid_wbptr;

/*
private:
  CComPtr<IInternetProtocol> m_spProtocol;
  CComPtr<IClassFactory>     m_spCFProtocol;
  CComPtr<IInternetSession>  spInternetSession;
*/

};

OBJECT_ENTRY_AUTO(__uuidof(ToolBarCOM), CToolBarCOM)

#endif
