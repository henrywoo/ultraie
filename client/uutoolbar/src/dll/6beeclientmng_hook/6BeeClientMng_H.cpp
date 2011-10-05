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
#define WIN32_LEAN_AND_MEAN
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#include <atlbase.h>
#include <atlstr.h>

#include "6BeeClientMng_H.h"
#include "APIHook.h"
#include <ShellAPI.h>
#include "../../dll/6beecommond/6beecommond.h"
#pragma comment(lib,"6beecommond.lib")


#pragma data_seg("SHAREDATA2")
HHOOK hhookIECreateProcess=NULL;
#pragma data_seg()
#pragma comment(linker, "/section:SHAREDATA2,rws")

HINSTANCE hins = NULL;//要全局并且不能是data_seg里面,否则会卸载不掉 http://topic.csdn.net/t/20061226/14/5256921.html
static const wchar_t* IEEXENAME=_T("iexplore.exe");
static const int IENAMELEN = (int)wcslen(IEEXENAME);
static const int Len=250;
static wchar_t ProgramFiles[Len]={};
static wchar_t iepath[Len]={};
static wchar_t lpcmd[Len*4]={};

BOOL APIENTRY DllMain( HMODULE hModule,DWORD ul_reason_for_call,LPVOID lpReserved )
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    hins = hModule;
    break;
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  default:break;
  }
  return TRUE;
}

//typedef BOOL (WINAPI *PFNCREATEPROCESSA)(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
typedef BOOL (WINAPI *PFNCREATEPROCESSW)(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);

//extern CAPIHook g_CreateProcessA;
extern CAPIHook g_CreateProcessW;

LRESULT WINAPI IECreatProcessProc(int nCode,WPARAM wParam,LPARAM lParam){
  return CallNextHookEx(hhookIECreateProcess, nCode, wParam, lParam);
}

BOOL WINAPI InstallIECreatProcessHook(){
  if (hhookIECreateProcess==NULL){
    if ((hhookIECreateProcess = ::SetWindowsHookEx(WH_GETMESSAGE,(HOOKPROC)IECreatProcessProc,hins,0))!=NULL){
      return TRUE;
    }
  }
  return FALSE;
}

BOOL WINAPI UnInstallIECreatProcessHook(){
  if(hhookIECreateProcess!=NULL && UnhookWindowsHookEx(hhookIECreateProcess)){
    hhookIECreateProcess = NULL;
    return TRUE;
  }
  return FALSE;
}
/*
BOOL WINAPI Hook_CreateProcessA(LPCSTR lpApplicationName,
                LPSTR lpCommandLine,
                LPSECURITY_ATTRIBUTES lpProcessAttributes,
                LPSECURITY_ATTRIBUTES lpThreadAttributes,
                BOOL bInheritHandles,
                DWORD dwCreationFlags,
                LPVOID lpEnvironment,
                LPCSTR lpCurrentDirectory,
                LPSTARTUPINFOA lpStartupInfo,
                LPPROCESS_INFORMATION lpProcessInformation)
{
  //::MessageBoxA(NULL,lpCommandLine,lpApplicationName,NULL);
  BOOL result = FALSE;
  result = ((PFNCREATEPROCESSA)(PROC)g_CreateProcessA)(lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,
    lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
  return result;
}
*/


BOOL WINAPI Hook_CreateProcessW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
                BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
  BOOL re=TRUE;
  BOOL willcreate=TRUE;
  try{
    /*
    UULog::R().logW(lpCommandLine);
    OutputDebugString(_T("lpApplicationName"));
    OutputDebugString(lpApplicationName);
    OutputDebugString(_T("lpCommandLine"));
    OutputDebugString(lpCommandLine);
    */
    if(lpApplicationName!=NULL && lpCommandLine!=NULL && _6beed_share::u_getieonewindow()){
      if(ProgramFiles[0]==L'\0')
        ::GetEnvironmentVariable(_T("ProgramFiles"),ProgramFiles,Len);
      swprintf_s(&iepath[0],Len,_T("%s\\Internet Explorer\\iexplore.exe"),ProgramFiles);
      if (_wcsicmp(iepath,lpApplicationName)==0){// is IExplorer.exe
        if(::FindWindowEx(NULL,NULL,_T("IEFrame"),NULL)){
          memcpy(lpcmd,lpCommandLine,Len*4);
          ATL::CString tmpCMD(lpcmd);
          tmpCMD.Trim(_T("\" "));//Be worthy to note lpCommandLine will include double quotation
          if(tmpCMD.Right(IENAMELEN).CompareNoCase(IEEXENAME)==0){
            ShellExecuteA(NULL,"open",_6beed_util::GetIEHomePage(),"","",SW_SHOW);
            willcreate=FALSE;
          }else{
            int t1=tmpCMD.Find(IEEXENAME);
            if(t1!=-1){
              ATL::CString URL=tmpCMD.Right(tmpCMD.GetLength()-t1-IENAMELEN).TrimLeft();
              ShellExecuteW(NULL,_T("open"),URL.GetString(),_T(""),_T(""),SW_SHOW);
              willcreate=FALSE;
            }
          }
        }
      }
    }
  }catch(...){}
  if(willcreate){
    re=((PFNCREATEPROCESSW)(PROC)g_CreateProcessW)(lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,
      dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
  }
  return re;
}

//CAPIHook g_CreateProcessA("Kernel32.dll", "CreateProcessA", (PROC) Hook_CreateProcessA);
CAPIHook g_CreateProcessW("Kernel32.dll", "CreateProcessW", (PROC) Hook_CreateProcessW);