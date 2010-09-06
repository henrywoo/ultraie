#include "imgcap.h"
#include "resource.h"

HMODULE hinst=NULL;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  reason, LPVOID lpReserved ){
  switch (reason){
  case DLL_PROCESS_ATTACH:
    {
      hinst = hModule;
      static nsplugin::imgcapture imgcapture_;

      HBITMAP hbitmap=(HBITMAP)LoadImage(hModule,MAKEINTRESOURCE(IDB_IMGCAP),
        IMAGE_BITMAP,16,16,LR_DEFAULTSIZE|LR_SHARED);
      HIMAGELIST himglst=ImageList_Create(16, 16, ILC_COLOR16|ILC_MASK, 1, 0);
      ImageList_AddMasked(himglst, hbitmap, RGB(255,0,255));
      for (int i=0;i<1;i++){
        imgcapture_.icons_.push_back(ImageList_GetIcon(himglst,i,ILD_NORMAL));
      }
      imgcapture_.active_icon_ = imgcapture_.icons_.at(0);
    }break;
  default:
    break;
  }
  return TRUE;
}