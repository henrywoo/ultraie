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

#ifndef __UULOGGING_GUARD__
#define __UULOGGING_GUARD__

#include "basictypes.h"
#include <windows.h>//CRITICAL_SECTION

/// Multithreaded Synchronization 
/// Implemented using CRITICAL_SECTION for speed (see MS Doc)
class cmutex {
public:
  cmutex(){
    InitializeCriticalSection (&m_cs);
  }
  ~cmutex(){
    DeleteCriticalSection (&m_cs); 
  }

  void acquire(){
    EnterCriticalSection (&m_cs); 
  }

  void release(){
    LeaveCriticalSection(&m_cs); 
  }
private:
  /// MS Doc states that CRITICAL_SECTION should not be copied
  DISALLOW_COPY_AND_ASSIGN(cmutex);
  CRITICAL_SECTION m_cs;
};


/// Guard for Multithreaded Synchronization objects
/// Ensure that resource is locked/unlocked regardless of exit path
class cguard {
public:

  /// @param l - reference the lock to use
  explicit cguard(cmutex& m) : m_cs(m){
    acquire();
  }

  ~cguard(){
    release();
  }

  void acquire(){
    m_cs.acquire();
  }

  void release(void){
    m_cs.release();
  }

private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(cguard);
  cmutex& m_cs;
};

#endif
