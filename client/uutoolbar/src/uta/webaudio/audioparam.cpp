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