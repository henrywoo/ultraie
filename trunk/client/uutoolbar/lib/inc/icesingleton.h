#ifndef _STUB_SKELTON_ICESINGLETON_
#define _STUB_SKELTON_ICESINGLETON_

#include "UUIce.h"
#include <Ice/LocalException.h>

#include "basictypes.h"
#include "guard.h"

class MyICE{

  static bool initialized;
  static MyICE* pinstance;
  static cmutex lock_;

  UUTOOLBAR::UserPrx _user;

  MyICE();
  MyICE(const MyICE&);
  MyICE operator=(const MyICE&);

  void InitialzieICE();

  static MyICE& Instance();

public:
  static UUTOOLBAR::UserPrx R();//UserPrx is a pointer then return it directly
};

#endif