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

#include "olbm.h"
#include "olbmdlg.h"
#include "resource.h"

#include "6bees_util.h"
#include "6bees_html.h"
#include "6bees_window.h"
#include "6bees_const.h"
#include "6bees_shell.h"
#include <6bees_net.h>
#include "IceSingleton.h"
#include "favengine.h"
#include "uulogging.h"
#include <map>
#include <algorithm>
#include <6bees_const.h>

extern HMODULE hinst;

namespace nsplugin{
  using namespace std;
  using namespace _6bees_str;
  using namespace _6bees_const;

#ifdef __CHINESE__
#define ktip_olbm L"在线书签"
#define kcap_olbm L"在线书签"
#else
#define ktip_olbm L"Online Bookmark"
#define kcap_olbm L"Online Bookmark"
#endif

  static const wchar_t olbmsettingurl[] = L"ultra://olbmsetting";
  static const wchar_t actionurl[] = L"ultra://action";
  static const wchar_t olbmuploadurl[] = L"ultra://olbmupload";
  static const wchar_t olbmdownloadurl[] = L"ultra://olbmdownload";
  static const wchar_t olbmloadingurl[] = L"ultra://olbmloading";
  static const wchar_t douploadurl[] = L"ultra://do_upload";
  static const wchar_t dodownloadurl[] = L"ultra://do_download";
  static const wchar_t* redirectflag = L"reolbmsetting";

  UUTOOLBAR::OLBMItems olbmitems;

  void SaveToOnLineBookMark();

  UINT WINAPI thread_prepareUploadIEFav(LPVOID);
  UINT WINAPI thread_prepareDownloadIEFav(LPVOID);
  UINT WINAPI thread_uploadIEFav(LPVOID);
  //UINT WINAPI thread_downloadIEFav(LPVOID);

  string GetDBPath(){
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      string sh_dbpatha_=stringmaker()<< upath << "onlinebookmark\\olbm.db";
      return sh_dbpatha_;
    }
    return string();
  }

  bool olbm::OnNavigateComplete2(int paneid,const wchar_t* url){
    return true;
  }

  int olbm::InsertandGetMenuImgIndex(){
    static set<DWORD> threadids;
    static int first_img_index = 0;
    DWORD tid = GetCurrentThreadId();
    if (threadids.find(tid)==threadids.end()){
      C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
      static const int picnum = 5;
      HBITMAP hbitmap=(HBITMAP)LoadImage(hinst,
        MAKEINTRESOURCE(IDB_OLBMMENU),
        IMAGE_BITMAP,picnum*16,16,LR_DEFAULTSIZE|LR_SHARED);
      threadids.insert(tid);
      first_img_index = pSB->AddToImgList(hbitmap,RGB(193,193,193));
    }
    return first_img_index;
  }

  void olbm::AssociateImage(int wID,int& _img){
    int first_img_index = InsertandGetMenuImgIndex();
    switch(wID){
      case ID_OLBM_DOWNLOAD:
        _img = first_img_index;
        break;
      case ID_OLBM_UPLOAD:
        _img = first_img_index + 1;
        break;
      case ID_OLBM_SETTING:
        _img = first_img_index + 2;
        break;
      case ID_OLBM_ADD:
        _img = first_img_index + 3;
        break;
      case ID_OLBM_MANAGE:
        _img = first_img_index + 4;
        break;
    }
  }

  bool olbm::OnClickMenuItem(int id){
    switch(id){
      case ID_OLBM_ADD:
        SaveToOnLineBookMark();
        break;
      case ID_OLBM_UPLOAD:
        olbmitems.clear();
        GetC6BeeMenuStatusBarPtr()->GetPWB2()->NavToURL("ultra://olbmloading");
        ::_beginthreadex(NULL,0,thread_prepareUploadIEFav,(LPVOID)(GetC6BeeMenuStatusBarPtr()->GetPWB2()),0,NULL);
        break;
      case ID_OLBM_DOWNLOAD:
        olbmitems.clear();
        GetC6BeeMenuStatusBarPtr()->GetPWB2()->NavToURL("ultra://olbmloading");
        ::_beginthreadex(NULL,0,thread_prepareDownloadIEFav,(LPVOID)(GetC6BeeMenuStatusBarPtr()->GetPWB2()),0,NULL);
        break;
      case ID_OLBM_MANAGE:
        GetC6BeeMenuStatusBarPtr()->GetPWB2()->PostNav("/admin/onlinebookmark.php");
        break;
      case ID_OLBM_SETTING:
        GetC6BeeMenuStatusBarPtr()->GetPWB2()->NavToURL(olbmsettingurl);
        break;
    }
    return true;
  }

  olbm::olbm(){
    enabled_ = true;
    len_ = 28;
    caption_=kcap_olbm;
    tip_ = ktip_olbm;
    menuwID_ = 1322;
  }

  olbm::~olbm(){
    for (vector<HICON>::const_iterator i=icons_.begin();i!=icons_.end();++i){
      DestroyIcon(*i);
    }
  }

  bool olbm::ProcessBasedInit(){
    return true;
  }

  bool olbm::ProcessBasedTerm(){
    return true;
  }

  bool olbm::ThreadBasedInit(){
    return true;
  }

  bool olbm::ThreadBasedTerm(){
    return true;
  }

  bool olbm::init(){
    return true;
  }

  bool olbm::StartPlugin(){
    return true;
  }

  bool olbm::StopPlugin(){
    return true;
  }

  bool olbm::OnLButtonDown(int paneid,int xpos,int ypos){
    menu_ = ::LoadMenuW(hinst,MAKEINTRESOURCE(IDR_OLBM));
    HMENU menuPopup=::GetSubMenu(menu_,0);
    if (menuPopup){
      C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
      TrackPopupMenu(menuPopup,
        TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_LEFTBUTTON|TPM_HORPOSANIMATION,
        xpos,ypos,0,pSB->m_hWnd,NULL);
      DestroyMenu(menuPopup);
      DestroyMenu(menu_);
    }
    return true;
  }

  wstring ConvertPostDataToWString(BYTE* pbuffer,ULONG buffersize){
    BYTE* pb = new BYTE[buffersize+1];
    memcpy(pb,pbuffer,buffersize);
    pb[buffersize]='\0';
    wstring url= wstringmaker() << (char*)pb;
    delete [] pb;
    return url;
  }

  void olbm::RemoveOLBMItems(wstring postdata){
    /// @todo verify postdata format
    vector<wstring> v = _6bees_util::Split(postdata,L"&");
    set<int> uploadurls_index;
    for (vector<wstring>::iterator j=v.begin();j!=v.end();++j){
      size_t t= j->find(L'=');
      if (t!=wstring::npos && t!=0){
        uploadurls_index.insert(_wtoi(j->substr(0,t).c_str()));
      }
    }

    int k=0;
    UUTOOLBAR::OLBMItems newolbms;
    for (map<string,set<string>>::iterator i=folder2urls.begin();i!=folder2urls.end();++i){
      for (set<string>::iterator j=i->second.begin();j!=i->second.end();++j){
        if (uploadurls_index.find(k++)!=uploadurls_index.end()){
          string& url = *j;
          newolbms[url] = olbmitems[url];
        }
      }
    }
    olbmitems = newolbms;
  }

  void olbm::StartUltraProtocol(const wchar_t* _url,
    std::string& dbcontent,
    std::wstring& fname,
    char*& flag,BYTE* pbuffer,ULONG buffersize)
  {
    wstring url(_url);
    LOWERWSTR(url);

    using namespace _6bees_str;
    if(startwith<wstring>(url,olbmsettingurl)){
      OLBMSetting(dbcontent,fname,flag);
    }else if (startwith<wstring>(url,olbmuploadurl)){
      OLBMUpload(dbcontent,fname,flag);
    }else if (startwith<wstring>(url,olbmloadingurl)){
      OLBMLoading(dbcontent,fname,flag);
    }else if (startwith<wstring>(url,douploadurl)){
      // we need to use post data here!
      RemoveOLBMItems(ConvertPostDataToWString(pbuffer,buffersize));
      OLBMDoUpload(dbcontent,fname,flag);
    }else if (startwith<wstring>(url,olbmdownloadurl)){
      OLBMDownload(dbcontent,fname,flag);
    }else if (startwith<wstring>(url,dodownloadurl)){
      // we need to use post data here!
      RemoveOLBMItems(ConvertPostDataToWString(pbuffer,buffersize));
      OLBMDoDownload(dbcontent,fname,flag);
    }else if (startwith<wstring>(url,actionurl)){
      ///@todo or to remove
    }
  }

  void olbm::OLBMSetting(string& dbcontent,wstring& fname,char*& flag){
    flag = "<!-- OLBMSetting -->";
    dbcontent =  "<table align=center cellpadding=1 cellspacing=1 width=100% "
    "bgcolor=#FFFFCC style='BORDER:#FFB900 1px solid;'>\n"
    "<tr bgcolor=#EEEBBB>\
    <td width=2%>ID</td>\
    <td width=66%>WebPage</td>\
    <td width=14%>Time Begin</td>\
    <td width=14%>Time Finish</td>\
    <td width=4%>Action</td>\
    </tr>";
    dbcontent += "</table>\n";

    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      fname = wstringmaker() << upath << L"onlinebookmark\\olbmsetting.html";
    }
  }

  string getpadding(int depth,const char* str){
    depth = depth>20 ? 20 : depth;
    string tmp;
    for (int i=0;i<depth;i++){
      tmp.append(str);
    }
    return tmp;
  }

  void olbm::constructiefavs(string& dbcontent,bool isupload){
    dbcontent += isupload ? "<b>Local IE favorites to upload</b>" : "<b>UltraIE onlinebookmarks to download</b>";
    dbcontent +=  "<br><br><form action='";
    dbcontent +=  isupload ? "ultra://do_upload" : "ultra://do_download";
    dbcontent +=  "' method=post>";

    folder2urls.clear();
    for (UUTOOLBAR::OLBMItems::iterator i=olbmitems.begin();i!=olbmitems.end();++i){
      folder2urls[i->second.folder].insert(i->first);
    }

    int n=0;
    for (map<string,set<string>>::iterator i=folder2urls.begin();i!=folder2urls.end();++i){
      int depth = (int)std::count(i->first.begin(), i->first.end(), '|');
      dbcontent +=  "<table align=center cellpadding=1 cellspacing=1 width=800 bgcolor=#FFFFCC style='border:0px'>";
      string padding = getpadding(depth,"<td width=20>");
      dbcontent +=  "<tr>"+padding +"<td colspan=2>";
      dbcontent += "<img src='ultra://folder.gif'> ";
      
      string folderpath(i->first);
      strreplace(folderpath,"|","/");
      size_t t1 = i->first.find_last_of('|');
      string lastfolder = i->first.substr(t1+1,i->first.size()-t1-1);//lastfolder is the folder which will be disaplayed
      dbcontent += lastfolder + " (/" + folderpath + ")";

      set<string>& urls = i->second;
      for (set<string>::iterator j=urls.begin();j!=urls.end();++j){
        string url = *j;
        string title = olbmitems[*j].title;
        if (url.size()>60){
          url = url.substr(0,60).append("...");
        }
        if (title.size()>100){
          title = title.substr(0,100).append("...");
        }
        string iconultraurl=_6bees_util::GetIconUltraURL(url);
        dbcontent += stringmaker() << "<tr>"<< padding << "<td width=10><input type='checkbox' checked=true name="<< n++ << "><td>"
        << n << ". <img src='" << iconultraurl << "' width=16 height=16> " << title << " <a href="<< *j <<">"<< url << "</a>\n";
      }
      dbcontent += "</table>";
    }
    dbcontent += "\n<br><input type=submit value='";
    dbcontent += isupload ? "upload" : "download";
    dbcontent += "'></from>";
  }

  void olbm::OLBMUpload(string& dbcontent,wstring& fname,char*& flag){
    flag = "<!-- OLBMUpload -->";
    if (olbmitems.empty()){
      dbcontent += "<b>You have uploaded all your favorites to onlinebookmark.</b>\
                   <br><img src=http://i212.photobucket.com/albums/cc237/Bonji-mon/Congratulations.jpg><br>\
                   <meta http-equiv=\"refresh\" content=\"5;url="+kolbmURLa+"\" />\
                   This webpage will go to your online bookmark after 5 seconds.\
                   ";

    }else{
      constructiefavs(dbcontent,true);
    }
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      fname = wstringmaker() << upath << L"onlinebookmark\\olbmupload.html";
    }
  }

  void olbm::OLBMDownload(string& dbcontent,wstring& fname,char*& flag){
    flag = "<!-- OLBMDownload -->";
    if (olbmitems.empty()){
      dbcontent += "<b>You have downloaded all your favorites from onlinebookmark.</b>\
                   <br><img src=http://i212.photobucket.com/albums/cc237/Bonji-mon/Congratulations.jpg><br>\
                   <meta http-equiv=\"refresh\" content=\"5;url="+kolbmURLa+"\" />\
                   This webpage will go to your online bookmark after 5 seconds.\
                   ";

    }else{
      constructiefavs(dbcontent,false);
    }
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      fname = wstringmaker() << upath << L"onlinebookmark\\olbmdownload.html";
    }
  }

  void olbm::OLBMLoading(string& dbcontent,wstring& fname,char*& flag){
    flag = "<!-- OLBMLoading -->";

    dbcontent +=  "<b>Comparing local favorites and online bookmark</b><br>";

    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      fname = wstringmaker() << upath << L"onlinebookmark\\olbmloading.html";
    }
  }

  void olbm::OLBMDoUpload(string& dbcontent,wstring& fname,char*& flag){
    //flag = "<!-- OLBMDoUpload -->";
    flag = "<!-- OLBMLoading -->";

    dbcontent +=  "<b>Uploading your favorites to online bookmark</b><br>";

    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      fname = wstringmaker() << upath << L"onlinebookmark\\olbmloading.html";
    }
    ::_beginthreadex(NULL,0,thread_uploadIEFav,(LPVOID)(GetC6BeeMenuStatusBarPtr()->GetPWB2()),0,NULL);
  }

  void olbm::OLBMDoDownload(string& dbcontent,wstring& fname,char*& flag){
    //flag = "<!-- OLBMDoUpload -->";
    flag = "<!-- OLBMLoading -->";

    dbcontent +=  "<b>Downloading UltraIE online bookmarks to your favorites...</b><br>";

    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      fname = wstringmaker() << upath << L"onlinebookmark\\olbmloading.html";
    }

    {
      //timer_notifier tn(CPYDATA_DOWNLOADIEFAV_START,CPYDATA_DOWNLOADBUSY_STOP);
      _6bees_shell::writeOLBMItemstoIEFav(olbmitems);//blocking
      //_6bees_window::NotifyClientMng(CPYDATA_DOWNLOADIEFAV_OK,_6bees_const::kUploader_WndTitle,NULL);
      GetC6BeeMenuStatusBarPtr()->GetPWB2()->PostNav(kolbmURLa);
    }
    
  }

  bool olbm::OnRButtonDown(int){
    return true;
  }

  bool olbm::disenable_plugin(){
    return true;
  }

  void olbm::OnHookMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){}



  void SaveToOnLineBookMark(){
    C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
    _6bees_html::c6beewb2ptr* _pwb = pSB->GetPWB2();
    if (!_pwb){
      return;
    }
    IHTMLDocument2Ptr& pHtmlDoc2 = _pwb->GetHTMLDoc2();
    if (!pHtmlDoc2){
      return;
    }
    wstring MagicStr=GenUniqStr();
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::picdir);
    if(!_6bees_util::MakeSureDir(upath.c_str())){
      return;
    }
    ATL::CString imgpath;
    imgpath.Format(_T("%s%s%s"),upath.c_str(),MagicStr.c_str(),L".jpg");
    ATL::CString imgpath_thumb;
    imgpath_thumb.Format(_T("%s%s_thumb%s"),upath.c_str(),MagicStr.c_str(),L".jpg");

    //Generate image for thumb    
    _6bees_html::EntirePageCapture( pSB->m_hWnd,
      _pwb,
      imgpath,
      imgpath_thumb,
      false,false,false,0);

    wstring wurl = _pwb->GetURL();
    wstring wtitle = _pwb->GetTitle();
    string tag;
    wstring faviconURL;
    _6bees_net::apr_uri_t t;
    string tmpurl=stringmaker()<<wurl;
    _6bees_net::_6beeparseuri(tmpurl.c_str(),t);
    wstring wscheme=wstringmaker()<<t.scheme;
    _6bees_html::getFavicon(pHtmlDoc2,wscheme.c_str(),faviconURL);

    IHTMLElement* pelement = _pwb->GetElementByTagName("body");
    CComBSTR bt;
    pelement->get_innerText(&bt);
    ATL::CString bodytext = bt;

    olbmdlg colbm;
    colbm.str_Name = wtitle;
    colbm.str_URL = wurl;
    colbm.str_Tag = tag;
    colbm.Desc = bodytext.Left(200);
    colbm.absThumbPath = imgpath_thumb.GetString();
    colbm.faviconURL = faviconURL.c_str();
    const wstring& upath2 = ultrapath::R().get(ultrapath::resdll);
    colbm.DoModal(upath2.c_str());
  }

  UINT WINAPI thread_prepareUploadIEFav(LPVOID p){
    CoInitialize(NULL);
    _6bees_html::c6beewb2ptr* wbptr = (_6bees_html::c6beewb2ptr*)(p);
    try{
      olbmitems.clear();
      UUTOOLBAR::UserPrx _user = MyICE::R();
      if(_user==0){
        return 0;
      }
      UUTOOLBAR::IEFavURLs existingOLBM;
      string email(_6beed_share::u_getEmail()),sid(_6beed_share::u_getSID());
      if (_user->GetOLBMURLs(email,sid,existingOLBM)){
        CFavoriteEngine FavoEng;
        PFAVORITELIST pFavoList = FavoEng.GetAllFavorites();
        wstring folder;
        UUTOOLBAR::ImgSeq icons;
        _6bees_shell::getOLBMItems(olbmitems,icons,pFavoList,folder,existingOLBM);//blocking
        FavoEng.CleanUp();
      }
      wbptr->NavToURL("ultra://olbmupload");
    }catch (Ice::TimeoutException&/* e*/){
      wbptr->NavToURL("ultra://error");
    }catch (...){}
    CoUninitialize();
    return 1;
  }

  /// @todo exception
  UINT WINAPI thread_prepareDownloadIEFav(LPVOID p){
    CoInitialize(NULL);
    _6bees_html::c6beewb2ptr* wbptr = (_6bees_html::c6beewb2ptr*)(p);
    try{
      olbmitems.clear();
      UUTOOLBAR::UserPrx _user = MyICE::R();
      if(_user==0){
        return 0;
      }
      if(_user->DownloadOLBM(_6beed_share::u_getEmail(),_6beed_share::u_getSID(),olbmitems)){
        _6bees_shell::FilterIEFav(olbmitems);
      }
      wbptr->NavToURL("ultra://olbmdownload");
    }catch (Ice::TimeoutException&/* e*/){
      wbptr->NavToURL("ultra://error");
    }catch (...){
      wbptr->NavToURL("ultra://error");
    }
    CoUninitialize();
    return 1;
  }

  /// @todo all ICE function need to handle Ice::TimeoutException
  UINT WINAPI thread_uploadIEFav(LPVOID p){
    CoInitialize(NULL);
    _6bees_html::c6beewb2ptr* wbptr = (_6bees_html::c6beewb2ptr*)(p);
    try{
      //Sleep(20*1000);
      timer_notifier tn(CPYDATA_UPLOADIEFAV_START,CPYDATA_UPLOADBUSY_STOP);
      UUTOOLBAR::UserPrx _user = MyICE::R();
      if(_user==0){
        ::MessageBoxA(NULL,
          "ERROR: NetWork failure. Please check your network setting.",
          "Information from UltraIE:",MB_OK|MB_ICONINFORMATION);
        return 0;
      }
      string email(_6beed_share::u_getEmail()),sid(_6beed_share::u_getSID());
      if(_user->UploadOLBM(olbmitems,email,sid)){
        _6bees_window::NotifyClientMng(CPYDATA_UPLOADIEFAV_OK,_6bees_const::kUploader_WndTitle,NULL);
      }
      wbptr->PostNav(kolbmURLa);
    }catch (Ice::TimeoutException&/* e*/){
      wbptr->NavToURL("ultra://error");
    }catch (...){
      wbptr->NavToURL("ultra://error");
    }
    CoUninitialize();
    return 1;
  }

  /*
  UINT WINAPI thread_downloadIEFav(LPVOID p){
  timer_notifier tn(CPYDATA_DOWNLOADIEFAV_START,CPYDATA_DOWNLOADBUSY_STOP);
  _6bees_shell::writeOLBMItemstoIEFav(olbmitems);//blocking
  _6bees_window::NotifyClientMng(CPYDATA_DOWNLOADIEFAV_OK,_6bees_const::kUploader_WndTitle,NULL);

  CoInitialize(NULL);
  _6bees_html::c6beewb2ptr* wbptr = (_6bees_html::c6beewb2ptr*)(p);
  wbptr->PostNav(kolbmURLa);
  CoUninitialize();

  return 1;
  }*/

};