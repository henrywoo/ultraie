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
#include "Util.h"
#include "CNWSPidlMgr.h"
#include "resource.h"

////////////////////////////////////////////
// Debug functions add here

//#ifdef _WRITETRACEINFO_ON
//
//TCHAR trString[MAX_STRLEN]={};
//FILE *fpDbg = NULL;
//UINT trNum = 0;
//
//LPTSTR DbgGetCF(CLIPFORMAT cf)
//{
//   static TCHAR szName[128];
//   *szName = _T('\0');
//   ::GetClipboardFormatName(cf, szName, 128);
//   return szName;
//}
//
//void trInit() 
//{
//	fpDbg = _tfopen(_T("C:\\NSEDnD_trace.txt"),_T("a+"));
//	trNum=0;
//}
//
//void trPuts(LPTSTR str) 
//{
//	if(fpDbg == NULL)
//		trInit();
//	if(fpDbg) 
//	{
//		_ftprintf(fpDbg,_T("No:%d -- %s\n\n"),trNum++,str);
//		fflush(fpDbg);
//	}
//}
//
//void trFinal() 
//{
//	if(fpDbg)
//		fclose(fpDbg);
//}
//
//void trPrintHex(BYTE *input,int len,LPTSTR str)
//{
//	int i,j;
//	i=0;
//	j=0;
//	for(i=0;i<len;i++)
//	{
//		j+=_stprintf(str+j,_T("%02x "),input[i]);
//	}
//}
//
//void trPrintFullPidl(LPCITEMIDLIST pidl,LPTSTR str)
//{
//	ATLASSERT(str != NULL);
//	if(str==NULL)
//	{
//		MessageBox(NULL,_T("trPrintFullPidl() str=NULL"),_T("NSE"),MB_OK);
//	}
//	CNWSPidlMgr pidlMgr;
//	TCHAR szTmp[MAX_PATH]=_T("");
//
//	if( pidl == NULL )
//	{
//		_tcscat( str, _T("<NULL>"));
//		return;
//	}
//
//	LPITEMIDLIST pidlTemp = (LPITEMIDLIST) pidl;
//
//	while (pidlTemp->mkid.cb != 0)
//	{
//		_tcsnset(szTmp,0,MAX_PATH);
//		trPrintHex((BYTE *)pidlTemp,(int)pidlTemp->mkid.cb,szTmp);
//		_tcscat(str,szTmp);
//		pidlTemp = pidlMgr.GetNextItem(pidlTemp);
//	}  
//	_tcscat(str,_T("00 00"));
//	return ;
//}
//
//#endif //_WRITETRACEINFO_ON


//////////////////////////////////////////////////////
// global tool functions
HRESULT GetGlobalSettings()
{
  HRESULT hr = S_OK;

  static TCHAR szValue[MAX_PATH];
  //TCHAR szValue[MAX_PATH]={};

  USES_CONVERSION;
  LPOLESTR wszNSECLSID = NULL;
  hr = ::StringFromCLSID(CLSID_MyVirtualFolder, &wszNSECLSID);

  CRegKey keyRoot,key;
  DWORD dwType, dw = sizeof(szValue);
  keyRoot.Open(HKEY_CLASSES_ROOT, _T("CLSID"), KEY_READ);

  TCHAR szSubKey[MAX_PATH]={};
  _tcscpy(szSubKey,OLE2T(wszNSECLSID));
  _tcscat(szSubKey,_T("\\InProcServer32"));

  if( key.Open(keyRoot, szSubKey, KEY_READ) == S_OK ) 
  {
    *szValue = _T('\0');
    if(::RegQueryValueEx(key.m_hKey, NULL, NULL, &dwType, (LPBYTE)szValue, &dw) == ERROR_SUCCESS)
    {
      TCHAR* pch = _tcsrchr(szValue, '\\');
      ::CopyMemory(_Module.m_szInstallPath,szValue,pch-szValue);

      CoTaskMemFree(wszNSECLSID);
      key.Close();
      keyRoot.Close();
      return S_OK;
    }
    else
    {
      CoTaskMemFree(wszNSECLSID);
      key.Close();
      keyRoot.Close();
      return E_FAIL;
    }
  }
  else
  {
    CoTaskMemFree(wszNSECLSID);
    keyRoot.Close();
    return E_FAIL;
  }

}
BOOL CreateImageLists()
{
  // Set the small image list
  int nSmallCx = ::GetSystemMetrics(SM_CXSMICON);
  int nSmallCy = ::GetSystemMetrics(SM_CYSMICON);

  HIMAGELIST imgSmall = ImageList_Create(nSmallCx, nSmallCy, ILC_COLOR32 | ILC_MASK, 4, 0);
  if( imgSmall==NULL ) return FALSE;

  // Set the large image list
  int nLargeCx = ::GetSystemMetrics(SM_CXICON);
  int nLargeCy = ::GetSystemMetrics(SM_CYICON);
  HIMAGELIST imgLarge = ImageList_Create(nLargeCx, nLargeCy, ILC_COLOR32 | ILC_MASK, 4, 0);
  if( imgLarge==NULL ) return FALSE;

  HICON hIcon;

  // Load NSF Root icon
  hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
    MAKEINTRESOURCE(IDI_NSFROOT),
    IMAGE_ICON, 
    nSmallCx, nSmallCy, 
    LR_DEFAULTCOLOR);
  ImageList_AddIcon(imgSmall, hIcon);
  hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
    MAKEINTRESOURCE(IDI_NSFROOT),
    IMAGE_ICON, 
    nLargeCx, nLargeCy, 
    LR_DEFAULTCOLOR);
  ImageList_AddIcon(imgLarge, hIcon);

  // Load Folder icon
  hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
    MAKEINTRESOURCE(IDI_FOLDER),
    IMAGE_ICON, 
    nSmallCx, nSmallCy, 
    LR_DEFAULTCOLOR);
  ImageList_AddIcon(imgSmall, hIcon);
  hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
    MAKEINTRESOURCE(IDI_FOLDER),
    IMAGE_ICON, 
    nLargeCx, nLargeCy, 
    LR_DEFAULTCOLOR);
  ImageList_AddIcon(imgLarge, hIcon);

  // Load the Open Folder icon
  hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
    MAKEINTRESOURCE(IDI_FOLDER_OPEN),
    IMAGE_ICON, 
    nSmallCx, nSmallCy, 
    LR_DEFAULTCOLOR);
  ImageList_AddIcon(imgSmall, hIcon);
  hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
    MAKEINTRESOURCE(IDI_FOLDER_OPEN),
    IMAGE_ICON, 
    nLargeCx, nLargeCy, 
    LR_DEFAULTCOLOR);
  ImageList_AddIcon(imgLarge, hIcon);

  // Load File icon (use the icon for this DLL)
  hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
    MAKEINTRESOURCE(IDI_FILE),
    IMAGE_ICON, 
    nSmallCx, nSmallCy, 
    LR_DEFAULTCOLOR);
  ImageList_AddIcon(imgSmall, hIcon);
  hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
    MAKEINTRESOURCE(IDI_FILE),
    IMAGE_ICON, 
    nLargeCx, nLargeCy, 
    LR_DEFAULTCOLOR);
  ImageList_AddIcon(imgLarge, hIcon);

  //save the created imagelist
  _Module.m_ImageLists.m_hImageListSmall = imgSmall;
  _Module.m_ImageLists.m_hImageListLarge = imgLarge;

  return TRUE;
}
// Sort List View item callback function.
int CALLBACK ListViewSortFuncCB(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  //return CMyVirtualFolder::_CompareIDs(lParamSort, (LPCITEMIDLIST)lParam1, (LPCITEMIDLIST)lParam2);  
  CListSortInfo* pSort = (CListSortInfo*) lParamSort;
  _ASSERT(NULL != pSort);
  return (int)pSort->pNSFolder->CompareIDs(lParamSort,(LPITEMIDLIST)lParam1,(LPITEMIDLIST)lParam2);
}
int WideCharToLocal(LPTSTR pLocal, LPWSTR pWide, DWORD dwChars)
{
  *pLocal = 0;

#ifdef UNICODE
  lstrcpyn(pLocal, pWide, dwChars);
#else
  WideCharToMultiByte( CP_ACP, 
    0, 
    pWide, 
    -1, 
    pLocal, 
    dwChars, 
    NULL, 
    NULL);
#endif

  return lstrlen(pLocal);
}


int LocalToWideChar(LPWSTR pWide, LPTSTR pLocal, DWORD dwChars)
{
  *pWide = 0;

#ifdef UNICODE
  lstrcpyn(pWide, pLocal, dwChars);
#else
  MultiByteToWideChar( CP_ACP, 
    0, 
    pLocal, 
    -1, 
    pWide, 
    dwChars); 
#endif

  return lstrlenW(pWide);
}

BOOL CALLBACK RefreshShellView( HWND hWnd, LPARAM lParam )
{
  if( hWnd ) 
  {
    TCHAR szClassName[MAX_PATH]={};
    DWORD dwLen=MAX_PATH;
    GetClassName(hWnd,szClassName,dwLen);
    if( (_tcscmp(szClassName,_T("ExploreWClass"))==0) || (_tcscmp(szClassName,_T("CabinetWClass"))==0) )
    {
      HWND hwndShellView = FindWindowEx(hWnd,NULL,_T("NSFViewClass"),NULL);
      if(hwndShellView !=NULL)
      {			
        HWND hwndExcept =(HWND)lParam;
        if((hwndExcept!=NULL && hwndExcept!=hwndShellView) ||
         (hwndExcept==NULL) )
          ::SendMessage(hwndShellView,WM_COMMAND,ID_VIEW_REFRESH,0);
      }
    }

  }
  return( TRUE );
}

void RefreshShellViewWndsExcept(HWND hwndExcept)
{
  for(; !EnumWindows( (WNDENUMPROC) RefreshShellView,(LPARAM) hwndExcept ); ); // continue looping until done
}

void OpenDirInCfgFile(IN LPCITEMIDLIST pidlCurDir, 
                      OUT LPTSTR szPath,
                      OUT LPTSTR szCfgFile)         
{
  _tcscpy(szCfgFile,_Module.m_szInstallPath);
  _tcscat(szCfgFile,_T("\\NSExtDragDrop.cfg"));

  CNWSPidlMgr pidlMgr;
  DWORD dwLen=MAX_PATH;
  if( (NULL==pidlCurDir)||(0 == pidlCurDir->mkid.cb))
    _tcscpy(szPath,_T("ROOT"));
  else
    pidlMgr.GetFullName(pidlCurDir,szPath,&dwLen);
}

HRESULT DeleteItemInCfgFile(IN LPITEMIDLIST pidlItemComplex)
{
  HRESULT Hr=S_OK;

  //1.Get the cfg file name
  TCHAR szCfgFile[MAX_PATH]=_T("");
  _tcscpy(szCfgFile,_Module.m_szInstallPath);
  _tcscat(szCfgFile,_T("\\NSExtDragDrop.cfg"));

  //2.Get the name of pidl
  CNWSPidlMgr pidlMgr;
  TCHAR szName[MAX_PATH]={};
  LPITEMIDLIST  pidlTemp = pidlMgr.GetLastItem(pidlItemComplex);
  HR(pidlMgr.GetName(pidlTemp,szName));
  //HR(pidlMgr.GetName(pidlItemComplex,szName));

  //3.Get the Section
  TCHAR szFullName[MAX_PATH]={0};
  DWORD dwLen=MAX_PATH;
  HR(pidlMgr.GetFullName(pidlItemComplex,szFullName,&dwLen));

  TCHAR szSection[MAX_PATH]=_T("");
  if(_tcslen(szName) == _tcslen(szFullName))
  {
    _tcscpy(szSection,_T("ROOT"));
  }
  else
  {
    _tcsncpy(szSection,szFullName,_tcslen(szFullName)-_tcslen(szName)-1);
  }

  //4.Get the type of pidl
  ITEM_TYPE iItemType;
  iItemType = pidlMgr.GetItemType(pidlItemComplex);
  Hr = DeleteItemInCfgFile(szSection,szName,iItemType,szCfgFile);
  return Hr;
}

HRESULT DeleteItemInCfgFile(IN LPCITEMIDLIST pidlCurDir, IN LPITEMIDLIST pidlItem)
{
  HRESULT Hr=S_OK;
  TCHAR szCfgFile[MAX_PATH]=_T("");
  TCHAR szSection[MAX_PATH]=_T("");
  OpenDirInCfgFile(pidlCurDir,szSection,szCfgFile);

  //3.Get the name of pidl
  CNWSPidlMgr pidlMgr;
  TCHAR szName[MAX_PATH]={};
  HR(pidlMgr.GetName(pidlItem,szName));
  //4.Get the type of pidl
  ITEM_TYPE iItemType;
  iItemType = pidlMgr.GetItemType(pidlItem);

  Hr = DeleteItemInCfgFile(szSection,szName,iItemType,szCfgFile);
  return Hr;
}
HRESULT DeleteItemInCfgFile(IN LPTSTR szSection, IN LPTSTR szName, IN ITEM_TYPE iItemType,LPTSTR szCfgFile)
{
  HRESULT Hr=S_OK;
  //	TCHAR szCfgFile[MAX_PATH]=_T("");
  //	TCHAR szSection[MAX_PATH]=_T("");
  //	OpenDirInCfgFile(pidlCurDir,szSection,szCfgFile);
  //
  //    //3.Get the name of pidl
  //	CNWSPidlMgr pidlMgr;
  //    TCHAR szName[MAX_PATH]={};
  //    HR(pidlMgr.GetName(pidlItem,szName));
  //    //4.Get the type of pidl
  //    ITEM_TYPE iItemType;
  //     iItemType = pidlMgr.GetItemType(pidlItem);

  DWORD dwCheck;
  TCHAR tmpStr[MAX_PATH]={};
  DWORD dwLen=MAX_PATH;
  if( iItemType == NWS_FOLDER )
  {
    dwCheck = GetPrivateProfileString( szSection,_T("dir"),_T(""),tmpStr,dwLen, szCfgFile);
  }
  else if( iItemType == NWS_FILE )
  {
    dwCheck = GetPrivateProfileString( szSection,_T("file"),_T(""),tmpStr,dwLen, szCfgFile);
  }

  //5. delete the name of pidl if it exist in tmpStr and form a newstr
  TCHAR szNewStr[MAX_PATH]={};
  if(_tcsstr(tmpStr,szName)==NULL) //basiclly filter
  {
    MessageBox(NULL,_T("DeleteItemInCfgFile: the pidl to be deleted can't be found in current folder!"),_T("Error"),MB_OK);
    return E_FAIL;
  }

  INT iNameLen= (INT)_tcslen(szName);
  INT iStrLen = (INT)_tcslen(tmpStr);
  INT iPos = 0;
  TCHAR *pSplitChr=NULL;
  BOOL bFound=FALSE;
  while(iPos + iNameLen <= iStrLen)
  {
    if( iPos + iNameLen == iStrLen) 
    {
      if(_tcsncmp(tmpStr+iPos,szName,iNameLen) == 0) //is the tail item
      {
        //iPos=0:is the only one item; iPos!=0:has more than one items
        if(iPos != 0) 
          _tcsncpy(szNewStr,tmpStr,iPos-1);
        bFound=TRUE;
        break;
      }
    }
    else
    {
      pSplitChr=_tcschr(tmpStr+iPos,_T(';'));
      if(pSplitChr == NULL)
        break;
      if( ((pSplitChr-(tmpStr+iPos)) == iNameLen) && 
        (_tcsncmp(tmpStr+iPos,szName,iNameLen) == 0) )
      {
        _tcsncpy(szNewStr,tmpStr,iPos);
        _tcscat(szNewStr,pSplitChr+1);
        bFound=TRUE;
        break;
      }
      else
      {
        iPos = (INT)(pSplitChr - (TCHAR*)tmpStr[0] + 1);
      }
    }
  }
  if(bFound == FALSE)
  {
    MessageBox(NULL,_T("DeleteItemInCfgFile: pidl ready to delete can't be find in current folder!"),_T("Error"),MB_OK);
    return E_FAIL;
  }

  //6. delete the old keyvalue and insert the new keyvalue
  if( iItemType == NWS_FOLDER )
  {
    if(_tcslen(szNewStr)!=0)
      dwCheck = WritePrivateProfileString( szSection,_T("dir"),szNewStr,szCfgFile);
    else
      dwCheck = WritePrivateProfileString( szSection,_T("dir"),NULL,szCfgFile);


    //Delete the deleted folder's corresponding section from cfg file
    //Notes:
    //To make this sample project perfect,indeed,we need to
    //recursively delete all the sub folders corresponding sections
    //from configuration file, to do this will spend lots.
    //
    //However, for our purpose is to describe how to delete/create folder,
    //for simplicity, we only delete the section which directly
    //referred by the deleted folder object from configuration file.

    if(_tcscmp(szSection,_T("ROOT"))==0)
    {
      _tcsnset(szSection,0,MAX_PATH);
      _tcscpy(szSection,szName);
    }
    else
    {
      _tcscat(szSection,_T("\\"));
      _tcscat(szSection,szName);
    }
    WritePrivateProfileString(szSection,NULL,NULL,szCfgFile);
  }
  else if( iItemType == NWS_FILE )
  {
    if(_tcslen(szNewStr)!=0)
      dwCheck = WritePrivateProfileString( szSection,_T("file"),szNewStr,szCfgFile);
    else
      dwCheck = WritePrivateProfileString( szSection,_T("file"),NULL,szCfgFile);
  }
  return Hr;
}

void CreateFolderInCfgFile(IN LPCITEMIDLIST pidlCurDir, OUT LPTSTR szTempNewName)
{

  TCHAR szCfgFile[MAX_PATH]={};
  TCHAR szSection[MAX_PATH]={};
  OpenDirInCfgFile(pidlCurDir,szSection,szCfgFile);

  TCHAR szDirKeyValue[MAX_PATH]={};
  DWORD dwLen=MAX_PATH;
  GetPrivateProfileString(szSection,_T("dir"),_T(""),szDirKeyValue,dwLen, szCfgFile);

  TCHAR szTempName[MAX_PATH]={};
  _tcscpy(szTempName,_T("New Folder"));
  UINT iIdx=1;
  TCHAR szKeyValue[MAX_PATH]={};

  // Back up szDirKeyValue for _tcstok will change szDirKeyValue's value
  _tcscpy(szKeyValue,szDirKeyValue); 
  while(NotUniqueName(szTempName,szKeyValue)==TRUE)
  {
    iIdx++;
    _stprintf(szTempName,_T("New Folder(%d)"),(INT)iIdx);
    _tcscpy(szKeyValue,szDirKeyValue);
  }

  if(_tcslen(szDirKeyValue) == 0)
    _tcscpy(szDirKeyValue,szTempName);
  else
  {
    _tcscat(szDirKeyValue,_T(";"));
    _tcscat(szDirKeyValue,szTempName);
  }

  WritePrivateProfileString(szSection,_T("dir"),szDirKeyValue,szCfgFile);
  ::CopyMemory(szTempNewName,szTempName,_tcslen(szTempName)*sizeof(TCHAR));
}
BOOL AddItemToCfgFile(IN LPCITEMIDLIST pidlCurDir, IN LPTSTR szItemName, IN ITEM_TYPE iItemType)
{

  TCHAR szCfgFile[MAX_PATH]={};
  TCHAR szSection[MAX_PATH]={};
  OpenDirInCfgFile(pidlCurDir,szSection,szCfgFile);

  TCHAR szItemKeyValue[MAX_PATH]={};
  DWORD dwLen=MAX_PATH;
  if(iItemType == NWS_FILE)
    GetPrivateProfileString(szSection,_T("file"),_T(""),szItemKeyValue,dwLen, szCfgFile);
  else
    GetPrivateProfileString(szSection,_T("dir"),_T(""),szItemKeyValue,dwLen, szCfgFile);

  // Back up szDirKeyValue for _tcstok will change szDirKeyValue's value
  TCHAR szKeyValue[MAX_PATH]={};
  _tcscpy(szKeyValue,szItemKeyValue); 
  if(NotUniqueName(szItemName,szKeyValue)==TRUE)
    return FALSE;

  if(_tcslen(szItemKeyValue) == 0)
    _tcscpy(szItemKeyValue,szItemName);
  else
  {
    _tcscat(szItemKeyValue,_T(";"));
    _tcscat(szItemKeyValue,szItemName);
  }

  if(iItemType == NWS_FILE)
    WritePrivateProfileString(szSection,_T("file"),szItemKeyValue,szCfgFile);
  else
    WritePrivateProfileString(szSection,_T("dir"),szItemKeyValue,szCfgFile);

  return TRUE;
}
BOOL AddFileToCfgFile(IN LPCITEMIDLIST pidlCurDir, IN LPTSTR szFileName)
{

  TCHAR szCfgFile[MAX_PATH]={};
  TCHAR szSection[MAX_PATH]={};
  OpenDirInCfgFile(pidlCurDir,szSection,szCfgFile);

  TCHAR szFileKeyValue[MAX_PATH]={};
  DWORD dwLen=MAX_PATH;
  GetPrivateProfileString(szSection,_T("file"),_T(""),szFileKeyValue,dwLen, szCfgFile);

  // Back up szDirKeyValue for _tcstok will change szDirKeyValue's value
  TCHAR szKeyValue[MAX_PATH]={};
  _tcscpy(szKeyValue,szFileKeyValue); 
  if(NotUniqueName(szFileName,szKeyValue)==TRUE)
    return FALSE;

  if(_tcslen(szFileKeyValue) == 0)
    _tcscpy(szFileKeyValue,szFileName);
  else
  {
    _tcscat(szFileKeyValue,_T(";"));
    _tcscat(szFileKeyValue,szFileName);
  }

  WritePrivateProfileString(szSection,_T("file"),szFileKeyValue,szCfgFile);
  return TRUE;
}

HRESULT ReplaceNameInCfgFile(IN LPCITEMIDLIST pidlCurDir, IN LPTSTR szNewName)
{
  TCHAR szCfgFile[MAX_PATH]=_T("");
  TCHAR szSection[MAX_PATH]=_T("");
  OpenDirInCfgFile(pidlCurDir,szSection,szCfgFile);

  TCHAR szKeyValue[MAX_PATH]=_T("");
  DWORD dwLen=MAX_PATH;
  GetPrivateProfileString(szSection,_T("dir"),_T(""),szKeyValue,dwLen,szCfgFile);
  if(_tcslen(szKeyValue)==0)
    return E_FAIL;

  //last item in keyvalue is the new created item.
  TCHAR szNewKeyValue[MAX_PATH]=_T("");
  TCHAR *pNewFolder=_tcsrchr(szKeyValue,_T(';'));
  if(pNewFolder==NULL) //current folder has only one item
    _tcscpy(szNewKeyValue,szNewName);
  else
  {	
    _tcsncpy(szNewKeyValue,szKeyValue,pNewFolder-szKeyValue+1);
    _tcscat(szNewKeyValue,szNewName);
  }
  WritePrivateProfileString(szSection,_T("dir"),szNewKeyValue,szCfgFile);

  return S_OK;
}
BOOL NotUniqueName(IN LPTSTR szObjName, IN LPTSTR szKeyValue)
{
  TCHAR seps[]= _T(";");
  TCHAR *token;    

  /* Establish string and get the first token: */
  token = _tcstok( szKeyValue, seps );
  while( token != NULL )
  {
    /* While there are tokens in "string" */
    //printf( " %s\n", token );
    if(_tcscmp(token,szObjName)==0)
    {
      return TRUE;
    }
    /* Get next token: */
    token = _tcstok( NULL, seps );
  }
  return FALSE;
}

BOOL _DragDropInSamePath(LPTSTR pszDroppedFileName,LPTSTR pszDropDest)
{
  /*
  ATLASSERT(pDataObj);
  // Query the IDataObject for the private clipboard format "AdfID".
  // Only folders from "ADF View" will know about it and return a
  // folder ID...
  DWORD dwFolderID;
  HRESULT Hr = DataObj_GetDWORD(pDataObj, _Module.m_CFSTR_ADFID, &dwFolderID);
  if( FAILED(Hr) ) return FALSE;
  return dwFolderID==m_dwFolderID; // Same ID? Then same CFolder...
  */
  BOOL RetVal=FALSE;
  ATLASSERT(pszDroppedFileName);
  ATLASSERT(pszDropDest);

  // if Drop Dest is the root folder.
  if(pszDropDest == NULL)
  {
    if(_tcschr(pszDroppedFileName,_T('\\'))==NULL)
      return TRUE;
    else
      return FALSE;
  }

  //else
  TCHAR *pStart =_tcsstr(pszDroppedFileName,pszDropDest);
  int pos=pStart-pszDroppedFileName +1;
  if((pStart != NULL) &&(pos == 1) )
  {
    if(_tcschr(pszDroppedFileName+_tcslen(pszDropDest)+1,_T('\\'))==NULL)
      RetVal=TRUE;
    else
      RetVal=FALSE;
  }
  else
    RetVal = FALSE;


  return RetVal;
}
