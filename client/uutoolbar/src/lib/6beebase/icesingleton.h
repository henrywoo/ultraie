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