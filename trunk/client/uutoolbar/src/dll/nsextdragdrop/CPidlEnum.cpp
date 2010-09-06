//========================================================================================
//
// Module:			CPidlEnum.cpp
// Author:          Zeng Xi
// Creation Date:	10.15.2005
//
//========================================================================================

#include "stdafx.h"
#include "CPidlEnum.h"
#include <ShObjIdl.h>

BOOL CPidlEnum::DeleteList(void)
{
	LPENUMLIST  pDelete;

	while(m_pFirst)
	{
		pDelete = m_pFirst;
		m_pFirst = pDelete->pNext;

		//free the pidl
		m_PidlMgr.Delete(pDelete->pidl);

		//free the list item
		_Module.m_Allocator.Free(pDelete);
	}

	return TRUE;
}
BOOL CPidlEnum::AddToEnumList(LPITEMIDLIST pidl)
{
	LPENUMLIST  pNew;

	pNew = (LPENUMLIST)_Module.m_Allocator.Alloc(sizeof(ENUMLIST));

	if(pNew)
	{
		//set the next pointer
		pNew->pNext = NULL;
		pNew->pidl = pidl;

		//is this the first item in the list?
		if(!m_pFirst)
		{
			m_pFirst = pNew;
			m_pCurrent = m_pFirst;
		}

		if(m_pLast)
		{
			//add the new item to the end of the list
			m_pLast->pNext = pNew;
		}

		//update the last item pointer
		m_pLast = pNew;

		return TRUE;
	}

	return FALSE;
}

HRESULT  CPidlEnum::_AddPidls(ITEM_TYPE iItemType, LPTSTR pszPath)
{
	TCHAR  tmpStr[MAX_PATH]=_T("");
	TCHAR  tmpName[MAX_PATH]=_T("");
	DWORD  dwLen=MAX_PATH;
	DWORD  dwCheck=0;
	LPITEMIDLIST   pidl=NULL;

	TCHAR szCfgFile[MAX_PATH]=_T("");
	_tcscpy(szCfgFile,_Module.m_szInstallPath);
	_tcscat(szCfgFile,_T("\\NSExtDragDrop.cfg"));

	if( iItemType == NWS_FOLDER )
	{
		dwCheck = GetPrivateProfileString( pszPath,_T("dir"),_T(""),tmpStr,dwLen, szCfgFile);
	}
	else if( iItemType == NWS_FILE )
	{
		dwCheck = GetPrivateProfileString( pszPath,_T("file"),_T(""),tmpStr,dwLen, szCfgFile);
	}

	if( _tcslen(tmpStr)==0 )
	{
		return S_OK;
	}
	TCHAR *pChr,*pszHandle;
	pszHandle=tmpStr;
	pChr=pszHandle;

	while((pChr = _tcschr(pszHandle,_T(';') ) )!=NULL)
	{
		_tcsnset(tmpName,0,MAX_PATH);
		_tcsncpy(tmpName,pszHandle,pChr-pszHandle);

		//create correspond simple PIDL
		pidl=m_PidlMgr.Create(iItemType,tmpName);
		if(pidl)
		{
			if(!AddToEnumList(pidl))
				return E_FAIL;
		}
		else
			return E_FAIL;

		if(pszHandle[0] == _T('\0'))
			break;
		pszHandle = pChr+1;
	}

	_tcsnset(tmpName,0,MAX_PATH);
	_tcscpy(tmpName,pszHandle);

	if(_tcslen(tmpName)==0)
		return E_FAIL;

	//create the simple PIDL for the last item in the key value
	pidl=m_PidlMgr.Create(iItemType,tmpName);
	if(pidl)
	{
		if(!AddToEnumList(pidl))
			return E_FAIL;
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CPidlEnum::_Init(LPCITEMIDLIST pidlRoot,DWORD dwFlags)
{
	TCHAR  tmpPath[MAX_PATH]=_T("");
	DWORD  dwLen=MAX_PATH;

	HRESULT  Hr;

	if( (NULL==pidlRoot)||(0 == pidlRoot->mkid.cb)  ) //current folder is root folder
	{
		_tcscpy(tmpPath,_T("ROOT"));
		
	}
	else  //sub-folder in NSE
	{
		HR(m_PidlMgr.GetFullName(pidlRoot,tmpPath,&dwLen));
	}

	if(dwFlags & SHCONTF_FOLDERS)// include folders
	{
		HR(_AddPidls(NWS_FOLDER,tmpPath));
	}
	if(dwFlags & SHCONTF_NONFOLDERS)//include files
	{
		HR(_AddPidls(NWS_FILE,tmpPath));	
	}
    Reset();
    return S_OK;
}

HRESULT CPidlEnum::Next(DWORD dwElements, LPITEMIDLIST apidl[], LPDWORD pdwFetched)
{ 
    ATLTRACE2(atlTraceCOM, 0, _T("IEnumIDList::Next\n"));
	DWORD    dwIndex;
	HRESULT  hr = S_OK;

	if(dwElements > 1 && !pdwFetched)
	return E_INVALIDARG;

	for(dwIndex = 0; dwIndex < dwElements; dwIndex++)
	{
		//is this the last item in the list?
		if(!m_pCurrent)
		{
			hr =  S_FALSE;
			break;
		}

		apidl[dwIndex] = m_PidlMgr.Copy(m_pCurrent->pidl);

		m_pCurrent = m_pCurrent->pNext;
	}

	if(pdwFetched)
	*pdwFetched = dwIndex;

	return hr;
}    

HRESULT CPidlEnum::Reset(void)
{ 
    ATLTRACE2(atlTraceCOM, 0, _T("IEnumIDList::Reset\n"));
	m_pCurrent = m_pFirst;
    return S_OK; 
}

HRESULT CPidlEnum::Skip(DWORD dwSkip)
{
    ATLTRACE2(atlTraceCOM, 0, _T("IEnumIDList::Skip"));
	DWORD    dwIndex;
	HRESULT  hr = S_OK;

	for(dwIndex = 0; dwIndex < dwSkip; dwIndex++)
	{
		//is this the last item in the list?
		if(!m_pCurrent)
		{
			hr = S_FALSE;
			break;
		}

		m_pCurrent = m_pCurrent->pNext;
	}

	return hr;
}

HRESULT CPidlEnum::Clone(IEnumIDList **)
{
    //ATLTRACENOTIMPL(_T("IEnumIDList::Clone"));
   return E_NOTIMPL;
}