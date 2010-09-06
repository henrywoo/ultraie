#include "audioparam.h"

namespace nsplugin{

  audioparam::audioparam():ori_lock_addr(NULL),
    ori_play_addr(NULL),
    lpDsb(NULL),
    isrecording(false),
    ismuting(false)
  {}

  audioparam::~audioparam(){/// @todo 
    if(lpDsb){
      lpDsb->Release();
    }
  }
  audioparam& audioparam::R(){
    static audioparam _ap;
    return _ap;
  }
  DWORD audioparam::getlockaddr(){
    return ori_lock_addr;
  }
  DWORD audioparam::getplayaddr(){
    return ori_play_addr;
  }
  void audioparam::setplayaddr(DWORD a){
    ori_play_addr=a;
  }
  void audioparam::setlockaddr(DWORD a){
    ori_lock_addr=a;
  }
  LPDIRECTSOUNDBUFFER audioparam::getlpDsb(){
    return lpDsb;
  }
  void audioparam::setlpDsb(LPDIRECTSOUNDBUFFER a){
    lpDsb=a;
  }

  bool audioparam::isinit(){
    return initialized;
  }
  void audioparam::setinit(bool b){
    initialized = b;
  }

};