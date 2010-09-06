#pragma once


// Change these values to use different versions
#define WINVER			0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE		0x0501

#include <atlbase.h>
#include <atlstr.h>
#include <atlapp.h>


extern CAppModule _Module;

#define _WTL_NO_CSTRING

#include <atlwin.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlddx.h>
#include <process.h>//_beginthreadex

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define END_MSG_MAP_EX	END_MSG_MAP

//Max Active Threads Number - 目前单线程,多线程太麻烦了!
