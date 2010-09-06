#include "uuplugins.h"
#include <atlbase.h>

namespace nsplugin{
  //int uuobj::menuid_ = 1314;

  uuobj::uuobj(){
    pid_ = 0;
  }

  uuplugin::uuplugin(){
    icons_.clear();
    tid2activeicon_.clear();
    tid2panetext_.clear();
    active_icon_ = NULL;
    menu_ = NULL;

    AddParser(this);
  }

  

};