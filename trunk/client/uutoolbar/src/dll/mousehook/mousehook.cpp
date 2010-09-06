/// @file mouse hook
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
#include "mousehook.h"
#include "../../lib/6beebase/6bees_const.h"
#include "../../lib/6beebase/6bees_window.h"
#include "../../dll/6beecommond/6beecommond.h"
#pragma comment(lib,"6beecommond.lib")
/*
For a specified hook type, thread hooks are called first, then global hooks.
The global hooks are a shared resource, and installing one affects all applications in 
the same desktop as the calling thread. All global hook functions must be in libraries.
Global hooks should be restricted to special-purpose applications or to use as a 
development aid during application debugging. Libraries that no longer need a hook 
should remove its hook procedure.
http://msdn2.microsoft.com/en-us/library/ms644990.aspx
*/
#pragma data_seg("SHAREDATA1")
int   type=0;
HWND  glhDisplayWnd=NULL;
HCURSOR hCur = NULL;
HHOOK hMouse=NULL;
HHOOK hKeyboard=NULL;
#pragma data_seg()
#pragma comment(linker, "/section:SHAREDATA1,rws")

// Must be global and should not be inside data_seg, or it cannot be uninstalled.
// http://topic.csdn.net/t/20061226/14/5256921.html
HINSTANCE hins = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,DWORD ul_reason_for_call,LPVOID lpReserved ){
  switch (ul_reason_for_call){
  case DLL_PROCESS_ATTACH:
    hins = hModule;
    break;
  default:break;
  }
  return TRUE;
}

LRESULT WINAPI MouseProc(int nCode,WPARAM wParam,LPARAM lParam){
  // Check mouse message. Mouse hook message is stored in wParam, lParam is a pointer of MOUSEHOOKSTRUCT, which
  // will store position of mouse, handle of the current window and mouse message
  LPMOUSEHOOKSTRUCT mousedetials=(LPMOUSEHOOKSTRUCT) lParam;
  HWND g_hwnd = mousedetials->hwnd;
  if(IsWindow(glhDisplayWnd)){
    switch (wParam){
      // 1. When capture the whole page, lock the mouse...
      case CAPWHOLEPAGE:
        return 1;
     // 2. When mouse moves...
      case WM_MOUSEMOVE:
        switch (type){
          case CAPWEBELEIMG:
            // Send message to hook client and make client side update data. Must use PostMessage.
            // Do not use SendMessage, which will continue to run after funtion returns.
            // Send message to toolbar about the position of mouse
            PostMessage(glhDisplayWnd,WM_HKMOUSEMOVE_WEBELEIMG,
              mousedetials->pt.x,mousedetials->pt.y);
            if(hCur){
              ::SetCursor(hCur);
            }
            return 1;
          case CAPWEBELEHTML:
            PostMessage(glhDisplayWnd,WM_HKMOUSEMOVE_WEBELEHTML,
              mousedetials->pt.x,mousedetials->pt.y);
            if(hCur){
              ::SetCursor(hCur);
            }
            return 1;
          }
        break;
      // 3. When click left button...
      //case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
        PostMessage(glhDisplayWnd,WM_HOOKMOUSEUNINSTALL,
          (WPARAM)type,(LPARAM)LP_WELLDONE);//can be deleted - the last parameter
        DestroyCursor(hCur);
        hCur=NULL;
        return 1;
      case WM_RBUTTONUP:
        PostMessage(glhDisplayWnd,WM_HOOKMOUSEUNINSTALL,
          (WPARAM)type,(LPARAM)LP_GIVEUP);
        DestroyCursor(hCur);
        hCur=NULL;
        return 1;
      // 4. When click the right button...
      /*case WM_RBUTTONDOWN:
        PostMessage(glhDisplayWnd,WM_HOOKMOUSEUNINSTALL,
          (WPARAM)type,(LPARAM)1);
        DestroyCursor(hCur);
        hCur=NULL;
        return 1;*/
    }
  }
  return CallNextHookEx(hMouse,nCode,wParam,lParam);
}

BOOL WINAPI InstallMouseHook(HWND hWnd,HCURSOR hCursor,cap_type mtype){
  hMouse=SetWindowsHookEx(WH_MOUSE,(HOOKPROC)MouseProc,hins,0);
  if (hMouse!=NULL){
    glhDisplayWnd = hWnd;
    hCur = hCursor;
    type = mtype;
    return TRUE;
  }
  return FALSE;
}

BOOL WINAPI UnInstallMouseHook(){
  if (hMouse && ::UnhookWindowsHookEx(hMouse)){
    glhDisplayWnd=NULL;
    type=0;
    hMouse=NULL;
    hCur = NULL;
    return TRUE;
  }
  return FALSE;
}

// A Low Level Keyboard Hook!
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT WINAPI KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam){
  KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *) lParam;
  if((HC_ACTION==nCode)){
    BOOL bCtrlKeyDown =  GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1);
    if(bCtrlKeyDown && (pkh->vkCode=='F')){
      HWND r= _6beed_util::GetReflectionWnd();
      if (r){
        ::PostMessage(r,WM_HOOKKEYBOARD_SHOWSEARCH,0,0);
        return 1;
      }
    }
  }
  return CallNextHookEx( hKeyboard, nCode, wParam, lParam );
}

BOOL WINAPI InstallKeyboardHook(){
  BOOL fOK = FALSE;
  hKeyboard=SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)KeyboardProc,hins,0);
  if (hKeyboard!=NULL){
    fOK = TRUE;
  }
  return fOK;
}

BOOL WINAPI UnInstallKeyboardHook(){
  BOOL bResult = FALSE;
  if(hKeyboard){
    bResult = ::UnhookWindowsHookEx(hKeyboard);
    if (bResult){
      hKeyboard=NULL;
    }
  }
  return bResult;
}

/*
1、WH_KEYBOARD和WH_KEYBOARD_LL有什么区别？
钩的位置不同，WH_KEYBOARD_LL要比WH_KEYBOARD先触发，WH_KEYBOARD_LL是由系统调用你的回调
函数，而WH_KEYBOARD则是你钩的进程来调用你的回调函数。
这就意味着，WH_KEYBOARD要插入进程，如果是全局钩子，就会插入每一个进程，WH_KEYBOARD_LL
不需要插入进程（所以据说根本不需要放在dll里面，放在exe里面也能运行）.
*/