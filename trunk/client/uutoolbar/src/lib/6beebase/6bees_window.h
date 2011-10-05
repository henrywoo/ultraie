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

#include <windows.h>
#include <vector>
#include <string>
#include "6bees_const.h"

namespace _6bees_window{
  using std::vector;

  namespace IE8{
    void GetBarHeight(HWND hWnd_HTMLClientArea,int& statusbarheight,int& shuipinbarheight);
    int IEProcNumbers();
    void CollectIEFrame(vector<HWND>& vIE);
    HWND GetReflectionWnd();
    void Notify6BeeTB(int msg,HWND myself);
  };

  namespace IE7{
    void GetBarHeight(HWND hWnd_HTMLClientArea,int& statusbarheight,int& shuipinbarheight);
    int IEProcNumbers();

    HWND FindReflectwndFromSBar(HWND h);
    void CollectIEFrame(vector<HWND>& vIE);
    void CollectShellFolderWnd(vector<HWND>& vShellFolder);
    void CollectITBarHost(HWND par,vector<HWND>& v6BeeTB);
    void GetAll6BeeTB(vector<HWND>& v6BeeTB);
    std::wstring GetAddressBarText(HWND);

    // eg:
    //  _6bees_window::IE7::Notify6BeeTB(CPYDATA_LOGIN,NULL);
    void Notify6BeeTB(int msg,HWND myself);

    HWND GetReflectionWnd();
  };

  namespace IE6{
    void GetBarHeight(HWND hWnd_HTMLClientArea,int& statusbarheight,int& shuipinbarheight);
    int IEProcNumbers();
    HWND GetReflectionWnd();
    void CollectIEFrame(vector<HWND>& vIE);
    void Notify6BeeTB(int msg,HWND myself);
  };

  void RefreshWindow (HWND h);
  BOOL CheckWindowValidity (HWND hwndDialog, HWND hwndToCheck, HWND g_hwndFoundWindow);
  long HighlightFoundWindow(HWND hwndDialog, HWND hwndFoundWindow, HPEN g_hRectanglePen,const HFONT& myfont);

  bool NotifyClientMng(int msg,const wchar_t* wndtitle,const char* MsgContents);
  bool IsWindowActive(HWND hwnd);

};