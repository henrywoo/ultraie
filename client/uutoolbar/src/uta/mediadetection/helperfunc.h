#ifndef __UUTOOLBAR_MEDIADETECTION_Helper__
#define __UUTOOLBAR_MEDIADETECTION_Helper__

#include "../../dll/6beecommond/6beecommond.h"
#include <string>
#include <set>
#include <map>

namespace nsplugin{

  using namespace _6beed_util;
  using namespace std;

  enum METYPE{FLV,SWF,RM,MP3,WMV,WMA,AVI,MTYPENUM};

  //helper function
  string GetDBPath();
  bool ReadSnipppet(const wchar_t* _filepath,wstring& _contents);

  typedef map<DWORD,set<wstring>> TIDMEDIAURLMAP;
  class mediabythread{
    TIDMEDIAURLMAP tid2mediaurl_;
    mediabythread(){}
  public:
    static mediabythread& R();
    TIDMEDIAURLMAP& GetMap();

    bool HasThisURL(const wstring& _url);
    void AddURL(const wstring& _url);
  };

  static const wchar_t tyname[MTYPENUM][4]={
    _T("flv"), _T("swf"), _T("rm"), _T("mp3"), _T("wmv"), _T("wma"), _T("avi")
  };

  struct CMediaRes{
    std::wstring title;
    std::wstring mediaurl;
    RECT rct;
    METYPE type;

    bool direct;
    int size;
  };

  //bool FilterMediaURL(const wchar_t* szUrl);

  UINT WINAPI thread_mediares(LPVOID p);
  int GetMimeResID(METYPE t);

  void NotifyUserMediaRes(HWND h,CMediaRes& mr);
  //void ProcessMediaURL(const wchar_t* szurl,METYPE mt);

  void InsertMedia(wstring title,wstring webpage,wstring mediaurl,wstring format);
  bool IsDirect(const std::string& a,METYPE b);

};

#endif