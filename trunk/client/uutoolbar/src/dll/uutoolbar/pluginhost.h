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
