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

#ifndef _6BEESBASE_SHELL_H_
#define _6BEESBASE_SHELL_H_

#include "UUIce.h"
#include "favengine.h"
#include "6bees_util.h"

namespace _6bees_shell{
  using namespace std;

  wstring GetSpecialFolderLocation(int nFolderID);
  bool CreateInternetShortcut(const wstring& LinkFilename,const wstring& Argument,
    const wchar_t* csIconLocalPath,const wchar_t* subfolder=NULL);
  void getOLBMItems(::UUTOOLBAR::OLBMItems& items,::UUTOOLBAR::ImgSeq& icons,const PFAVORITELIST& pFavoListCurFolderRoot,
    const wstring& folder,const ::UUTOOLBAR::IEFavURLs& existingOLBM);
  void getexistingIEFavURLs(::UUTOOLBAR::IEFavURLs& existingIEFavURLs,const PFAVORITELIST& pFavoListCurFolderRoot,const wstring& folder);
  void writeOLBMItemstoIEFav(const ::UUTOOLBAR::OLBMItems& items);
  void FilterIEFav(::UUTOOLBAR::OLBMItems& items);
  ////bool PutIcontoBuffer(const ATL::CStringA& url,const ATL::CStringA& hostname,int iconsize,::UUTOOLBAR::Image& img);

  int GetFileSizeA( const char * szFileName );
  bool GenICEImg(const string& imgpath,::UUTOOLBAR::Image& img);
};

#endif