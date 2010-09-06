// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER        0x0500
//#define _WIN32_WINNT  0x0501
#define _WIN32_IE      0x0501
#define _RICHEDIT_VER  0x0200

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#define WM_PLAYMMSOUND_PLAYFILE     WM_USER+301
#define WM_PLAYMMSOUND_CLOSEFILE    WM_USER+302
#define WM_PLAYMMSOUND_PLAYSOUNDPTR WM_USER+303
#define WM_PLAYMMSOUND_ENDTHREAD    WM_USER+304

#define WM_PLAYSOUND_STARTPLAYING WM_USER+600
#define WM_PLAYSOUND_STOPPLAYING  WM_USER+601
#define WM_PLAYSOUND_PLAYBLOCK    WM_USER+602
#define WM_PLAYSOUND_ENDTHREAD    WM_USER+603

#define WM_WRITESOUNDFILE_FILENAME      WM_USER+700
#define WM_WRITESOUNDFILE_WRITEBLOCK    WM_USER+701
#define WM_WRITESOUNDFILE_CLOSEFILE     WM_USER+702
#define WM_WRITESOUNDFILE_ENDTHREAD     WM_USER+703
#define WM_WRITESOUNDFILE_FILENAME_MP3  WM_USER+704
#define WM_WRITESOUNDFILE_CLOSEFILE_MP3 WM_USER+705

//#define WM_FINISHPLAY WM_USER+800

const int HERTZ           = 44000;
const int SOUNDSAMPLES    = 1000;
const int MAXINPUTBUFFERS = 5;

#define _Error_MessageBox(M) ::MessageBox(NULL,M,_T("UltraIE Recorder"),NULL)



#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
