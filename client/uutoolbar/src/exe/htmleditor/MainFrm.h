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


