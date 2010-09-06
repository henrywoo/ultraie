#include "recordsurfhistory.hpp"
#include "smarthistory.hpp"
#include "thunk.h"
#include "6bees_net.h"
#include "6bees_util.h"
#include "6bees_html.h"
#include "6bees_const.h"
#include "6bees_str.h"
using namespace _6bees_str;

namespace nsplugin{
  using namespace std;

  //1.opening several tabs and visit the same url will generate
  //  several records
  //2.one tab, visit the same url
  UINT WINAPI StartSurf(LPVOID p){
    thunk* pt = static_cast<thunk*>(p);
    Sqlite_StartSurf(pt);
    return 1;
  }

  // 1. download icon of URL to icon/
  // 2. insert into table
  void Sqlite_StartSurf(const thunk* pt)
  {
    const wstring& url    =pt->url;
    const wstring& title  =pt->title;
    const wstring& iconurl=pt->iconurl;
    DWORD tid = pt->tid;

    if(url.empty()||title.empty()||url.substr(0,5).compare(_T("ultra"))==0){
      return;
    }
    std::string urla_=stringmaker()<<url;
    wstring iconpath = _6bees_util::GetIconPath(urla_);
    if (!PathFileExists(iconpath.c_str())){
      string realicon;
      USES_CONVERSION;
      if(_6bees_html::getValidFavicon(W2CA(iconurl.c_str()),realicon)>0){
        if(_6bees_net::DownloadURLToFile(realicon.c_str(),W2CA(iconpath.c_str()))==-1){
          /// @todo make an empty icon for this url, currently ignore it.
        }
      }
    }

    CppSQLite3DB& DBRef=CppSQLite3DB::instance(GetDBPath().c_str());
    ATL::CStringA cmd;
    std::string titlea_=stringmaker()<<title;
    strreplace(titlea_,"'","''");
    //When using ultraie's search box, if your keyword includes single quotation,
    //urla_ needs to be processed.
    strreplace(urla_,"'","''");
    cmd.Format(
      "insert or ignore into SurfHistory(url,title,time_begin,"
      "time_finish) values ('%s','%s',DATETIME('NOW'),0)",
      urla_.c_str(),titlea_.c_str());
    /// @todo check here again
    if(DBRef.execDML(cmd) != -1){
      cmd="SELECT last_insert_rowid() as lastid";
      CppSQLite3Table& tbl = DBRef.getTable(cmd);
      int lastid = atoi(tbl.fieldValue("lastid"));

      TidURLRowIDMap& tmap =tidurlrowidmap::R().GetMap();
      tmap[tid][url] = lastid;
    }
  }

  void FinishSurf(const wstring& _url){
    TidURLRowIDMap& tmap =tidurlrowidmap::R().GetMap();
    int rowid = tmap[GetCurrentThreadId()][_url];
    if (rowid>0){
      CppSQLite3DB& DBRef=CppSQLite3DB::instance(GetDBPath().c_str());
      ATL::CStringA cmd;
      cmd.Format("update SurfHistory set time_finish=DATETIME('NOW') where id='%d'",rowid);
      DBRef.execDML(cmd);
    }
    return;
  }

};