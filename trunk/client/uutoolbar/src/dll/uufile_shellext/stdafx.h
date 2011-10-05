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