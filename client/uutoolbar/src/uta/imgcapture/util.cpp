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

#include "util.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "cppsqlite3.h"
#include "6bees_const.h"
#include "6bees_util.h"
#include "6bees_str.h"
using namespace _6bees_str;

namespace nsplugin{

  std::string GetDBPath(){
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      return stringmaker()<< upath << "imgcapture\\imgcapture.db";
    }
    return string();
  }

  void GetImgCapParam(bool& border,bool& shadow,unsigned long& color){
    CppSQLite3DB& DBRef = CppSQLite3DB::instance(GetDBPath().c_str());
    CppSQLite3Table tbl = DBRef.getTable("select * from imgcapsetting");
    shadow=ISTRUE(tbl.fieldValue(I_IMGHASSHADOW));
    border  =ISTRUE(tbl.fieldValue(I_IMGHASBORDER));
    color=(COLORREF)atol(tbl.fieldValue(I_IMGBORDERCOLOR));
  }

  void u_setImg_has_shadow(bool b){
    CppSQLite3DB& DBRef = CppSQLite3DB::instance(GetDBPath().c_str());
    string cmd = stringmaker() << "update imgcapsetting set img_has_shadow='" << (b?"true":"false") << "'";
    DBRef.execDML(cmd.c_str());
  }

  void u_setImg_has_border(bool b){
    CppSQLite3DB& DBRef = CppSQLite3DB::instance(GetDBPath().c_str());
    string cmd = stringmaker() << "update imgcapsetting set img_has_border='" << (b?"true":"false") << "'";
    DBRef.execDML(cmd.c_str());
  }

  void u_setImg_border_color(COLORREF color){
    CppSQLite3DB& DBRef = CppSQLite3DB::instance(GetDBPath().c_str());
    string cmd = stringmaker() << "update imgcapsetting set img_border_color='" << (int)color << "'";
    DBRef.execDML(cmd.c_str());
  }

};
