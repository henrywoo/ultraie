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

#include "mediadetection.hpp"

#include "6bees_util.h"
#include "6bees_net.h"
#include "cppsqlite3.h"
#include "6bees_const.h"
#include "6bees_html.h"
#include "6bees_window.h"
#include "helperfunc.h"
#include <atlrx.h>
#include <sstream>
#include "6bees_str.h"
using namespace _6bees_str;

namespace nsplugin{
  using namespace std;

  static wchar_t* redirectflag = L"remediares";

  //////////////////////////////////////////////////////////////////////////
  bool mediadetection::OnNavigateComplete2(int paneid,const wchar_t* url){
    return true;
  }

  void mediadetection::OnDocumentComplete(const wstring& u, const wstring& ti,
    const wstring& _iconurl)
  {}

  void mediadetection::OnBeforeNaviagte2(const wstring& _url)
  {}
  void mediadetection::OnQuit(const wstring& _url)
  {}

  void mediadetection::StartUltraProtocol(const wchar_t* _url,
    std::string& dbcontent,
    std::wstring& fname,
    char*& flag,BYTE* pbuffer,ULONG buffersize)
  {
    ATL::CStringW url(_url);
    url.MakeLower();
    
    static const wchar_t mediaurl[] = L"ultra://mediares";
    static const wchar_t actionurl[] = L"ultra://action";
    static int len_mediaurl = sizeof(mediaurl) / sizeof(mediaurl[0]) - 1;
    static int len_actionurl = sizeof(actionurl) / sizeof(actionurl[0]) - 1;

    //OutputDebugStringW(tmp);
    if(url.Left(len_mediaurl) == mediaurl){
      MediaRes(dbcontent,fname,flag);
    }else if (url.Left(len_actionurl) == actionurl){
      string cmd;
      wstring redirectpage;
      if(_6bees_html::ParseAction(L"mediares",fname,cmd,redirectpage)){
        CppSQLite3DB& DBRef=CppSQLite3DB::instance(GetDBPath().c_str());
        DBRef.execDML(cmd.c_str());
        if (redirectpage == redirectflag){
          MediaRes(dbcontent,fname,flag);
        }else{
          //fname = redirectpage;
        }
      }
    }
  }

  void mediadetection::MediaRes(string& dbcontent,wstring& fname,char*& flag){
    flag = "<!-- mediaresources -->";
    ATL::CString extension;

    USES_CONVERSION;
    CppSQLite3DB& DBRef=CppSQLite3DB::instance(GetDBPath().c_str());
    ATL::CString cmd = _T("select count(*) as totalnum from mediares") + extension;
    CppSQLite3Table _table = DBRef.getTable(W2CA(cmd));
    const char* totalnum = _table.fieldValue("totalnum");
    int hnum = atoi(totalnum)/100;

    string urlbase = "ultra://mediares/";
    wstring sfield,sword,snum;
    wstring op;
    int pnum = 0;
    if (fname.size()>7){
      op = fname.substr(7+1,fname.size()-7-1);
      if (snum.empty()){
        pnum = atoi(W2CA(op.c_str()));
      }else{
        pnum = atoi(W2CA(snum.c_str()));
      }
    }

    string nav;
    _6bees_html::GetNavCode(urlbase,totalnum,pnum,nav);

    dbcontent += nav;
    dbcontent +=  
      "<table align=center cellpadding=1 cellspacing=1 width=80% bgcolor=#FFFFCC"
      " style='BORDER:#FFB900 1px solid;'>"
      "<tr bgcolor=#EEEBBB>"
      "<td width=2%>ID</td>"
      "<td width=70%>Meida Resource</td>"
      "<td width=20%>Web Page</td>"
      "<td width=4%>Format</td>"
      "<td width=4%>Action</td>"
      "</tr>";

    CppSQLite3Table tbl = DBRef.getTable("select * from mediares order by id desc limit 20");
    int rownum=tbl.numRows();
    if(rownum==0){
      dbcontent += "<tr><td colspan=5>There is no Media Resource detected!</td></tr>";
    }else{
      for (int j=0;j<rownum;j++){
        tbl.setRow(j);
        dbcontent += "<tr>\n";
        string webpagetitle=tbl.fieldValue("title");
        string webpage_url=tbl.fieldValue("webpage");
        string id=tbl.fieldValue("id");
        string meidaurl=tbl.fieldValue("mediaurl");
        string format=tbl.fieldValue("format");
        string _redirectflag=stringmaker()<<redirectflag;
        dbcontent += "<td>"+id+"</td><td><a href=\""+meidaurl+"\">"+
          meidaurl.substr(0,80)+"...</a></td><td><a href=\""+webpage_url+"\">"+\
          webpagetitle.substr(0,30)+"...</a></td><td>"+format+
          "</td><td><a href=\"ultra://action@delete&table@mediares&condition@id="
          +id+"&redirectpage@"+_redirectflag
          +"\"><img src=ultra://delete.png></a></td></tr>\n";
      }
    }
    dbcontent += "</table>"+ nav;

    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      fname = wstringmaker()<< upath << L"mediadetection\\mediares.html";
    }
  }

  void mediadetection::APPURLFilter(const wchar_t* _url){
    FilterMediaURL(_url);
  }

  void mediadetection::ProcessMediaURL(const wstring& lowerurl,METYPE mt){
    if(!mediabythread::R().HasThisURL(lowerurl)){
      C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
      if(pSB==NULL)
        return;
      //wstring weburl = _6bees_window::IE7::GetAddressBarText(pSB->m_hWnd);
      //LOWERWSTR(weburl);
      //if (weburl == lowerurl){
      //  return;
      //}
      IHTMLElement* pElement = pSB->GetElementByIDName("_6bee_div");
      wstringstream imgurl;
      imgurl << L"ultra://" << tyname[mt] << L".gif";
      if (pElement==NULL){
        // 1. show media info in web page
        using _6bees_util::ultrapath;
        const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
        if(_6bees_util::MakeSureDir(upath.c_str())){
          wstring mediaressnippet1_path = wstringmaker()<< upath << L"mediadetection\\mediares_snippet1.html";
          wstring flvhtml;
          ReadSnipppet(mediaressnippet1_path.c_str(),flvhtml);
          strreplace(flvhtml,L"$mediaurl",lowerurl.c_str());
          strreplace(flvhtml,L"$ultraimgurl",imgurl.str().c_str());
          wstring shortmurl=lowerurl.substr(0,80) + L"...";
          strreplace(flvhtml,L"$shortmediaurl",shortmurl.c_str());
          pSB->InsertAdjacentHTML(flvhtml.c_str(),L"afterBegin");// afterBegin

          // 2. update database
          wchar_t* _wurl = pSB->GetWebURL();
          wchar_t* _wtitle=pSB->GetWebTitle();
          wstring murl(_wurl);
          wstring mtitle(_wtitle);
          delete [] _wurl;
          delete [] _wtitle;
          InsertMedia(mtitle,murl,lowerurl,tyname[mt]);
        }
      }else{
        wstringstream htmlcode;
        static int i=1;/// @todo  - to be changed
        wstring shortmurl=lowerurl.substr(0,80) + L"...";
        htmlcode << L"<div style='text-align:left'>" << ++i << L". <img src='" << imgurl.str() << L"' border=0><a href=\"" 
          << lowerurl << L"\">" << shortmurl.c_str() << L"</a><div>";
        pElement->insertAdjacentHTML(CComBSTR("afterBegin"),CComBSTR(htmlcode.str().c_str()));
        pElement->Release();
      }
      mediabythread::R().AddURL(lowerurl);
    }
  }

  bool mediadetection::FilterMediaURL(const wchar_t* szUrl){
    if(enabled_){
      std::wstring lowerurl(szUrl);
      LOWERWSTR(lowerurl);
      if (lowerurl.substr(0,5)==L"ultra"){
        return true;
      }
      if(lowerurl.find(_T(".flv"))!=std::wstring::npos||
        lowerurl.find(_T("get_video?video_id="))!=std::wstring::npos)
      {
        ProcessMediaURL(lowerurl,FLV);
      }else if((lowerurl.find(_T(".swf"))!=-1)){
        ProcessMediaURL(lowerurl,SWF);
      }/*else if(CConfig::R().filter_rm && (lowerurl.find(_T(".rm"))!=-1)){
        ProcessMediaURL(lowerurl,RM);
      }else if(CConfig::R().filter_mp3 && (lowerurl.find(_T(".mp3"))!=-1)){
        ProcessMediaURL(lowerurl,MP3);
      }else if(CConfig::R().filter_wmv && (lowerurl.find(_T(".wmv"))!=-1)){
        ProcessMediaURL(lowerurl,WMV);
      }else if(CConfig::R().filter_wma && (lowerurl.find(_T(".wma"))!=-1)){
        ProcessMediaURL(lowerurl,WMA);
      }else if(CConfig::R().filter_avi && (lowerurl.find(_T(".avi"))!=-1)){
        ProcessMediaURL(lowerurl,AVI);
      }*/

      /// @todo  - add filter
      if(lowerurl.find(_T(".flv"))!=-1||lowerurl.find(_T("get_video?video_id="))!=-1){
        ProcessMediaURL(lowerurl,FLV);
      }else if((lowerurl.find(_T(".swf"))!=-1)){
        ProcessMediaURL(lowerurl,SWF);
      }else if(lowerurl.find(_T(".rm"))!=-1){
        ProcessMediaURL(lowerurl,RM);
      }else if(lowerurl.find(_T(".mp3"))!=-1){
        ProcessMediaURL(lowerurl,MP3);
      }else if(lowerurl.find(_T(".wmv"))!=-1){
        ProcessMediaURL(lowerurl,WMV);
      }else if(lowerurl.find(_T(".wma"))!=-1){
        ProcessMediaURL(lowerurl,WMA);
      }else if(lowerurl.find(_T(".avi"))!=-1){
        ProcessMediaURL(lowerurl,AVI);
      }
    }
    return true;
  }

  void mediadetection::AssociateImage(int wID,int& _img)
  {}

  bool mediadetection::OnClickMenuItem(int id){
    return true;
  }

  mediadetection::mediadetection(){
    //init image list
    enabled_ = true;
    len_ = 30;
    caption_=L"Media Detection";
    tip_ = L"Media Detection - Capture web media resource.";
    menuwID_ = 1318;
  }

  mediadetection::~mediadetection(){
    for (vector<HICON>::const_iterator i=icons_.begin();i!=icons_.end();++i){
      DestroyIcon(*i);
    }
  }

  bool mediadetection::ProcessBasedInit(){
    return true;
  }

  bool mediadetection::ProcessBasedTerm(){
    return true;
  }

  bool mediadetection::ThreadBasedInit(){
    return true;
  }

  bool mediadetection::ThreadBasedTerm(){
    return true;
  }

  bool mediadetection::init(){
    //::MessageBoxA(NULL,"mediadetection init","mediadetection",NULL);
    return true;
  }
  bool mediadetection::StartPlugin(){
    return true;
  }
  bool mediadetection::StopPlugin(){
    return true;
  }
  bool mediadetection::OnLButtonDown(int paneid,int xpos,int ypos){
    C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
    pSB->opennewtab(L"ultra://mediares/",true);
    return true;
  }
  bool mediadetection::OnRButtonDown(int){
    return true;
  }
  bool mediadetection::disenable_plugin(){
    return true;
  }

};
