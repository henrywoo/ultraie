#pragma once

#ifndef STRICT
#define STRICT
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT                 
#define _WIN32_WINNT 0x0501
#endif            

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#define _WTL_NO_CSTRING

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>

#include <atlstr.h>
#include <atlapp.h>
#include <atlgdi.h>
#include <atlctrls.h>

#include <shlobj.h>
#include <comdef.h>

using namespace ATL;