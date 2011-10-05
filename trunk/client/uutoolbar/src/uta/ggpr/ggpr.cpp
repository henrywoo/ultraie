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

#include "ggpr.hpp"
#include "ggpralgorithm.hpp"

#include "../../dll/6beecommond/6beecommond.h"
#include "6bees_util.h"
#include "6bees_window.h"

#ifdef __CHINESE__
#define ktip_gg L"¹È¸èÍøÒ³ÆÀ·Ö"
#define kcap_gg L"¹È¸èÍøÒ³ÆÀ·Ö"
#define kinfo_gg L"¹È¸è¶Ô¸ÃÍøÒ³µÄÆÀ·ÖÎª %d/10"
#else
#define ktip_gg L"Google Page Rank"
#define kcap_gg L"Google PR"
#define kinfo_gg L"This page's Google Page rank is %d/10"
#endif

namespace nsplugin{

  std::map<std::wstring,int> ggpr::url2ggpr;
  ggprinfo ggpr::gas;
  HICON ggpr::pr_icons[11];

  UINT WINAPI ggpr::thread_getggpr(LPVOID p){
    ggprinfo* pas = static_cast<ggprinfo*>(p);
    int _pr = 0;
    std::wstring& url = pas->weburl;
    if(url2ggpr.find(url)==url2ggpr.end()){
      _pr=GetGGPR(url.c_str(),L"toolbarqueries.google.com");
      url2ggpr[url] = _pr;
    }else{
      _pr = url2ggpr[url];
    }
    ATL::CString strGGPR;
    strGGPR.Format(kinfo_gg,_pr);
    if (::IsWindow(pas->psb->m_hWnd)){
      pas->psb->SetTipText(pas->paneid,strGGPR);
      pas->psb->SetIcon(pas->paneid,pr_icons[_pr]);
      //pas->ggprptr->active_icon_ = pr_icons[_pr];
      /// @todo raise exception map.. deferenceable
      pas->ggprptr->tid2activeicon_[pas->tid] = pr_icons[_pr];
    }
    return 0;
  }

  bool ggpr::OnNavigateComplete2(int paneid,const wchar_t* url){
    C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
    if (pSB==NULL){
      return false;
    }
    static ATL::CString myurl;
    myurl = url;
    if(myurl.Find(_T("http://"))!=0){
      return false;
    }
    int cut=myurl.Find(_T("/"),8);
    if (cut<0){return false;}
    //myurl.Truncate(cut);
    gas.paneid = paneid;
    gas.weburl=myurl;
    gas.psb = pSB;
    gas.tid = GetCurrentThreadId();
    gas.ggprptr = this;
    ::_beginthreadex(NULL,0,thread_getggpr,(LPVOID)&ggpr::gas,0,NULL);
    return true;
  }

  void ggpr::AssociateImage(int wID,int& _img){}

  bool ggpr::OnClickMenuItem(int id){
    return true;
  }

  ggpr::ggpr(){
    //init image list
    enabled_ = true;
    len_ = 42;
    caption_= kcap_gg;
    tip_ = ktip_gg;
    menuwID_ = 1315;
  }

  ggpr::~ggpr(){
    for (vector<HICON>::const_iterator i=icons_.begin();i!=icons_.end();++i){
      DestroyIcon(*i);
    }
  }

  bool ggpr::ProcessBasedInit(){
    return true;
  }
  
  bool ggpr::ProcessBasedTerm(){
    return true;
  }

  bool ggpr::ThreadBasedInit(){
    return true;
  }
  
  bool ggpr::ThreadBasedTerm(){
    return true;
  }

  bool ggpr::init(){
    //::MessageBoxA(NULL,"GGPR init","GGPR",NULL);
    return true;
  }
  bool ggpr::StartPlugin(){
    return true;
  }
  bool ggpr::StopPlugin(){
    return true;
  }

  bool ggpr::OnLButtonDown(int paneid,int xpos,int ypos){
    return true;
  }
  bool ggpr::OnRButtonDown(int){
    return true;
  }
  bool ggpr::disenable_plugin(){
    return true;
  }

};
