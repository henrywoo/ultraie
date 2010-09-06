#ifndef __UUTOOLBAR_UUFILTER__
#define __UUTOOLBAR_UUFILTER__

#include "resource.h"
#include "uutoolbar.h"
#include "6bees_util.h"
#include <atlcom.h>

using namespace _6bees_util;

class ATL_NO_VTABLE CUUFilter :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<CUUFilter, &CLSID_UUFilter>,
  public IObjectWithSiteImpl<CUUFilter>,
  public IUUFilter
{
public:

  DECLARE_REGISTRY_RESOURCEID(IDR_UUFILTER)
  BEGIN_COM_MAP(CUUFilter)
    COM_INTERFACE_ENTRY(IUUFilter)
    COM_INTERFACE_ENTRY(IObjectWithSite)
  END_COM_MAP()
  DECLARE_PROTECT_FINAL_CONSTRUCT()

  CUUFilter();

  HRESULT FinalConstruct();

  void FinalRelease();

  STDMETHOD(SetSite)(IUnknown* pUnk);

};

OBJECT_ENTRY_AUTO(__uuidof(UUFilter), CUUFilter)

#endif

