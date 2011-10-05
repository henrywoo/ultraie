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

#ifndef __UUTOOLBAR_PLUGIN_HOST__
#define __UUTOOLBAR_PLUGIN_HOST__

#include "C6BeeMenuStatusBar.h"

#ifdef COMPILE_HOST
#define HOSTAPI __declspec(dllexport) // when the host is compiling
#else
#define HOSTAPI __declspec(dllimport) // when the plugins are compiling
#endif

namespace nsplugin{
  void AddParser( class uuplugin *parser );
  C6BeeMenuStatusBar* GetC6BeeMenuStatusBarPtr();
  C6BeeMenuStatusBar** GetPPSB();
  int GetSBSize();
  int GetCurrentPaneID(uuplugin* p);
  //bool GetC6BeeMenuStatusBarArray(C6BeeMenuStatusBar** ppSB,int size);
};



#endif
