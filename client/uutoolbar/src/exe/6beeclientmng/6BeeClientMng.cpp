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

int Run(LPTSTR lpstrCmdLine=NULL, int nCmdShow=SW_SHOWDEFAULT){
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

int WINAPI _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpstrCmdLine,int nCmdShow){
  if (FindWindowEx(NULL,NULL,NULL,kUploader_WndTitle) != NULL){
    return -1;
  }else{
    HRESULT hRes = ::CoInitialize(NULL);
    ::DefWindowProc(NULL, 0, 0, 0L);
    AtlInitCommonControls(ICC_BAR_CLASSES);
    hRes = _Module.Init(NULL, hInstance);
    int nRet = Run(lpstrCmdLine, nCmdShow);
    _Module.Term();
    ::CoUninitialize();
    return nRet;
  }
}