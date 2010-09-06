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

