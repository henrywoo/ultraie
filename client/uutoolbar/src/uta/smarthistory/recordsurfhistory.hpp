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