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

#pragma once

class CMainFrame : public CFrameWnd{
protected: // create from serialization only
  CMainFrame();
  DECLARE_DYNCREATE(CMainFrame)

public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual ~CMainFrame();

protected:
  CStatusBar  m_wndStatusBar;
  CToolBar    m_wndToolBar;
  CToolBar    m_wndFormatBar;
  CReBar      m_wndReBar;

protected:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  DECLARE_MESSAGE_MAP()
};


