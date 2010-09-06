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