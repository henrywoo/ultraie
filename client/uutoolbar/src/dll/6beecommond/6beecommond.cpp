#define MY6BEECOMMOND_API __declspec(dllexport)

#include "6beecommond.h"
#include <comutil.h>//_variant_t
#include <atlapp.h>
#include <atlstr.h>
#include <atlmisc.h>
#include <atlgdi.h>
#include <uxtheme.h>
#include "../../lib/6beebase/copydatamsg.h"
#include "../../lib/6beebase/cppsqlite3.h"
#include "../../lib/6beebase/6bees_util.h"
#include "../../lib/6beebase/6bees_net.h"
#include "../../lib/6beebase/6bees_socket.h"
#include "../../lib/6beebase/6bees_window.h"
#include "../../lib/6beebase/6bees_ftpclient.h"
#include "../../lib/6beebase/uulogging.h"
#include "../../lib/6beebase/icesingleton.h"
#include "../../lib/6beebase/6bees_str.h"
#include "../../lib/6beebase/6bees_iwebbrowser2.h"//IWebBrowser2Ptr
#include <shellapi.h>
using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;

#pragma comment(lib,"version.lib")//GetFileVersion

#pragma data_seg(".shared")
_6beed_share::gconf globalconfig(0);
#pragma data_seg()
#pragma comment(linker,"/SECTION:.shared,RWS")

BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved){
  switch (ul_reason_for_call){
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

namespace _6beed_util{
  using namespace _6bees_const;

  UINT WINAPI runIE_internal(PVOID p){
    try{
      _variant_t varURL = (wchar_t*)p;
      CoInitialize(NULL);
      IWebBrowser2Ptr pIE(0);
      if (SUCCEEDED(CoCreateInstance(CLSID_InternetExplorer,NULL,CLSCTX_LOCAL_SERVER,
        IID_IWebBrowser2,(void**)&pIE)))
      {
        pIE->put_Visible(VARIANT_TRUE);
        _variant_t varEmpty;
        pIE->Navigate2(&varURL,&varEmpty,&varEmpty,&varEmpty,&varEmpty);
      }
      CoUninitialize();
    }catch (...){}
    return 0;
  }

  wstring runIE_url;
  void runIE(const wchar_t* url){
    runIE_url = url;
    ::_beginthreadex(NULL,0,runIE_internal,(LPVOID)runIE_url.c_str(),0,0);
  }

  bool urlencode(char* buf,int bufsize,const char* str){
    static const char special[] = "!\"#$%&()*+,/:;<=>?@[\\]^`{|}~%";
    char tmp[10];/* temp buffer */ 
    buf[0] = '\0';
    int j=(int)strlen(str);
    if (j>1024*2){
      return false;
    }
    for(int i=0; i<j; i++){ 
      char c = str[i]; 
      if(strchr(special,c)){ /* special character */ 
        sprintf_s(tmp,"%%%02X",(unsigned char)c); 
        strcat_s(buf,bufsize,tmp); 
      }else if(c == ' '){ /* blank */ 
        strcat_s(buf,bufsize,"+");
      }else{ 
        if(c < 0){ /* none ASCII character */
          sprintf_s(tmp,"%%%02X%%%02X",(unsigned char)str[i],(unsigned char)str[i+1]); 
          strcat_s(buf,bufsize,tmp); 
          ++i; 
        }else{ /* ASCII character */ 
          sprintf_s(tmp,"%c",c); 
          strcat_s(buf,bufsize,tmp); 
        } 
      } 
    }
    return true;
  }

  bool urldecode_gb2312(char *dest,int destsize,const char* src){
    ::memset(dest,'\0',destsize);
    const char *p = src;
    char code[3] = {0};
    unsigned long ascii = 0;
    char *end = NULL;
    while(*p){
      if(*p == '%'){
        ::memcpy(code,++p,2);
        ascii = strtoul(code,&end,16);
        *dest++ = (char)ascii;
        p += 2;
      }else
        *dest++ = *p++;
    }
    return true;
  }

  bool GetFileSizeA(const char* fpath,int& fsize){
    WIN32_FILE_ATTRIBUTE_DATA  fileattr;
    if (::GetFileAttributesExA((LPCSTR)fpath,::GetFileExInfoStandard,&fileattr)){
      fsize = (fileattr.nFileSizeHigh * (MAXWORD+1)) + fileattr.nFileSizeLow;
      return true;
    }
    return false;
  }

  bool GetFileSizeW(const wchar_t* fpath,int& fsize){
    WIN32_FILE_ATTRIBUTE_DATA  fileattr;
    if (::GetFileAttributesExW((LPCTSTR)fpath,::GetFileExInfoStandard,&fileattr)){
      fsize = (fileattr.nFileSizeHigh * (MAXWORD+1)) + fileattr.nFileSizeLow;
      return true;
    }
    return false;
  }

  bool Get6BeeLocation(wchar_t* pszPathBuffer,int bufsize=MAX_PATH){
    DWORD dwRetVal=MAX_PATH;
    HKEY hkey=NULL;
    bool result = false;
    ::wmemset(pszPathBuffer,L'\0',bufsize);
    if(RegOpenKey(HKEY_LOCAL_MACHINE,kregistryname,&hkey) == ERROR_SUCCESS){
      DWORD dwType = REG_EXPAND_SZ;
      wchar_t szPath[ MAX_PATH ]={};
      if(RegQueryValueEx(hkey,_T("Location"),0,&dwType,(LPBYTE)szPath,&dwRetVal) == ERROR_SUCCESS){
        dwRetVal = ExpandEnvironmentStrings(szPath,pszPathBuffer,MAX_PATH);
        if(!hkey)RegCloseKey(hkey);
        result = true;
      }
    }
    if (!result){
      _6bees_util::quitfatalerror(
        "Your registry is corrupted.\nYou can solve this problem by reinstall ultraie.");
    }
    return result;
  }

  const wchar_t* Get6BEEPath(const wchar_t* source,bool appendslash){
    static wchar_t tmp[MAX_PATH];
    Get6BeeLocation(tmp);
    if(source==0)
      return tmp;
    ::wcscat_s(tmp,MAX_PATH,source);
    if(appendslash)
      ::wcscat_s(tmp,MAX_PATH,_T("\\"));
    return tmp;
  }

  /// @todo remove static for multithread consideration
  const char* Get6BEEPathA(const wchar_t* source,bool appendslash){
    USES_CONVERSION;
    wchar_t tmp[MAX_PATH]={};
    if(Get6BeeLocation(tmp)){
      if(source==0){
        const char *p = W2CA(tmp);
        static char tmpA[MAX_PATH];
        strcpy_s(tmpA,MAX_PATH,p);
        return tmpA;
      }
      ::wcscat_s(tmp,MAX_PATH,source);
      if(appendslash)
        ::wcscat_s(tmp,MAX_PATH,_T("\\"));

      const char *p = W2CA(tmp);
      static char tmpA[MAX_PATH];
      strcpy_s(tmpA,MAX_PATH,p);
      return tmpA;
    }
    return NULL;
  }

  void SendMsgToUploader(HWND hwndTo,const char* filelist,int msg){
    int slen = (int)strlen(filelist);
    ::memset((void*)(filelist+slen),'\0',1);
    COPYDATASTRUCT cds;
    cds.dwData = msg;
    cds.lpData = (PVOID)filelist;
    cds.cbData = slen+1;
    ::SendMessage(hwndTo,WM_COPYDATA,(WPARAM)0,(LPARAM)&cds);
  }

  bool gettimestr(char* tstr,int len){
    if(len<50) return false;
    try{
      time_t rawtime=0;
      time(&rawtime);
      static char t[50]={0};
      ctime_s(t,50,&rawtime);
      ATL::CStringA ct(t);
      ct.TrimRight();
      strcpy_s(tstr,len,ct.GetString());
    }catch(...){
      return false;
    }
    return true;
  }


  char* GetIEHomePage(){
    static char szlpData[MAX_PATH+1]={};
    HKEY hKey;
    DWORD lResult = RegOpenKeyExA(HKEY_CURRENT_USER,"Software\\Microsoft\\Internet Explorer\\Main\\",0,KEY_ALL_ACCESS,&hKey);
    if (lResult == ERROR_SUCCESS){
      DWORD  cbData = MAX_PATH,dwType = REG_SZ;
      ::RegQueryValueExA(hKey,"Start Page",NULL,&dwType,(LPBYTE)szlpData,&cbData);
      RegCloseKey(hKey);
    }
    return szlpData;
  }
  wchar_t* GetIEHomePageW(){
    USES_CONVERSION;
    return A2W(GetIEHomePage());
  }

  OST GetOS(){
    OSVERSIONINFO osvi={};
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx (&osvi);
    if(osvi.dwPlatformId==VER_PLATFORM_WIN32_NT){
      if((osvi.dwMajorVersion==5)&&(osvi.dwMinorVersion>=1)){
        return XP;
      }else if(osvi.dwMajorVersion>=6){
        return VISTA;
      }else{
        return OTHERNT;
      }
    }else{
      return OLDWIN;
    }
  }

  bool IsXPTheme(){
    typedef HTHEME (__stdcall * POPENTHEMEDATA)(HWND hwnd,LPCWSTR pszClassList);
    typedef HRESULT (__stdcall * PCLOSETHEMEDATA)(HTHEME hTheme);
    OST osType=GetOS();
    if (osType!=XP && osType!=VISTA){
      return false;
    }
    bool bXpVisualStyle=false;
    HMODULE hUxTheme=::LoadLibrary(_T("UxTheme.dll"));
    if(hUxTheme){
      POPENTHEMEDATA pOpenThemeData=(POPENTHEMEDATA)::GetProcAddress(hUxTheme,"OpenThemeData");
      PCLOSETHEMEDATA pCloseThemeData=(PCLOSETHEMEDATA)::GetProcAddress (hUxTheme,"CloseThemeData");
      HTHEME hButtonTheme=(*pOpenThemeData)(GetDesktopWindow(),L"BUTTON");
      bXpVisualStyle=(hButtonTheme!=NULL);
      (*pCloseThemeData)(hButtonTheme);
    }
    ::FreeLibrary(hUxTheme);
    return bXpVisualStyle;
  }

  bool isChineseOS(){
    return GetSystemDefaultLangID()==0x804;
  }

  const char* getLangEncoding(){
    switch (GetSystemDefaultLangID()){
  case 0x804:
    return "GB2312";
  default:
    return "UTF-8";
    }
  }

  void CallExeW(const wchar_t* c){
    STARTUPINFOW si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    if (CreateProcessW(NULL,const_cast<wchar_t*>(c),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }else{
      ::MessageBoxW(NULL,c,L"ERROR: Failed to launch.",MB_ICONINFORMATION);
    }
  }

  void CallExeA(const char* c){
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    if (CreateProcessA(NULL,const_cast<char*>(c),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }else{
      ::MessageBoxA(NULL,c,"ERROR: Failed to launch.",MB_ICONINFORMATION);
    }
  }

  bool SetMMFA(const char* name,const char* contents,int bufsize){
#ifdef _DEBUG
    UUDEBUG((LOG_ERROR,"------------- SetMMF --------------"));
#endif
    HANDLE s_hFileMap = CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,bufsize,name);
    if (s_hFileMap!=NULL){
        if (GetLastError()==ERROR_ALREADY_EXISTS){
          // mapping already exists
          CloseHandle(s_hFileMap);
        }else{
          char* pView = reinterpret_cast<char *>(MapViewOfFile(s_hFileMap,FILE_MAP_READ|FILE_MAP_WRITE,0,0,0));
          if (pView!=NULL){
            //wcscpy_s((wchar_t*)pView,bufsize,contents);/// @todo  - to check should it be bufsize/2???
            string c(contents);
            if (c.size()>MMF_SIZE-1){
              c = c.substr(0,MMF_SIZE-1);
            }
            memset(pView,'\0',bufsize);
            memcpy(pView,c.c_str(),c.size()+1);
#ifdef _DEBUG
            UUDEBUG((LOG_ERROR,name));
            UUDEBUG((LOG_ERROR,c.c_str()));
#endif
            UnmapViewOfFile(pView);
          }else{
            // cannot map view of file
          }
        }
    }else{
      // cannot open mapping
    }
    return true;
  }

  bool SetMMF(const wchar_t* n,const wchar_t* c,int bufsize){
    string name = stringmaker() << n;
    string contents = stringmaker() << c;
    return SetMMFA(name.c_str(),contents.c_str(),bufsize);
  }

  void InvokeUploaderA(const char* filepath,const char* desc,const char* tag,const char* thumbpath){
    HWND hwndTo = ::FindWindow(0,kUploader_WndTitle);
    string mmfname_desc,mmfname_tag,mmfname_thumb;
    string fname = PathFindFileNameA(filepath);
    _6bees_util::GenMMFName(fname,mmfname_desc,mmfname_tag,mmfname_thumb);
    SetMMFA(mmfname_desc.c_str(),desc);
    SetMMFA(mmfname_tag.c_str(),tag);
    SetMMFA(mmfname_thumb.c_str(),thumbpath);
    if(!hwndTo){
      using _6bees_util::ultrapath;
      const wstring& upath = ultrapath::R().get(ultrapath::uploader);
      wstring UploaderCmd = wstringmaker(CP_ACP) 
        << upath
        << L" -m directupload -f \"" << filepath <<L"\"";
      STARTUPINFO si = {sizeof(si)};
      PROCESS_INFORMATION pi;
      if (CreateProcess(NULL,(LPWSTR)UploaderCmd.c_str(),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
      }else{
        ::MessageBox(NULL,_T("Cannot launch UltraIE Uploader!"),_T("ERROR"),MB_ICONINFORMATION);
      }
    }else{
      string afilepath=stringmaker(CP_ACP)<<filepath;
      SendMsgToUploader(hwndTo,afilepath.c_str(),CPYDATA_UPLOAD_2_UPLOAD_DIRECT);
    }
  }

  /// @todo for english OS, if parameter is chinese, funny character will come out!!!
  /// I can not work it out. Maybe I need to avoid the conversion from Unicode to MBCS.
  void InvokeUploader(const wchar_t* filepath,
    const wchar_t* desc,
    const wchar_t* tag,
    const wchar_t* thumbpath)
  {
    string fpath_ = stringmaker(CP_ACP) << filepath;
    string desc_ = stringmaker(CP_ACP) << desc;
    string tag_ = stringmaker(CP_ACP) << tag;
    string thumbpath_ = stringmaker(CP_ACP) << thumbpath;
    InvokeUploaderA(fpath_.c_str(),desc_.c_str(),tag_.c_str(),thumbpath_.c_str());
  }

  void DrawGradient(HDC hdc,LPRECT lpRect,COLORREF rgbTop,COLORREF rgbBottom){
    GRADIENT_RECT grdRect = { 0,1 };
    TRIVERTEX triVertext[ 2 ] = {
      lpRect->left,
      lpRect->top,
      GetRValue(rgbTop) << 8,
      GetGValue(rgbTop) << 8,
      GetBValue(rgbTop) << 8,
      0x0000,
      lpRect->right,
      lpRect->bottom,
      GetRValue(rgbBottom) << 8,
      GetGValue(rgbBottom) << 8,
      GetBValue(rgbBottom) << 8,
      0x0000
    };
    ::GradientFill(hdc,triVertext,2,&grdRect,1,GRADIENT_FILL_RECT_V);
  }

  void DrawRoundRect(HDC hdc,LPRECT rcRect,COLORREF rgbOuter,COLORREF rgbInner ,COLORREF m_rgbBackground){
    CDCHandle cdchandle(hdc);
    RECT tmprect;
    ::CopyRect(&tmprect,rcRect);

    CPen penBorder;
    penBorder.CreatePen(PS_SOLID,1,rgbOuter);
    CBrush bshInterior;
    bshInterior.CreateSolidBrush(m_rgbBackground);

    SelectObject(hdc,(HGDIOBJ)penBorder);
    SelectObject(hdc,(HGDIOBJ)bshInterior);

    POINT p={};
    cdchandle.RoundRect(&tmprect,p);
    ::InflateRect(&tmprect,-1,-1);

    CPen penInnerBorder;
    penInnerBorder.CreatePen(PS_SOLID,1,rgbInner);
    SelectObject(hdc,(HGDIOBJ)penInnerBorder);

    cdchandle.RoundRect(&tmprect,p);
  }

  versionblk GetVersionBlk(const wchar_t* sFileName){
    versionblk ver;
    UINT jj = (UINT)sizeof(VS_FIXEDFILEINFO);
    wchar_t *pp,pp2[100];
    VS_FIXEDFILEINFO *lpBuffer2;
    void **pp1 = (void **)&lpBuffer2;
    pp2[0] = 0;
    int ii = ::GetFileVersionInfoSize(sFileName,NULL);
    if(ii>0){
      pp = new wchar_t[ii];
      if(::GetFileVersionInfo(sFileName,0,ii,pp)){
        if(::VerQueryValue(pp,_T("\\"),pp1,&jj)){
          ver.wMajor   = HIWORD(lpBuffer2->dwFileVersionMS);
          ver.wMinor   = LOWORD(lpBuffer2->dwFileVersionMS);
          ver.wRelease = HIWORD(lpBuffer2->dwFileVersionLS);
          ver.wBuild   = LOWORD(lpBuffer2->dwFileVersionLS);
        }
      }
      delete [] pp;
    }
    return ver;
  }

  bool Login_v2(){
    if (!_6bees_window::NotifyClientMng(CPYDATA_LOGIN,kUploader_WndTitle,NULL)){
      STARTUPINFO si = {sizeof(si)};
      PROCESS_INFORMATION pi;
      using _6bees_util::ultrapath;
      wstring clientmngpath(ultrapath::R().get(ultrapath::uploader));
      clientmngpath += L" -m login";
      if (CreateProcess(NULL,(LPWSTR)clientmngpath.c_str(),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
      }else{
        ::MessageBox(NULL,clientmngpath.c_str(),_T("ERROR When Starting Login Program."),MB_ICONINFORMATION);
        return false;
      }
    }
    return true;
  }

  bool QuietUploadFile(const wchar_t* localname,
    const wchar_t* remotename,
    const wchar_t* id,
    UPLOADERMODE mode)
  {
    int msize;
    if(!GetFileSizeW(localname,msize))
      return false;
    if(msize>1024*20)
      return false;

    _6bees_socket::ftpclient myftpClient;
    _6bees_timer::CTimer m_Timer;
    myftpClient.SetTimer(& m_Timer);
    int static tmpid=0;
    myftpClient.SetThreadID(9999 + tmpid++);
    myftpClient.SetFL(localname);
    myftpClient.Setufilesize(msize);

    UUTOOLBAR::UserPrx _user = MyICE::R();
    if(_user==0){
      return false;
    }
    static string ftp_ip;
    static short ftp_port=0;
    string email(_6beed_share::u_getEmail());
    string ftp_password(_6beed_share::u_getPassword());
    if(ftp_port==0){  
      if(!_user->GetFTPInfoForUpload(email,ftp_ip,ftp_port)){
        return 0;
      }
    }

    _6bees_socket::CLogonInfo logonInfo(wstringmaker()<<ftp_ip,
      ftp_port,
      wstringmaker()<<email,
      wstringmaker()<<ftp_password);
    if(myftpClient.Login(logonInfo)){
      bool ulfinished = myftpClient.UploadFile(localname,wstringmaker()<<L"/"<<remotename);
      if (ulfinished){
        long OLBKid=_wtol(id);
        string sid(_6beed_share::u_getSID());
        string rname = stringmaker() << remotename;
        bool re=false;
        if (mode==UPLOAD_THUMB_MODE){
          if(_user->PostUploadThumb(rname,OLBKid,sid)){
            ::DeleteFileW(localname);
          }
        }else if (mode==UPLOAD_ICON_MODE){
          if(_user->PostUploadIcon(rname,OLBKid,sid)){
            ::DeleteFileW(localname);
          }
        }
      }
      return true;
    }else{
      return false;
    }
  }

  void Notify6BeeTB(int msg,HWND h){
    switch(_6beed_share::u_getieversion()){
      case 8:
        _6bees_window::IE8::Notify6BeeTB(msg,h);
        break;
      case 7:
        _6bees_window::IE7::Notify6BeeTB(msg,h);
        break;
      case 6:
        _6bees_window::IE6::Notify6BeeTB(msg,h);
        break;
    }
  }

  HWND GetReflectionWnd(){
    switch(_6beed_share::u_getieversion()){
    case 8:
      return _6bees_window::IE8::GetReflectionWnd();
    case 7:
      return _6bees_window::IE7::GetReflectionWnd();
    case 6:
      return _6bees_window::IE6::GetReflectionWnd();
    }
    return NULL;
  }

  UINT WINAPI CheckVersion_internal(PVOID p){
    try{
      string url=(const char*)p;
      int clen=0;
      char *data = _6bees_net::GetDataByHTTP(url.c_str(),clen);
      if (data!=NULL){
        string aurl = stringmaker()  << data;
        delete [] data;
        if(aurl!="0" && _6bees_str::startwith<string>(aurl,"http://")){
#ifdef __6BEEUPLOADER__
          string info = stringmaker() << "您好,"<< PRODUCTNAME <<"有更新的版本发布了,现在去下载升级新版本吗?";
          if(::MessageBoxA(NULL,info.c_str(),"探测到新版本",MB_YESNO)==IDYES){
            ShellExecuteA(NULL,"open","iexplore.exe",aurl.c_str(),"",SW_SHOW);
          }
#else
#ifdef __CHINESE__
          string info = stringmaker() << "您好,"<< PRODUCTNAME <<"有更新的版本发布了,现在去下载升级新版本吗?";
          if(::MessageBoxA(NULL,info.c_str(),"探测到新版本",MB_YESNO|MB_ICONINFORMATION)==IDYES){
            ShellExecuteA(NULL,"open","iexplore.exe",aurl.c_str(),"",SW_SHOW);
          }
#else
          string info = stringmaker() << PRODUCTNAME <<" has new version available. Would you like to go to download the latest version?";
          if(::MessageBoxA(NULL,info.c_str(),"New Version Available",MB_YESNO|MB_ICONINFORMATION)==IDYES){
            ShellExecuteA(NULL,"open","iexplore.exe",aurl.c_str(),"",SW_SHOW);
          }
#endif
#endif
          
        }
      }
    }catch (...){}
    return 0;
  }

  string checkurl_url;
  void CheckVersion(const wchar_t* filepath){
    wstring filename= PathFindFileNameW(filepath);
    LOWERWSTR(filename);
    versionblk verblk=GetVersionBlk(filepath);
    checkurl_url = _6bees_str::stringmaker()<< _6bees_const::kVersionChkURL << 
      "?filename=" << filename << "&version=" << verblk.wMajor <<
      "." << verblk.wMinor <<
      "." << verblk.wRelease <<
      "." << verblk.wBuild;
    ::_beginthreadex(NULL,0,CheckVersion_internal,(LPVOID)checkurl_url.c_str(),0,0);
  }

};



namespace _6beed_share{
  using namespace _6bees_const;
  using namespace _6beed_util;

  // helper func
  CppSQLite3DB& GetDBRef(){
    using _6bees_util::ultrapath;
    string dbpath = ultrapath::R().getA(ultrapath::maindb);
    return CppSQLite3DB::instance(dbpath.c_str());
  }

  gconf::gconf(int):ieversion(GetVersionBlk(L"C:\\Program Files\\Internet Explorer\\iexplore.exe").wMajor){
    Get6BeeLocation(uhome_,MAX_PATH);
    CppSQLite3DB& DBRef = CppSQLite3DB::instance(Get6BEEPathA(L"main.db"));
    if (FindWindowEx(NULL,NULL,NULL,kUploader_WndTitle) == NULL){
      DBRef.execDML("update UUConfig set SID=''");
    }
    CppSQLite3Table tbl = DBRef.getTable("select * from UUConfig limit 1");
    strcpy_s(version,11,tbl.fieldValue(U_6BEEVER));
    strcpy_s(email,50,tbl.fieldValue(U_NAME));
    strcpy_s(password,32+1,tbl.fieldValue(U_PASS));
    strcpy_s(sid,36+1,tbl.fieldValue(U_SID));//len('F63F32ED-4E70-4981-9F02-32CF529478CB') = 36

    loggedin  = ISTRUE(tbl.fieldValue(U_LOGGEDIN)) && strlen(sid)==36;
    autologin = ISTRUE(tbl.fieldValue(U_AUTOLOGIN));

    searchboxlen=atoi(tbl.fieldValue(U_SEBOXLEN));

    ie_hidebuiltinSEBox=ISTRUE(tbl.fieldValue(U_IE_BUILTINSEBOX));
    ie_onewindow=ISTRUE(tbl.fieldValue(U_IE_ONEWINDOW));
    loadforexplorer=ISTRUE(tbl.fieldValue(U_LOADFOREXPLORER));
  }

  // 1. User Data
  //////////////////////////////////////////////////////////////////////////
  void update_GeneralSetting(){
    CppSQLite3Table tbl = GetDBRef().getTable("select * from UUConfig limit 1");
    globalconfig.ie_hidebuiltinSEBox=ISTRUE(tbl.fieldValue(U_IE_BUILTINSEBOX));
    globalconfig.ie_onewindow  =ISTRUE(tbl.fieldValue(U_IE_ONEWINDOW));
    globalconfig.loadforexplorer = ISTRUE(tbl.fieldValue(U_LOADFOREXPLORER));
  }
  bool u_getLoggedin(){
    return globalconfig.loggedin;
  }
  void u_setLoggedin(bool b){
    if (globalconfig.loggedin!=b){
      globalconfig.loggedin=b;
      ATL::CStringA cmd;
      cmd.Format("update UUConfig set loggedin='%s'",b?"true":"false");
      UUDEBUG((LOG_ERROR,cmd.GetString()));
      GetDBRef().execDML(cmd);
    }
  }
  const char* u_getSID(){
    return globalconfig.sid;
  }
  void u_setSID(const char* sid){
    if (strcmp(globalconfig.sid,sid)!=0){
      strcpy_s(globalconfig.sid,36+1,sid);
      ATL::CStringA cmd;
      cmd.Format("update UUConfig set sid='%s'",sid);
      UUDEBUG((LOG_ERROR,cmd.GetString()));
      GetDBRef().execDML(cmd);
    }
  }
  const char* get_version(){
    return globalconfig.version;
  }
  const wchar_t* get_uhome(){
    return globalconfig.uhome_;
  }
  bool u_getAutologin(){
    return globalconfig.autologin;
  }
  void u_setAutologin(bool b){
    if (globalconfig.autologin!=b){
      globalconfig.autologin=b;
      ATL::CStringA cmd;
      cmd.Format("update UUConfig set autologin='%s'",b?"true":"false");
      GetDBRef().execDML(cmd);
    }
  }
  const char* u_getPassword(){
    return globalconfig.password;
  }
  void u_setPassword(const char* p){
    if (strcmp(globalconfig.password,p)!=0){
      strcpy_s(globalconfig.password,32+1,p);
      ATL::CStringA cmd;
      cmd.Format("update UUConfig set password='%s'",p);
      GetDBRef().execDML(cmd);
    }
  }
  const char* u_getEmail(){
    return globalconfig.email;
  }
  void u_setEmail(const char* email){
    if (strcmp(globalconfig.email,email)!=0){
      strcpy_s(globalconfig.email,50,email);
      ATL::CStringA cmd;
      cmd.Format("update UUConfig set username='%s'",email);
      GetDBRef().execDML(cmd);
    }
  }

  //2. Other global setting
  //////////////////////////////////////////////////////////////////////////
  int u_getSearchboxlen(){
    return globalconfig.searchboxlen;
  }
  void u_setSearchboxlen(int len){
    globalconfig.searchboxlen=len;
    ATL::CStringA cmd;
    cmd.Format("update UUConfig set searchboxlength=%d",len);
    GetDBRef().execDML(cmd);
  }
  bool u_getiebuiltinSEBox(){
    return globalconfig.ie_hidebuiltinSEBox;
  }
  bool u_setiebuiltinSEBox(bool b,bool isupdatedb){
    // b: true - hide,false - show
    bool successful=false;
    CRegKey crk;
    long rk2=crk.Open(HKEY_LOCAL_MACHINE,
      _T("Software\\Policies\\Microsoft\\Internet Explorer\\Infodelivery\\Restrictions"),
      KEY_READ|KEY_WRITE);
    if (rk2==ERROR_SUCCESS){
      rk2=crk.SetDWORDValue(_T("NoSearchBox"),b?1:0);
      successful = (rk2==ERROR_SUCCESS);
    }else{
      DWORD v=REG_CREATED_NEW_KEY;
      rk2=crk.Create(HKEY_LOCAL_MACHINE,
        _T("Software\\Policies\\Microsoft\\Internet Explorer\\Infodelivery\\Restrictions"),
        NULL, REG_OPTION_NON_VOLATILE,KEY_WRITE|KEY_READ,NULL,&v);
      if (rk2==ERROR_SUCCESS){
        rk2=crk.SetDWORDValue(_T("NoSearchBox"),b?1:0);
        successful = (rk2==ERROR_SUCCESS);
      }
    }
    if (successful){
      globalconfig.ie_hidebuiltinSEBox=b;
      if (isupdatedb){
        ATL::CStringA cmd;
        cmd.Format("update UUConfig set ie_hidebuiltinSEBox='%s'",b?"true":"false");
        GetDBRef().execDML(cmd);
      }
    }
    return successful;
  }
  bool u_getieonewindow(){
    return globalconfig.ie_onewindow;
  }
  bool u_setieonewindow(bool b){
    globalconfig.ie_onewindow=b;
    ATL::CStringA cmd;
    cmd.Format("update UUConfig set ie_onewindow='%s'",b?"true":"false");
    GetDBRef().execDML(cmd);
    return true;
  }
  bool u_getloadforexplorer(){
    return globalconfig.loadforexplorer;
  }
  bool u_setloadforexplorer(bool b){
    globalconfig.loadforexplorer=b;
    ATL::CStringA cmd;
    cmd.Format("update UUConfig set loadforexplorer='%s'",b?"true":"false");
    GetDBRef().execDML(cmd);
    return true;
  }
  short u_getieversion(){
	  return globalconfig.ieversion;
  }
  void u_setieversion(short v){
    globalconfig.ieversion = v;
  }
};