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

#ifndef WINVER
#define WINVER 0x0501
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif            
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif
#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#define _ATL_APARTMENT_THREADED
//#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS


#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib,"uuplugins.lib")

#include "uutoolbar.h"
#include "ToolBarCOM.h"
#if 0
#include "H_HorizontalBar.h"
#endif

class CUUToolBarModule : public CAtlDllModuleT<CUUToolBarModule>{
public :
  DECLARE_LIBID(LIBID_UUToolBarLib)
  DECLARE_REGISTRY_APPID_RESOURCEID(IDR_UUTOOLBAR, "{D83938A7-C16E-45E4-B2E6-CC28B1128984}")
};

CUUToolBarModule _AtlModule;

BEGIN_OBJECT_MAP(ObjectMap)
  OBJECT_ENTRY(CLSID_ToolBarCOM, CToolBarCOM)
#if 0
  OBJECT_ENTRY(CLSID_HorizontalBar, Chorizontalbar)
#endif
END_OBJECT_MAP()

/// @todo Give choice to load for Explorer or not!
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance,DWORD dwReason,LPVOID lpReserved){
  hInstance;
  // temp comment this
  if (dwReason == DLL_PROCESS_ATTACH){
    if (!_6beed_share::u_getloadforexplorer()){
      wchar_t pszLoader[MAX_PATH];
      GetModuleFileNameW(NULL, pszLoader, MAX_PATH);
      _wcslwr_s(pszLoader,MAX_PATH);
      if (wcsstr(pszLoader, L"explorer.exe")){
        return FALSE;
      }
    }
  }
  return _AtlModule.DllMain(dwReason, lpReserved);
}

STDAPI DllCanUnloadNow(void){
  return _AtlModule.DllCanUnloadNow();
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv){
  return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void){
  static const char* const lpdata = "uutoolbar";
  HKEY hKeyLocal = NULL;
  RegCreateKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Internet Explorer\\Toolbar",
    0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyLocal, NULL);
  RegSetValueExA(hKeyLocal, "{0E1F40A1-4D93-435A-A8C5-273FE2ECF99C}", 0, REG_SZ, (LPBYTE)lpdata, (DWORD)strlen(lpdata)+1);
  RegCloseKey(hKeyLocal);
  return _AtlModule.DllRegisterServer();
}

STDAPI DllUnregisterServer(void){
  HKEY hKeyLocal = NULL;
  HRESULT hr = S_OK;
  if(RegCreateKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Internet Explorer\\Toolbar"),
    0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyLocal, NULL)==ERROR_SUCCESS)
  {
    if(RegDeleteValue(hKeyLocal, _T("{0E1F40A1-4D93-435A-A8C5-273FE2ECF99C}"))==ERROR_SUCCESS){
      hr = _AtlModule.DllUnregisterServer();
    }
    RegCloseKey(hKeyLocal);
  }
  return hr;
}
