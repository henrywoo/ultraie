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

#ifndef __UUTOOLBAR_GLOBALVAR__
#define __UUTOOLBAR_GLOBALVAR__

#include <set>
#include <map>
#include "C6BeeStatusWnd.h"
#include "guard.h"

namespace uutoolbar{
  
  typedef std::map<DWORD,C6BeeStatusWnd*> TidStatusWndMap;

  int getoffset();

  // Singleton for Process-based Variables
  class GlobalVar
  {
  private:
    static bool initialized;
    static GlobalVar* pinstance;
    static cmutex lock_;

    DISALLOW_COPY_AND_ASSIGN(GlobalVar);
    GlobalVar();

    // 1. Map of thread id -> status bar window
    TidStatusWndMap tidstatuswndmap_;

  public:
    static GlobalVar& R();

    TidStatusWndMap& GetTidStatusWndMap();
    
    C6BeeStatusWnd* GetCurrentStatusWnd();

    // offset for IE7
    const int kstatusbar_offset;

  };


  class bmpsgton
  {
  private:
    static bool initialized;
    static bmpsgton* pinstance;
    static cmutex lock_;

    DISALLOW_COPY_AND_ASSIGN(bmpsgton);
    bmpsgton();

  public:
    static bmpsgton& R();

    HBITMAP h_;
    HBITMAP h2_;
  };

};

#endif