#include "thunk.h"

namespace nsplugin{

  thunk& thunk::R(){
    static thunk _t;
    return _t;
  }

  thunk::thunk(){}


};