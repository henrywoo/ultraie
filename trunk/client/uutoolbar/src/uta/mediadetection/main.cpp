#include "mediadetection.hpp"
#include "resource.h"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  reason, LPVOID lpReserved ){
  switch (reason){
  case DLL_PROCESS_ATTACH:
    {
      static nsplugin::mediadetection _smediadetection;

      HBITMAP hbitmap=(HBITMAP)LoadImage(hModule,
        MAKEINTRESOURCE(IDB_MEDIADETECTION),
        IMAGE_BITMAP,1*16,16,LR_DEFAULTSIZE|LR_SHARED);
      HIMAGELIST himglst=ImageList_Create(16, 16, ILC_COLOR16|ILC_MASK, 1, 1);
      ImageList_AddMasked(himglst, hbitmap, RGB(193,193,193));
      HICON hi1=ImageList_GetIcon(himglst,0,ILD_NORMAL);
      _smediadetection.icons_.push_back(hi1);

      // an important variable
      _smediadetection.enabled_ = true;
    }break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}