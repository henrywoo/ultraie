#ifndef _6BEECOMMOND_DLL_H_
#define _6BEECOMMOND_DLL_H_

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

#ifdef MY6BEECOMMOND_EXPORTS
#define MY6BEECOMMOND_API __declspec(dllexport)
#else
#define MY6BEECOMMOND_API __declspec(dllimport)
#endif

#include <atlbase.h>
#include <commctrl.h>
#include "../../lib/6beebase/6bees_const.h"
#include "../../lib/6beebase/guard.h"

#define ISTRUE(x)  (strcmp(x,"true")==0)

namespace _6beed_util{
  extern "C" {

    static const int MMF_SIZE = 10*1024;
    // 1. Functions
    MY6BEECOMMOND_API void runIE(const wchar_t*);
    MY6BEECOMMOND_API bool urlencode(char* result, int bufsize, const char* rawurl);
    MY6BEECOMMOND_API bool urldecode_gb2312(char *dest, int destsize,const char* src);
    MY6BEECOMMOND_API bool GetFileSizeA(const char* fpath, int& fsize);
    MY6BEECOMMOND_API bool GetFileSizeW(const wchar_t* fpath, int& fsize);
    MY6BEECOMMOND_API const wchar_t* Get6BEEPath(const wchar_t* source=0,
      bool appendslash=false);
    MY6BEECOMMOND_API const char* Get6BEEPathA(const wchar_t* source=0,
      bool appendslash=false);
    MY6BEECOMMOND_API void SendMsgToUploader(HWND hwndTo, const char* filelist,
      int msg/*=CPYDATA_UPLOAD_2_UPLOAD*/);
    MY6BEECOMMOND_API char* GetIEHomePage();
    MY6BEECOMMOND_API wchar_t* GetIEHomePageW();
    MY6BEECOMMOND_API bool gettimestr(char* tstr,int len);
    MY6BEECOMMOND_API const char* getLangEncoding();
    MY6BEECOMMOND_API _6bees_const::OST GetOS();
    MY6BEECOMMOND_API bool IsXPTheme();
    MY6BEECOMMOND_API void CallExeW(const wchar_t* c);
    MY6BEECOMMOND_API void CallExeA(const char* c);
    MY6BEECOMMOND_API bool SetMMF(const wchar_t* name,const wchar_t* contents,int bufsize=MMF_SIZE);
    MY6BEECOMMOND_API bool SetMMFA(const char* name,const char* contents,int bufsize=MMF_SIZE);
    MY6BEECOMMOND_API void DrawGradient(HDC hdc,LPRECT lpRect,COLORREF rgbTop,
      COLORREF rgbBottom);
    MY6BEECOMMOND_API void DrawRoundRect(HDC hdc,LPRECT rcRect,COLORREF rgbOuter,
      COLORREF rgbInner,COLORREF m_rgbBackground);
    MY6BEECOMMOND_API bool isChineseOS();
    struct versionblk{
      unsigned short wMajor;
      unsigned short wMinor;
      unsigned short wRelease;
      unsigned short wBuild;
    };
    MY6BEECOMMOND_API versionblk GetVersionBlk(const wchar_t* sFileName);
    MY6BEECOMMOND_API void InvokeUploader(const wchar_t* filepath,const wchar_t* desc,
      const wchar_t* tag,const wchar_t* thumbpath);
    MY6BEECOMMOND_API bool Login_v2();
    MY6BEECOMMOND_API bool QuietUploadFile(const wchar_t* localname,
      const wchar_t* remotename,
      const wchar_t* id,
      _6bees_const::UPLOADERMODE mode);
    MY6BEECOMMOND_API void Notify6BeeTB(int msg,HWND h=NULL);
    MY6BEECOMMOND_API HWND GetReflectionWnd();
    MY6BEECOMMOND_API void CheckVersion(const wchar_t*);

  };
};

/// @brief shared variables with different processes
namespace _6beed_share{
  extern "C" {

    enum UUCONFIG{
      U_6BEEVER,U_NAME,U_PASS,U_SEBOXLEN,U_LOGGEDIN,U_AUTOLOGIN,
      U_SID,U_IE_ONEWINDOW,U_IE_BUILTINSEBOX,U_LOADFOREXPLORER
    };

    struct gconf{
      gconf(int);
      char version[10+1];//99.999.999
      char sid[36+1];
      char email[50];
      char password[32+1];

      bool loggedin;
      bool autologin;
      int searchboxlen;
      bool ie_hidebuiltinSEBox;
      bool ie_onewindow;
      bool loadforexplorer;

      // C:/Program Files/6BeeNetWorks/UltraIE/
      wchar_t uhome_[_MAX_PATH];

	    short ieversion;
    };

    MY6BEECOMMOND_API const char* get_version();
    MY6BEECOMMOND_API const wchar_t* get_uhome();

    MY6BEECOMMOND_API bool u_getLoggedin();
    MY6BEECOMMOND_API void u_setLoggedin(bool);
    MY6BEECOMMOND_API bool u_getAutologin();
    MY6BEECOMMOND_API void u_setAutologin(bool);

    MY6BEECOMMOND_API const char* u_getSID();
    MY6BEECOMMOND_API void u_setSID(const char*);
    MY6BEECOMMOND_API const char* u_getPassword();
    MY6BEECOMMOND_API void u_setPassword(const char*);
    MY6BEECOMMOND_API const char* u_getEmail();
    MY6BEECOMMOND_API void u_setEmail(const char*);

    MY6BEECOMMOND_API int u_getSearchboxlen();
    MY6BEECOMMOND_API void u_setSearchboxlen(int len);

    MY6BEECOMMOND_API bool u_getiebuiltinSEBox();
    MY6BEECOMMOND_API bool u_setiebuiltinSEBox(bool b,bool isupdatedb=true);
    MY6BEECOMMOND_API bool u_getieonewindow();
    MY6BEECOMMOND_API bool u_setieonewindow(bool);
    MY6BEECOMMOND_API void update_GeneralSetting();

    MY6BEECOMMOND_API bool u_getloadforexplorer();
    MY6BEECOMMOND_API bool u_setloadforexplorer(bool);

    MY6BEECOMMOND_API short u_getieversion();
    MY6BEECOMMOND_API void u_setieversion(short);
  };
};
#endif
