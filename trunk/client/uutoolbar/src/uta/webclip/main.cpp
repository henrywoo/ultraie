#include "webclip.h"
#include "resource.h"

HMODULE hinst=NULL;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  reason, LPVOID lpReserved ){
  switch (reason){
  case DLL_PROCESS_ATTACH:
    {
      hinst = hModule;
      static nsplugin::webclip webclip_;

      HBITMAP hbitmap=(HBITMAP)LoadImage(hModule,MAKEINTRESOURCE(IDB_WEBCLIP),
        IMAGE_BITMAP,16,16,LR_DEFAULTSIZE|LR_SHARED);
      HIMAGELIST himglst=ImageList_Create(16, 16, ILC_COLOR16|ILC_MASK, 1, 0);
      ImageList_AddMasked(himglst, hbitmap, RGB(255,0,255));
      for (int i=0;i<1;i++){
        webclip_.icons_.push_back(ImageList_GetIcon(himglst,i,ILD_NORMAL));
      }
      webclip_.active_icon_ = webclip_.icons_.at(0);
    }break;
  }
  return TRUE;
}