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

// MyVirtualFolder.h : Declaration of the CMyVirtualFolder

#ifndef __MYVIRTUALFOLDER_H_
#define __MYVIRTUALFOLDER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "CNWSPidlMgr.h"

class ATL_NO_VTABLE CMyVirtualFolder : 
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<CMyVirtualFolder, &CLSID_MyVirtualFolder>,
  public IShellFolder, 
  public IPersistFolder,
  public IQueryInfo

{
public:
  HRESULT FinalConstruct();
  void FinalRelease();

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  BEGIN_COM_MAP(CMyVirtualFolder)
    COM_INTERFACE_ENTRY_IID(IID_IShellFolder,IShellFolder)
    COM_INTERFACE_ENTRY_IID(IID_IPersistFolder,IPersistFolder)
    COM_INTERFACE_ENTRY_IID(IID_IQueryInfo,IQueryInfo)
    COM_INTERFACE_ENTRY(IPersist)
  END_COM_MAP()

public:

  static HRESULT WINAPI CMyVirtualFolder::UpdateRegistry(BOOL bRegister)
  {
    CComBSTR bstrDescription;
    CComBSTR bstrProject;
    CComBSTR bstrInfoTip;
    CComBSTR bstrCLSID(CLSID_MyVirtualFolder);

    bstrDescription.LoadString(IDS_DESCRIPTION);
    bstrProject.LoadString(IDS_PROJNAME);
    bstrInfoTip.LoadString(IDS_INFOTIP);

    _ATL_REGMAP_ENTRY rm[] = { 
      { OLESTR("DESCRIPTION"), bstrDescription },
      { OLESTR("PROJECTNAME"), bstrProject },
      { OLESTR("CLSID"), bstrCLSID },
      { OLESTR("INFOTIP"), bstrInfoTip },
      { NULL,NULL } };

      return _Module.UpdateRegistryFromResource(IDR_MYVIRTUALFOLDER, bRegister, rm);
  }

  //IPersist methods
  STDMETHODIMP GetClassID(LPCLSID);

  //IPersistFolder methods
  STDMETHODIMP Initialize(LPCITEMIDLIST);

  //IShellFolder methods
  STDMETHOD(BindToObject)(LPCITEMIDLIST, LPBC, REFIID, LPVOID*);
  STDMETHOD(BindToStorage) (LPCITEMIDLIST, LPBC, REFIID, LPVOID*);
  STDMETHOD(CompareIDs)(LPARAM lParam, LPCITEMIDLIST, LPCITEMIDLIST);
  STDMETHOD(CreateViewObject)(HWND, REFIID, LPVOID*);
  STDMETHOD(EnumObjects)(HWND, DWORD, LPENUMIDLIST*);
  STDMETHOD(GetAttributesOf)(UINT, LPCITEMIDLIST*, LPDWORD);
  STDMETHOD(GetDisplayNameOf)(LPCITEMIDLIST pidl, DWORD, LPSTRRET);
  STDMETHOD(GetUIObjectOf)(HWND, UINT, LPCITEMIDLIST*, REFIID, LPUINT, LPVOID*);
  STDMETHOD(ParseDisplayName) (HWND, LPBC, LPOLESTR, LPDWORD, LPITEMIDLIST*, LPDWORD);
  STDMETHOD(SetNameOf)(HWND, LPCITEMIDLIST, LPCOLESTR, DWORD, LPITEMIDLIST*);

  //IQueryInfo
  STDMETHOD(GetInfoTip)(DWORD dwFlags,LPWSTR *ppwszTip);
  STDMETHOD(GetInfoFlags)(DWORD *pdwFlags);

  //User Defined functions
  int _CompareIDs(LPARAM lParam, LPCITEMIDLIST, LPCITEMIDLIST);

  //Drag and Drop
  HRESULT _DoDrop(LPDATAOBJECT pDataObject, DWORD dwDropEffect, LPITEMIDLIST pidlDropDest,UINT iFmtIdx);
  HRESULT _DoDrop_HDROP(HGLOBAL hMem, DWORD dwDropEffect,LPITEMIDLIST pidlDropDest);
  HRESULT _DoDrop_NSEDRAGDROP(HGLOBAL hMem, DWORD dwDropEffect,LPITEMIDLIST pidlDropDest);

public:
  CNWSPidlMgr m_PidlMgr;	
  LPITEMIDLIST m_pidlPath;   

};

#endif //__MYVIRTUALFOLDER_H_
