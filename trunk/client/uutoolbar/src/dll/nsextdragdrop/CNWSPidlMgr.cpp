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
#include "CNWSPidlMgr.h"
#include "Util.h"

//==================================================================
LPITEMIDLIST CNWSPidlMgr::Concatenate(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
  LPITEMIDLIST pidlNew;
  UINT cb1 = 0; 
  UINT cb2 = 0;

  //are both of these NULL?
  if(!pidl1 && !pidl2)
    return NULL;

  //if pidl1 is NULL, just return a copy of pidl2
  if(!pidl1)
  {
    pidlNew = Copy(pidl2);
    return pidlNew;
  }

  //if pidl2 is NULL, just return a copy of pidl1
  if(!pidl2)
  {
    pidlNew = Copy(pidl1);
    return pidlNew;
  }

  cb1 = GetByteSize(pidl1) - sizeof(ITEMIDLIST);
  cb2 = GetByteSize(pidl2);

  //create the new PIDL
  pidlNew = (LPITEMIDLIST)_Module.m_Allocator.Alloc(cb1 + cb2);
  if(pidlNew){
    ::ZeroMemory(pidlNew,cb1+cb2);
    //copy the first PIDL
    ::CopyMemory(pidlNew, pidl1, cb1);
    //copy the second PIDL
    ::CopyMemory(((LPBYTE)pidlNew) + cb1, pidl2, cb2);
  }
  return pidlNew;
}

void CNWSPidlMgr::Delete(LPITEMIDLIST pidl)
{
  if (pidl)
  {
    _Module.m_Allocator.Free(pidl);
  }
}

LPITEMIDLIST CNWSPidlMgr::GetNextItem(LPCITEMIDLIST pidl)
{
  ATLASSERT(pidl != NULL);
  if (!pidl)
    return NULL;
  return (LPITEMIDLIST)(LPBYTE)(((LPBYTE)pidl) + pidl->mkid.cb);
}

LPITEMIDLIST CNWSPidlMgr::GetLastItem(LPCITEMIDLIST pidl)
{
  LPITEMIDLIST pidlLast = NULL;

  //get the PIDL of the last item in the list
  if(pidl){
    while(pidl->mkid.cb)
    {
      pidlLast = (LPITEMIDLIST)pidl;
      pidl = GetNextItem(pidl);
    }      
  }
  return pidlLast;
}

LPITEMIDLIST CNWSPidlMgr::Copy(LPCITEMIDLIST pidlSrc)
{
  LPITEMIDLIST pidlTarget = NULL;
  UINT Size = 0;

  if (pidlSrc == NULL)
    return NULL;

  // Allocate memory for the new PIDL.
  Size = GetByteSize(pidlSrc);
  pidlTarget = (LPITEMIDLIST) _Module.m_Allocator.Alloc(Size);

  if (pidlTarget == NULL)
    return NULL;

  // Copy the source PIDL to the target PIDL.
  ::ZeroMemory(pidlTarget,Size);
  ::CopyMemory(pidlTarget, pidlSrc, Size);

  return pidlTarget;
}

UINT CNWSPidlMgr::GetByteSize(LPCITEMIDLIST pidl)
{
  UINT Size = 0;
  LPITEMIDLIST pidlTemp = (LPITEMIDLIST) pidl;

  ATLASSERT(pidl != NULL);
  if (!pidl)
    return 0;

  while (pidlTemp->mkid.cb != 0)
  {
    Size += pidlTemp->mkid.cb;
    pidlTemp = GetNextItem(pidlTemp);
  }  

  // add the size of the NULL terminating ITEMIDLIST
  Size += sizeof(ITEMIDLIST);

  return Size;
}

bool CNWSPidlMgr::IsSingle(LPCITEMIDLIST pidl)
{
  LPITEMIDLIST pidlTemp = GetNextItem(pidl);
  return pidlTemp->mkid.cb == 0;
}

BOOL CNWSPidlMgr::HasSubFolder(LPCITEMIDLIST pidl)
{
  TCHAR szPath[MAX_PATH]=_T("");
  DWORD dwLen=MAX_PATH;

  GetFullName(pidl,szPath,&dwLen);
  if(dwLen>0)
  {
    //return HasSubFolder(szPath);
    TCHAR szCfgFile[MAX_PATH]=_T("");
    _tcscpy(szCfgFile,_Module.m_szInstallPath);
    _tcscat(szCfgFile,_T("\\NSExtDragDrop.cfg"));

    TCHAR tmpStr[MAX_PATH]=_T("");    
    GetPrivateProfileString(szPath,_T("dir"),_T("NotFound"),tmpStr,dwLen, szCfgFile);

    if( (_tcscmp(tmpStr,_T("NotFound"))==0 ) || (_tcslen(tmpStr)==0 ) ){
      return FALSE;       
    }
    else 
    {
      return TRUE;
    }
  }
  else
    return FALSE;
}

//get the pathname releative to NSF's root folder of folder object use its complex PIDL
HRESULT CNWSPidlMgr::GetFullName(LPCITEMIDLIST pidl,LPTSTR szFullName,DWORD *pdwLen)
{
  if(!pdwLen)
    return E_FAIL;

  *pdwLen=0;

  if(!pidl) //NSE's root folder
  {
    return S_OK;
  }

  LPITEMIDLIST pidlTemp = (LPITEMIDLIST) pidl;

  while (pidlTemp->mkid.cb != 0)
  {
    TCHAR szTemp[MAX_PATH]={};
    GetName(pidlTemp,szTemp);

    if( szFullName)
    {
      if(0 == *pdwLen)
      {
        _tcscpy(szFullName,szTemp);
      }
      else
      {
        _tcscat(szFullName,_T("\\"));
        _tcscat(szFullName,szTemp);
      }
      *pdwLen =(DWORD)_tcslen(szFullName);
    }
    else
    {
      *pdwLen+=(DWORD)_tcslen(szTemp);
    }
    pidlTemp = GetNextItem(pidlTemp);
  }  

  *pdwLen += 1;

  return S_OK;
}


HRESULT CNWSPidlMgr::GetItemAttributes(LPCITEMIDLIST pidl,USHORT iAttrNum,LPTSTR pszAttrOut)
{
  TCHAR szTemp[MAX_PATH]={};

  switch (iAttrNum)
  {
  case ATTR_TYPE :
    {
      LPITEMIDLIST pidlTemp;
      pidlTemp=GetLastItem(pidl);

      if( NWS_FOLDER == GetItemType(pidlTemp))
      {
        _tcscpy(pszAttrOut,_T("文件夹"));
      }
      else
      {
        GetName(pidlTemp,szTemp);

        SHFILEINFO sfi;  
        ZeroMemory(&sfi,sizeof(sfi));

        if (SHGetFileInfo(szTemp, 0, &sfi, sizeof(sfi),SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME)){
          _tcscpy(pszAttrOut,sfi.szTypeName);                                                     
        }else{
          _tcscpy(pszAttrOut,_T("文件"));
        }
      }
    }
    break;

  default :
    return E_INVALIDARG;
  }
  return S_OK;
}

//=====================================================================
LPPIDLDATA CNWSPidlMgr::GetDataPointer(LPCITEMIDLIST pidl)
{
  if(!pidl)
    return NULL;

  return (LPPIDLDATA)(pidl->mkid.abID);
}

LPITEMIDLIST CNWSPidlMgr::Create(ITEM_TYPE iItemType,LPTSTR pszName)
{
  USHORT TotalSize =(USHORT) (sizeof(ITEMIDLIST) + sizeof(ITEM_TYPE) + (_tcslen(pszName)+1)*sizeof(TCHAR));

  // Also allocate memory for the final null SHITEMID.
  LPITEMIDLIST pidlNew = NULL;
  pidlNew = (LPITEMIDLIST) _Module.m_Allocator.Alloc(TotalSize + sizeof(ITEMIDLIST));
  if (pidlNew)
  {
    //::ZeroMemory(pidlNew,TotalSize + sizeof(USHORT));
    ::ZeroMemory(pidlNew,TotalSize + sizeof(ITEMIDLIST));
    LPITEMIDLIST pidlTemp = pidlNew;
    // Prepares the PIDL to be filled with actual data
    pidlTemp->mkid.cb = (USHORT)TotalSize;
    LPPIDLDATA     pData;
    pData = GetDataPointer(pidlTemp);
    // Fill the PIDL
    pData->type = iItemType;
    ::CopyMemory(pData->szName, pszName, (_tcslen(pszName)+1) * sizeof(TCHAR));
    // Set an empty PIDL at the end
    //set the NULL terminator to 0
    pidlTemp = GetNextItem(pidlTemp);
    pidlTemp->mkid.cb = 0;
    pidlTemp->mkid.abID[0] = 0;
  }
  return pidlNew;
}

ITEM_TYPE CNWSPidlMgr::GetItemType(LPCITEMIDLIST pidl)
{
  LPITEMIDLIST  pidlTemp = GetLastItem(pidl);
  LPPIDLDATA pData;
  pData = GetDataPointer(pidlTemp);
  return pData->type;
}

HRESULT CNWSPidlMgr::GetName(LPCITEMIDLIST pidl,LPTSTR pszName)
{
  LPPIDLDATA pData;
  pData = GetDataPointer(pidl);
  _tcscpy(pszName,pData->szName);
  return S_OK;
}