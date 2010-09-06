#include "alexa.hpp"
#include "resource.h"

HMODULE hinst=NULL;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  reason, LPVOID lpReserved ){
  switch (reason){
  case DLL_PROCESS_ATTACH:
    {
      hinst = hModule;

      //nsplugin::uuplugin::hm_ = hModule;
      static nsplugin::alexa alexa_;

      // 1. load icons
      HBITMAP hbitmap=(HBITMAP)LoadImage(hModule,
        MAKEINTRESOURCE(IDB_DLLALEXA),
        IMAGE_BITMAP,2*16,16,LR_DEFAULTSIZE|LR_SHARED);
      HIMAGELIST himglst=ImageList_Create(16, 16,
        ILC_COLOR16|ILC_MASK, 2, 0);
      ImageList_AddMasked(himglst, hbitmap, RGB(255,0,255));
      for (int i=0;i<2;i++){
        alexa_.icons_.push_back(
          ImageList_GetIcon(himglst,i,ILD_NORMAL));
      }
      alexa_.active_icon_ = alexa_.icons_.at(0);

      // 2. load menus
      //alexa_.menu_ = 
      //  ::LoadMenuW(hModule,MAKEINTRESOURCE(IDR_ALEXAMENU));
    }break;
  default:
    break;
  }
  return TRUE;
}