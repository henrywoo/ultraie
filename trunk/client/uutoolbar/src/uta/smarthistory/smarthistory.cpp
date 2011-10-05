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

#include "smarthistory.hpp"
#include "recordsurfhistory.hpp"
#include "thunk.h"
#include "6bees_util.h"
#include "6bees_net.h"
#include "6bees_const.h"
#include "6bees_html.h"
#include <atlrx.h>
#include "6bees_str.h"
using namespace _6bees_str;

namespace nsplugin{
  using namespace std;

#ifdef __CHINESE__
#define ktip_sh L"浏览历史记录"
#define kcap_sh L"浏览历史"
#else
#define ktip_sh L"Smart History - Your browsing history manager"
#define kcap_sh L"Smart History"
#endif

  //helper function
  static wchar_t* redirectflag = L"resurfhistory";

  string GetDBPath(){
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      string sh_dbpatha_=stringmaker()<< upath << "smarthistory\\smarthistory.db";
      return sh_dbpatha_;
    }
    return string();
  }

  //////////////////////////////////////////////////////////////////////////
  bool smarthistory::OnNavigateComplete2(int paneid,const wchar_t* url){
    return true;
  }

  void smarthistory::OnDocumentComplete(const wstring& u, const wstring& ti,
    const wstring& _iconurl)
  {
    thunk& _t = thunk::R();
    _t.url = u;
    _t.title = ti;
    _t.iconurl = _iconurl;
    _t.tid = GetCurrentThreadId();
    ::_beginthreadex(NULL,0,StartSurf,(LPVOID)&_t,0,NULL);
  }
  void smarthistory::OnBeforeNaviagte2(const wstring& _url)
  {
    FinishSurf(_url);
  }
  void smarthistory::OnQuit(const wstring& _url){
    FinishSurf(_url);
  }

  void smarthistory::StartUltraProtocol(const wchar_t* _url,
    string& dbcontent,
    wstring& fname,
    char*& flag,BYTE* pbuffer,ULONG buffersize)
  {
    ATL::CStringW url(_url);
    url.MakeLower();

    static const wchar_t historyurl[] = L"ultra://history";
    static const wchar_t actionurl[] = L"ultra://action";
    static int len_historyurl = sizeof(historyurl) / sizeof(historyurl[0]) - 1;
    static int len_actionurl  = sizeof(actionurl) / sizeof(actionurl[0]) - 1;

    if(url.Left(len_historyurl) == L"ultra://history"){
      History(dbcontent,fname,flag);
    }else if (url.Left(len_actionurl) == actionurl){
      string cmd;
      wstring redirectpage;
      if(_6bees_html::ParseAction(L"SurfHistory",fname,cmd,redirectpage)){
        CppSQLite3DB& DBRef=CppSQLite3DB::instance(GetDBPath().c_str());
        DBRef.execDML(cmd.c_str());
        if (redirectpage == redirectflag){
          History(dbcontent,fname,flag);
        }else{
          //fname = redirectpage;
        }
      }
    }
  }

  /// @todo - to replace ?
  bool MatchHistoryPaging(const wchar_t* str,wstring& tmp1,wstring& tmp2,wstring& tmp3){
    wstring str_(str);
    LOWERWSTR(str_);
    wchar_t strMatch[] = L"history/search\\?sel={\\w+}&sword={[^\\$]+}(\\$*){\\d*}";
    CAtlRegExp<> regexp;
    regexp.Parse(strMatch);
    CAtlREMatchContext<> defCtx;
    int match = regexp.Match(str_.c_str(), &defCtx);
    if(match!=0){
      const CAtlREMatchContext<>::RECHAR* szStart = 0;
      const CAtlREMatchContext<>::RECHAR* szEnd = 0;
      {
        defCtx.GetMatch(0,&szStart, &szEnd);
        CString result(szStart, (int)(szEnd - szStart));
        tmp1=result;
      }
      {
        defCtx.GetMatch(1,&szStart, &szEnd);
        CString result(szStart, (int)(szEnd - szStart));
        tmp2=result;
      }
      {
        defCtx.GetMatch(2,&szStart, &szEnd);
        CString result(szStart, (int)(szEnd - szStart));
        tmp3=result;
      }
      return true;
    }else{
      return false;
    }
  }

  void smarthistory::History(string& dbcontent,wstring& fname,char*& flag){
    flag = "<!-- History -->";
    wstring extension;
    string urlbase = "ultra://history/";
    wstring sfield,sword,snum;

    wstring _tmp1,_tmp2,_tmp3;
    if (MatchHistoryPaging(fname.c_str(),_tmp1,_tmp2,_tmp3)){
      sfield = _tmp1;
      OutputDebugStringW(sfield.c_str());
      static const unsigned int MAXLEN=1024;

      char r[MAXLEN]={};
      string t=stringmaker()<<_tmp2;
      if(!_6beed_util::urldecode_gb2312(r,MAXLEN,t.c_str())){
        return;
      }
      sword = wstringmaker() << r;
      snum   = _tmp3;
      extension = L" where " + sfield + L" like '%"+sword+L"%'";
      
      wstring _t= L"ultra://History/Search?sel=" + sfield + L"&sword=" + sword + L"$";
      urlbase = stringmaker() << t;
    }
    wstring op;
    int pnum = 0;
    if (fname.size()>7){
      op = fname.substr(7+1,fname.size()-7-1);
      if (snum.empty()){
        pnum = _wtoi(op.c_str());
      }else{
        pnum = _wtoi(snum.c_str());
      }
    }
    CppSQLite3DB& DBRef=CppSQLite3DB::instance(GetDBPath().c_str());
    ATL::CString cmd = _T("select count(*) as totalnum from SurfHistory");
    cmd += extension.c_str();
    string _cmda = stringmaker() << cmd.GetString();
    CppSQLite3Table tbl = DBRef.getTable(_cmda.c_str());
    const char* totalnum = tbl.fieldValue("totalnum");
    int hnum = atoi(totalnum)/200;

    string nav;
    _6bees_html::GetNavCode(urlbase,totalnum,pnum,nav);

    dbcontent += nav;
    dbcontent +=  "<table align=center cellpadding=1 cellspacing=1 width=100% "
      "bgcolor=#FFFFCC style='BORDER:#FFB900 1px solid;'>";
      "<tr bgcolor=#EEEBBB>\
      <td width=2%>ID</td>\
      <td width=66%>WebPage</td>\
      <td width=14%>Time Begin</td>\
      <td width=14%>Time Finish</td>\
      <td width=4%>Action</td>\
      </tr>";
    cmd.Format(_T("select * from SurfHistory %s order by id desc limit %d,%d"),
      extension.c_str(),
      pnum*200,
      200);
    _cmda = stringmaker()<< cmd.GetString();
    CppSQLite3Table tbl2 = DBRef.getTable(_cmda.c_str());
    int rownum=tbl2.numRows();
    if(rownum==0){
      dbcontent += "<tr><td colspan=4> There is no Visit Page!</td></tr>";
    }else{
      for (int j=0;j<rownum;j++){
        tbl2.setRow(j);
        dbcontent += "<tr>\n";
        string id=tbl2.fieldValue("id");
        string url=tbl2.fieldValue("url");
        string iconultraurl=_6bees_util::GetIconUltraURL(url);
        string title=tbl2.fieldValue("title");
        string time_begin=tbl2.fieldValue("time_begin");
        string time_finish=tbl2.fieldValue("time_finish");
        string _redirectflag = stringmaker() << redirectflag;
        dbcontent += "<td>"+id+"</td><td><img src=\""+iconultraurl+
          "\" width=16 height=16><a href=\""+url+"\">"+
          (title.size()>90?title.substr(0,90).append("..."):title)+"</a></td><td>"+
          time_begin+"</td><td>"+time_finish+"</td>";
        dbcontent += "<td><a href=\"ultra://action@delete&table@SurfHistory&condition@id="
          +id+"&redirectpage@"+_redirectflag
          +"\"><img src=ultra://delete.png></a></td></tr>\n";
      }
    }
    dbcontent += "</table>\n";
    dbcontent += nav;

    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      fname = wstringmaker() << upath << L"smarthistory\\smarthistory.html";
    }
  }

  void smarthistory::AssociateImage(int wID,int& _img){}

  bool smarthistory::OnClickMenuItem(int id){
    return true;
  }

  smarthistory::smarthistory(){
    enabled_ = true;
    len_ = 30;
    caption_= kcap_sh;
    tip_ = ktip_sh;
    menuwID_ = 1317;
  }

  smarthistory::~smarthistory(){
    for (vector<HICON>::const_iterator i=icons_.begin();i!=icons_.end();++i){
      DestroyIcon(*i);
    }
  }

  bool smarthistory::ProcessBasedInit(){
    return true;
  }

  bool smarthistory::ProcessBasedTerm(){
    return true;
  }

  bool smarthistory::ThreadBasedInit(){
    return true;
  }

  bool smarthistory::ThreadBasedTerm(){
    return true;
  }

  bool smarthistory::init(){
    //::MessageBoxA(NULL,"smarthistory init","smarthistory",NULL);
    return true;
  }
  bool smarthistory::StartPlugin(){
    return true;
  }
  bool smarthistory::StopPlugin(){
    return true;
  }
  bool smarthistory::OnLButtonDown(int paneid,int xpos,int ypos){
    C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
    pSB->opennewtab(L"ultra://history/",true);
    return true;
  }
  bool smarthistory::OnRButtonDown(int){
    return true;
  }
  bool smarthistory::disenable_plugin(){
    return true;
  }

};
