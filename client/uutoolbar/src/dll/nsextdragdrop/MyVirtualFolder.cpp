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

#include "stdafx.h"
#include "MyVirtualFolder.h"
#include "CPidlEnum.h"
#include "NSFShellView.h"
#include "CExtractIcon.h"

//add for drag and drop
#include "CNSFDropTarget.h"
#include "CNSFDataObject.h"

// CMyVirtualFolder
HRESULT CMyVirtualFolder::FinalConstruct(){
  m_pidlPath = NULL;
  return S_OK;
}

void CMyVirtualFolder::FinalRelease()
{
  ATLTRACE(_T("MyVirtualFolder::FinalRelease\n"));

  if(m_pidlPath != NULL)
  {
    m_PidlMgr.Delete(m_pidlPath);
    m_pidlPath = NULL;
  }
}

//interfaces functions
STDMETHODIMP CMyVirtualFolder::GetClassID(LPCLSID pClsid){
  if(NULL == pClsid )
    return E_POINTER;
  // Return our GUID to the shell.
  *pClsid = CLSID_MyVirtualFolder;
  return S_OK;
}

STDMETHODIMP CMyVirtualFolder::Initialize(LPCITEMIDLIST pidl){
  ATLTRACE(_T("CMyVirtualFolder(0x%08x)::Initialize() \n"), this);

  if(pidl && (!_Module.m_pidlNSFROOT))
  {
    _Module.m_pidlNSFROOT = m_PidlMgr.Copy(pidl);
    HRESULT Hr;
    HR(GetGlobalSettings());
  }

  return S_OK;
}

///////////////////////////////////////////////////////////////////////////
//
// IShellFolder Implementation
//

//BindToObject() is called when a folder in our part of the namespace is being browsed.
//Retrieves an IShellFolder object for a subfolder. 
STDMETHODIMP CMyVirtualFolder::BindToObject(LPCITEMIDLIST pidl,
                                            LPBC,// pbcReserved, 
                                            REFIID riid, 
                                            LPVOID *ppRetVal)
{

  *ppRetVal = NULL;

  HRESULT hr=S_OK;

  if( riid != IID_IShellFolder) 
    return E_NOINTERFACE;

  CComObject<CMyVirtualFolder> *pVFObj=0;

  hr=CComObject<CMyVirtualFolder>::CreateInstance(&pVFObj);
  if(FAILED(hr))
    return hr;

  pVFObj->AddRef();

  LPITEMIDLIST pidlNew = m_PidlMgr.Concatenate(m_pidlPath,pidl);
  pVFObj->m_pidlPath = m_PidlMgr.Copy(pidlNew);	
  m_PidlMgr.Delete(pidlNew);

  hr=pVFObj->QueryInterface(riid, ppRetVal);

  ATLASSERT(pVFObj);
  pVFObj->Release();

  return hr;
}
STDMETHODIMP CMyVirtualFolder::BindToStorage(	LPCITEMIDLIST,// pidl, 
                                             LPBC,// pbcReserved, 
                                             REFIID,// riid, 
                                             LPVOID *ppvOut)
{
  *ppvOut = NULL;
  return E_NOTIMPL;
}

int CMyVirtualFolder::_CompareIDs(LPARAM lParam, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
  HRESULT Hr;

  CListSortInfo* pSort = (CListSortInfo*) lParam;
  if(NULL==lParam)
  {
    CListSortInfo sort = { this, COL_NAME, TRUE };
    pSort=&sort;
  }
  else
  {
    pSort = (CListSortInfo*) lParam;
    _ASSERT(NULL != lParam);
  }


  BOOL bIsEmpty1 =(( NULL==pidl1 )||( 0 == pidl1->mkid.cb) );
  BOOL bIsEmpty2 =(( NULL==pidl2 )||( 0 == pidl2->mkid.cb) );

  if( bIsEmpty1 && bIsEmpty2 ) 
    return 0;

  if( bIsEmpty1 ) 		
  {
    Hr=-1;
    return pSort->bForwardSort? Hr: Hr*(-1);
  }


  if( bIsEmpty2 ) 
  {
    Hr=1;
    return pSort->bForwardSort? Hr: Hr*(-1);
  }

  TCHAR szTemp1[MAX_PATH]=_T("");
  TCHAR szTemp2[MAX_PATH]=_T("");

  switch (pSort->iSortColumn)
  {
  case COL_NAME:
  case COL_TYPE:

    if( m_PidlMgr.GetItemType(pidl1)!=m_PidlMgr.GetItemType(pidl2) ) 
    {
      Hr = (m_PidlMgr.GetItemType(pidl1)==NWS_FOLDER) ? -1 : 1;
      return pSort->bForwardSort? Hr: Hr*(-1);
    }

    if(COL_NAME == pSort->iSortColumn) //COL_NAME:
    {
      DWORD dwLen1=MAX_PATH;
      DWORD dwLen2=MAX_PATH;

      HR(m_PidlMgr.GetFullName(pidl1,szTemp1,&dwLen1));
      HR(m_PidlMgr.GetFullName(pidl2,szTemp2,&dwLen2));


      Hr= lstrcmpi(szTemp1, szTemp2);
      return pSort->bForwardSort? Hr: Hr*(-1);
    }
    else//COL_TYPE:
    {
      HR(m_PidlMgr.GetItemAttributes(pidl1,ATTR_TYPE,szTemp1));
      HR(m_PidlMgr.GetItemAttributes(pidl2,ATTR_TYPE,szTemp2));

      Hr= lstrcmpi(szTemp1, szTemp2);
      return pSort->bForwardSort? Hr: Hr*(-1);
    }
    break;
  }
  return 0;
}

STDMETHODIMP CMyVirtualFolder::CompareIDs(LPARAM,// lParam, 
                                          LPCITEMIDLIST pidl1, 
                                          LPCITEMIDLIST pidl2)
{
  TCHAR szName1[MAX_PATH]={};
  TCHAR szName2[MAX_PATH]={};
  DWORD dwLen1=MAX_PATH;
  DWORD dwLen2=MAX_PATH;

  m_PidlMgr.GetFullName(pidl1,szName1,&dwLen1);
  m_PidlMgr.GetFullName(pidl2,szName2,&dwLen2);

  ITEM_TYPE it1,it2;

  it1=m_PidlMgr.GetItemType(pidl1);
  it2=m_PidlMgr.GetItemType(pidl2);

  if( it1 == it2 )
    return _tcscmp(szName1, szName2);
  else
  {
    if( NWS_FOLDER == it1)//pidl1 correspond a folder object
    {
      return -1;
    }
    else
    {
      return 1;
    }
  }
}

STDMETHODIMP CMyVirtualFolder::CreateViewObject(HWND,// hWnd, 
                                                REFIID riid, 
                                                LPVOID* ppRetVal)
{

  VALIDATE_OUT_POINTER(ppRetVal);

  HRESULT Hr;
  if( riid == IID_IShellView ) 
  {

    CComObject<CNSFShellView> *pView;

    Hr= CComObject<CNSFShellView>::CreateInstance(&pView);
    if(FAILED(Hr))
    {
      return Hr;
    }

    pView->AddRef();

    pView->_Init(this,m_pidlPath) ;

    Hr=pView->QueryInterface(IID_IShellView, ppRetVal);

    pView->Release();

    return Hr;
  }

  if( riid == IID_IDropTarget ) 
  {

    CComObject<CNSFDropTarget> *pDropTarget;
    HR( CComObject<CNSFDropTarget>::CreateInstance(&pDropTarget) );
    if(FAILED(Hr))
      return Hr;

    pDropTarget->AddRef();

    HR( pDropTarget->_Init(this, NULL) );

    Hr=pDropTarget->QueryInterface(IID_IDropTarget, ppRetVal);
    pDropTarget->Release();

    return Hr;
  }
  return E_NOINTERFACE;
}

// EnumObjects() creates a COM object that implements IEnumIDList.
STDMETHODIMP CMyVirtualFolder::EnumObjects(HWND,// hWnd, 
                                           DWORD dwFlags, 
                                           LPENUMIDLIST* ppEnumIDList)
{
  ATLTRACE(_T("CMyVirtualFolder(0x%08x)::EnumObjects(dwFlags=0x%04x)\n"), this, dwFlags);

  VALIDATE_OUT_POINTER(ppEnumIDList);

  HRESULT Hr;

  // Create an enumerator with CComEnumOnCArray<> and our copy policy class.
  CComObject<CPidlEnum> *pEnum;
  Hr = CComObject<CPidlEnum>::CreateInstance(&pEnum);
  if (SUCCEEDED(Hr))
  {
    // AddRef() the object while we're using it.
    pEnum->AddRef();

    // Init the enumerator.  Init() will AddRef() our IUnknown (obtained with
    // GetUnknown()) so this object will stay alive as long as the enumerator 
    // needs access to the collection m_NEFileArray.
    Hr = pEnum->_Init(m_pidlPath, dwFlags);

    // Return an IEnumIDList interface to the caller.
    if (SUCCEEDED(Hr))
      Hr = pEnum->QueryInterface(IID_IEnumIDList, (void**)ppEnumIDList);

    pEnum->Release();
  }

  return Hr;
}
STDMETHODIMP CMyVirtualFolder::GetAttributesOf(UINT uCount, 
                                               LPCITEMIDLIST pidls[], 
                                               LPDWORD pdwAttribs)
{
  *pdwAttribs = (DWORD)-1;

  if(( uCount==0 )||(pidls[0]->mkid.cb == 0)) 
  {
    // Can happen on Win95. Queries the view folder.
    DWORD dwAttr;
    dwAttr |= SFGAO_FOLDER | SFGAO_HASSUBFOLDER |SFGAO_HASPROPSHEET |SFGAO_DROPTARGET;
    *pdwAttribs &= dwAttr;
  }
  else
  {
    for( UINT i=0; i<uCount; i++ ) 
    {
      DWORD dwAttr = 0;

      switch( m_PidlMgr.GetItemType(pidls[i]) ) 
    		{
      case NWS_FOLDER:

        dwAttr |=SFGAO_FOLDER;
        dwAttr |=SFGAO_DROPTARGET;

        //get the full pidl
        LPITEMIDLIST   tmpPidl;
        tmpPidl = m_PidlMgr.Concatenate(m_pidlPath, pidls[i]);				

        //decide whether current folder object has subfolder
        if( TRUE == m_PidlMgr.HasSubFolder(tmpPidl) )
        { 
          dwAttr |= SFGAO_HASSUBFOLDER;
        }
        m_PidlMgr.Delete(tmpPidl);
        break;
    		}
      *pdwAttribs &= dwAttr;
    }
  }

  return S_OK;
}

#define GET_SHGDN_FOR(dwFlags)         ((DWORD)dwFlags & (DWORD)0x0000FF00)
#define GET_SHGDN_RELATION(dwFlags)    ((DWORD)dwFlags & (DWORD)0x000000FF)

//GetDisplayNameOf() Retrieves the display name for the specified file object 
//                   or subfolder, returning it in a STRRET structure. 

STDMETHODIMP CMyVirtualFolder::GetDisplayNameOf(LPCITEMIDLIST pidl, 
                                                DWORD dwFlags, 
                                                LPSTRRET lpName)
{
  ATLTRACE(_T("CMyVirtualFolder(0x%08x)::GetDisplayNameOf(uFlags=0x%04x) \n"), this, dwFlags);

  HRESULT Hr;
  TCHAR szText[MAX_PATH]=_T("");
  TCHAR szTmp[MAX_PATH]=_T("");
  DWORD dwLen=MAX_PATH;

  if (NULL==pidl || pidl->mkid.cb == 0)//root folder
  {
    ::LoadString(_Module.GetResourceInstance(),IDS_VFNAME,szText,MAX_PATH);
  }
  else
  {
    switch(GET_SHGDN_FOR(dwFlags))
    {
    case SHGDN_FORADDRESSBAR:
    case SHGDN_NORMAL:
      switch(GET_SHGDN_RELATION(dwFlags))
      {
      case SHGDN_INFOLDER:
        m_PidlMgr.GetName(pidl,szText);
        HR(_tcslen(szText)>0);
        break;
      case SHGDN_NORMAL:
        ::LoadString(_Module.GetResourceInstance(),IDS_VFNAME,szText,MAX_PATH);
        _tcscat(szText,_T("\\"));
        //get the full pidl
        LPITEMIDLIST   tmpPidl;

        tmpPidl = m_PidlMgr.Concatenate(m_pidlPath, pidl);    
        HR(m_PidlMgr.GetFullName(tmpPidl,szTmp,&dwLen));
        if( dwLen > 0 )
        {
          _tcscat(szText,szTmp);
        }
        m_PidlMgr.Delete(tmpPidl);
        break;

      default:
        return E_INVALIDARG;
      }
      break;
    default:
      return E_INVALIDARG;
    }
  }

  USES_CONVERSION;
#ifdef _UNICODE
  // Allocate the wide character string 
  int cchOleStr = (int)_tcslen(szText) + 1;  
  lpName->pOleStr = (LPWSTR)_Module.m_Allocator.Alloc(cchOleStr * sizeof(OLECHAR)); 

  if( lpName->pOleStr==NULL ) 
    return E_OUTOFMEMORY;  

  ::ZeroMemory(lpName->pOleStr,cchOleStr * sizeof(OLECHAR));

  lpName->uType = STRRET_WSTR;
  _tcscpy( lpName->pOleStr, T2CW(szText));
#else
  lpName->uType = STRRET_CSTR;
  _tcscpy(lpName->cStr, T2CA(szText)); //cStr had allocated space
#endif
  return S_OK;
}

STDMETHODIMP CMyVirtualFolder::GetUIObjectOf(HWND,// hWnd, 
                                             UINT nCount, 
                                             LPCITEMIDLIST* pidls, 
                                             REFIID riid, LPUINT, 
                                             LPVOID* ppRetVal)
{
  //ATLTRACE(_T("CMyVirtualFolder::GetUIObjectOf <%s> #%ld '%s'...\n"), DbgGetIID(riid), nCount, PidlToString(pidls==NULL ? NULL : pidls[0]));
  VALIDATE_OUT_POINTER(ppRetVal);

  ATLASSERT(nCount>0);
  if( nCount == 0 ) 
  {
    MessageBox(NULL,_T("CMyVirtualFolder::GetUIObjectOf() nCount=0"),_T("NSF"),MB_OK);
    return E_INVALIDARG;
  }

  HRESULT Hr;

  if( riid == IID_IExtractIcon ) 
  {
    CComObject< CExtractIcon > *pExtractIcon;
    HR( CComObject< CExtractIcon >::CreateInstance(&pExtractIcon) );

    pExtractIcon->AddRef();

    pExtractIcon->_Init(*pidls);
    Hr=pExtractIcon->QueryInterface(IID_IExtractIcon, ppRetVal);

    pExtractIcon->Release();
    return Hr;
  }

  if( riid == IID_IDropTarget ) 
  {

    if( nCount !=1)
      return E_FAIL;

    CComObject<CNSFDropTarget>* pDropTarget;
    HR( CComObject<CNSFDropTarget>::CreateInstance(&pDropTarget) );

    pDropTarget->AddRef();	    

    HR( pDropTarget->_Init(this, *pidls) );

    Hr=pDropTarget->QueryInterface(IID_IDropTarget, ppRetVal);

    pDropTarget->Release();

    return Hr;
  }

  return E_NOINTERFACE;
}


STDMETHODIMP CMyVirtualFolder::ParseDisplayName( HWND,// hwndOwner, 
                                                LPBC,// pbcReserved, 
                                                LPOLESTR,// lpDisplayName, 
                                                LPDWORD,// pdwEaten, 
                                                LPITEMIDLIST *,//ppidl, 
                                                LPDWORD// pdwAttributes
                                                )
{
  ATLTRACENOTIMPL(_T("CMyVirtualFolder::ParseDisplayName"));
}

STDMETHODIMP CMyVirtualFolder::SetNameOf(HWND, 
                                         LPCITEMIDLIST pidlOld, 
                                         LPCOLESTR pstrName, 
                                         DWORD, 
                                         LPITEMIDLIST* ppidlOut)
{
  ATLTRACENOTIMPL(_T("CMyVirtualFolder::SetNameOf"));
}

/////////////////////////////////////////////////////////////////////////////
// IQueryInfo
//

//GetInfoFlags() Retrieves the information flags for an item. 
//				 This method is not currently used. 
STDMETHODIMP CMyVirtualFolder::GetInfoFlags(DWORD* pdwFlags)
{
  ATLTRACENOTIMPL(_T("CMyVirtualFolder::GetInfoFlags"));
}

//GetInfoTip() Retrieves the info tip text information for an item. 
STDMETHODIMP CMyVirtualFolder::GetInfoTip(DWORD, LPWSTR* ppwszTip)
{
  VALIDATE_OUT_POINTER(ppwszTip);
  CComBSTR bstr;
  bstr.LoadString(IDS_INFOTIP);
  *ppwszTip = (LPWSTR)_Module.m_Allocator.Alloc((bstr.Length()+1) * sizeof(WCHAR)); 
  if( *ppwszTip==NULL ) 
    return E_OUTOFMEMORY;  

  ::ZeroMemory(*ppwszTip,(bstr.Length()+1) * sizeof(WCHAR));

  wcscpy(*ppwszTip, bstr);
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// User defined functions

/////////////////////////////////////////////////////////////////////////////
//   Drag-and-Drop  Functions  
//
//

//pidlDropDest is the relative pidl to m_pidlPath
HRESULT CMyVirtualFolder::_DoDrop(LPDATAOBJECT pDataObj, 
                                  DWORD dwDropEffect,
                                  LPITEMIDLIST pidlDropDest,
                                  UINT iFmtIdx)
{
  VALIDATE_POINTER(pDataObj);

  HRESULT Hr = E_INVALIDARG;
  STGMEDIUM stgmed;

  if( FMT_NSEDRAGDROP_INDEX == iFmtIdx )
  {
    // Check for CFSTR_NSEDRAGDROP
    FORMATETC fe2 = { _Module.m_CFSTR_NSEDRAGDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    if( SUCCEEDED( pDataObj->GetData(&fe2, &stgmed) ) ) 
    {
      Hr = _DoDrop_NSEDRAGDROP(stgmed.hGlobal, dwDropEffect,pidlDropDest);
      ::ReleaseStgMedium(&stgmed);
      return Hr;
    }
  }
  else if(FMT_HDROP_INDEX == iFmtIdx)
  {
    // Check for HDROP
    FORMATETC fe1 = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    if( SUCCEEDED( pDataObj->GetData(&fe1, &stgmed) ) ) 
    {
      Hr = _DoDrop_HDROP(stgmed.hGlobal, dwDropEffect,pidlDropDest);
      ::ReleaseStgMedium(&stgmed);
      return Hr;
    }
  }

  return Hr;
}

HRESULT CMyVirtualFolder::_DoDrop_HDROP(HGLOBAL hMem, 
                                        DWORD dwDropEffect,
                                        LPITEMIDLIST pidlDropDest)
{
  VALIDATE_POINTER(hMem);
  TCHAR szMsg[MAX_PATH]={0};

  HRESULT Hr= S_OK;
  // Get target path
  TCHAR szDropDest[MAX_PATH]={};
  DWORD dwSize=MAX_PATH;
  LPITEMIDLIST pidlComplexDest = m_PidlMgr.Concatenate(m_pidlPath,pidlDropDest);
  if(pidlComplexDest){
    HR(m_PidlMgr.GetFullName(pidlComplexDest,szDropDest,&dwSize));
    if(dwSize == 0){
      m_PidlMgr.Delete(pidlComplexDest);
      return E_FAIL;
    }
  }

  DWORD dwFileOpFlags = 0;
  // Iterate over HDROP information
  HDROP hDrop = (HDROP) ::GlobalLock(hMem);
  if( hDrop==NULL ){
    m_PidlMgr.Delete(pidlComplexDest);
    return E_OUTOFMEMORY;
  }

  TCHAR szItemName[MAX_PATH]={0};
  PTCHAR pChr = NULL;
  TCHAR szFileName[MAX_PATH]={0};
  UINT nFiles = ::DragQueryFile(hDrop, (UINT) -1, NULL, 0);
  for( UINT i=0; i<nFiles; i++ ){
    // Get dragged filename
    if( ::DragQueryFile(hDrop, i, szFileName, MAX_PATH)==0 ){
      m_PidlMgr.Delete(pidlComplexDest);
      ::GlobalUnlock(hMem);
      return E_FAIL;
    }
    // dragged a folder
    if((::GetFileAttributes(szFileName) & FILE_ATTRIBUTE_DIRECTORY)!=0){
      // TODO
      MessageBox(NULL,_T("Drag & Drop operation be stopped!\n\
                         Our NSE doesn't support drag&drop operation on folder object,\n\
                         Please make sure to drag file objects only!"),
                         _T("NSExtDragDrop"),
                         MB_OK);
      m_PidlMgr.Delete(pidlComplexDest);
      ::GlobalUnlock(hMem);
      return E_FAIL;
    }
  }

  for(int i=0; i<(int)nFiles; i++ ) 
  {
    // Get dragged filename

    if( ::DragQueryFile(hDrop, i, szFileName, MAX_PATH)==0 ) 
    {
      m_PidlMgr.Delete(pidlComplexDest);
      ::GlobalUnlock(hMem);
      return E_FAIL;
    }

    pChr = _tcsrchr(szFileName,_T('\\'));
    _tcscpy(szItemName,pChr+1);

    AddItemToCfgFile(pidlComplexDest,szItemName,NWS_FILE);

    RefreshShellViewWndsExcept( NULL);

  }

  m_PidlMgr.Delete(pidlComplexDest);
  ::GlobalUnlock(hMem);

  return Hr;

}
HRESULT CMyVirtualFolder::_DoDrop_NSEDRAGDROP(HGLOBAL hMem, DWORD dwDropEffect,LPITEMIDLIST pidlDropDest)
{
  HRESULT Hr;

  TCHAR szDropDest[MAX_PATH]={};
  DWORD dwSize=MAX_PATH;
  LPITEMIDLIST pidlComplexDest = m_PidlMgr.Concatenate(m_pidlPath,pidlDropDest);
  if(pidlComplexDest)
  {
    HR(m_PidlMgr.GetFullName(pidlComplexDest,szDropDest,&dwSize));
    if(dwSize == 0)
    {
      m_PidlMgr.Delete(pidlComplexDest);
      return E_FAIL;
    }
  }

  // Get Out Complex PIDL of drag object
  LPITEMIDLIST pidlDragObject = (LPITEMIDLIST) ::GlobalLock(hMem);
  if( pidlDragObject == NULL )
  {
    m_PidlMgr.Delete(pidlComplexDest);
    return E_FAIL;
  }

  TCHAR szDragObject[MAX_PATH]={};
  dwSize=MAX_PATH;
  HR(m_PidlMgr.GetFullName(pidlDragObject,szDragObject,&dwSize));

  if( _DragDropInSamePath(szDragObject,szDropDest)) 
  {
    m_PidlMgr.Delete(pidlComplexDest);
    ::GlobalUnlock(hMem);
    return E_FAIL; 
  }

  _tcsnset(szDragObject,0,MAX_PATH);

  LPITEMIDLIST  pidlTemp = m_PidlMgr.GetLastItem(pidlDragObject);
  HR(m_PidlMgr.GetName(pidlTemp,szDragObject));

  ITEM_TYPE iItemType = m_PidlMgr.GetItemType(pidlDragObject);
  if(AddItemToCfgFile(pidlComplexDest,szDragObject,iItemType)==FALSE)
  {
    MessageBox(NULL,_T("Destination Folder had include a same name file."),_T("Name conflict"),MB_OK);
    m_PidlMgr.Delete(pidlComplexDest);
    ::GlobalUnlock(hMem);
    return E_FAIL;
  }

  m_PidlMgr.Delete(pidlComplexDest);
  ::GlobalUnlock(hMem);

  return Hr;
}