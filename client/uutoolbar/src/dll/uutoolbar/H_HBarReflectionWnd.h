#ifndef __UUTOOLBAR_HBARREFLECTIONWND__
#define __UUTOOLBAR_HBARREFLECTIONWND__

#include "H_HBarWnd.h"

class CH_HBarReflectionWnd : public CWindowImpl<CH_HBarReflectionWnd>
{
public:
  DECLARE_WND_CLASS(NULL)
  BEGIN_MSG_MAP(CH_HBarReflectionWnd)
    MESSAGE_HANDLER(WM_CREATE,OnCreate)
    CHAIN_MSG_MAP_MEMBER(m_CH_HBarWnd)
  END_MSG_MAP()

  LRESULT OnCreate(UINT,WPARAM,LPARAM,BOOL&);
  CH_HBarReflectionWnd(void);
  virtual ~CH_HBarReflectionWnd(void);

  CH_HBarWnd m_CH_HBarWnd;
};

#endif