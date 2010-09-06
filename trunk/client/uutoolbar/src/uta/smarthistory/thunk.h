#ifndef __PLUGINS_SMARTHISTORY_THUNK__
#define __PLUGINS_SMARTHISTORY_THUNK__

#include "basictypes.h"
#include <string>

namespace nsplugin{
  
  using std::wstring;
  
  struct thunk{
    unsigned long tid;
    wstring url;
    wstring title;
    wstring iconurl;

    static thunk& R();
  private:
    DISALLOW_COPY_AND_ASSIGN(thunk);
    thunk();
  };

};

#endif