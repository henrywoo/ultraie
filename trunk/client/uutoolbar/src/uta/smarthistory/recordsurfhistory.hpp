#ifndef __PLUGINS_RECORDSURFHISTORY__
#define __PLUGINS_RECORDSURFHISTORY__

#include <map>
#include "thunk.h"
#include "cppsqlite3.h"
#include "6bees_util.h"

namespace nsplugin{
  using namespace std;

  static const int kNotInsertedURL = -99;
  static const int kInsertedURL = -100;

  // Thread ID -> URL -> row Id
  // if row id is equal to -100, it means it is a finished URL.
  typedef map<DWORD,map<wstring,int>> TidURLRowIDMap;

  class tidurlrowidmap{
  private:
    TidURLRowIDMap tidurlrowidmap_;
  public:
    static tidurlrowidmap& R(){
      static tidurlrowidmap _t;
      return _t;
    }

    TidURLRowIDMap& GetMap(){
      return tidurlrowidmap_;
    }
  };
  
  UINT WINAPI StartSurf(LPVOID p);

  //1.opening several tabs and visit the same url will generate
  //  several records
  //2.one tab, visit the same url
  void Sqlite_StartSurf(const thunk* p);
  void FinishSurf(const wstring& _url);

};

#endif