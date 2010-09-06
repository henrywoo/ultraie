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