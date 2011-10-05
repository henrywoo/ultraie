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
#include "My6BeeShellExt.h"
#include "UUShellExt.h"


#if 0
class CMy6BeeShellExtModule : public CAtlDllModuleT< CMy6BeeShellExtModule >
{
public :
  DECLARE_LIBID(LIBID_My6BeeShellExtLib)
  DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MY6BEESHELLEXT, "{1CFAE4B0-1A46-409F-B0A7-B12059134809}")
};

CMy6BeeShellExtModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
  return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
  HRESULT hr = _AtlModule.DllUnregisterServer();
  return hr;
}

#endif

#define __POP_FILE__

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
  OBJECT_ENTRY(CLSID_UUShellExt, CUUShellExt)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
  if (dwReason == DLL_PROCESS_ATTACH){
    _Module.Init(ObjectMap, hInstance, &LIBID_My6BeeShellExtLib);
    DisableThreadLibraryCalls(hInstance);
  }
  else if (dwReason == DLL_PROCESS_DETACH)
    _Module.Term();
  return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
  return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
  return _Module.GetClassObject(rclsid, riid, ppv);
}

//***************************************
//* Date            : 5.26.99
//* Last Modified   : 1.20.2000
//* Function name	: DllRegisterServer
//* Description	    : 
//***************************************
//
STDAPI DllRegisterServer(void)
{
  _TCHAR		strCLSID[50];
  OLECHAR		strWideCLSID[50];
  CRegKey		key;
  HRESULT		hr;
  USES_CONVERSION;

  hr = _Module.RegisterServer(TRUE);

  if (SUCCEEDED(hr)) {
    if (::StringFromGUID2(CLSID_UUShellExt, strWideCLSID, 50) > 0) {
      _tcscpy(strCLSID, OLE2CT(strWideCLSID));
      hr = key.SetValue(HKEY_CLASSES_ROOT, _T("*\\shellex\\ContextMenuHandlers\\6beeshellext\\"), strCLSID);
      hr = key.SetValue(HKEY_CLASSES_ROOT, _T("directory\\shellex\\ContextMenuHandlers\\6beeshellext\\"), strCLSID);
      hr = key.SetValue(HKEY_CLASSES_ROOT, _T("drive\\shellex\\ContextMenuHandlers\\6beeshellext\\"), strCLSID);
#ifndef __POP_FILE__
      hr = key.SetValue(HKEY_CLASSES_ROOT, _T("Directory\\Background\\shellex\\ContextMenuHandlers\\6beeshellext\\"), strCLSID);
#endif
    }
  }
  return hr;
}

//***************************************
//* Date            : 5.26.99
//* Last Modified   : 5.27.99
//* Function name	: DllUnregisterServer
//* Description	    : 
//***************************************
//
STDAPI DllUnregisterServer(void)
{
  CRegKey		key;
  HRESULT		hr;

  hr = _Module.UnregisterServer(TRUE);

  if (SUCCEEDED(hr)) {
    if (key.Open(HKEY_CLASSES_ROOT, _T("*\\shellex\\ContextMenuHandlers\\")) == ERROR_SUCCESS) {
      hr = key.DeleteValue(NULL);
      if (hr != ERROR_SUCCESS && hr != ERROR_FILE_NOT_FOUND)
        return hr;
      hr = key.DeleteSubKey(_T("6beeshellext"));
      if (hr != ERROR_SUCCESS && hr != ERROR_FILE_NOT_FOUND)
        return hr;
    }
    if (key.Open(HKEY_CLASSES_ROOT, _T("directory\\shellex\\ContextMenuHandlers\\")) == ERROR_SUCCESS) {
      hr = key.DeleteSubKey(_T("6beeshellext"));
      if (hr != ERROR_SUCCESS && hr != ERROR_FILE_NOT_FOUND)
        return hr;
    }
    if (key.Open(HKEY_CLASSES_ROOT, _T("drive\\shellex\\ContextMenuHandlers\\")) == ERROR_SUCCESS) {
      hr = key.DeleteSubKey(_T("6beeshellext"));
      if (hr != ERROR_SUCCESS && hr != ERROR_FILE_NOT_FOUND)
        return hr;
    }

#ifndef __POP_FILE__
    if (key.Open(HKEY_CLASSES_ROOT, _T("Directory\\Background\\shellex\\ContextMenuHandlers\\")) == ERROR_SUCCESS) {
      hr = key.DeleteSubKey(_T("6beeshellext"));
      if (hr != ERROR_SUCCESS && hr != ERROR_FILE_NOT_FOUND)
        return hr;
    }
#endif
  }

  return hr;
}
