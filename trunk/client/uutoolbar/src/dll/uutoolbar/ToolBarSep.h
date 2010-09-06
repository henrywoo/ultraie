#ifndef __UUTOOLBAR_TOOLBARSEP__
#define __UUTOOLBAR_TOOLBARSEP__

//COM
#include <comdef.h>//_bstr_t

//ATL
#include <atlbase.h>

//WTL
#include <atlapp.h>
#include <atlctrls.h>//CComboBox
#include <commctrl.h>

//GDI+
#include <gdiplus.h>

class CToolBarCOM;

const int ksepwidth  = 6;
const int ksepheight = 24;
const int kminsearchboxlen = 50;
const int kmaxsearchboxlen = 600;

const int WM_SEP_TOOLBAR_MOUSEMOVE    = WM_USER+1666;


class CToolBarSep:
  public CWindowImpl< CToolBarSep,CButton >
{
public:
  //DECLARE_WND_SUPERCLASS(TEXT("CToolBarSep"), TEXT("BUTTON"))
  BEGIN_MSG_MAP(CToolBarSep)
    MESSAGE_HANDLER(WM_CREATE,OnCreate)
    MESSAGE_HANDLER(WM_SETCURSOR,OnSetCursor)
    MESSAGE_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONUP,OnLButtonUp)
    MESSAGE_HANDLER(WM_MOUSEMOVE,OnMouseMove)
    MESSAGE_HANDLER(WM_PAINT,OnPaint)
    //MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
  END_MSG_MAP()

  CToolBarSep(void);
  ~CToolBarSep(void);

  LRESULT OnPaint      (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnCreate   (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnSetCursor  (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonDown(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonUp  (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnMouseMove  (UINT,WPARAM,LPARAM,BOOL&);

  //BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR,Image * &pImg);
  Gdiplus::GdiplusStartupInput    m_gdiplusStartupInput;
  ULONG_PTR        m_gdiplusToken;
  Gdiplus::Image *m_pngLogo;
private:
  CToolBarCOM* m_CToolBarCOM;
  bool m_bMouseDown;
protected:
    CBitmap m_bmp;
};

#endif