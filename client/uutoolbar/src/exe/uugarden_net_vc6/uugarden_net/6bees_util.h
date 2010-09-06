#ifndef _STUB_SKELETON_UTIL_H_
#define _STUB_SKELETON_UTIL_H_

#include <atlbase.h>

#include <string>
#include <vector>

#include <algorithm>//transform
#include <cctype>//tolower,toupper

#include <map>
//#include <hash_map>
#include <set>

#include <mmsystem.h>

//#include "guard.h"

//#include "6bees_window.h"//NotifyClientMng

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
  UINT HexToDec(const wchar_t *szValue);
 


};




#endif
