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
