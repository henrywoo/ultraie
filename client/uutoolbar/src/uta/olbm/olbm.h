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

#ifndef __PLUGINS_ONLINEBOOKMARK_OLBM__
#define __PLUGINS_ONLINEBOOKMARK_OLBM__

#include "../uuplugin/uuplugins.h"
#include <string>
#include <set>

namespace nsplugin{
  using namespace std;

  // Thread-bound,  no singleton...
  class olbm: public uuplugin
  {
  private:
    void OLBMSetting(string& dbcontent,wstring& fname,char*& flag);
    void OLBMUpload(string& dbcontent,wstring& fname,char*& flag);
    void OLBMLoading(string& dbcontent,wstring& fname,char*& flag);
    void OLBMDoUpload(string& dbcontent,wstring& fname,char*& flag);
    void OLBMDownload(string& dbcontent,wstring& fname,char*& flag);
    void OLBMDoDownload(string& dbcontent,wstring& fname,char*& flag);

    map<string,set<string>> folder2urls;
    void constructiefavs(string& dbcontent,bool isupload);
    void RemoveOLBMItems(wstring postdata);
  public:
    olbm();
    ~olbm();

    virtual  bool ProcessBasedInit();
    virtual  bool ProcessBasedTerm();

    virtual  bool ThreadBasedInit();
    virtual  bool ThreadBasedTerm();

    //Called when triggering DISPID_NAVIGATECOMPLETE2
    virtual bool OnNavigateComplete2(int paneid,const wchar_t* url);
    virtual void OnDocumentComplete(const wstring& _url,const wstring& _title,
      const wstring& _iconurl){};
    virtual void OnBeforeNaviagte2(const wstring& _url){};
    virtual void OnQuit(const wstring& _url){};
    virtual bool OnClickMenuItem(int id);

    virtual bool init();
    virtual bool StartPlugin();
    virtual bool StopPlugin();

    virtual bool OnLButtonDown(int paneid,int xpos,int ypos);
    virtual bool OnRButtonDown(int);
    virtual bool disenable_plugin();
    virtual void AssociateImage(int wID,int& _img);
    virtual void StartUltraProtocol(const wchar_t* _url,
      std::string& dbcontent,
      std::wstring& fname,
      char*& flag,BYTE* pbuffer,ULONG buffersize);
    virtual void APPURLFilter(const wchar_t* _url){};
    virtual void OnHookMsg(UINT,WPARAM,LPARAM,BOOL&);

    int InsertandGetMenuImgIndex();
  };

};

#endif