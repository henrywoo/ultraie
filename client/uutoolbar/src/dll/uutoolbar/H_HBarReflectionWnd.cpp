#include "H_HBarReflectionWnd.h"

CH_HBarReflectionWnd::CH_HBarReflectionWnd(void){}
CH_HBarReflectionWnd::~CH_HBarReflectionWnd(void)
{
  if (IsWindow()) DestroyWindow();
}

/// @note WS_CLIPCHILDREN to avoid flash!
const DWORD DEFAULT_H_HBar_STYLE = 
WS_CLIPCHILDREN |
WS_CHILD | 
WS_CLIPSIBLINGS |
WS_VISIBLE | 
WS_TABSTOP |
TBSTYLE_TOOLTIPS | 
TBSTYLE_FLAT | 
TBSTYLE_TRANSPARENT | 
TBSTYLE_LIST | 
TBSTYLE_CUSTOMERASE | 
TBSTYLE_WRAPABLE |
CCS_TOP | 
CCS_NODIVIDER | 
CCS_NOPARENTALIGN 
| CCS_NORESIZE;

LRESULT CH_HBarReflectionWnd::OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  RECT rect;
  GetClientRect(&rect);

  //在这里检查H_HBar是否有新的版本！
  /// @todo 
  //CheckUpdates();
  ///////////////////////////////////////////////////////////////////////////
  //m_CH_HBarWnd.Create(this->m_hWnd 说明CH_HBarWnd的父窗口是ReflectionWnd。
  ///////////////////////////////////////////////////////////////////////////
  m_CH_HBarWnd.Create(this->m_hWnd, rect, NULL, DEFAULT_H_HBar_STYLE);
  //m_CH_HBarWnd.SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_MIXEDBUTTONS);
  //m_CH_HBarWnd.Create(m_hWnd, rect, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
  
  return 0;
}
