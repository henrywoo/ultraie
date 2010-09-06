#include "stdafx.h"
#include "uuplugin.h"

namespace nsplugin{
  vector<uuplugin*> uuplugin::global_plugins_;

  uuplugin::uuplugin(){
    global_plugins_.push_back(this);
    init();
  }

  bool uuplugin::init(){
    len_ = 100;
    img_ = NULL;
    hwnd_= NULL;
    caption_=L"";
    tip_ = L"";
    return true;
  }

  bool uuplugin::SystemBasedInit(){
    return true;
  }

  bool uuplugin::SystemBasedTerm(){
    return true;
  }
};