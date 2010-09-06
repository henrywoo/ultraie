#include "olbm.h"
#include "resource.h"

HMODULE hinst=NULL;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  reason, LPVOID lpReserved ){
  switch (reason){
  case DLL_PROCESS_ATTACH:
    {
      hinst = hModule;
      static nsplugin::olbm olbm_;

      HBITMAP hbitmap=(HBITMAP)LoadImage(hModule,MAKEINTRESOURCE(IDB_OLBM),
        IMAGE_BITMAP,1*16,16,LR_DEFAULTSIZE|LR_SHARED);
      HIMAGELIST himglst=ImageList_Create(16, 16, ILC_COLOR16|ILC_MASK, 1, 0);
      ImageList_AddMasked(himglst, hbitmap, RGB(193,193,193));
      for (int i=0;i<1;i++){
        olbm_.icons_.push_back(ImageList_GetIcon(himglst,i,ILD_NORMAL));
      }
      olbm_.active_icon_ = olbm_.icons_.at(0);
    }break;
  }
  return TRUE;
}