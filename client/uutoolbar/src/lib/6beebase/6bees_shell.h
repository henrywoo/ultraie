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