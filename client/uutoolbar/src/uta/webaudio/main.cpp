#include "webaudio.h"
#include "webaudiorec.h"
#include <commctrl.h>//HIMAGELIST
#include "resource.h"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  reason, LPVOID lpReserved ){
  switch (reason){
  case DLL_PROCESS_ATTACH:
    {
      {
        nsplugin::webaudio::R();
        HBITMAP hbitmap=(HBITMAP)LoadImage(hModule,MAKEINTRESOURCE(IDB_WEBAUDIO),
          IMAGE_BITMAP,2*16,16,LR_DEFAULTSIZE|LR_SHARED);
        HIMAGELIST himglst=ImageList_Create(16, 16, ILC_COLOR16|ILC_MASK, 2, 1);
        ImageList_AddMasked(himglst, hbitmap, RGB(193,193,193));
        HICON hi1=ImageList_GetIcon(himglst,0,ILD_NORMAL);
        HICON hi2=ImageList_GetIcon(himglst,1,ILD_NORMAL);
        if(hi1==NULL){
          MessageBoxA(NULL,"E","hi1",NULL);
        }
        if(hi2==NULL){
          MessageBoxA(NULL,"E","hi2",NULL);
        }
        nsplugin::webaudio::R().icons_.push_back(hi1);
        nsplugin::webaudio::R().icons_.push_back(hi2);
        // an important variable
        nsplugin::webaudio::R().enabled_ = true;
      }
      
      {
        nsplugin::webaudiorec::R();
        HBITMAP hbitmap=(HBITMAP)LoadImage(hModule,MAKEINTRESOURCE(IDB_WEBAUDIOREC),
          IMAGE_BITMAP,2*16,16,LR_DEFAULTSIZE|LR_SHARED);
        HIMAGELIST himglst=ImageList_Create(16, 16, ILC_COLOR16|ILC_MASK, 2, 0);
        ImageList_AddMasked(himglst, hbitmap, RGB(193,193,193));
        for (int i=0;i<2;i++){
          nsplugin::webaudiorec::R().icons_.push_back(ImageList_GetIcon(himglst,i,ILD_NORMAL));
        }
        nsplugin::webaudiorec::R().active_icon_ = nsplugin::webaudiorec::R().icons_.at(0);
      }
      
    }break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}