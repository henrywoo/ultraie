// UUShellExt.h : Declaration of the CUUShellExt

#pragma once
#include "resource.h"       // main symbols

#include "uufile_shellext.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



#include <vector>

class ATL_NO_VTABLE CUUShellExt :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<CUUShellExt, &CLSID_UUShellExt>,
  public IShellExtInit,
  public IContextMenu3  
{
public:
  CUUShellExt(){}

  DECLARE_REGISTRY_RESOURCEID(IDR_UUSHELLEXT)
  DECLARE_NOT_AGGREGATABLE(CUUShellExt)

  BEGIN_COM_MAP(CUUShellExt)
    COM_INTERFACE_ENTRY_IID(IID_IContextMenu3, IContextMenu3)
    COM_INTERFACE_ENTRY_IID(IID_IContextMenu2, IContextMenu2)
    COM_INTERFACE_ENTRY_IID(IID_IContextMenu, IContextMenu)
    COM_INTERFACE_ENTRY(IShellExtInit)
  END_COM_MAP()

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  HRESULT FinalConstruct();
  void FinalRelease();

  // IShellExtInit
  STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
  // IContextMenu
  STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
  STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
  STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);
  // IContextMenu2 interface
  STDMETHOD(HandleMenuMsg)(UINT uMsg, WPARAM wParam, LPARAM lParam);
  // IContextMenu3 interface
  STDMETHOD(HandleMenuMsg2)(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);

private:
  WTL::CImageList m_ImageList;
  std::vector<ATL::CString> selectedFiles;
};

OBJECT_ENTRY_AUTO(__uuidof(UUShellExt), CUUShellExt)
