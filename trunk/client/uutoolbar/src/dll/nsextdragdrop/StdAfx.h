// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__8640B0BF_551C_441B_92FB_A3FDF6FF0A8D__INCLUDED_)
#define AFX_STDAFX_H__8640B0BF_551C_441B_92FB_A3FDF6FF0A8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#define _ATL_APARTMENT_THREADED
#include <atlbase.h>


#include <shlobj.h>
#include "CShellImageList.h"
#include <dinput.h>
#include "CNWSModule.h"


//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CNWSModule _Module;
#define MAX_STRLEN 2048

//#ifdef _DEBUG
//	#define _WRITETRACEINFO_ON
//#endif
//
//#ifdef _WRITETRACEINFO_ON
//extern FILE *fpDbg;
//extern TCHAR trString[MAX_STRLEN];
//extern UINT trNum;
//#endif //_WRITETRACEINFO_ON

#include <atlcom.h>

extern const CLSID CLSID_MyVirtualFolder;
extern const CLSID CLSID_NSFShellView;

#include "Util.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8640B0BF_551C_441B_92FB_A3FDF6FF0A8D__INCLUDED)
