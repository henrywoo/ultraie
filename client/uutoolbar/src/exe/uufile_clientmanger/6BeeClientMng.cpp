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

// Change these values to use different versions
//#define WINVER    0x0500
//#define _WIN32_WINNT  0x0501
//#define _WIN32_IE  0x0501
#define _RICHEDIT_VER  0x0200

#include <atlbase.h>
#include <atlstr.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlmisc.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "MainDlg.h"
#include "6bees_window.h"

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib,"6beeclientmnghook.lib")
#pragma comment(lib,"6beemousehook.lib")
#pragma comment(lib,"6beecommond.lib")

CAppModule _Module;

int Run(LPTSTR cmdline=NULL, int nCmdShow=SW_SHOWDEFAULT){
  CMessageLoop theLoop;
  _Module.AddMessageLoop(&theLoop);
  CMainDlg dlgMain;
  if(dlgMain.Create(NULL) == NULL){
    return 0;
  }
  dlgMain.ShowWindow(nCmdShow);
  int nRet = theLoop.Run();
  _Module.RemoveMessageLoop();
  return nRet;
}



int WINAPI _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR cmdline,int nCmdShow){
  if (FindWindowEx(NULL,NULL,NULL,_T("µÇÂ½UUFile")) != NULL){
    return -1;
  }else{
    HRESULT hRes = ::CoInitialize(NULL);
    ::DefWindowProc(NULL, 0, 0, 0L);
    AtlInitCommonControls(ICC_BAR_CLASSES);
    hRes = _Module.Init(NULL, hInstance);
    int nRet = Run(cmdline, nCmdShow);
    _Module.Term();
    ::CoUninitialize();
    return nRet;
  }
}