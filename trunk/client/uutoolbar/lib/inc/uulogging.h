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

#ifndef __UULOGGING_UULOGGING__
#define __UULOGGING_UULOGGING__

#include <vadefs.h>
#include "guard.h"

/**
* Levels of logging
*/
enum RmLevel{
  LOG_CALL,
  LOG_DBG,
  LOG_ERROR = 9
};

/**
* Logging interface
*/
class uulogging{
  static bool initialized;
  static uulogging* pinstance;
  static cmutex lock_;
public:
  /**
  * Get the singleton instance
  */
  static uulogging & R();

  /**
  * Initialize the object
  */
  virtual bool Initialize(const wchar_t* _fname) = 0;

  /**
  * Set data
  */
  virtual void Set(const char* file, int line) = 0;

  /**
  * Write data to log
  */
  virtual void Write(RmLevel level, const char* format_ptr, ...) = 0;

  /**
  * Write data to log - W
  */
  virtual void Write(RmLevel level, const wchar_t* format_ptr, ...) = 0;

  /**
  * Write with arguments
  */
  virtual void Write_v(RmLevel level, const char* format_ptr, va_list& vp) = 0;

  /**
  * Destroy log object
  */
  virtual void Destroy() = 0;

};


#ifdef _UU_TRACE_

class CCall{
public:
  CCall(const char* proc,const char* file="c:\\ultraie_trace.log", int line=0)
    :proc_(proc)
  {
    uulogging& log = uulogging::R();
    log.Set(__FILE__, __LINE__);
    log.Write(LOG_CALL, "Entering %s", proc);
  }

  ~CCall (void){
    uulogging& log = uulogging::R();
    log.Set(__FILE__, __LINE__);
    log.Write(LOG_CALL, "Leaving %s", proc_);
  }

  const char* proc_;
};
#define UUTRACE(X) \
  CCall ccall(X, __FILE__, __LINE__);

#endif//_UU_TRACE_

// Usage:
//   UUDEBUG((rmDetail,"Starting UltraIE"));
#ifdef NDEBUG
#define UUDEBUG(X)
#else
#define UUDEBUG(X)  \
  uulogging::R().Set(__FILE__, __LINE__);\
  uulogging::R().Write X;
#endif



#endif