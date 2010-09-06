#pragma once

#ifndef __AFXWIN_H__
  #error you should include ¡°stdafx.h¡±
#endif

#include "resource.h"
#include "CapRawVideo.h"

//#include <boost/shared_ptr.hpp>

class CCapVideoApp : public CWinApp
{
public:
  CCapVideoApp();
  virtual ~CCapVideoApp();

public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();

  DECLARE_MESSAGE_MAP()

protected:
  //const boost::shared_ptr<CCapRawVideo> m_caprawvideo;
  CCapRawVideo* m_caprawvideo;
  BOOL InitCapture();
};

extern CCapVideoApp theApp;