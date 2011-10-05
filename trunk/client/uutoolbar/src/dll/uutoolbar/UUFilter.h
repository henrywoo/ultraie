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

