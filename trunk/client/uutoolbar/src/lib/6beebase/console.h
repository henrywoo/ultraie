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

#ifndef __UULOGGING__
#define __UULOGGING__

#include "guard.h"
#include <string>
#include <atlstr.h>
#include <time.h>
typedef const wchar_t*  wcstr;

class cconsole{
public:
  cconsole(const wchar_t* name, ...);
  ~cconsole();

  void Write(wcstr fmt,...);
  void WriteV(wcstr fmt, va_list argList);

  static std::wstreambuf* Redirect(std::wostream& strm);
  static bool m_enabled;	

private:
  static cmutex m_lock;
  static long m_isn;
  ATL::CString m_name;
  DWORD m_threadid;
  long m_id;
};

#endif


