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

#include "6bees_util.h"
#include "uulogging.h"
#include <time.h>//time
#include <direct.h>//_mkdir
#include <wininet.h>
#include "6bees_str.h"
#include "6bees_net.h"
#include <comdef.h>
#include <comdefsp.h>//_bstr_t
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

  bool ProcessURL(wstring& a){
    wstring b,c;
    size_t i=0,j=0;
    do{
      if ((i=a.find(_T("../"))) != wstring::npos){
        c=a.substr(i);
        a=a.erase(i,3);
        if (j==0){
          b=a.substr(0,i-1);
        }
        size_t k=b.rfind(_T("/"));
        if (k==wstring::npos){
          // that means errors happen
          // http://www.cppblog.com/../../../../../Modules/CaptchaImage/JpegImage.aspx
          _6bees_str::strreplace(a,L"../",L"");
          return true;
        }
        b = b.erase(k);
        j++;
      }
    }while(i!=wstring::npos);

    if (j>0 && c.size()>=2)
      a = b+c.substr(2);
    return true;
  }

  bool ProcessURLA(string& url){
    string b,c;
    size_t i=0,j=0;
    do{
      if ((i=url.find("../")) != string::npos){
        c=url.substr(i);
        url.erase(i,3);
        if (j==0){
          b=url.substr(0,i-1);
        }
        size_t k=b.rfind("/");
        if (k==string::npos){
          // that means errors happen
          // http://www.cppblog.com/../../../../../Modules/CaptchaImage/JpegImage.aspx
          _6bees_str::strreplace(url,"../","");
          return true;
        }
        b.erase(k);
        j++;
      }
    }while(i!=string::npos);
    if (j>0 && c.size()>=2)
      url = b + c.substr(2);
    return true;
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

  /// Get Contents of Memory Mapping File
  bool GetMMF(const string& MMFName,string& contents){
#ifdef _DEBUG
    UUDEBUG((LOG_ERROR,"------------- GetMMF --------------"));
    UUDEBUG((LOG_ERROR,MMFName.c_str()));
#endif
    HANDLE hFileMapT = OpenFileMappingA(FILE_MAP_READ|FILE_MAP_WRITE,FALSE,MMFName.c_str());
    if (hFileMapT){
      char* pView = reinterpret_cast<char *>(MapViewOfFile(hFileMapT, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0));
      if(pView){
        contents = pView;
#ifdef _DEBUG
        UUDEBUG((LOG_ERROR,contents.c_str()));
#endif
        UnmapViewOfFile(pView);
        CloseHandle(hFileMapT);
        return true;
      }
    }return false;
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
    return wstringmaker() << (int)time(0)+(++count);
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
              char tmp[16]={};
              for (int i = 0; i<(int)dwHashLen; i++){
                //tmp.Format("%02x", bHash[i]);
                sprintf_s(tmp,16,"%02x",bHash[i]);
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

  BOOL GetStartupregistry(map<string,string>& startupregistry_localmachine,
    map<string,string>& startupregistry_currentuser)
  {
    HKEY hKey;
    BOOL bResult = TRUE;
    DWORD lResult;
    lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\", 0, KEY_QUERY_VALUE,&hKey);
    if (lResult != ERROR_SUCCESS){
      return FALSE;
    }
    for (int i = 0; ;i++){
      char szValueName[MAX_PATH+1];
      char szlpData[MAX_PATH+1];
      DWORD  cbData = MAX_PATH;
      DWORD dwValueName = sizeof(szValueName);
      DWORD dwValueType = 0;
      lResult = ::RegEnumValueA(hKey,i,szValueName,&dwValueName, NULL,&dwValueType,NULL,NULL);
      if (lResult != ERROR_SUCCESS){
        if (lResult != ERROR_NO_MORE_ITEMS) bResult = FALSE;
        break;
      }
      szValueName[dwValueName] = '\0';
      string rkey = szValueName;
      DWORD dwType = REG_SZ;
      ::RegQueryValueExA(hKey,rkey.c_str(),NULL,&dwType,(LPBYTE)szlpData,&cbData);
      string rvalue = szlpData;
      startupregistry_localmachine[rkey] = rvalue;
    }
    RegCloseKey(hKey);

    lResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\", 0, KEY_QUERY_VALUE,&hKey);
    if (lResult != ERROR_SUCCESS){
      return FALSE;
    }
    for (int i = 0; ;i++){
      char szValueName[MAX_PATH+1];
      char szlpData[MAX_PATH+1];
      DWORD  cbData = MAX_PATH;
      DWORD dwValueName = sizeof(szValueName);
      DWORD dwValueType = 0;
      lResult = ::RegEnumValueA(hKey,i,szValueName,&dwValueName, NULL,&dwValueType,NULL,NULL);
      if (lResult != ERROR_SUCCESS){
        if (lResult != ERROR_NO_MORE_ITEMS) bResult = FALSE;
        break;
      }
      szValueName[dwValueName] = '\0';
      string rkey = szValueName;
      DWORD dwType = REG_SZ;
      ::RegQueryValueExA(hKey,rkey.c_str(),NULL,&dwType,(LPBYTE)szlpData,&cbData);
      string rvalue = szlpData;
      startupregistry_currentuser[rkey] = rvalue;
    }
    RegCloseKey(hKey);
    return TRUE;
  }

  BOOL ClearRunOnce(){
    HKEY hKey;
    BOOL bResult = TRUE;
    DWORD lResult = 0;
    lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce\\", 0, KEY_ALL_ACCESS,&hKey);
    if (lResult != ERROR_SUCCESS){
      return FALSE;
    }
    for (int i = 0; ;i++){
      char szValueName[MAX_PATH+1];
      DWORD  cbData = MAX_PATH;
      DWORD dwValueName = sizeof(szValueName);
      DWORD dwValueType = 0;
      lResult = ::RegEnumValueA(hKey,i,szValueName,&dwValueName, NULL,&dwValueType,NULL,NULL);
      if (lResult != ERROR_SUCCESS){
        if (lResult != ERROR_NO_MORE_ITEMS) bResult = FALSE;
        break;
      }
      szValueName[dwValueName] = '\0';
      ::RegDeleteValueA(hKey,szValueName);
    }
    RegCloseKey(hKey);

    //////////////////////////////////////////////////////////////////////////////
    lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx\\", 0, KEY_ALL_ACCESS,&hKey);
    if (lResult != ERROR_SUCCESS){
      return FALSE;
    }
    for (int i = 0; ;i++){
      char szValueName[MAX_PATH+1];
      DWORD  cbData = MAX_PATH;
      DWORD dwValueName = sizeof(szValueName);
      DWORD dwValueType = 0;
      lResult = ::RegEnumValueA(hKey,i,szValueName,&dwValueName, NULL,&dwValueType,NULL,NULL);
      if (lResult != ERROR_SUCCESS){
        if (lResult != ERROR_NO_MORE_ITEMS) bResult = FALSE;
        break;
      }
      szValueName[dwValueName] = '\0';
      ::RegDeleteValueA(hKey,szValueName);
    }
    RegCloseKey(hKey);

    //////////////////////////////////////////////////////////////////////////////
    lResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce\\", 0, KEY_ALL_ACCESS,&hKey);
    if (lResult != ERROR_SUCCESS){
      return FALSE;
    }
    for (int i = 0; ;i++){
      char szValueName[MAX_PATH+1];
      DWORD  cbData = MAX_PATH;
      DWORD dwValueName = sizeof(szValueName);
      DWORD dwValueType = 0;
      lResult = ::RegEnumValueA(hKey,i,szValueName,&dwValueName, NULL,&dwValueType,NULL,NULL);
      if (lResult != ERROR_SUCCESS){
        if (lResult != ERROR_NO_MORE_ITEMS) bResult = FALSE;
        break;
      }
      szValueName[dwValueName] = '\0';
      ::RegDeleteValueA(hKey,szValueName);
    }
    RegCloseKey(hKey);

    //////////////////////////////////////////////////////////////////////////////
    lResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx\\", 0, KEY_ALL_ACCESS,&hKey);
    if (lResult != ERROR_SUCCESS){
      return FALSE;
    }
    for (int i = 0; ;i++){
      char szValueName[MAX_PATH+1];
      DWORD  cbData = MAX_PATH;
      DWORD dwValueName = sizeof(szValueName);
      DWORD dwValueType = 0;
      lResult = ::RegEnumValueA(hKey,i,szValueName,&dwValueName, NULL,&dwValueType,NULL,NULL);
      if (lResult != ERROR_SUCCESS){
        if (lResult != ERROR_NO_MORE_ITEMS) bResult = FALSE;
        break;
      }
      szValueName[dwValueName] = '\0';
      ::RegDeleteValueA(hKey,szValueName);
    }
    RegCloseKey(hKey);
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

  

  /// UINT bcolor = HexToDec(_T("0x003399"));
  UINT HexToDec(const wchar_t *szValue){
    UINT iValue=0;
    swscanf_s(szValue, L"%x", &iValue);
    return iValue;
  }

  void GetAllPluginAbsPath(const wchar_t* dirpath,vector<wstring>& ultras){
    wstring spec=dirpath;
    spec += L"*";
    WIN32_FIND_DATA data;
    HANDLE h=FindFirstFile(spec.c_str(),&data);
    if (h!=INVALID_HANDLE_VALUE) {
      do{
        if (wcscmp(data.cFileName,L".")==0 ||wcscmp(data.cFileName,L"..")==0){
          continue;
        }
        wstring plugpath = dirpath;
        plugpath += data.cFileName;
        if(PathIsDirectory(plugpath.c_str())){
          plugpath += L"\\";
          GetAllPluginAbsPath(plugpath.c_str(),ultras);
        }else{
          static const wchar_t ksuffix[] = L".uta";
          static const unsigned int ksuffix_size = arraysize(ksuffix);
          wstring tmp=plugpath.substr(plugpath.size()-ksuffix_size+1,ksuffix_size);
          LOWERWSTR(tmp);
          if (tmp==ksuffix){
            ultras.push_back(plugpath);
          }
        }
      }while(FindNextFile(h,&data));
      FindClose(h);
    }
  }

  bool IsReservedName(const wstring& filename) {
    // This list is taken from the MSDN article "Naming a file"
    // http://msdn2.microsoft.com/en-us/library/aa365247(VS.85).aspx
    // I also added clock$ because GetSaveFileName seems to consider it as a
    // reserved name too.
    static const wchar_t* const known_devices[] = {
      L"con", L"prn", L"aux", L"nul", L"com1", L"com2", L"com3", L"com4", L"com5",
      L"com6", L"com7", L"com8", L"com9", L"lpt1", L"lpt2", L"lpt3", L"lpt4",
      L"lpt5", L"lpt6", L"lpt7", L"lpt8", L"lpt9", L"clock$"
    };
    wstring filename_lower = filename;
    LOWERWSTR(filename_lower);
    for (int i = 0; i < arraysize(known_devices); ++i) {
      // Exact match.
      if (filename_lower == known_devices[i])
        return true;
      // Starts with "DEVICE.".
      if (filename_lower.find(wstring(known_devices[i]) + L".") == 0)
        return true;
    }
    static const wchar_t* const magic_names[] = {
      // These file names are used by the "Customize folder" feature of the shell.
      L"desktop.ini",
      L"thumbs.db",
    };
    for (int i = 0; i < arraysize(magic_names); ++i) {
      if (filename_lower == magic_names[i])
        return true;
    }
    return false;
  }

  bool IsShellIntegratedExtension(const wstring& extension) {
    wstring extension_lower = extension;
    LOWERWSTR(extension_lower);
    static const wchar_t* const integrated_extensions[] = {
      // See <http://msdn.microsoft.com/en-us/library/ms811694.aspx>.
      L"local",
      // Right-clicking on shortcuts can be magical.
      L"lnk",
    };

    for (int i = 0; i < arraysize(integrated_extensions); ++i) {
      if (extension_lower == integrated_extensions[i])
        return true;
    }

    // See <http://www.juniper.net/security/auto/vulnerabilities/vuln2612.html>.
    // That vulnerability report is not exactly on point, but files become magical
    // if their end in a CLSID.  Here we block extensions that look like CLSIDs.
    if (extension_lower.size() > 0 && extension_lower.at(0) == L'{' &&
      extension_lower.at(extension_lower.length() - 1) == L'}')
      return true;

    return false;
  }

  /// @note caller release memory
  /// @return FAIL - NULL
  wchar_t* GetUltraHome(){
    wchar_t* szPath = new wchar_t[MAX_PATH];
    ::wmemset(szPath,L'\0',MAX_PATH);
    HKEY hkey=NULL;
    DWORD dwRetVal=MAX_PATH;
    bool r=false;
    if(RegOpenKey(HKEY_LOCAL_MACHINE,kregistryname,&hkey)==ERROR_SUCCESS){
      DWORD dwType = REG_EXPAND_SZ;
      r=RegQueryValueEx(hkey,L"Location",0,&dwType,(LPBYTE)szPath,&dwRetVal)==ERROR_SUCCESS;
      if(hkey)
        RegCloseKey(hkey);
    }
    return r?szPath:NULL;
  }

  /// @return FAIL - NULL, bug!!!
  wstring GetUltraPath(const wchar_t* source,bool appendslash){
    static wchar_t* up = NULL;
    if (up==NULL){
      up = GetUltraHome();
      if (up==NULL){
        return wstring();
      }
    }
    wstring tmp(up);
    return appendslash ? wstring(tmp+source+L"\\") : wstring(tmp+source);
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

  bool MakeSureDir(const wchar_t* path){
    if(PathFileExistsW(path)){
      return true;
    }else{
      wstring directory;
      wchar_t buffer[_MAX_PATH];
      wchar_t drive[_MAX_DRIVE];
      wchar_t dir[_MAX_DIR];
      _wsplitpath_s(path,drive,_MAX_DRIVE,dir,_MAX_DIR,0,0,0,0);
      _wmakepath_s(buffer,_MAX_PATH,drive,dir,0,0);
      LPWSTR context = NULL;
      wchar_t* p = wcstok_s(buffer,L"\\",&context);
      while(p){
        directory += p;
        directory += L"\\";
        if (!PathFileExistsW(directory.c_str())){
          CreateDirectory(directory.c_str(),NULL);
        }
        p = wcstok_s(NULL,L"\\",&context);
      }
      return (PathIsDirectoryW(directory.c_str())?true:false);
    }
  }

  wstring GetIconPath(const string& urla_){
    _6bees_net::apr_uri_t t;
    if(_6bees_net::_6beeparseuri(urla_.c_str(),t)){
      const wstring& upath = ultrapath::R().get(ultrapath::tempicondir);
      if(MakeSureDir(upath.c_str())){
        return wstringmaker()<< upath << t.hostname << L".ico";
      }
    }
    return wstring();
  }

  string GetIconUltraURL(const string& urla_){
    _6bees_net::apr_uri_t t;
    if(_6bees_net::_6beeparseuri(urla_.c_str(),t)){
      const wstring& upath = ultrapath::R().get(ultrapath::tempicondir);
      static bool b = MakeSureDir(upath.c_str());
      if(b){
        if (t.hostname){/// for "place" schema, t.host is null.
          wstring p = wstringmaker() << upath << t.hostname << L".ico";
          if (PathFileExists(p.c_str())){
            return stringmaker() << "ultra://Icon/" << t.hostname << ".ico";
          }
        }
      }
    }
    return string("ultra://htm.gif");
  }

  static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
  }

  std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    while (in_len--) {
      char_array_3[i++] = *(bytes_to_encode++);
      if (i == 3) {
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        for(i = 0; (i <4) ; i++)
          ret += base64_chars[char_array_4[i]];
        i = 0;
      }
    }
    if (i)
    {
      for(j = i; j < 3; j++)
        char_array_3[j] = '\0';
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;
      for (j = 0; (j < i + 1); j++)
        ret += base64_chars[char_array_4[j]];
      while((i++ < 3))
        ret += '=';
    }
    return ret;

  }

  std::string base64_decode(std::string const& encoded_string) {
    int in_len = (int)encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    while (in_len-- &&(encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
      char_array_4[i++] = encoded_string[in_]; in_++;
      if (i ==4) {
        for (i = 0; i <4; i++)
          char_array_4[i] = (unsigned char)base64_chars.find(char_array_4[i]);
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        for (i = 0; (i < 3); i++)
          ret += char_array_3[i];
        i = 0;
      }
    }
    if (i) {
      for (j = i; j <4; j++)
        char_array_4[j] = 0;
      for (j = 0; j <4; j++)
        char_array_4[j] = (unsigned char)base64_chars.find(char_array_4[j]);
      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
      for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    return ret;
  }

  DWORD FileCrc32Assembly(LPCTSTR szFilename, DWORD &dwCrc32){
    // Static CRC table
    static DWORD s_arrdwCrc32Table[256] =
    {
      0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
      0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
      0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
      0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
      0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
      0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
      0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
      0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
      0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
      0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
      0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
      0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
      0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
      0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
      0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
      0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

      0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
      0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
      0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
      0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
      0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
      0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
      0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
      0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
      0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
      0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
      0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
      0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
      0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
      0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
      0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
      0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

      0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
      0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
      0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
      0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
      0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
      0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
      0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
      0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
      0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
      0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
      0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
      0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
      0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
      0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
      0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
      0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

      0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
      0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
      0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
      0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
      0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
      0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
      0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
      0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
      0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
      0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
      0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
      0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
      0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
      0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
      0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
      0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
    };

    _ASSERTE(szFilename);
    _ASSERTE(lstrlen(szFilename));
    DWORD dwErrorCode = NO_ERROR;
    HANDLE hFile = NULL;
    dwCrc32 = 0xFFFFFFFF;
    try{
      // Open the file
      hFile = CreateFile(szFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | 
        FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);
      if(hFile == INVALID_HANDLE_VALUE){
        dwErrorCode = GetLastError();
      }else{
        // There is a bug in the Microsoft compilers where inline assembly
        // code cannot access static member variables.  This is a work around
        // for that bug.  For more info see Knowledgebase article Q88092
        LPVOID ptrCrc32Table = &s_arrdwCrc32Table;

        // Read 4K of data at a time (used in the C++ streams, Win32 I/O, and assembly functions)
        BYTE buffer[4096];
        DWORD dwBytesRead;
        BOOL bSuccess = ReadFile(hFile, buffer, sizeof(buffer), &dwBytesRead, NULL);
        while(bSuccess && dwBytesRead){
          // Register use:
          //		eax - CRC32 value
          //		ebx - a lot of things
          //		ecx - CRC32 value
          //		edx - address of end of buffer
          //		esi - address of start of buffer
          //		edi - CRC32 table
          __asm{
            // Save the esi and edi registers
            push esi
              push edi

              mov eax, dwCrc32			// Load the pointer to dwCrc32
              mov ecx, [eax]				// Dereference the pointer to load dwCrc32

            mov edi, ptrCrc32Table		// Load the CRC32 table

              lea esi, buffer				// Load buffer
              mov ebx, dwBytesRead		// Load dwBytesRead
              lea edx, [esi + ebx]		// Calculate the end of the buffer

crc32loop:
            xor eax, eax				// Clear the eax register
              mov bl, byte ptr [esi]		// Load the current source byte

            mov al, cl					// Copy crc value into eax
              inc esi						// Advance the source pointer

              xor al, bl					// Create the index into the CRC32 table
              shr ecx, 8

              mov ebx, [edi + eax * 4]	// Get the value out of the table
            xor ecx, ebx				// xor with the current byte

              cmp edx, esi				// Have we reached the end of the buffer?
              jne crc32loop

              // Restore the edi and esi registers
              pop edi
              pop esi

              mov eax, dwCrc32			// Load the pointer to dwCrc32
              mov [eax], ecx				// Write the result
          }
          bSuccess = ReadFile(hFile, buffer, sizeof(buffer), &dwBytesRead, NULL);
        }
      }
    }catch(...){
      // An unknown exception happened
      dwErrorCode = ERROR_CRC;
    }
    if(hFile != NULL) CloseHandle(hFile);
    dwCrc32 = ~dwCrc32;
    return dwErrorCode;
  }

  bool ultrapath::initialized = false;
  ultrapath* ultrapath::pinstance = NULL;
  wchar_t ultrapath::cstr[] = L"6beenetworks\\ultraie";
  cmutex ultrapath::lock_;

  ultrapath::ultrapath(){}
  ultrapath::~ultrapath(){}
  ultrapath& ultrapath::R(){
    if(!initialized){
      cguard g(lock_);
      if(pinstance == NULL){ 
        static ultrapath p_;
        pinstance = &p_;
        if(pinstance){ // make sure compiler cannot reorder 
          initialized = true; // only set to true after creation 
        }
      } 
    } 
    return *pinstance;
  }

  const wstring& ultrapath::get(FileDirEnum f){
    if(_m.find(f)==_m.end()){
      const wchar_t* k = NULL;
      switch (f){
      // APP DATA
      case cachedir:k=kcachedir;break;
      case webcontentsdir:k=kwebcontentsdir;break;
      case picdir:k=kpicdir;break;
      case audiodir:k=kaudiodir;break;
      case videodir:k=kvideodir;break;
      case tempicondir:k=ktempicondir;break;

      // Program Files
      case tempdir:k=ktempdir;break;
      //case tempicondir:k=L"template\\icon";break;
      case plugindir:k=kplugindir;break;

      case maindb:k=kmaindb;break;
      case uploader:k=kuploader;break;
      case recorder:k=krecorder;break;
      case capvideo:k=kcapvideo;break;
      case htmleditor:k=khtmleditor;break;
      case clientmng:k=kclientmng;break;
      case resdll:k=kresdll;break;
      case uutoolbardll:k=kuutoolbardll;break;
      }
      wstring path;
      if (f<=videodir){
        wchar_t strPath[MAX_PATH];
        ::wmemset(strPath,L'\0',MAX_PATH);
        SHGetSpecialFolderPathW(0,strPath,CSIDL_APPDATA,FALSE);
        path = wstringmaker() << strPath << L"\\" << ultrapath::cstr << L"\\" << k << L"\\";
      }else{
        wchar_t* chome = GetUltraHome();
        wstring tmp(chome);
        delete [] chome; chome = NULL;
        if (tmp.at(tmp.size()-1)==L'\\'){
          tmp.erase(tmp.end()-1);
        }
        if (f<=plugindir && f >=tempdir){
          path = wstringmaker() << tmp << L"\\" << k << L"\\";
        }else{
          path = wstringmaker() << tmp << L"\\" << k;
        }
      }
      _m[f] = path;
    }
    return _m[f];
  }

  string ultrapath::getA(FileDirEnum f){
    const wstring& p = get(f);
    return stringmaker() << p;
  }

  bool c6beeres::initialized = false;
  c6beeres* c6beeres::pinstance = NULL;
  cmutex c6beeres::lock_;
  HMODULE c6beeres::hModule=NULL;

  c6beeres& c6beeres::R(){
    if(!initialized){
      cguard g(lock_);
      if(pinstance == NULL){
        static c6beeres p_;
        pinstance = &p_;
        if(pinstance){ //make sure compiler cannot reorder 
          initialized = true; // only set to true after creation 
        }
      }
    }
    return *pinstance;
  }

  c6beeres::c6beeres(){}

  c6beeres::~c6beeres(){
    if(!hModule)
      FreeLibrary(hModule);
  }

  HANDLE c6beeres::GetImg(int resid,short restype,int x,int y){
    HANDLE h=NULL;
    try{
      if(hModule==NULL){
        const wstring& respath = ultrapath::R().get(ultrapath::resdll);
        hModule = ::LoadLibraryEx(respath.c_str(),NULL,LOAD_LIBRARY_AS_DATAFILE);
      }
      h = LoadImage(hModule,MAKEINTRESOURCE(resid),restype,x,y,LR_DEFAULTSIZE|LR_SHARED);
    }catch(...){}
    return h;
  }

  /*HIMAGELIST c6beeres::GetImgList(int resid,int width){
    HIMAGELIST h=NULL;
    try{
      if(hModule==NULL)
        hModule = ::LoadLibraryEx(GetUltraPath(kResDLLName,false).c_str(),NULL,LOAD_LIBRARY_AS_DATAFILE);
      h = ::ImageList_LoadImage(hModule,MAKEINTRESOURCE(resid),width,1,CLR_DEFAULT,IMAGE_BITMAP,
        LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_SHARED);
    }catch(...){}
    return h;
  }*/

  HMENU c6beeres::GetMenu(int resid){
    HMENU h=NULL;
    try{
      if(hModule==NULL){
        const wstring& respath = ultrapath::R().get(ultrapath::resdll);
        hModule = ::LoadLibraryEx(respath.c_str(),NULL,LOAD_LIBRARY_AS_DATAFILE);
      }
      h = ::LoadMenu(hModule,MAKEINTRESOURCE(resid));
    }catch(...){}
    return h;
  }

};

