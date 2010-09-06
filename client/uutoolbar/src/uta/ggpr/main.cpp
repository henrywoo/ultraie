#include "ggpr.hpp"
#include "resource.h"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  reason, LPVOID lpReserved ){
  switch (reason){
  case DLL_PROCESS_ATTACH:
    {
      static nsplugin::ggpr ggpr_;

      HBITMAP hbitmap=(HBITMAP)LoadImage(hModule,MAKEINTRESOURCE(IDB_DLLGGPR),
        IMAGE_BITMAP,11*32,32,LR_DEFAULTSIZE|LR_SHARED);
      HIMAGELIST himglst=ImageList_Create(32, 32, ILC_COLOR16|ILC_MASK, 11, 0);
      ImageList_AddMasked(himglst, hbitmap, RGB(255,0,255));
      for (int i=0;i<11;i++){
        nsplugin::ggpr::pr_icons[i] = ImageList_GetIcon(himglst,i,ILD_NORMAL);
        ggpr_.icons_.push_back(nsplugin::ggpr::pr_icons[i]);
      }
      ggpr_.active_icon_ = ggpr_.icons_.at(10);
    }break;
  case DLL_PROCESS_DETACH:
    for (int i=0;i<11;i++){
      DestroyIcon(nsplugin::ggpr::pr_icons[i]);
    }
    break;
  default:
    break;
  }
  return TRUE;
}