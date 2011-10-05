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