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

#ifndef _STUB_SKELETON_UTIL_H_
#define _STUB_SKELETON_UTIL_H_

#include <atlbase.h>

#include <string>
#include <vector>

#include <algorithm>//transform
#include <cctype>//tolower,toupper

#include <map>
#include <hash_map>
#include <set>

#include <mmsystem.h>

#include "guard.h"

#include "6bees_window.h"//NotifyClientMng

using namespace std;

#define LOWERWSTR(x) transform(x.begin(), x.end(), x.begin(), (int (*)(int))tolower)
#define UPPERWSTR(x) transform(x.begin(), x.end(), x.begin(), (int (*)(int))toupper)

namespace _6bees_util{
  inline void quitfatalerror(const char* msg){MessageBoxA(NULL,msg,"ERROR",MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL);exit(1);}

  wstring ComVar2wstr(const CComVariant& x);
  
  bool ProcessURL(wstring& a);
  bool ProcessURLA(string& url);

  vector<wstring> Split(wstring line, wstring delims);
  vector<string> Split(string line, string delims);

  void GenMMFName(const string& fname,string& desc,string& tag,string& thumb);
  bool GetMMF(const string& name,string& contents);  

  string int2str(int num);
  wstring GenUniqStr();
  wstring GetErrorDescW(int errorcode);
  string GetErrorDescA(int errorcode);
  string GetMD5(const char* csBuffer);
  wstring FormatTime(long a);
  string GetEmailShort(const char* _email);
#if 0
  wstring GetFileVer(const wstring &FileName);
#endif
  long GetMuteValue(HMIXER hmixer ,MIXERCONTROL *mxc);
  bool WINAPI GetMute(long dev);
  bool GetVolumeControl(HMIXER hmixer,long componentType,long ctrlType,MIXERCONTROL* mxc);
  bool WINAPI SetMute(long dev,long vol);
  bool SetMuteValue(HMIXER hmixer,MIXERCONTROL *mxc,bool mute);
  BOOL SetHttpProxy(string ip, string port);
  BOOL GetStartupregistry(map<string,string> &a,map<string,string> &b);
  BOOL ClearRunOnce();
  BOOL GetStartupFolderFiles(set<string> &startup_lnk);
  
  void GetAllPluginAbsPath(const wchar_t* dirpath,vector<wstring>& ultras);
  UINT HexToDec(const wchar_t *szValue);
  bool IsReservedName(const wstring& filename);
  bool IsShellIntegratedExtension(const wstring& extension);
  wstring GetUltraPath(const wchar_t* source,bool appendslash);
  bool WriteCharToFile_UTF8(const wchar_t* data,const wchar_t* saveFileName,bool flag);
  bool MakeSureDir(const wchar_t* dirpath);
  wstring GetIconPath(const string& urla_);
  string GetIconUltraURL(const string& urla_);
  std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
  std::string base64_decode(std::string const& encoded_string);
  DWORD FileCrc32Assembly(LPCTSTR szFilename, DWORD &dwCrc32);

  /// @brief the dir class for cache directory
  /// cache/picture, cache/video, cache/audio, cache/html
  class ultrapath{
    DISALLOW_IMPLICIT_CONSTRUCTORS(ultrapath);
    static bool initialized;
    static ultrapath* pinstance;
    static wchar_t cstr[];
    static cmutex lock_;
  public:
    enum FileDirEnum{
      cachedir=0,webcontentsdir,tempicondir,picdir,audiodir,videodir,tempdir,plugindir,
      maindb,uploader,recorder,capvideo,htmleditor,clientmng,resdll,
      uutoolbardll
    };
  private:
    stdext::hash_map<FileDirEnum,wstring> _m;
  public:
    ~ultrapath();
    static ultrapath& R();
    const wstring& get(FileDirEnum);//true - has trailing slash
    string getA(FileDirEnum);
  };

  /// @brief a small helper class for load lib
  class auto_libloader{
    HMODULE hlib_;
    DISALLOW_IMPLICIT_CONSTRUCTORS(auto_libloader);
  public:
    explicit auto_libloader(const HMODULE& _hlib){
      hlib_ = _hlib;
    }
    ~auto_libloader(){
      if (hlib_){
        FreeLibrary(hlib_);
      }
    }
    HMODULE GetLibHandle(){
      return hlib_;
    }
  };

  class timer_notifier{
    int start_;//CPYDATA_DOWNLOADIEFAV_START
    int end_;//CPYDATA_DOWNLOADBUSY_STOP
    DISALLOW_COPY_AND_ASSIGN(timer_notifier);
  public:
    explicit timer_notifier(int start,int end):start_(start),end_(end){
      _6bees_window::NotifyClientMng(start_, _6bees_const::kUploader_WndTitle,NULL);
    }
    ~timer_notifier(){
      _6bees_window::NotifyClientMng(end_, _6bees_const::kUploader_WndTitle,NULL);
    }
  };

  /// @brief a singleton class for Resource Management including Image(icon/bitmap)/String/Menu.
  ///  For PNG/GIF, which need gdiplus, leave them in the original project.
  ///  For Dialog, which WTL class will call it direclty, leave them there.
  ///  todo: Multithreading ...???
  class c6beeres{
  private:
    static bool initialized;
    static c6beeres* pinstance;
    static cmutex lock_;

    c6beeres();
    DISALLOW_COPY_AND_ASSIGN(c6beeres);
  public:
    static HMODULE hModule;
    static c6beeres& R();

    HANDLE GetImg(int resid,short restype=IMAGE_ICON,int x=16,int y=16);
    //HIMAGELIST GetImgList(int resid,int width=16);
    HMENU GetMenu(int resid);
    ~c6beeres();
  };

};




#endif
