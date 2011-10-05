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

#include "globalvar.h"
#include "6bees_util.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "ToolBarCom.h"
using namespace _6bees_util;

namespace uutoolbar{

  int getoffset(){
    using namespace _6bees_const;
    static OST ostype = _6beed_util::GetOS();
    switch(ostype){
      case VISTA:
        return _6beed_util::IsXPTheme() ? 560 : 536;
      case XP:
        switch(_6beed_share::u_getieversion()){
      case 8:
        return _6beed_util::IsXPTheme()?442:437;
      case 7:
        return 400;
      case 6:
        return 290;
        }
      default:
        return 400;
    }
  }

  bool GlobalVar::initialized = false;
  GlobalVar* GlobalVar::pinstance = NULL;
  cmutex GlobalVar::lock_;

  GlobalVar::GlobalVar():kstatusbar_offset(getoffset()){}

  GlobalVar& GlobalVar::R(){
    if(!initialized){
      cguard g(lock_);
      if(pinstance == NULL){ 
        static GlobalVar p_;
        pinstance = &p_;
        if(pinstance){ //make sure compiler cannot reorder 
          initialized = true; // only set to true after creation 
        }
      } 
    } 
    return *pinstance;
  }

  TidStatusWndMap& GlobalVar::GetTidStatusWndMap(){
    return tidstatuswndmap_;
  }

  C6BeeStatusWnd* GlobalVar::GetCurrentStatusWnd(){
    if(tidstatuswndmap_.find(GetCurrentThreadId()) == tidstatuswndmap_.end()){
      return NULL;
    }else{
      return tidstatuswndmap_[GetCurrentThreadId()];
    }
  }


  bool bmpsgton::initialized = false;
  bmpsgton* bmpsgton::pinstance = NULL;
  cmutex bmpsgton::lock_;
  bmpsgton& bmpsgton::R(){
    if(!initialized){
      cguard g(lock_);
      if(pinstance == NULL){ 
        static bmpsgton p_;
        pinstance = &p_;
        if(pinstance){ //make sure compiler cannot reorder 
          initialized = true; // only set to true after creation 
        }
      } 
    } 
    return *pinstance;
  }

  bmpsgton::bmpsgton():
  h_((HBITMAP)c6beeres::R().GetImg(IDB_COMBOX_CENTER,IMAGE_BITMAP,1,24)),
  h2_((HBITMAP)c6beeres::R().GetImg(IDB_COMBOX_RIGHT,IMAGE_BITMAP,20,24)){
  }


};