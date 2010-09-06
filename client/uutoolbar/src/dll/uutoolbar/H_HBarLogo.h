#ifndef __UUTOOLBAR_HBARLOGO__
#define __UUTOOLBAR_HBARLOGO__

#include <atlbase.h>
#include <atlwin.h>

#include <atlapp.h>
#include <atlctrls.h>

#include <gdiplus.h>

class CH_HBarLogo :
  public CWindowImpl< CH_HBarLogo,CButton >
{
public:
  BEGIN_MSG_MAP(CToolBarLogo)
    MESSAGE_HANDLER(WM_PAINT,OnPaint)
  END_MSG_MAP()

  LRESULT OnPaint(UINT,WPARAM,LPARAM,BOOL&);
  BOOL SetPNG(UINT);
  //BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR,Image * &pImg);

  CH_HBarLogo();
  ~CH_HBarLogo();

protected:
  Gdiplus::GdiplusStartupInput    m_gdiplusStartupInput;
  ULONG_PTR        m_gdiplusToken;
  BOOL                    m_gdiplus_started;

  Gdiplus::Image*   m_pngLogo;
  CBitmap m_bmp;
  HFONT myfont2;
};

#endif