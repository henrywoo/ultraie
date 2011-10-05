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

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>

CNWSModule _Module;
//61ce4538-2bbd-42ae-867b-38f1168ccb31
const CLSID CLSID_MyVirtualFolder =  {0x61ce4538,0x2bbd,0x42ae,{0x86,0x7b,0x38,0xf1,0x16,0x8c,0xcb,0x31}};
//15b8d3f5-a37c-47cd-93ef-cf5913e3f957
const CLSID CLSID_NSFShellView =     {0x15b8d3f5,0xa37c,0x47cd,{0x93,0xef,0xcf,0x59,0x13,0xe3,0xf9,0x57}};

#include "MyVirtualFolder.h"
#include "NSFShellView.h"

BEGIN_OBJECT_MAP(ObjectMap)
  OBJECT_ENTRY(CLSID_MyVirtualFolder, CMyVirtualFolder)
  OBJECT_ENTRY(CLSID_NSFShellView, CNSFShellView)
END_OBJECT_MAP()

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
  if (dwReason == DLL_PROCESS_ATTACH)
  {
    _Module.Init (ObjectMap,hInstance);

    CreateImageLists();

    DisableThreadLibraryCalls(hInstance);

#ifdef _ATL_DEBUG_INTERFACES
    _Module.m_nIndexBreakAt = 1;
#endif

  }
  else if (dwReason == DLL_PROCESS_DETACH)
  {
    _Module.Term();
  }
  return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
  if(_Module.GetLockCount()==0)
  {
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
    return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
  return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
  // registers object, not include typelib and all interfaces in typelib
  HRESULT Hr;
  Hr=_Module.RegisterServer(FALSE);

  SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

  if(_Module.m_wWinMajor == 5 && _Module.m_wWinMinor == 1) //XP can't auto refresh explore
  {
    RefreshShellViewWndsExcept(NULL);
  }
  return Hr;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
  HRESULT Hr;
  Hr= _Module.UnregisterServer(FALSE);

  SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
  if(_Module.m_wWinMajor == 5 && _Module.m_wWinMinor == 1) //XP can't auto refresh explore
  {
    ::SHChangeNotify( SHCNE_RMDIR , SHCNF_IDLIST | SHCNF_FLUSH, _Module.m_pidlNSFROOT, NULL);
  }

  ::CoFreeUnusedLibraries();

  return Hr;
}