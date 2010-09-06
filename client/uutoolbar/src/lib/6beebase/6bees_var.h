#ifndef __6BEES_SINGLETON__
#define __6BEES_SINGLETON__

#include "basictypes.h"
#include "guard.h"

namespace _6bees_var{
  
  class globalvar{
  private:
    DISALLOW_COPY_AND_ASSIGN(globalvar);

    static bool initialized;
    static globalvar* pinstance;
    static cmutex lock_;

    globalvar();

  public:
    static globalvar& R();

  };

};

#endif