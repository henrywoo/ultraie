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

#ifndef _FAVORITEENGINE_H_INCLUDED_
#define _FAVORITEENGINE_H_INCLUDED_

#include <shlobj.h>
#include <IntShCut.h>
#include <shellapi.h>
#include <string>


typedef struct tagFavoriteList{
  int          m_dwSize;
  DWORD        m_favoType;
  std::wstring      m_pFavoTitle;
  std::wstring      m_pFavoURL;
  struct tagFavoriteList*  m_pOwnerFolder;
  struct tagFavoriteList*  m_pSubFolder;
  struct tagFavoriteList*  m_pPrevFolder;
  struct tagFavoriteList*  m_pNextFolder;
  struct tagFavoriteList*  m_pPrevURL;
  struct tagFavoriteList*  m_pNextURL;
}FAVORITELIST, *PFAVORITELIST;

class CFavoriteEngine{
public:
  CFavoriteEngine();
  virtual ~CFavoriteEngine();

  void CleanUp();
  PFAVORITELIST GetAllFavorites();
  PFAVORITELIST m_pFavoListRoot;

private:
  IMalloc *m_pMalloc;

  void Clean(PFAVORITELIST pFavoListCurFolderRoot);
  PFAVORITELIST GetFavorite(IShellFolder* pFavFolder, PFAVORITELIST &pFavoListCurFolderRoot, IEnumIDList* pItems);
  void StrRetToWString(const STRRET& StrRet, std::wstring& str, LPITEMIDLIST pidl);
};

#endif
