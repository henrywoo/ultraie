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

#include "6bees_window.h"
#include <tchar.h>

namespace _6bees_window{
  using std::vector;
  using namespace _6bees_const;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  namespace IE8{
    void GetBarHeight(HWND hWnd_HTMLClientArea,int& statusbarheight,int& shuipinbarheight){
      /// @todo 
    }
    
    int IEProcNumbers(){
      /// @todo 
      return 0;
    }

    // by IE process
    void CollectIEFrame(vector<HWND>& vIE){
      HWND h=NULL,chi_after=NULL,fr=NULL,chi_after2=NULL;
      do{
        h = ::FindWindowEx(NULL,chi_after,_T("IEFrame"),NULL);
        if(h){
          do {
            fr = ::FindWindowEx(h,chi_after2,_T("Frame Tab"),NULL);
            if (fr){
              vIE.push_back(fr);
            }
			chi_after2 = fr;
          }while(fr!=NULL);
          chi_after = h;
        }
      } while (h!=NULL);
    }

    void Notify6BeeTB(int msg,HWND myself){
      COPYDATASTRUCT cds;
      cds.dwData = msg;
      cds.lpData = NULL;
      cds.cbData = NULL;
      vector<HWND> vIE;
      CollectIEFrame(vIE);
      for (vector<HWND>::const_iterator i=vIE.begin();i!=vIE.end();++i){
        HWND h=*i;
        if(h!=NULL){
          HWND h1 = ::FindWindowEx(h,NULL,_T("InternetToolbarHost"),_T("ITBarHost"));
          HWND h2 = ::FindWindowEx(h1,NULL,_T("WorkerW"),_T("Menu Bar"));
          HWND h3 = ::FindWindowEx(h2,NULL,_T("ReBarWindow32"),_T(""));
          HWND h4 = ::FindWindowEx(h3,NULL,kWND_NAME_REFLECTION,_T(""));
          if(h4!=NULL && h4!=myself)
            ::SendMessage(h4,WM_COPYDATA,(WPARAM)0,(LPARAM)(LPVOID)&cds);
        }
      }
#if 0
      vector<HWND> vShellFolder;
      CollectShellFolderWnd(vShellFolder);
      for (vector<HWND>::const_iterator i=vShellFolder.begin();i!=vShellFolder.end();++i){
        HWND h=*i;
        if(h!=NULL){
          HWND h1 = ::FindWindowEx(h,NULL,_T("WorkerW"),NULL);
          if (!IsWindowVisible(h1)){
            h1 = ::FindWindowEx(h,h1,_T("WorkerW"),NULL);
          }
          HWND h2 = ::FindWindowEx(h1,NULL,_T("ReBarWindow32"),NULL);
          HWND h3 = ::FindWindowEx(h2,NULL,kWND_NAME_REFLECTION,_T(""));
          if(h3!=NULL && h3!=myself)
            ::SendMessage(h3,WM_COPYDATA,(WPARAM)0,(LPARAM)(LPVOID)&cds);
        }
      }
#endif
    }

    HWND GetReflectionWnd(){
      HWND activeIE = GetForegroundWindow();
      wchar_t ClassName[256]={};
      ::GetClassNameW(activeIE,ClassName,256);
      if (_wcsicmp(ClassName,L"IEFrame")==0){
        HWND _frametabWnd = FindWindowEx(activeIE,NULL,L"Frame Tab",L"");
        if (_frametabWnd){
          HWND _targetWnd = FindWindowEx(_frametabWnd,NULL,L"InternetToolbarHost",L"ITBarHost");
          if(_targetWnd){
            HWND _targetWnd2 = FindWindowEx(_targetWnd,NULL,L"WorkerW",L"Menu Bar");
            if(_targetWnd2){
              HWND _targetWnd3 = FindWindowEx(_targetWnd2,NULL,L"ReBarWindow32",L"");
              if(_targetWnd3){
                return FindWindowEx(_targetWnd3,NULL,kWND_NAME_REFLECTION,L"");
              }
            }
          }
        }
      }
      return NULL;
    }
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  namespace IE7{
    void GetBarHeight(HWND hWnd_HTMLClientArea,int& statusbarheight,int& shuipinbarheight){
      RECT tmpret={};
      HWND hstatusbar = ::FindWindowEx(hWnd_HTMLClientArea,NULL,_T("msctls_statusbar32"),NULL);
      if(hstatusbar==NULL)
        statusbarheight=0;
      else{
        if(::IsWindowVisible(hstatusbar)){
          ::GetWindowRect(hstatusbar,&tmpret);
          statusbarheight=tmpret.bottom-tmpret.top;
        }else{
          statusbarheight=0;
        }
      }
      HWND hshuipinbar = FindWindowEx(hWnd_HTMLClientArea,NULL,_T("BaseBar"),_T("Horizontal Explorer Bar"));
      if(hshuipinbar==NULL)
        shuipinbarheight=0;
      else{
        if(::IsWindowVisible(hshuipinbar)){
          ::GetWindowRect(hshuipinbar,&tmpret);
          shuipinbarheight=tmpret.bottom-tmpret.top;
        }else{
          shuipinbarheight=0;
        }
      }
    }

    // The number of IE Processes for IE6
    int IEProcNumbers(){
      return IE6::IEProcNumbers();
    }

    HWND FindReflectwndFromSBar(HWND h){
      HWND a0 = ::GetParent(h);
      HWND a = ::GetNextWindow(::GetParent(a0),GW_HWNDNEXT);
      HWND b = ::FindWindowEx(a,NULL,_T("WorkerW"),_T("Menu Bar"));
      HWND c = ::FindWindowEx(b,NULL,_T("ReBarWindow32"),NULL);
      HWND result = ::FindWindowEx(c,NULL,kWND_NAME_REFLECTION,NULL);
      return result;
    }

    void CollectIEFrame(vector<HWND>& vIE){
      IE6::CollectIEFrame(vIE);
    }

    void CollectShellFolderWnd(vector<HWND>& vShellFolder){
      HWND h=NULL,chi_after=NULL;
      do{
        h = ::FindWindowEx(NULL,chi_after,_T("CabinetWClass"),NULL);
        if(h){
          vShellFolder.push_back(h);
          chi_after = h;
        }
      } while (h!=NULL);
    }

    void CollectITBarHost(HWND par,vector<HWND>& v6BeeTB){
      HWND h=NULL,chi_after=NULL;
      do{
        h = ::FindWindowEx(par,chi_after,_T("InternetToolbarHost"),_T("ITBarHost"));
        if(h){
          HWND h2 = ::FindWindowEx(h,NULL,_T("WorkerW"),_T("Menu Bar"));
          HWND h3 = ::FindWindowEx(h2,NULL,_T("ReBarWindow32"),_T(""));
          HWND h4 = ::FindWindowEx(h3,NULL,kWND_NAME_TBWND,_T(""));
          if (h!=NULL){
            if(h4!=NULL)v6BeeTB.push_back(h4);
            chi_after = h;
          }
        }
      } while (h!=NULL);
    }

    // Description:
    // put all 6BeeToolBar window handle into a vector
    // Dont know why send msg to all 6BeeTooBar windows cannot login/logout at the same time. this function is useless temperary.
    void GetAll6BeeTB(vector<HWND>& v6BeeTB){
      vector<HWND> vIE;
      CollectIEFrame(vIE);
      if (!vIE.empty()){
        for (vector<HWND>::const_iterator i=vIE.begin();i!=vIE.end();++i){
          HWND h=*i;
          if(h!=NULL)
            CollectITBarHost(h,v6BeeTB);
        }
      }
    }

    std::wstring GetAddressBarText(HWND hstatusbarhwnd){
      HWND h = GetParent(hstatusbarhwnd);
      if(h==NULL) return std::wstring();
      HWND i = GetParent(h);
      if(i==NULL) return std::wstring();
      HWND a = ::GetNextWindow(i,GW_HWNDPREV);
      if(a==NULL) return std::wstring();
      HWND b = ::FindWindowEx(a,NULL,_T("ReBarWindow32"),NULL);
      if(b==NULL) return std::wstring();
      HWND c = ::FindWindowEx(b,NULL,_T("Address Band Root"),NULL);
      if(c==NULL) return std::wstring();
      HWND d = ::FindWindowEx(c,NULL,_T("ComboBoxEx32"),NULL);
      if(d==NULL) return std::wstring();
      wchar_t tmp[1024]={};
      ::GetWindowTextW(d,tmp,1024);
      return tmp;
    }

    void Notify6BeeTB(int msg,HWND myself){
      COPYDATASTRUCT cds;
      cds.dwData = msg;
      cds.lpData = NULL;
      cds.cbData = NULL;
      vector<HWND> vIE;
      CollectIEFrame(vIE);
      for (vector<HWND>::const_iterator i=vIE.begin();i!=vIE.end();++i){
        HWND h=*i;
        if(h!=NULL){
          HWND h1 = ::FindWindowEx(h,NULL,_T("InternetToolbarHost"),_T("ITBarHost"));
          HWND h2 = ::FindWindowEx(h1,NULL,_T("WorkerW"),_T("Menu Bar"));
          HWND h3 = ::FindWindowEx(h2,NULL,_T("ReBarWindow32"),_T(""));
          HWND h4 = ::FindWindowEx(h3,NULL,kWND_NAME_REFLECTION,_T(""));
          if(h4!=NULL && h4!=myself)
            ::SendMessage(h4,WM_COPYDATA,(WPARAM)0,(LPARAM)(LPVOID)&cds);
        }
      }
      vector<HWND> vShellFolder;
      CollectShellFolderWnd(vShellFolder);
      for (vector<HWND>::const_iterator i=vShellFolder.begin();i!=vShellFolder.end();++i){
        HWND h=*i;
        if(h!=NULL){
          HWND h1 = ::FindWindowEx(h,NULL,_T("WorkerW"),NULL);
          if (!IsWindowVisible(h1)){
            h1 = ::FindWindowEx(h,h1,_T("WorkerW"),NULL);
          }
          HWND h2 = ::FindWindowEx(h1,NULL,_T("ReBarWindow32"),NULL);
          HWND h3 = ::FindWindowEx(h2,NULL,kWND_NAME_REFLECTION,_T(""));
          if(h3!=NULL && h3!=myself)
            ::SendMessage(h3,WM_COPYDATA,(WPARAM)0,(LPARAM)(LPVOID)&cds);
        }
      }
    }

    HWND GetReflectionWnd(){
      HWND activeIE = GetForegroundWindow();
      wchar_t ClassName[256]={};
      ::GetClassNameW(activeIE,ClassName,256);
      if (_wcsicmp(ClassName,L"IEFrame")==0){
        HWND _targetWnd = FindWindowEx(activeIE,NULL,L"InternetToolbarHost",L"ITBarHost");
        if(_targetWnd){
          HWND _targetWnd2 = FindWindowEx(_targetWnd,NULL,L"WorkerW",L"Menu Bar");
          if(_targetWnd2){
            HWND _targetWnd3 = FindWindowEx(_targetWnd2,NULL,L"ReBarWindow32",L"");
            if(_targetWnd3){
              return FindWindowEx(_targetWnd3,NULL,kWND_NAME_REFLECTION,L"");
            }
          }
        }
      }
      return NULL;
    }
    
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  namespace IE6{
    void GetBarHeight(HWND hWnd_HTMLClientArea,int& statusbarheight,int& shuipinbarheight){
      /// @todo 
    }

    // The number of IE Processes for IE6
    int IEProcNumbers(){
      int result=0;
      HWND h=FindWindowEx(NULL,NULL,_T("IEFrame"),NULL);
      if (h){
        ++result;
        while ((h = FindWindowExW(NULL,h,_T("IEFrame"),NULL)) != NULL){
          ++result;
        }
      }
      return result;
    }

    HWND GetReflectionWnd(){
      HWND activeIE = GetForegroundWindow();
      wchar_t ClassName[256]={};
      ::GetClassNameW(activeIE,ClassName,256);
      if (_wcsicmp(ClassName,L"IEFrame")==0){
        HWND _targetWnd = FindWindowEx(activeIE,NULL,L"WorkerW",L"");
        if(_targetWnd){
          HWND _targetWnd2 = FindWindowEx(_targetWnd,NULL,L"ReBarWindow32",L"");
          if(_targetWnd2){
            return FindWindowEx(_targetWnd2,NULL,kWND_NAME_REFLECTION,L"");
          }
        }
      }
      return NULL;
    }

  void CollectIEFrame(vector<HWND>& vIE){
    HWND h=NULL,chi_after=NULL;
    do{
      h = ::FindWindowEx(NULL,chi_after,_T("IEFrame"),NULL);
      if(h){
        vIE.push_back(h);
        chi_after = h;
      }
    } while (h!=NULL);
  }

  void Notify6BeeTB(int msg,HWND myself){
    COPYDATASTRUCT cds;
    cds.dwData = msg;
    cds.lpData = NULL;
    cds.cbData = NULL;
    vector<HWND> vIE;
    CollectIEFrame(vIE);
    for (vector<HWND>::const_iterator i=vIE.begin();i!=vIE.end();++i){
      HWND h=*i;
      if(h!=NULL){
        HWND h1 = ::FindWindowEx(h,NULL,_T("WorkerW"),_T(""));
        HWND h2 = ::FindWindowEx(h1,NULL,_T("ReBarWindow32"),_T(""));
        HWND h4 = ::FindWindowEx(h2,NULL,kWND_NAME_REFLECTION,_T(""));
        if(h4!=NULL && h4!=myself)
          ::SendMessage(h4,WM_COPYDATA,(WPARAM)0,(LPARAM)(LPVOID)&cds);
      }
    }

#if 0
    vector<HWND> vShellFolder;
    CollectShellFolderWnd(vShellFolder);
    for (vector<HWND>::const_iterator i=vShellFolder.begin();i!=vShellFolder.end();++i){
      HWND h=*i;
      if(h!=NULL){
        HWND h1 = ::FindWindowEx(h,NULL,_T("WorkerW"),NULL);
        if (!IsWindowVisible(h1)){
          h1 = ::FindWindowEx(h,h1,_T("WorkerW"),NULL);
        }
        HWND h2 = ::FindWindowEx(h1,NULL,_T("ReBarWindow32"),NULL);
        HWND h3 = ::FindWindowEx(h2,NULL,kWND_NAME_REFLECTION,_T(""));
        if(h3!=NULL && h3!=myself)
          ::SendMessage(h3,WM_COPYDATA,(WPARAM)0,(LPARAM)(LPVOID)&cds);
      }
    }
#endif
  }


  };


  void RefreshWindow(HWND h){
    ::InvalidateRect(h ,NULL,TRUE);
    ::UpdateWindow (h );
    ::RedrawWindow (h ,NULL,NULL,
      RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW|RDW_ALLCHILDREN);
  }

  // Synopsis :
  // 1. This function checks a hwnd to see if it is actually the "Search Window" Dialog's or Main Window's
  // own window or one of their children. If so a FALSE will be returned so that these windows will not
  // be selected. 
  //
  // 2. Also, this routine checks to see if the hwnd to be checked is already a currently found window.
  // If so, a FALSE will also be returned to avoid repetitions.
  BOOL CheckWindowValidity (HWND hwndDialog, HWND hwndToCheck, HWND g_hwndFoundWindow){
    HWND hwndTemp = NULL;
    BOOL bRet = TRUE;
    // The window must not be NULL.
    if (hwndToCheck == NULL){
      bRet = FALSE;
      goto CheckWindowValidity_0;
    }
    // It must also be a valid window as far as the OS is concerned.
    if (IsWindow(hwndToCheck) == FALSE){
      bRet = FALSE;
      goto CheckWindowValidity_0;
    }
    // Ensure that the window is not the current one which has already been found.
    if (hwndToCheck == g_hwndFoundWindow){
      bRet = FALSE;
      goto CheckWindowValidity_0;
    }
    // It also must not be the "Search Window" dialog box itself.
    if (hwndToCheck == hwndDialog){
      bRet = FALSE;
      goto CheckWindowValidity_0;
    }
    // It also must not be one of the dialog box's children...
    hwndTemp = GetParent (hwndToCheck);
    if ((hwndTemp == hwndDialog)){
      bRet = FALSE;
      goto CheckWindowValidity_0;
    }
CheckWindowValidity_0:
    return bRet;
  }

  // Performs a highlighting of a found window.
  long HighlightFoundWindow(HWND hwndDialog, HWND hwndFoundWindow, HPEN g_hRectanglePen,const HFONT& myfont){
    HDC    hWindowDC = NULL;  // The DC of the found window.
    HGDIOBJ  hPrevPen = NULL;   // Handle of the existing pen in the DC of the found window.
    HGDIOBJ  hPrevBrush = NULL; // Handle of the existing brush in the DC of the found window.
    RECT  rect;          // Rectangle area of the found window.
    long  lRet = 0;
    // Get the screen coordinates of the rectangle of the found window.
    GetWindowRect (hwndFoundWindow, &rect);
    //InflateRect(&rect,2,2);
    // Get the window DC of the found window.
    hWindowDC = GetWindowDC (hwndFoundWindow);
    if (hWindowDC){
      // Select our created pen into the DC and backup the previous pen.
      hPrevPen = SelectObject (hWindowDC, g_hRectanglePen);
      // Select a transparent brush into the DC and backup the previous brush.
      hPrevBrush = SelectObject (hWindowDC, GetStockObject(HOLLOW_BRUSH));
      SelectObject (hWindowDC, myfont);
      // Draw a rectangle in the DC covering the entire window area of the found window.
      Rectangle (hWindowDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top);
      // Reinsert the previous pen and brush into the found window's DC.
      SelectObject (hWindowDC, hPrevPen);
      SelectObject (hWindowDC, hPrevBrush);
      // Finally release the DC.
      ReleaseDC (hwndFoundWindow, hWindowDC);
    }
    return lRet;
  }

  bool NotifyClientMng(int msg,const wchar_t* wndtitle,const char* MsgContents){
    HWND hwndTo = ::FindWindowEx(NULL,NULL,NULL,wndtitle);
    if (hwndTo){
      if (MsgContents){
        int slen = (int)strlen(MsgContents);
        ::memset((void*)(MsgContents+slen),'\0',1);
        COPYDATASTRUCT cds;
        cds.dwData = msg;
        cds.lpData = (PVOID)MsgContents;
        cds.cbData = slen+1;
        ::SendMessage(hwndTo,WM_COPYDATA,(WPARAM)0,(LPARAM)(LPVOID)&cds);
        return true;
      }else{
        COPYDATASTRUCT cds;
        cds.dwData = msg;
        cds.lpData = NULL;
        cds.cbData = NULL;
        ::SendMessage(hwndTo,WM_COPYDATA,(WPARAM)0,(LPARAM)(LPVOID)&cds);
        return true;
      }
    }else{
      return false;
    }
  }

  bool IsWindowActive(HWND hwnd) {
    WINDOWINFO info;
    return ::GetWindowInfo(hwnd, &info) &&
      ((info.dwWindowStatus & WS_ACTIVECAPTION) != 0);
  }

};

