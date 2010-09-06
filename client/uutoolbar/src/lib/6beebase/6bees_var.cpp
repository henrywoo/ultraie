#include "6bees_var.h"

namespace _6bees_var{

  bool globalvar::initialized = false;
  globalvar* globalvar::pinstance = NULL;
  cmutex globalvar::lock_;

  globalvar& globalvar::R(){
    if(!initialized){
      cguard g(lock_);
      if(pinstance == NULL){ 
        static globalvar p_;
        pinstance = &p_;
        if(pinstance){ //make sure compiler cannot reorder 
          initialized = true; // only set to true after creation 
        }
      } 
    } 
    return *pinstance;
  }

};