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
