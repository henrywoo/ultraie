#include "favengine.h"
#include "6bees_util.h"
#include "6bees_str.h"
using _6bees_str::wstringmaker;
using namespace std;

CFavoriteEngine::CFavoriteEngine(){
  m_pFavoListRoot = NULL;
  m_pMalloc = NULL;
}

CFavoriteEngine::~CFavoriteEngine(){
  CleanUp();
}

void CFavoriteEngine::CleanUp(){
  Clean(m_pFavoListRoot);
  m_pFavoListRoot = NULL;
  return;
}

PFAVORITELIST CFavoriteEngine::GetAllFavorites(){
  if(FAILED(CoInitialize(NULL)))
    return NULL;
  HRESULT hRet = ::SHGetMalloc(&m_pMalloc);
  if(FAILED(hRet)){
    m_pMalloc->Release();
    return NULL;
  }
  LPITEMIDLIST pidl;
  hRet = ::SHGetSpecialFolderLocation( NULL, CSIDL_FAVORITES, &pidl);
  if(FAILED(hRet)){
    m_pMalloc->Release();
    return NULL;
  }

  IShellFolder *pShellFolder = NULL;
  hRet = ::SHGetDesktopFolder (&pShellFolder);
  if (FAILED (hRet)){
    m_pMalloc->Free (pidl);
    m_pMalloc->Release ();
    return NULL;
  }

  IShellFolder *pFavFolder = NULL;
  hRet = pShellFolder->BindToObject (pidl, NULL, IID_IShellFolder, reinterpret_cast<void **>(&pFavFolder));

  long nItems = 0;
  IEnumIDList* pItems = NULL;
  hRet = pFavFolder->EnumObjects(NULL, SHCONTF_FOLDERS|SHCONTF_NONFOLDERS, &pItems);

  if(m_pFavoListRoot){
    CleanUp();
  }
  m_pFavoListRoot = new FAVORITELIST;
  ZeroMemory(m_pFavoListRoot, sizeof(FAVORITELIST));
  PFAVORITELIST pFavoListCur = GetFavorite( pFavFolder, m_pFavoListRoot, pItems);
  if (NULL != pItems){
    pItems->Release();
  }
  m_pMalloc->Free(pidl);
  m_pMalloc->Release();

  return m_pFavoListRoot;
}

void CFavoriteEngine::StrRetToWString(const STRRET& StrRet, wstring& str, LPITEMIDLIST pidl){
  switch (StrRet.uType){
    case STRRET_WSTR: 
      str=StrRet.pOleStr;
      return;
    case STRRET_OFFSET: 
      str=(wchar_t *)pidl + StrRet.uOffset;
      return;
    case STRRET_CSTR:
      str = wstringmaker()<<StrRet.cStr;
      return;
  }
}

PFAVORITELIST CFavoriteEngine::GetFavorite(IShellFolder *pFavFolder,PFAVORITELIST& pFavoListCurFolderRoot,IEnumIDList* pItems){
  if((!pFavFolder) || (!pItems))
    return NULL;

  PFAVORITELIST pFavoListCurFolder = NULL;
  PFAVORITELIST pFavoListCurURL = NULL;
  LPITEMIDLIST pidlNext = NULL;
  STRRET StrRetName;
  STRRET StrRetFile;
  wstring lpszName, lpszFileName, lpszURL;
  
  HRESULT hr = S_OK , hRet = S_OK;
  while (hr == S_OK){
    hr = pItems->Next(1, &pidlNext, NULL);
    if (NOERROR == hr){
      pFavFolder->GetDisplayNameOf(pidlNext, SHGDN_NORMAL, &StrRetName);
      pFavFolder->GetDisplayNameOf(pidlNext, SHGDN_FORPARSING, &StrRetFile);
      StrRetToWString(StrRetName, lpszName, pidlNext);
      StrRetToWString(StrRetFile, lpszFileName, pidlNext);
      SHFILEINFO fileInfo;
      WIN32_FILE_ATTRIBUTE_DATA fileAttrData;

      if (::GetFileAttributesEx(lpszFileName.c_str(), GetFileExInfoStandard,  &fileAttrData)){
        //GetFileTimeValues (&fileAttrData);
      }

      SHGetFileInfo(lpszFileName.c_str(), NULL, &fileInfo, sizeof(fileInfo),SHGFI_ATTRIBUTES|SHGFI_TYPENAME);
      HRESULT hrSub = S_OK;
      if ((fileInfo.dwAttributes & SFGAO_FOLDER)!=0){
        // Bind to the sub-folder and then call this function again.
        IShellFolder* pSubFolder=NULL;
        hrSub = pFavFolder->BindToObject(pidlNext, NULL, IID_IShellFolder,(LPVOID*)&pSubFolder);
        if (FAILED(hrSub))
          continue;
        PFAVORITELIST PFavoList = new FAVORITELIST;
        ZeroMemory(PFavoList, sizeof(FAVORITELIST));
        PFavoList->m_favoType  = SFGAO_FOLDER;
        PFavoList->m_pFavoTitle  = lpszName;
        PFavoList->m_pFavoURL  = L"";
        if(pFavoListCurFolder){
          pFavoListCurFolder->m_pNextFolder  = PFavoList;
          PFavoList->m_pPrevFolder      = pFavoListCurFolder;
        }else{
          if(!m_pFavoListRoot)
            m_pFavoListRoot            = pFavoListCurFolderRoot;
          pFavoListCurFolderRoot->m_pNextFolder  = PFavoList;
          PFavoList->m_pPrevFolder        = pFavoListCurFolderRoot;
        }
        pFavoListCurFolder = PFavoList;

        //Go to the sub-folder
        IEnumIDList* pSubItems = NULL;
        hrSub = pSubFolder->EnumObjects(NULL, SHCONTF_FOLDERS|SHCONTF_NONFOLDERS, &pSubItems);
        if(SUCCEEDED(hrSub)){
          PFAVORITELIST pFavoListCurFolderRootEx  = new FAVORITELIST;
          ZeroMemory(pFavoListCurFolderRootEx, sizeof(FAVORITELIST));
          pFavoListCurFolder->m_pSubFolder    = pFavoListCurFolderRootEx;
          pFavoListCurFolderRootEx->m_pOwnerFolder= pFavoListCurFolder;
          GetFavorite(pSubFolder,pFavoListCurFolderRootEx,pSubItems);
        }
        pSubFolder->Release();
      }else{
        if((fileInfo.dwAttributes & SFGAO_LINK) != 0){
          IUniformResourceLocator* pUrlLink = NULL;
          hRet = CoCreateInstance(CLSID_InternetShortcut,NULL,CLSCTX_INPROC_SERVER,IID_IUniformResourceLocator, (LPVOID*)&pUrlLink);
          if (SUCCEEDED(hRet)){
            IPersistFile* pPersistFile = NULL;
            hRet = pUrlLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPersistFile);
            if (SUCCEEDED(hRet)){
              hRet = pPersistFile->Load(lpszFileName.c_str(), STGM_READ);
              if (SUCCEEDED(hRet)){
                static wchar_t tmp[1024*4]={};
                wchar_t* ptmp = &tmp[0];
                hRet = pUrlLink->GetURL(&ptmp);
                if(SUCCEEDED(hRet) && ptmp!=NULL){
                  lpszURL=ptmp;
                  PFAVORITELIST pFavoList = new FAVORITELIST;
                  ZeroMemory(pFavoList, sizeof(FAVORITELIST));
                  pFavoList->m_favoType    = SFGAO_LINK;
                  pFavoList->m_pFavoTitle  = lpszName;
                  pFavoList->m_pFavoURL    = lpszURL;
                  if(pFavoListCurURL){
                    pFavoListCurURL->m_pNextURL      = pFavoList;
                    pFavoList->m_pPrevURL        = pFavoListCurURL;
                  }else{
                    pFavoListCurFolderRoot->m_pNextURL  = pFavoList;
                    pFavoList->m_pOwnerFolder      = pFavoListCurFolderRoot;
                  }
                  pFavoListCurURL = pFavoList;
                }else{
                  //Local short cut!
                }
              }
              pPersistFile->Release();
            }
            pUrlLink->Release();
          }
        }
      }
    }
  }
  
  return pFavoListCurFolder;
}

void CFavoriteEngine::Clean(PFAVORITELIST pFavoListCurFolderRoot){
  if(!pFavoListCurFolderRoot)
    return;
  PFAVORITELIST pFavoListCur = pFavoListCurFolderRoot;
  BOOL bFolderFinished = FALSE, bURLFinished = FALSE;
  do {
    switch(pFavoListCur->m_favoType){
    case SFGAO_FOLDER:
      {
        Clean(pFavoListCur->m_pSubFolder);
        PFAVORITELIST pFavoListTemp = pFavoListCur;
        if(pFavoListCur->m_pNextFolder)
          pFavoListCur  = pFavoListCur->m_pNextFolder;
        else {
          pFavoListCur  = pFavoListCurFolderRoot;
          bFolderFinished  = TRUE;
        }
        delete pFavoListTemp;
        pFavoListTemp = NULL;
      }
      break;
    case SFGAO_LINK:
      {
        PFAVORITELIST pFavoListTemp = pFavoListCur;
        if(pFavoListCur->m_pNextURL)
          pFavoListCur  = pFavoListCur->m_pNextURL;
        else {
          pFavoListCur  = pFavoListCurFolderRoot;
          bURLFinished  = TRUE;
        }
        delete pFavoListTemp;
        pFavoListTemp = NULL;
      }
      break;
    default:
      {
        if(pFavoListCur->m_pNextFolder){
          if(!bFolderFinished){
            pFavoListCur  = pFavoListCur->m_pNextFolder;
            bFolderFinished  = FALSE;
            break;
          }
        } else {
          bFolderFinished = TRUE;
        }
        if(pFavoListCur->m_pNextURL){
          if(!bURLFinished){
            pFavoListCur  = pFavoListCurFolderRoot->m_pNextURL;
            bURLFinished  = FALSE;
          }
        } else {
          bURLFinished  = TRUE;
        }
      }
      break;
    }
    if(bFolderFinished && bURLFinished){
      delete pFavoListCurFolderRoot;
      pFavoListCurFolderRoot = NULL;
    }
  }while(!(bFolderFinished && bURLFinished));
}
