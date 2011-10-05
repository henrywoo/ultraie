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

// CopyPathContextMenu.h : Declaration of the CCopyPathContextMenu

#ifndef __COPYPATHCONTEXTMENU_H_
#define __COPYPATHCONTEXTMENU_H_

#pragma warning(disable:4786)

#include "resource.h"       // main symbols
#include <shlguid.h>
#include <comdef.h>
#include <shlobj.h>
#include <list>
#include <string>

#define ID_COPY_PATH	0

/////////////////////////////////////////////////////////////////////////////
// CCopyPathContextMenu
class ATL_NO_VTABLE CCopyPathContextMenu : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCopyPathContextMenu, &CLSID_CopyPathContextMenu>,
	public IContextMenu3,
	public IShellExtInit
{
public:
	CCopyPathContextMenu()
	{
	}

	// IContextMenu interface
    STDMETHOD(QueryContextMenu)(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpici);
    STDMETHOD(GetCommandString)(UINT idCmd, UINT  uType, UINT *pwReserved, LPSTR pszName, UINT cchMax);   
    
	// IContextMenu2 interface
	STDMETHOD(HandleMenuMsg)(UINT uMsg, WPARAM wParam, LPARAM lParam);    
    
	// IContextMenu3 interface
	STDMETHOD(HandleMenuMsg2)(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
    
	// IShellExtInit interface
	STDMETHOD(Initialize)(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID);

DECLARE_REGISTRY_RESOURCEID(IDR_COPYPATHCONTEXTMENU)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCopyPathContextMenu)
  COM_INTERFACE_ENTRY_IID(IID_IContextMenu3, IContextMenu3)
  COM_INTERFACE_ENTRY_IID(IID_IContextMenu2, IContextMenu2)
  COM_INTERFACE_ENTRY_IID(IID_IContextMenu, IContextMenu)
	COM_INTERFACE_ENTRY(IShellExtInit)
END_COM_MAP()

private:
	typedef std::basic_string<_TCHAR>		string;	
	std::list<string>						m_listFileNames;
};

#endif //__COPYPATHCONTEXTMENU_H_
