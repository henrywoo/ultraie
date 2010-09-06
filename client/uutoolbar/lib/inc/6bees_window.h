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