#include "helperfunc.h"

#include "6bees_util.h"
#include "6bees_net.h"
#include "cppsqlite3.h"
#include "6bees_const.h"
#include "6bees_str.h"
using namespace _6bees_str;

#include <fstream> // wifstream
#include <algorithm>// transform
#include <cctype>// tolower

namespace nsplugin{

#define RES_MEDIA 115
#define RES_FLV   116
#define RES_SWF   117
#define RES_RM    118
#define RES_WM    119

  //helper function
  mediabythread& mediabythread::R(){
    static mediabythread _t;
    return _t;
  }

  TIDMEDIAURLMAP& mediabythread::GetMap(){
    return tid2mediaurl_;
  }

  bool mediabythread::HasThisURL(const wstring& _url){
    DWORD tid = GetCurrentThreadId();
    if (tid2mediaurl_.find(tid)==tid2mediaurl_.end()){
      return false;
    }else{
      set<wstring>& urlset = tid2mediaurl_[tid];
      return (urlset.find(_url) != urlset.end());
    }
  }

  void mediabythread::AddURL(const wstring& _url){
    DWORD tid = GetCurrentThreadId();
    tid2mediaurl_[GetCurrentThreadId()].insert(_url);
  }

  std::string GetDBPath(){
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      std::string sh_dbpatha_=stringmaker()<< upath << "mediadetection\\mediadetection.db";
      return sh_dbpatha_;
    }
    return string();
  }

  bool ReadSnipppet(const wchar_t* _filepath,wstring& _contents){
    if (!PathFileExists(_filepath)){
      return false;
    }

    int length;
    wchar_t* buffer;

    wifstream is;
    is.open(_filepath, ios::binary);

    // get length of file:
    is.seekg(0, ios::end);
    length = is.tellg();
    is.seekg(0, ios::beg);

    // allocate memory:
    buffer = new wchar_t[length+1];
    
    // read data as a block:
    is.read (buffer,length);
    is.close();

    buffer[length] = '\0';
    _contents.reserve(length+1);
    _contents = buffer;

    delete [] buffer;
    return true;
  }


  /// @deprecated this function is incorrect - should not use string for binary data
  UINT WINAPI thread_mediares(LPVOID p){
    CMediaRes* m = static_cast<CMediaRes*>(p);

    std::string html;
    int clen;
    char* tmp=_6bees_net::GetDataByHTTP(m->mediaurl.c_str(),clen,0,5);
    if (tmp==NULL){
      return 0;
    }else{
      html = tmp;
      delete [] tmp;
      m->direct = IsDirect(html,m->type);
      USES_CONVERSION;
      m->size = _6bees_net::GetContentLen(W2CA(m->mediaurl.c_str()));

    }
    return 0;
  }

  int GetMimeResID(METYPE t){
    switch (t){
    case FLV:
      return RES_FLV;
    case SWF:
      return RES_SWF;
    default:
      return RES_RM;
    }
  }

  void NotifyUserMediaRes(HWND,CMediaRes& mr){
    /*
    CToolBarAPP::MR = mr;/// @todo  - Multiple threads problem
    HWND h = CToolBarAPP::MediaDlg.m_hWnd;
    if(!::IsWindow(h)){
      /// @todo 
      //_beginthreadex(NULL,0,thread_mediares,(LPVOID)&CToolBarAPP::MR,0,NULL);
    }else{
      /// @todo 
      //::PostMessage(h,WM_ADDMEDIA,0,(LPARAM)&CToolBarAPP::MR);
    }*/
  }

  

  void InsertMedia(wstring title,wstring webpage,wstring mediaurl,wstring format){
    
    strreplace(title,_T("'"),_T("''"));
    strreplace(webpage,_T("'"),_T("''"));
    strreplace(mediaurl,_T("'"),_T("''"));
    
    wchar_t cmd[1024]={};
    swprintf_s(cmd,1024,
      L"insert or ignore into mediares(title,webpage,mediaurl,format) values ('%s','%s','%s','%s')",
      title.c_str(),webpage.c_str(),mediaurl.c_str(),format.c_str());

    CppSQLite3DB& DBRef=CppSQLite3DB::instance(GetDBPath().c_str());
    std::string cmda_=stringmaker()<<cmd;
    OutputDebugStringA(cmda_.c_str());
    DBRef.execDML(cmda_.c_str());
    //CConfig::R().updatedb_direct(CW2A(cmd,CP_UTF8));/// @todo 
  }

  bool IsDirect(const std::string& a,METYPE b){
    if( a.size()<3 ) return false;
    switch (b){
  case FLV:
    return (a.substr(0,3).compare("FLV")==0);
  case SWF:
    return (a.substr(0,3).compare("CWS")==0);
  case RM:
    return (a.substr(0,4).compare(".RMF")==0);
  case MP3:
    return (a.substr(0,4).compare("CWS-")==0);
  case WMV:
    return (a.substr(0,2).compare("0&")==0);
  case WMA:
    return (a.substr(0,4).compare("CWS-")==0);
  case AVI:
    return (a.substr(0,4).compare("CWS-")==0);
  default:
    break;
    }
    return false;
  }


};