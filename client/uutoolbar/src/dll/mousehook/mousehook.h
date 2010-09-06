#ifndef __MOUSEDLL_H__
#define __MOUSEDLL_H__

#ifdef MOUSEDLL_EXPORTS
#define MOUSEDLL_API __declspec(dllexport)
#else
#define MOUSEDLL_API __declspec(dllimport)
#endif

#define WM_HOOK WM_USER+1234
#define WM_UNHOOK WM_USER+1235

#define WM_HKMOUSEMOVE_WEBELEIMG WM_USER+1236
#define WM_HKMOUSEMOVE_WEBELEHTML WM_USER+1237
#define WM_HOOKMOUSEMOVE_4 WM_USER+1238

#define WM_HOOKMOUSEUNINSTALL WM_USER+1240
#define WM_HOOKKEYBOARD_SHOWSEARCH WM_USER+1241

#define LP_WELLDONE 0
#define LP_GIVEUP 1

enum cap_type{
  CAPWEBELEIMG=10,
  CAPWHOLEPAGE,
  CAPWEBELEHTML
};

extern "C"{
  MOUSEDLL_API LRESULT WINAPI MouseProc(int nCode,WPARAM wParam,LPARAM lParam);
  MOUSEDLL_API BOOL    WINAPI InstallMouseHook(HWND hWnd,HCURSOR hCursor,cap_type mtype);
  MOUSEDLL_API BOOL    WINAPI UnInstallMouseHook();

  MOUSEDLL_API LRESULT WINAPI KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam);
  MOUSEDLL_API BOOL    WINAPI InstallKeyboardHook();
  MOUSEDLL_API BOOL    WINAPI UnInstallKeyboardHook();
}

#endif
