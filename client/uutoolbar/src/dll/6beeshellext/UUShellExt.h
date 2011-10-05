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

#pragma once
#include "resource.h"
#include "My6BeeShellExt.h"
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
