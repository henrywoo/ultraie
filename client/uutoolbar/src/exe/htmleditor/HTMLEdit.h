#pragma once

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif
#include "resource.h"

class CHTMLEditApp : public CWinApp{
public:
  CHTMLEditApp();
  ~CHTMLEditApp(){}

public:
  virtual BOOL InitInstance();
  void OpenHTMLFile();
  void ParseCommandLine(CCommandLineInfo& rCmdInfo);

  afx_msg void OnAppAbout();
  DECLARE_MESSAGE_MAP()
};

extern CHTMLEditApp theApp;