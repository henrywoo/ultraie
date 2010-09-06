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

#include "MainDlg_ads.h"
#include "getaddress.h"

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
  /*
  string url="http://s201.uu.avgarden.com|http://s200.uu.avgarden.com|http://s101.uu.avgarden.com|http://s300.uu.avgarden.com|http://s301.uu.avgarden.com|http://s100.uu.avgarden.com|http://s4.avgarden.net|http://s3.avgarden.net|http://uu2.avgarden.net|http://uu3.avgarden.net|http://o0i0o.avgarden.net|http://uu1.avgarden.net|http://f1.uu.lishaochang.com|http://x.uu.lishaochang.com|http://y.uu.lishaochang.com|http://z.uu.lishaochang.com|http://f2.uu.lishaochang.com|http://f3.hh.lishaochang.com|http://5.s5.uufile.com|http://o0i0o.uufile.com|http://3.s3.uufile.com|http://4.s4.uufile.com|http://2.s2.uufile.com|http://1.s1.uufile.com|http://u.uufile.net|http://8.uufile.net|http://i.uufile.net|http://9.uufile.net|http://o.uufile.net|http://0.uufile.net|http://bbs.uugarden.com|http://bbs2.uugarden.com|http://u2.s1.uugarden.com|http://u1.s1.uugarden.com|http://bbs.uugarden.net|http://f1.gg.uugarden.net|http://f2.gg.uugarden.net|http://f3.hh.uugarden.net|http://f3.gg.uugarden.net|http://www.uugarden.net|http://bbs.uugarden.net|http://www.uugarden.net|http://f1.gg.uugarden.net|http://f2.gg.uugarden.net|http://f3.hh.uugarden.net|http://f3.gg.uugarden.net";
  string encoded = base64_encode(reinterpret_cast<const unsigned char*>(url.c_str()), (unsigned int)url.length());
  FILE * pFile;
  pFile = fopen ("c:\\urls.txt","w");
  if (pFile!=NULL){
    fputs (encoded.c_str(),pFile);
    fclose (pFile);
  }
  string decoded = base64_decode(encoded);*/

  HRESULT hRes = ::CoInitialize(NULL);
  ::DefWindowProc(NULL, 0, 0, 0L);
  AtlInitCommonControls(ICC_BAR_CLASSES);
  hRes = _Module.Init(NULL, hInstance);

  AtlAxWinInit();
  int nRet = Run(lpstrCmdLine, nCmdShow);
  _Module.Term();
  ::CoUninitialize();
  return nRet;
}