#ifndef __UUTOOLBAR_ULTRAPROTOCOL__
#define __UUTOOLBAR_ULTRAPROTOCOL__

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

#include "resource.h"
#include "uutoolbar.h"
#include <string>
#include "../../dll/6beecommond/6beecommond.h"

class ATL_NO_VTABLE CUltraProtocol :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<CUltraProtocol, &CLSID_UltraProtocol>,
  public IInternetProtocol,
  public IInternetProtocolInfo
{
public:
  CUltraProtocol():m_postData(NULL),m_postDataLen(0){}
  DECLARE_REGISTRY_RESOURCEID(IDR_ULTRAPROTOCOL)
  BEGIN_COM_MAP(CUltraProtocol)
    COM_INTERFACE_ENTRY(IInternetProtocolInfo)
    COM_INTERFACE_ENTRY(IInternetProtocol)
    COM_INTERFACE_ENTRY(IInternetProtocolRoot)
  END_COM_MAP()
  DECLARE_PROTECT_FINAL_CONSTRUCT()

  HRESULT FinalConstruct(){
    return S_OK;
  }
  void FinalRelease(){}
// IInternetProtocol interface
public:
  STDMETHOD(Start)(LPCWSTR szUrl,IInternetProtocolSink *pIProtSink,IInternetBindInfo *pIBindInfo,DWORD grfSTI,DWORD dwReserved);
  STDMETHOD(Continue)(PROTOCOLDATA *pStateInfo);
  STDMETHOD(Abort)(HRESULT hrReason,DWORD dwOptions);
  STDMETHOD(Terminate)(DWORD dwOptions);
  STDMETHOD(Suspend)();
  STDMETHOD(Resume)();
  STDMETHOD(Read)(void *pv,ULONG cb,ULONG *pcbRead);
  STDMETHOD(Seek)(LARGE_INTEGER dlibMove,DWORD dwOrigin,ULARGE_INTEGER *plibNewPosition);
  STDMETHOD(LockRequest)(DWORD dwOptions);
  STDMETHOD(UnlockRequest)();

//IInternetProtocolInfo interface
public:
  STDMETHOD(CombineUrl)(LPCWSTR pwzBaseUrl, LPCWSTR pwzRelativeUrl, DWORD dwCombineFlags,LPWSTR pwzResult, DWORD cchResult, DWORD *pcchResult, DWORD dwReserved);
  STDMETHOD(CompareUrl)(LPCWSTR pwzUrl1, LPCWSTR pwzUrl2, DWORD dwCompareFlags);
  STDMETHOD(ParseUrl)(LPCWSTR pwzUrl, PARSEACTION ParseAction, DWORD dwParseFlags,LPWSTR pwzResult, DWORD cchResult, DWORD *pcchResult, DWORD dwReserved);
  STDMETHOD(QueryInfo)( LPCWSTR pwzUrl, QUERYOPTION QueryOption, DWORD dwQueryFlags,LPVOID pBuffer, DWORD cbBuffer, DWORD *pcbBuf, DWORD dwReserved);
  void GetPostData(IInternetBindInfo *pIBindInfo);

private:
  CComPtr<IInternetProtocolSink> m_spSink;
  CComPtr<IInternetBindInfo> m_spBindInfo;
  ATL::CString m_url;
  DWORD m_dwPos;
  ULONG content_length;
  std::string filecontent;
  char * memblock;

  BINDINFO m_bindinfo;
  DWORD m_bindf;
  BYTE *m_postData;
  ULONG m_postDataLen;
};

OBJECT_ENTRY_AUTO(__uuidof(UltraProtocol), CUltraProtocol)

#endif