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