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