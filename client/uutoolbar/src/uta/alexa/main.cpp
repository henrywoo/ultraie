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