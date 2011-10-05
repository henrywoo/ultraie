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

#include "stdafx.h"
#include "6bees_util.h"
#include <time.h>//time
#include <direct.h>//_mkdir
#include <wininet.h>
#include "6bees_str.h"
#include "6bees_net.h"
#include <comdef.h>
//#include <comdefsp.h>//_bstr_t
#include <shlobj.h>//CSIDL_APPDATA,SHGetSpecialFolderPathW

#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"version.lib")//GetFileVersion

namespace _6bees_util{

  using namespace _6bees_const;
  using namespace _6bees_str;

  wstring ComVar2wstr(const CComVariant& x){
    CComVariant y = x;
    CComBSTR tmp;
    if (SUCCEEDED(y.ChangeType(VT_BSTR)))
      tmp = y.bstrVal;
    _bstr_t bstr = tmp;
    return wstring((wchar_t*)bstr);
  }





  vector<wstring> Split(wstring line, wstring delims){
    wstring::size_type bi, ei;
    vector<wstring> words;
    bi = line.find_first_not_of(delims);
    while (bi != wstring::npos) {
      ei = line.find_first_of(delims, bi);
      if (ei == wstring::npos) {
        ei = line.length();
      }
      words.push_back(line.substr(bi, ei - bi));
      bi = line.find_first_not_of(delims, ei);
    }
    return words;
  }

  vector<string> Split(string line, string delims){
    string::size_type bi, ei;
    vector<string> words;
    bi = line.find_first_not_of(delims);
    while (bi != string::npos) {
      ei = line.find_first_of(delims, bi);
      if (ei == string::npos) {
        ei = line.length();
      }
      words.push_back(line.substr(bi, ei - bi));
      bi = line.find_first_not_of(delims, ei);
    }
    return words;
  }

  /// Generate Memory Mapping File Name pointing to different contents
  /// @param fname - filename
  /// @param desc  - MMF name pointing to description of file
  /// @param tag   - MMF name pointing to tag of file
  /// @param thumb - MMF name pointing to thumb filepath
  void GenMMFName(const string& fname,string& desc,string& tag,string& thumb){
    desc  = fname + "_desc";
    tag   = fname + "_tag";
    thumb = fname + "_thumb";
  }

 
  

  string int2str(int num){ 
    if(num == 0) 
      return "0"; 
    string str;
    while(num){ 
      str = (char)(num % 10 + 48) + str; 
      num /= 10; 
    }
    return str;
  }

  wstring GenUniqStr(){
    static int count=0;
    return wstringmaker() << L"_ultraIE.com_" << (int)time(0)+(++count);
  }

  wstring GetErrorDescW(int errorcode){
    LPVOID lpMsgBuf=NULL;
    FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER|
      FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      errorcode,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      reinterpret_cast<wchar_t*>(&lpMsgBuf),
      0,
      NULL);
    wstring strErrorDescription;
    if( lpMsgBuf ){
      strErrorDescription = reinterpret_cast<wchar_t*>(lpMsgBuf);
      LocalFree(lpMsgBuf);
    }
    return strErrorDescription;
  }

  string GetErrorDescA(int iErrorCode){
    return stringmaker(CP_ACP) << GetErrorDescW(iErrorCode);
  }

  string GetMD5(const char* csBuffer){
    static DWORD dwHashLen= 16; // The MD5 algorithm always returns 16 bytes.
    HCRYPTPROV hCryptProv;
    HCRYPTHASH hHash;
    BYTE bHash[0x7f];
    DWORD cbContent= (DWORD)strlen(csBuffer);
    BYTE* pbContent= (BYTE*)csBuffer;
    string result;
    if(CryptAcquireContext(&hCryptProv,NULL,NULL,PROV_RSA_FULL,
      CRYPT_VERIFYCONTEXT|CRYPT_MACHINE_KEYSET)){
        // algorithm identifier definitions see: wincrypt.h
        if(CryptCreateHash(hCryptProv,CALG_MD5,0,0,&hHash)){
          if(CryptHashData(hHash, pbContent, cbContent, 0)){
            if(CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0)){
              // Make a string version of the numeric digest value
              char tmp[16];
              for (int i = 0; i<(int)dwHashLen; i++){
                //tmp.Format("%02x", bHash[i]);
                sprintf(tmp,"%02x",bHash[i]);
                result+=tmp;
              }
            }
          }
        }
    }
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    return result;
  }

  // a - by second
  wstring FormatTime(long a){
    if(a<60){
      return wstringmaker() << a << L"s";
    }else if(a>=60 && a<3600){
      long m = a/60;
      long s = a - 60*m;
      return wstringmaker() << m << L"m " << s << L"s";
    }else if(a>=3600 && a<24*60*60){
      long h = a/3600;
      long m = (a - 3600*h)/60;
      long s = a - 3600*h - 60*m;
      return wstringmaker() << h << L"h " << m << L"m" << s << L"s";
    }else{
      long d = a/(3600*24);
      long h = (a - 3600*24*d)/3600;
      long m = (a - 3600*24*d - 3600*h)/60;
      long s = a - 3600*24*d - 3600*h - 60*m;
      return wstringmaker() << d << "d" << h << L"h " << m << L"m" << s << L"s";
    }
  }

  string GetEmailShort(const char* _email){
    string email = _email;
    string::size_type i = email.find('@');
    string::size_type j = email.find_last_of('.');
    if (i==string::npos || j==string::npos){
      return "";
    }
    email.replace(i,j-i,"@...");
    return email;
  }

#if 0
  wstring GetFileVer(const wstring &FileName){ 
    UINT jj = sizeof( VS_FIXEDFILEINFO );
    TCHAR *pp, pp2[100];
    wstring ss;
    wstring sFileName;
    VS_FIXEDFILEINFO *aa;
    void **pp1 = (void **)&aa;

    sFileName=FileName;
    pp2[0] = 0;
    ss=_T("");
    int ii = ::GetFileVersionInfoSize(sFileName.c_str(), NULL );
    if( ii > 0 ){
      pp = new TCHAR[ii];
      if(::GetFileVersionInfo(sFileName.c_str(),0, ii, pp )){
        if( ::VerQueryValue( pp, _T("\\"), pp1, &jj ) ){
          ss = wstringmaker() << HIWORD(aa->dwFileVersionMS) << L"."
            << LOWORD(aa->dwFileVersionMS) << L"."
            << HIWORD(aa->dwFileVersionLS) << L"."
            << LOWORD(aa->dwFileVersionLS);
          //ss.Format(_T("%d.%d.%02d.%04d"),HIWORD(aa->dwFileVersionMS),
          //  LOWORD(aa->dwFileVersionMS),HIWORD(aa->dwFileVersionLS),LOWORD(aa->dwFileVersionLS));
        }
      }
      delete [] pp;
    }
    return ss;
  }
#endif

  bool GetVolumeControl(HMIXER hmixer,long componentType,long ctrlType,MIXERCONTROL* mxc){  
    MIXERLINECONTROLS   mxlc;  
    MIXERLINE   mxl;  
    mxl.cbStruct   =   sizeof(mxl);  
    mxl.dwComponentType   =   componentType;  
    if(!mixerGetLineInfo((HMIXEROBJ)hmixer,   &mxl,   MIXER_GETLINEINFOF_COMPONENTTYPE)){  
      mxlc.cbStruct   =   sizeof(mxlc);  
      mxlc.dwLineID   =   mxl.dwLineID;  
      mxlc.dwControlType   =   ctrlType;  
      mxlc.cControls   =   1;  
      mxlc.cbmxctrl   =   sizeof(MIXERCONTROL);  
      mxlc.pamxctrl   =   mxc;  
      return !mixerGetLineControls((HMIXEROBJ)hmixer,&mxlc,MIXER_GETLINECONTROLSF_ONEBYTYPE); 
    }  
    return false;
  }

  long GetMuteValue(HMIXER hmixer ,MIXERCONTROL *mxc){
    MIXERCONTROLDETAILS mxcd;
    MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
    mxcd.hwndOwner = 0;
    mxcd.cbStruct = sizeof(mxcd);
    mxcd.dwControlID = mxc->dwControlID;
    mxcd.cbDetails = sizeof(mxcdMute);
    mxcd.paDetails = &mxcdMute;
    mxcd.cChannels = 1;
    mxcd.cMultipleItems = 0;
    if (mixerGetControlDetails((HMIXEROBJ)hmixer, &mxcd,MIXER_OBJECTF_HMIXER|MIXER_GETCONTROLDETAILSF_VALUE))
      return -1;
    return mxcdMute.fValue;
  }

  bool WINAPI GetMute(long dev){//检查设备是否静音
    //dev =0,1,2 分别表示主音量，波形,MIDI ,LINE IN
    /// retrun false 表示没有静音
    /// retrun true 表示静音
    long device;
    bool rc=false;
    MIXERCONTROL volCtrl;
    HMIXER hmixer;
    switch (dev)  {
  case 1:
    device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT; break;
  case 2:
    device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER; break;
  case 3:
    device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; break;
  default:
    device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
    }
    if(mixerOpen(&hmixer, 0, 0, 0, 0)) return 0;
    if(GetVolumeControl(hmixer,device,MIXERCONTROL_CONTROLTYPE_MUTE,&volCtrl))
      rc=GetMuteValue(hmixer,&volCtrl)==0;
    mixerClose(hmixer);
    return rc;
  }

  bool WINAPI SetMute(long dev,long vol){//设置设备静音
    ///   dev   =0,1,2       分别表示主音量,波形,MIDI   ,LINE   IN  
    ///   vol=0,1             分别表示取消静音,设置静音  
    ///   retrun   false   表示取消或设置静音操作不成功  
    ///   retrun   true     表示取消或设置静音操作成功  
    long   device;  
    bool   rc=false;  
    MIXERCONTROL   volCtrl;  
    HMIXER   hmixer;  
    switch(dev){  
  case   1:  
    device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;   break;  
  case   2:  
    device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;   break;  
  case   3:  
    device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;
  default:  
    device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;  
    }  

    if(mixerOpen(&hmixer,   0,   0,   0,   0))   return   0;  
    if(GetVolumeControl(hmixer,device,MIXERCONTROL_CONTROLTYPE_MUTE,&volCtrl))  
      if(SetMuteValue(hmixer,&volCtrl,(bool)(vol==1)))  
        rc=true;  
    mixerClose(hmixer);  
    return   rc;  
  }

  //---------------------------------------------------------------------------  
  bool   SetMuteValue(HMIXER   hmixer   ,MIXERCONTROL   *mxc,   bool   mute)  
  {  
    MIXERCONTROLDETAILS   mxcd;  
    MIXERCONTROLDETAILS_BOOLEAN   mxcdMute;mxcdMute.fValue=mute;  
    mxcd.hwndOwner   =   0;  
    mxcd.dwControlID   =   mxc->dwControlID;  
    mxcd.cbStruct   =   sizeof(mxcd);  
    mxcd.cbDetails   =   sizeof(mxcdMute);  
    mxcd.paDetails   =   &mxcdMute;  
    mxcd.cChannels   =   1;  
    mxcd.cMultipleItems   =   0;  
    if   (mixerSetControlDetails((HMIXEROBJ)hmixer,   &mxcd,   MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE))  
      return false;  
    return true;
  }

  BOOL SetHttpProxy(string ip, string port){
    string l_just = "http=" + ip + ":" + port;
    //l_just.Format(_T("http=%s:%d"), ip.GetString(), port);
    //下面的代码将注册表项
    //HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ProxyServer的内容取出来
    HKEY hKeyIn = HKEY_CURRENT_USER, hKeyOut;
    if(ERROR_SUCCESS!=RegOpenKeyExA(hKeyIn,"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",0,KEY_CREATE_LINK|KEY_WRITE|KEY_READ|KEY_NOTIFY,&hKeyOut)){
      return FALSE;
    }
    ULONG regsize = 0;
    if(ERROR_SUCCESS!=RegQueryValueExA(hKeyOut, "ProxyServer", NULL, NULL, NULL, &regsize)){
      return FALSE;
    }

    LPBYTE pValue = new BYTE[regsize];
    memset(pValue, 0x00, regsize);
    if(ERROR_SUCCESS!=RegQueryValueExA(hKeyOut,"ProxyServer", NULL, NULL, pValue, &regsize)){
      return FALSE;
    }
    string oldproxy((char *)pValue);
    delete [] pValue;
    pValue = NULL;
    //从注册表中读出来的数据格式为:http=111.111.111.111:80;ftp=222.222.222.222:21;......,
    //如果你只想改变http的代理的话， 就只要把其中的111.111.111.111:80换成你想要的代理就行了，
    //类似的你可以改变其他的代理.
    //下面的代码就替换http代理成为参数所指定的代理.
    int pos = 0;
    //如果没有字符串中没有找到"http="说明用户没有设置http代理，这时候直接加在最前面.
    if(oldproxy.find("http=") == string::npos){
      pos = 0;
    }
    int pos1 = 0;
    if(oldproxy.find(";", pos)==string::npos){
      pos1 = (int)oldproxy.size();
    }
    oldproxy.erase(pos, pos1 - pos);
    oldproxy.insert(pos, l_just.c_str());
    if(ERROR_SUCCESS != RegSetValueExA(hKeyOut, "ProxyServer", 0, REG_SZ, (const unsigned char *)oldproxy.c_str(), (long)oldproxy.size() + 1)){
      return FALSE;
    }
    RegCloseKey(hKeyOut);
    //make setting effective
    if(!InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0)){
      return FALSE;
    }
    return TRUE;
  }

  

  

  BOOL GetStartupFolderFiles(set<string> &startup_lnk){
    HKEY hKey;
    DWORD lResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders\\", 0, KEY_ALL_ACCESS,&hKey);
    if (lResult == ERROR_SUCCESS){
      char szlpData[MAX_PATH+1];
      DWORD  cbData = MAX_PATH, dwType = REG_SZ;
      ::RegQueryValueExA(hKey,"Startup",NULL,&dwType,(LPBYTE)szlpData,&cbData);
      RegCloseKey(hKey);
      string startupfile = szlpData;
      string startupfolder = szlpData;
      startupfolder += "\\*";
      WIN32_FIND_DATAA ffd;
      HANDLE hFind;
      hFind= FindFirstFileA(startupfolder.c_str(), &ffd);
      if (INVALID_HANDLE_VALUE != hFind){
        while(FindNextFileA(hFind, &ffd) != 0){
          if(strcmp(ffd.cFileName,"..")!=0){
            startup_lnk.insert(startupfile + "\\" +ffd.cFileName);
          }
        }
      }
      FindClose(hFind);
    }

    //////////////////////////////////////////////////////////////////////////////
    lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders\\", 0, KEY_ALL_ACCESS,&hKey);
    if (lResult == ERROR_SUCCESS){
      char szlpData[MAX_PATH+1];
      DWORD  cbData = MAX_PATH, dwType = REG_SZ;
      ::RegQueryValueExA(hKey,"Common Startup",NULL,&dwType,(LPBYTE)szlpData,&cbData);
      RegCloseKey(hKey);
      string startupfile = szlpData;
      string startupfolder = szlpData;
      startupfolder += "\\*";
      WIN32_FIND_DATAA ffd;
      HANDLE hFind;
      hFind= FindFirstFileA(startupfolder.c_str(), &ffd);
      if (INVALID_HANDLE_VALUE != hFind){
        while(FindNextFileA(hFind, &ffd) != 0){
          if(strcmp(ffd.cFileName,"..")!=0){
            startup_lnk.insert(startupfile + "\\" +ffd.cFileName);
          }
        }
      }
      FindClose(hFind);
    }
    return FALSE;
  }

  










  bool WriteCharToFile_UTF8(const wchar_t* data,const wchar_t* saveFileName,bool flag){
    const wchar_t* pwszUnicode = data;
    char* pszMultiByte=0;
    try{
      int iSize = WideCharToMultiByte(CP_UTF8, 0, pwszUnicode, -1, NULL, 0, NULL, NULL);
      if (iSize==0){
        return false;
      }
      pszMultiByte = (char*)malloc((iSize+1)/**sizeof(char)*/);
      iSize = WideCharToMultiByte(CP_UTF8, 0, pwszUnicode, -1, pszMultiByte, iSize, NULL, NULL);
      if (iSize==0){
        free(pszMultiByte);
        return false;
      }
      HANDLE hFile = CreateFile(saveFileName,GENERIC_WRITE,FILE_SHARE_READ,NULL,
        flag?CREATE_ALWAYS:OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
      if (INVALID_HANDLE_VALUE == hFile)
        _tprintf(_T("Open File Failed!\n"));
      DWORD dwWrite=0;
      static const char convBuff[3]={(char)0xEF,(char)0xBB,(char)0xBF};//Unicode BOM
      ::WriteFile(hFile,convBuff, 3, &dwWrite, NULL);
      ::WriteFile(hFile,pszMultiByte,iSize,&dwWrite,NULL);
      ::CloseHandle(hFile);
      free(pszMultiByte);
    }catch(...){
      return false;
    }
    return true;
  }







 



};

