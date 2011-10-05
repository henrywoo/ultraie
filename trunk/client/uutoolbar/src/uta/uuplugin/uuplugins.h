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

#ifndef __UUTOOLBAR_UUPLUGIN__
#define __UUTOOLBAR_UUPLUGIN__

#include <windows.h>
#include <string>
#include <vector>
#include "../../dll/uutoolbar/pluginhost.h"


namespace nsplugin{

  extern "C" {
    using namespace std;
    //////////////////////////////////////////////////////////////////////////
    class uuobj{
    public:
      //static int menuid_;
      DWORD pid_;//belongs to obj, inheritable

      // For the first Process, use external conditions
      //virtual  bool SystemBasedInit()=0;

      // For the last process
      //virtual  bool SystemBasedTerm()=0;

      virtual  bool ProcessBasedInit()=0;
      virtual  bool ProcessBasedTerm()=0;
      virtual  bool ThreadBasedInit()=0;
      virtual  bool ThreadBasedTerm()=0;

      HOSTAPI uuobj();
    };


    class uuplugin:public uuobj{
    public:
      bool enabled_;
      int len_;
      wstring caption_;
      wstring tip_;
      int menuwID_;

      //icons the plugin will use for pane icon, menu icon...
      //the first one is the default pane icon
      HICON active_icon_;
      HMENU menu_;
      vector<HICON> icons_;
      map<DWORD,HICON> tid2activeicon_;
      map<DWORD,wstring> tid2panetext_;

    public:
      HOSTAPI uuplugin();

      //init len_,img_,hwnd_... read from DB or XML
      virtual bool init()=0;
      virtual bool StartPlugin()=0;
      virtual bool StopPlugin()=0;

      virtual bool OnLButtonDown(int paneid,int xpos,int ypos)=0;
      virtual bool OnRButtonDown(int)=0;
      virtual bool disenable_plugin()=0;
      virtual bool OnClickMenuItem(int id)=0;
      virtual void AssociateImage(int wID,int& _img)=0;

      // Web Event Function
      virtual bool OnNavigateComplete2(int paneid,const wchar_t* url)=0;
      virtual void OnDocumentComplete(const wstring& _url,const wstring& _title,
        const wstring& _iconurl)=0;
      virtual void OnBeforeNaviagte2(const wstring& _url)=0;
      virtual void OnQuit(const wstring& _url)=0;
      virtual void StartUltraProtocol(const wchar_t* _url,
        std::string& dbcontent,
        std::wstring& fname,
        char*& flag,BYTE* pbuffer,ULONG buffersize)=0;
      virtual void APPURLFilter(const wchar_t* _url)=0;
      virtual void OnHookMsg(UINT,WPARAM,LPARAM,BOOL&)=0;
    };
  };
  


};

#endif
