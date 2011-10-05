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

#ifndef __MYUTIL_H_
#define __MYUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <crtdbg.h>
#include <stdio.h>
#include "MyVirtualFolder.h"

// Neat macros someone always leaves out
#ifndef lengthof
   #define lengthof(x) (sizeof(x)/sizeof(*x))
#endif

#ifndef offsetof
  #define offsetof(type, field) ((int)&((type*)0)->field)
#endif

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#ifdef _DEBUG
#define HR(expr) { if(FAILED(Hr=expr)) { _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr); _CrtDbgBreak(); return Hr; } }
#define VALIDATE_POINTER(p) ATLASSERT(p); if(!p) return E_POINTER
#define VALIDATE_OUT_POINTER(p) ATLASSERT(p); if(!p) return E_POINTER; else *p=NULL
#else
#define HR(expr) if(FAILED(Hr=expr)) return Hr
#define VALIDATE_POINTER(p) if(!p) return E_POINTER
#define VALIDATE_OUT_POINTER(p) if(!p) return E_POINTER; else *p=NULL
#endif//_DEBUG

#define PidlToString

//#ifdef _WRITETRACEINFO_ON
//LPTSTR DbgGetCF(CLIPFORMAT cf);
//void trInit();
//void trPuts(LPTSTR str);
//void trFinal();
//void trPrintHex(BYTE *input,int len,LPTSTR str);
//void trPrintFullPidl(LPCITEMIDLIST pidl,LPTSTR str);
//#endif //_WRITETRACEINFO_ON

typedef enum
{
    FMT_HDROP_INDEX=1,
    FMT_NSEDRAGDROP_INDEX,
}NSF_SUPPORT_FMT;

typedef enum
{
	ATTR_TYPE =0,
}NEF_ATTRIBUTES;


// Constants that tell us the order of
// columns in the ListView control.
typedef enum 
{
   COL_NAME = 0,
   COL_TYPE,
} LISTVIEW_COLUMNS;

// Constants defining where in the shared
// ImageList control the icons are placed.
typedef enum
{
   ICON_INDEX_NSFROOT = 0,
   ICON_INDEX_FOLDER,
   ICON_INDEX_FOLDER_OPEN,
   ICON_INDEX_FILE,
} ICONINDEX;

struct CListSortInfo
{
    CMyVirtualFolder*  pNSFolder;       // parent folder of the view
	USHORT            iSortColumn;            //which column to sort by
    BOOL              bForwardSort;     // which direction to sort
};

HRESULT GetGlobalSettings();
BOOL CreateImageLists();
int CALLBACK ListViewSortFuncCB(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
 
int WideCharToLocal(LPTSTR, LPWSTR, DWORD);
int LocalToWideChar(LPWSTR, LPTSTR, DWORD);
void RefreshShellViewWndsExcept(HWND hwndExcept);
BOOL CALLBACK RefreshShellView( HWND hWnd, LPARAM lParam );

/*  Configuration file management functions */
void OpenDirInCfgFile(IN LPCITEMIDLIST pidlCurDir, 
                      OUT LPTSTR szPath,
                      OUT LPTSTR szCfgFile);
HRESULT DeleteItemInCfgFile(IN LPITEMIDLIST pidlItemComplex);
HRESULT DeleteItemInCfgFile(IN LPCITEMIDLIST pidlCurDir, IN LPITEMIDLIST pidlItem);
HRESULT DeleteItemInCfgFile(IN LPTSTR szSection, IN LPTSTR szName, IN ITEM_TYPE iItemType,LPTSTR szCfgFile);

void CreateFolderInCfgFile(IN LPCITEMIDLIST pidlCurDir, OUT LPTSTR szTempNewName);
BOOL NotUniqueName(IN LPTSTR szObjName, IN LPTSTR szKeyValue);
HRESULT ReplaceNameInCfgFile(IN LPCITEMIDLIST pidlCurDir, IN LPTSTR szNewName);

/* Drag-and-drop  */
BOOL _DragDropInSamePath(LPTSTR pszDroppedFileName,LPTSTR pszDropDest);
BOOL AddFileToCfgFile(IN LPCITEMIDLIST pidlCurDir, IN LPTSTR szFileName);
BOOL AddItemToCfgFile(IN LPCITEMIDLIST pidlCurDir, IN LPTSTR szItemName, IN ITEM_TYPE iItemType);


#endif //__MYUTIL_H_