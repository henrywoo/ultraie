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