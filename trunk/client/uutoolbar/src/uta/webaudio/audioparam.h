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

#ifndef __UUTOOLBAR_AUDIOPARAM__
#define __UUTOOLBAR_AUDIOPARAM__

#include <windows.h>
#include <mmsystem.h>//HWAVEOUT
#include <dsound.h>//LPDIRECTSOUNDBUFFER
#include "basictypes.h"

namespace nsplugin{

  class audioparam{
  private:
    DISALLOW_COPY_AND_ASSIGN(audioparam);
    audioparam();

    DWORD  ori_lock_addr;
    DWORD  ori_play_addr;
    LPDIRECTSOUNDBUFFER lpDsb;
    bool initialized;

  public:

    bool isrecording;
    bool ismuting;

    virtual ~audioparam();
    static audioparam& R();

    DWORD getlockaddr();
    void setlockaddr(DWORD);

    DWORD getplayaddr();
    void setplayaddr(DWORD);

    bool isinit();
    void setinit(bool);

    LPDIRECTSOUNDBUFFER getlpDsb();
    void setlpDsb(LPDIRECTSOUNDBUFFER);
  };

};

#endif