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

#include "UltraProtocol.h"
#include <string>
#include "6bees_util.h"
#include <iostream>
#include <fstream>
#include "ReflectionWnd.h"
#include<wininet.h>
#pragma comment(lib,"wininet.lib")
#include "6bees_const.h"
#include <6bees_str.h>
#include "6bees_html.h"
#include "pluginmng.h"
#include "../../dll/6beecommond/6beecommond.h"
#pragma comment(lib,"6beecommond.lib")

using namespace std;

static const int PROTOCOLLENGTH = 8;

STDMETHODIMP CUltraProtocol::Continue(PROTOCOLDATA *pStateInfo){
  return S_OK;
}
STDMETHODIMP CUltraProtocol::Abort(HRESULT hrReason,DWORD dwOptions){
  return S_OK;
}
STDMETHODIMP CUltraProtocol::Terminate(DWORD dwOptions){
  return S_OK;
}
STDMETHODIMP CUltraProtocol::Suspend(){
  return E_NOTIMPL;
}
STDMETHODIMP CUltraProtocol::Resume(){
  return E_NOTIMPL;
}
STDMETHODIMP CUltraProtocol::LockRequest(DWORD dwOptions){
  return S_OK;
}
STDMETHODIMP CUltraProtocol::UnlockRequest(){
  //m_dwPos = 0;
  return S_OK;
}

void GeneralSetting(string& dbcontent,wstring& fname,char*& flag,bool isupdateregistry=false);

void Action(const wchar_t* tbname,string& dbcontent,wstring& fname,char*& flag){
  string cmd;
  wstring redirectpage;
  _6bees_html::ParseAction(tbname,fname,cmd,redirectpage);
  if (!cmd.empty()){
    using _6bees_util::ultrapath;
    string dbpath = ultrapath::R().getA(ultrapath::maindb);
    CppSQLite3DB& DBRef=CppSQLite3DB::instance(dbpath.c_str());
    DBRef.execDML(cmd.c_str());  
  }
   if (redirectpage==L"reGeneralSetting.html"){
    _6beed_share::update_GeneralSetting();
    GeneralSetting(dbcontent,fname,flag,true);
  }else{
    fname = redirectpage;
  }
}

#ifdef __CHINESE__
#define strhidesb L"隐藏IE内置搜索框(需要重启IE以生效)"
#define stroneinst L"强制IE单进程"
#define strloadfwe L"对windows的对象浏览器加载六只蜜蜂搜索框"
#else
#define strhidesb "Hide IE builtin Search Box (IE restart required.)"
#define stroneinst "Enforce IE one instance"
#define strloadfwe "Load UltraIE Search Bar for Windows Explorer"
#endif
void GeneralSetting(string& dbcontent,wstring& fname,char*& flag,bool isupdateregistry){
  using namespace _6beed_share;
  flag = "<!-- GeneralSetting -->";
  string b_getiebuiltinSEBox = u_getiebuiltinSEBox() ? "checked" : "unchecked";
  string b_getieonewindow = u_getieonewindow() ? "checked" : "unchecked";
  string b_explorerloadultraie = u_getloadforexplorer() ? "checked" : "unchecked";
  dbcontent = "";
  int i=0;
  if (u_getieversion()>6){
    dbcontent += _6bees_str::stringmaker(CP_UTF8)<<
      "<tr><td>" << ++i << ". "
      << strhidesb
      << "<td width=30><INPUT TYPE=checkbox "
      << b_getiebuiltinSEBox << " ID=chk1 name=chkbox1>\n";
  }
  if (u_getieversion()==7){
    dbcontent += _6bees_str::stringmaker(CP_UTF8)<<
      "<tr><td>" << ++i << ". "
      << stroneinst
      << "<td width=30><INPUT TYPE=checkbox "
      << b_getieonewindow << " ID=chk2 name=chkbox2>\n";
  }else{
    dbcontent += _6bees_str::stringmaker(CP_UTF8)<<
      "<INPUT TYPE=hidden ID=chk2 name=chkbox2>\n";
  }
  dbcontent += _6bees_str::stringmaker(CP_UTF8)<<
    "<tr><td>" << ++i << ". "
    << strloadfwe
    << "<td width=30><INPUT TYPE=checkbox "
    << b_explorerloadultraie << " ID=chk1 name=chkbox3>";
  fname = _T("GeneralSetting.html");
  if (isupdateregistry){
    u_setiebuiltinSEBox(u_getiebuiltinSEBox());
  }
}

void CUltraProtocol::GetPostData(IInternetBindInfo *pIBindInfo){
  m_bindinfo.cbSize = sizeof(BINDINFO);
  HRESULT hr;
  if (pIBindInfo)
    hr = pIBindInfo->GetBindInfo(&m_bindf, &m_bindinfo);

  switch (m_bindinfo.dwBindVerb){
    case    BINDVERB_POST:
    void *pData;
    UINT cPostData;	// Post data size.

    if (m_bindinfo.stgmedData.tymed != TYMED_HGLOBAL)
      break;

    cPostData = m_bindinfo.cbstgmedData;
    if (!cPostData)
      break;

    pData = GlobalLock(m_bindinfo.stgmedData.hGlobal);
    if (pData){
      // Allocate space to store the POST data if required.
      // For instance, a member variable, m_postData, 
      // declared as "BYTE *m_postData;", could be used 
      // as below:
      m_postData = new BYTE[cPostData];
      memcpy(m_postData, pData, cPostData);
      m_postDataLen = cPostData;
      // After checking the data, unlock buffer.
      GlobalUnlock(m_bindinfo.stgmedData.hGlobal);

      // Retrieve MIME type of the post data.
      LPOLESTR pszMIMEType;	
      ULONG dwSize;
      hr = pIBindInfo->GetBindString(BINDSTRING_POST_DATA_MIME, &pszMIMEType, 1, &dwSize);

      if(hr == S_OK){
        // pszMIMEType now contains the MIME type of the post data.
        // This would typically be "application/x-www-form-urlencoded" 
        // for a POST. In general, it could be any (standard or 
        // otherwise) MIME type. Many of the standard MIME type strings 
        // are #defined in <URLMon.h>. For instance, CFSTR_MIME_TEXT 
        // is L"text/plain".
        // Store the MIME type in a member variable here, if required.
        // Finally, free pszMIMEType via CoTaskMemFree.
        if (pszMIMEType){
          CoTaskMemFree(pszMIMEType);
          pszMIMEType = NULL;
        }
      }else{
        // Assume "application/x-www-form-urlencoded".
      }
    }
    break;
  default:
    // It's a GET.
    break;
  }
}

STDMETHODIMP CUltraProtocol::Start(LPCWSTR szUrl,IInternetProtocolSink *pIProtSink,
  IInternetBindInfo *pIBindInfo,DWORD grfSTI,DWORD dwReserved)
{
  HRESULT hr = S_OK;
  USES_CONVERSION;
  m_dwPos = 0;
  const wchar_t* strData=_T("");
  pIProtSink->ReportProgress(BINDSTATUS_FINDINGRESOURCE, strData);
  pIProtSink->ReportProgress(BINDSTATUS_CONNECTING, strData);
  pIProtSink->ReportProgress(BINDSTATUS_SENDINGREQUEST, strData);
  //pIProtSink->ReportProgress(BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE, CAtlString(m_url.GetMimeType()));

  GetPostData(pIBindInfo);

  wstring temp = szUrl;
  wstring fname(temp,PROTOCOLLENGTH,
    temp.rfind(_T("/"))==temp.size()-1 ? 
    temp.size()-1-PROTOCOLLENGTH : temp.size()-PROTOCOLLENGTH);
  string dbcontent;
  char* flag="flagplacement";// flag place-holder

  if(fname.find(_T(".png"))!=wstring::npos){
    pIProtSink->ReportProgress(BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE, _T("image/png"));
  }else if(fname.find(_T(".gif"))!=wstring::npos){
    pIProtSink->ReportProgress(BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE, _T("image/gif"));
  }else if(fname.find(_T(".ico"))!=wstring::npos){
    pIProtSink->ReportProgress(BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE, _T("image/x-icon"));
  }else if(fname.find(_T(".jpg"))!=wstring::npos){
    pIProtSink->ReportProgress(BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE, _T("image/jpg"));
  }else if(fname.find(_T(".js"))!=wstring::npos){
    pIProtSink->ReportProgress(BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE, _T("application/x-javascript"));
    ///////////////////////////////////////////////////////////////////////////
#if 0
    if(fname.compare(_T("config_mediadetection.js"))==0){
      flag = "<!-- MediaDetection -->";
      // todo - redesign the code
      /*
      sql_result = sqlite3_get_table( db,"select mediadetection from UUConfig", &dbResult, &sql_nRow, &sql_nColumn, &errmsg );
      if(SQLITE_OK== sql_result){
        if(sql_nRow==1){
          char *media_detection = dbResult[1];
          filter_flv = strstr(media_detection,"flv")!=NULL;
          filter_swf = strstr(media_detection,"swf")!=NULL;
          filter_rm  = strstr(media_detection,"rm") !=NULL;
          filter_mp3 = strstr(media_detection,"mp3")!=NULL;
          filter_wmv = strstr(media_detection,"wmv")!=NULL;
          filter_wma = strstr(media_detection,"wma")!=NULL;
          filter_avi = strstr(media_detection,"avi")!=NULL;
        }
      }
      if(filter_flv){
        dbcontent  += "<div><img src=ultra://flv.gif><input type=checkbox checked name=flv id=flv>FLV</div>";
      }else{
        dbcontent  += "<div><img src=ultra://flv.gif><input type=checkbox name=flv id=flv>FLV</div>";
      }
      if(filter_swf){
        dbcontent  += "<div><img src=ultra://swf.gif><input type=checkbox checked name=swf id=swf>SWF</div>";
      }else{
        dbcontent  += "<div><img src=ultra://swf.gif><input type=checkbox name=swf id=swf>SWF</div>";
      }
      if(filter_rm){
        dbcontent  += "<div><img src=ultra://rm.gif><input type=checkbox checked name=rm id=rm>RM</div>";
      }else{
        dbcontent  += "<div><img src=ultra://rm.gif><input type=checkbox name=rm id=rm>RM</div>";
      }
      if(filter_mp3){
        dbcontent  += "<div><img src=ultra://wm.gif><input type=checkbox checked name=mp3 id=mp3>MP3</div>";
      }else{
        dbcontent  += "<div><img src=ultra://wm.gif><input type=checkbox name=mp3 id=mp3>MP3</div>";
      }
      if(filter_wmv){
        dbcontent  += "<div><img src=ultra://wm.gif><input type=checkbox checked name=wmv id=wmv>WMV</div>";
      }else{
        dbcontent  += "<div><img src=ultra://wm.gif><input type=checkbox name=wmv id=wmv>WMV</div>";
      }
      if(filter_wma){
        dbcontent  += "<div><img src=ultra://wm.gif><input type=checkbox checked name=wma id=wma>WMA</div>";
      }else{
        dbcontent  += "<div><img src=ultra://wm.gif><input type=checkbox name=wma id=wma>WMA</div>";
      }
      if(filter_avi){
        dbcontent  += "<div><img src=ultra://wm.gif><input type=checkbox checked name=avi id=avi>AVI</div>";
      }else{
        dbcontent  += "<div><img src=ultra://wm.gif><input type=checkbox name=avi id=avi>AVI</div>";
      }
      */
      dbcontent  += "</form>";
      dbcontent  += "<hr><input type='button' value='Confirm' onClick='confirm_mediadetection()'>";
    }

    /*if(fname.compare(_T("config_httpproxy.js"))==0){
      flag = "<!-- http_ip_port -->";
      dbcontent = "Proxy IP: <input type=text id='http_ip' name='http_ip' value='";
      bool proxy_enabled = false;
      CRegKey crk;
      if (ERROR_SUCCESS == crk.Open(HKEY_CURRENT_USER,
        _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"))){
        DWORD pee=0;
        if(ERROR_SUCCESS==crk.QueryDWORDValue(_T("ProxyEnable"),pee) && pee==1){
          proxy_enabled = true;
        }
        ULONG pe=256;
        TCHAR pszValue[256];
        if(ERROR_SUCCESS==crk.QueryStringValue(_T("ProxyServer"),pszValue,&pe)){
          wstring p = pszValue;
          if(p.substr(0,5).compare(_T("http="))==0){ //应该用正则表达式来判定!!
            p = p.substr(5,p.size()-5);
          }
          vector<wstring> pp = Split(p,_T(":"));
          dbcontent += W2CA(pp[0].c_str());
          dbcontent += "' size=20> Port: <input type=text id='http_port' name='http_port' value='";
          dbcontent += W2CA(pp[1].c_str());
        }else{
          dbcontent += "' size=20> Port: <input type=text id='http_port' name='http_port' value='";
        }
        crk.Close();
      }
      dbcontent +="' size=4>";
      if (proxy_enabled){
        dbcontent += "<br><br>Enable Proxy: <INPUT TYPE=checkbox CHECKED ID=chk1 name=chkbox>";
      }else{
        dbcontent += "<br><br>Enable Proxy: <INPUT TYPE=checkbox UNCHECKED ID=chk1 name=chkbox>";
      }
    }*/
    if(fname.compare(_T("config_surfhistory.js"))==0){
      flag = "<!-- surfhistory -->";
      dbcontent = "<input type=checkbox checked ID='enablesurfhistory'>Enable Surf History<br>";
      //if(_6beed_util::CConfig::R().GetHistasHP()){
      if(false){
        dbcontent += "<input type=checkbox checked ID='setsurfhistoryashomepage' >Set Surf History as my homepage<br>";
      }else{
        dbcontent += "<input type=checkbox unchecked ID='setsurfhistoryashomepage >Set Surf History as my homepage<br>";
        dbcontent += "IE Home Page: <input type=text ID='homepage' size=80><br>";
      }
      dbcontent += "    <SELECT name='pcount'>\
            <OPTION value='100'>100</OPTION>\
            <OPTION value='150'>150</OPTION>\
            <OPTION selected value='200'>200</OPTION>\
            <OPTION value='250'>250</OPTION>\
            <OPTION value='300'>300</OPTION>\
            <OPTION value='400'>400</OPTION>\
            <OPTION value='500'>500</OPTION>\
            </SELECT>\
            items per page";
      /// @todo 
    }
    if(fname.compare(_T("config_adsfilter.js"))==0){
      flag = "<!-- adsfilter -->";
      /// @todo 
    }
    if(fname.compare(_T("config_antivirus.js"))==0){
      flag = "<!-- antivirus -->";
      dbcontent = "<input type=checkbox checked ID='newprocess'>Mornitoring virus loading new process<br>\
            <input type=checkbox checked ID='newprocess'>Mornitoring virus modifying registry<br>\
            <input type=checkbox checked ID='newprocess'>Mornitoring virus modifying startup setting<br>\
            <input type=checkbox checked ID='newprocess'>Mornitoring virus inject remote process/thread<br>\
            <input type=checkbox checked ID='newprocess'>Mornitoring virus loading cross site Trojan horse<br>\
            ";
      /// @todo 
    }
#endif
  }else{
    nsplugin::PluginMng::R().StartUltraProtocol(szUrl,dbcontent,fname,flag,m_postData,m_postDataLen);//

    using _6bees_util::ultrapath;
    string dbpath = ultrapath::R().getA(ultrapath::maindb);
    CppSQLite3DB& DBRef=CppSQLite3DB::instance(dbpath.c_str());

    pIProtSink->ReportProgress(BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE, _T("text/html"));//_T("text/text")
    ///////////////////////////////////////////////////////////////////////////
#if 0
    if(fname.compare(_T("Last"))==0){
      flag = "<!-- Last -->";
      CppSQLite3Table tbl = DBRef.getTable("select * from SurfHistory where time_finish=0");
      int rownum=tbl.numRows();
      if(rownum==0){
        dbcontent += "<tr><td colspan=4> There is no Crashed Page!</td></tr>";
      }else{
        for (int j=0;j<rownum;j++){
          tbl.setRow(j);
          dbcontent += "<tr>\n";
          string id=tbl.fieldValue("id");
          string url=tbl.fieldValue("url");
          string title=tbl.fieldValue("title");
          string time_begin=tbl.fieldValue("time_begin");
          string time_finish=tbl.fieldValue("time_finish");
          dbcontent += "<td>"+id+"</td><td><a href=\""+url+"\">"+title+
            "</a></td><td>"+time_begin+"</td><td>"+time_finish+"</td>\n</tr>\n";
        }
      }
      fname = fname + _T(".html");
    }else 
#endif
    if(fname.substr(0,6).compare(_T("action"))==0){
      Action(L"UUConfig",dbcontent,fname,flag);
    }else if(fname.substr(0,7).compare(_T("Setting"))==0){
      // 数据格式: Setting@[MeidaResources|Proxy[|confirm=1.2.3.4:8080]|...]
      wstring op = fname.substr(7,fname.size()-7);
      if (op.substr(0,15).compare(_T("@MediaDetection"))==0){
        // todo -  修改Media detection设置
        // 数据格式: Setting@MeidaDetection|confirm=rm:flv:mp3
        /*wstring op2 = op.substr(15,op.size()-15);
        if(op2.substr(0,9).compare(_T("|confirm="))==0){
          wstring cmd = _T("update UUConfig set mediadetection='") + op2.substr(9,op2.size()-9) + _T("'");
          if(SQLITE_OK==sqlite3_exec(db,W2CA(cmd.c_str()),NULL,NULL,&errmsg)){
            char *tmp = W2A(op2.substr(9,op2.size()-9).c_str());
            filter_flv = strstr(tmp,"flv")!=NULL;
            filter_swf = strstr(tmp,"swf")!=NULL;
            filter_rm  = strstr(tmp,"rm") !=NULL;
            filter_mp3 = strstr(tmp,"mp3")!=NULL;
            filter_wmv = strstr(tmp,"wmv")!=NULL;
            filter_wma = strstr(tmp,"wma")!=NULL;
            filter_avi = strstr(tmp,"avi")!=NULL;
          }else{
            ::MessageBox(NULL,A2CW(errmsg),_T("Info"),NULL);
          }
        }
        */
        fname = _T("Setting.html");
      }else{
        fname = _T("Setting.html");
      }
    }
    else if(fname == _T("GeneralSetting")){
      GeneralSetting(dbcontent,fname,flag);
    }else{}

    ///////////////////////////////////////////////////////////////////////////
  }
  wstring TemplateFileName;
  if (PathFileExists(fname.c_str())){
    TemplateFileName = fname;
  }else{
    using _6bees_util::ultrapath;
    bool is_icon_url = _6bees_str::startwith<wstring>(fname,L"Icon/");
    static int const isize = 5;//  size of L"Icon/"
    const wstring& upath = is_icon_url ? ultrapath::R().get(ultrapath::tempicondir) 
      : ultrapath::R().get(ultrapath::tempdir);
    TemplateFileName = upath + (is_icon_url ? fname.substr(isize) : fname);
    if (!PathFileExists(TemplateFileName.c_str())){
      TemplateFileName += _T(".html");
    }
  }

  delete [] m_postData;
  m_postData = NULL;

  if (PathFileExists(TemplateFileName.c_str())){
    int filesize=0;
    _6beed_util::GetFileSizeW(TemplateFileName.c_str(),filesize);
    content_length = filesize+(unsigned long)dbcontent.size();

    ifstream myfile(W2CA(TemplateFileName.c_str()),ios::in|ios::binary);
    if(!myfile.is_open()){return S_FALSE;}
    memblock = new char [content_length];
    myfile.read (memblock, filesize);
    myfile.close();

    for(register int i=0;i<filesize;i++){
      if(memblock[i]==flag[0]){
        register size_t j=0,flaglen=strlen(flag);
        for (;j<flaglen;++j){
          if(memblock[i+j]!=flag[j])
            break;
        }
        if(j==flaglen){
          memmove_s(memblock+content_length-(filesize-i), filesize-i, memblock+i, filesize-i);
          memcpy_s(memblock+i, content_length-i, dbcontent.c_str(), dbcontent.size());
          break;
        }
      }
    }
    pIProtSink->ReportData(BSCF_FIRSTDATANOTIFICATION, 0, content_length);
    pIProtSink->ReportData(BSCF_LASTDATANOTIFICATION | BSCF_DATAFULLYAVAILABLE, content_length, content_length);
    return hr;
  }else{
    return S_FALSE;
  }
}
/*
在Read 方法中，IE会传过来一个内部缓冲区的指针pv，同时cb参数表示缓冲区的大小，电子书的数据有可能会很大，而IE的缓冲区不会无限大，因此IE会分多次来读取电子书的数据，
我们每次应该尽可能读取cb大小的数据，将其移动到IE的缓冲区内，读取完成后减少BytesLeft的值，同时增加 CurrPos 的值来记录当前以发送给IE的数据位置，并返回cbRead告诉IE
传送的数据到底有多少。如果一次没有返回全部的数据，则返回S_OK 通知IE还有没传送完的数据，这样IE就会继续调用Read方法来完成数据下载，最后当所有的数据都处理完毕后，
则返回S_False通知IE已经没有要传的数据了，同时，调用事件接口ProtSink的ReportData方法通知IE，协议处理完毕。
*/
STDMETHODIMP CUltraProtocol::Read(void *pv, ULONG cb, ULONG *pcbRead){
  if (m_dwPos >= content_length)
    return S_FALSE;
  DWORD cbAvail = content_length - m_dwPos;
  memcpy_s(pv, cb, memblock, cbAvail>cb ? cb : cbAvail);

  m_dwPos += cbAvail>cb ? cb : cbAvail;
  *pcbRead = cbAvail>cb ? cb : cbAvail;
  if(cbAvail>cb)
    memmove_s(memblock, content_length, memblock+cb, content_length-cb); 
  else
    delete [] memblock;
  return S_OK;
}

STDMETHODIMP CUltraProtocol::Seek(LARGE_INTEGER dlibMove,
  DWORD dwOrigin,ULARGE_INTEGER *plibNewPosition)
{
  return E_NOTIMPL;
}

STDMETHODIMP CUltraProtocol::CombineUrl(LPCWSTR pwzBaseUrl,LPCWSTR pwzRelativeUrl,
  DWORD dwCombineFlags,LPWSTR pwzResult,DWORD cchResult,
  DWORD *pcchResult,DWORD dwReserved)
{
  return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CUltraProtocol::CompareUrl(LPCWSTR pwszUrl1,
  LPCWSTR pwszUrl2, DWORD dwCompareFlags)
{
  /*
  ATLTRACE(_T("CompareUrl\n"));
  if (pwszUrl1 == NULL || pwszUrl2 == NULL)
  return E_POINTER;
  HRESULT hr = S_FALSE;
  CDataURL url1, url2;
  if (url1.Parse(pwszUrl1) && url2.Parse(pwszUrl2) && url1 == url2){
  hr = S_OK;
  }
  return hr;
  */
  return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CUltraProtocol::ParseUrl(LPCWSTR pwzUrl, PARSEACTION parseAction,
  DWORD dwParseFlags, LPWSTR pwzResult, DWORD cchResult,
  DWORD *pcchResult, DWORD dwReserved)
{
  return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CUltraProtocol::QueryInfo( LPCWSTR pwzUrl, QUERYOPTION QueryOption,
  DWORD dwQueryFlags, LPVOID pBuffer, DWORD cbBuffer,
  DWORD *pcbBuf, DWORD dwReserved)
{
  return INET_E_DEFAULT_ACTION;
}