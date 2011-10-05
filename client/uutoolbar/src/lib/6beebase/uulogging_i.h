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

#ifndef __UULOGGING_I__
#define __UULOGGING_I__

#include "uulogging.h"

#include <fstream>//std::ofstream
#include "guard.h"//cmutex


/**
* Log implementation
*/
class uulogging_i : public uulogging{
public:
  // construct
  uulogging_i();

  // destroy
  virtual ~uulogging_i();

  // initialize the class
  virtual bool Initialize(const wchar_t* _fname);

  // terminate logging
  virtual void Destroy();

  // set data for next write 
  virtual void Set(const char* file, int line);

  // write - A
  virtual void Write(RmLevel level, const char* format_ptr, ...);

  // write - W
  virtual void Write(RmLevel level, const wchar_t* format_ptr, ...);

  // write (this is used by MPX callback)
  virtual void Write_v(RmLevel level, const char* format_ptr, va_list& vp);

private:

  // All writes lead here.
  void WriteImpl(RmLevel level, const char* format_ptr, va_list& vp);

  // protect update to the active buffer and writing to destFile
  cmutex lock_;

  // indicates that the object is ready
  bool m_bopen;

  // operating parameters
  int             m_line;    // line of code reporting 
  const char*     m_file;    // file reporting
  std::ofstream  destFile;    // file

  // unused
  DISALLOW_COPY_AND_ASSIGN(uulogging_i);
};

#endif