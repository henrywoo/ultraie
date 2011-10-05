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

class CHTMLEditView : public CHtmlEditView
{
protected: // create from serialization only
  CHTMLEditView();
  DECLARE_DYNCREATE(CHTMLEditView)

// Attributes
public:
  CHTMLEditDoc* GetDocument() const;

// Operations
public:

// Overrides
  public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
  virtual ~CHTMLEditView();

protected:
  BOOL m_bDesignMode;

// Generated message map functions
protected:
  DECLARE_DHTMLEDITING_CMDMAP(CHTMLEditView)
  DECLARE_MESSAGE_MAP()
public:
  void OnToggleBrowse(void);
  void OnInitialUpdate(void);
  void OnUpdateModeEdit(CCmdUI *pCmdUI);
  void OnUpdateModeView(CCmdUI *pCmdUI);
  void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);
  void OnDownloadBegin();
  BOOL CreateSnapshotThumb_NotifyWnd();

  time_t time_begin;
};

#ifndef _DEBUG  // debug version in HTMLEditView.cpp
inline CHTMLEditDoc* CHTMLEditView::GetDocument() const
   { return reinterpret_cast<CHTMLEditDoc*>(m_pDocument); }
#endif

