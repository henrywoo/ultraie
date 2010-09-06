#ifndef __UUTOOLBAR__DRAW__
#define __UUTOOLBAR__DRAW__




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CBufferDC : public CDCHandle 
{
  HDC     m_hDestDC;
  CBitmap m_bitmap;     // Bitmap in Memory DC
  CRect   m_rect;
  HGDIOBJ m_hOldBitmap; // Previous Bitmap

public:
  CBufferDC (HDC hDestDC, const CRect& rcPaint = CRect(0,0,0,0)): m_hDestDC (hDestDC)
  {
    if ( rcPaint.IsRectEmpty() )
    {
      ::GetClipBox (m_hDestDC, m_rect);
    }
    else
    {
      m_rect = rcPaint;
    }
    Attach (::CreateCompatibleDC (m_hDestDC));
    m_bitmap.Attach (::CreateCompatibleBitmap (m_hDestDC, m_rect.right, m_rect.bottom));
    m_hOldBitmap = ::SelectObject (m_hDC, m_bitmap);

    if ( m_rect.top > 0 )
    {
      ExcludeClipRect (0, 0, m_rect.right, m_rect.top);
    }
    if ( m_rect.left > 0 )
    {
      ExcludeClipRect (0, m_rect.top, m_rect.left, m_rect.bottom);
    }
  }
  ~CBufferDC (){
    ::BitBlt (m_hDestDC, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
      m_hDC, m_rect.left, m_rect.top, SRCCOPY);
    ::SelectObject (m_hDC, m_hOldBitmap);
  }
};


///////////////////////////////////////////////////////////////////////////////
class CPenDC
{
protected:
  CPen m_pen;
  HDC m_hDC;
  HPEN m_hOldPen;

public:
  CPenDC (HDC hDC, COLORREF crColor = CLR_NONE): m_hDC (hDC)
  {
    m_pen.CreatePen (PS_SOLID, 1, crColor);
    m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
  }

  ~CPenDC (){
    ::SelectObject (m_hDC, m_hOldPen);
  }

  void Color (COLORREF crColor){
    ::SelectObject (m_hDC, m_hOldPen);
    m_pen.DeleteObject();
    m_pen.CreatePen (PS_SOLID, 1, crColor);
    m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
  }
  COLORREF Color () const{
    LOGPEN logPen;
    ((CPenDC*)this)->m_pen.GetLogPen (&logPen);
    return logPen.lopnColor;
  }
};

///////////////////////////////////////////////////////////////////////////////
class CBrushDC
{
protected:
  CBrush m_brush;
  HDC m_hDC;
  HBRUSH m_hOldBrush;

public:
  CBrushDC (HDC hDC, COLORREF crColor=CLR_NONE):m_hDC(hDC)
  {
    if (crColor==CLR_NONE)
      m_brush.Attach ((HBRUSH)::GetStockObject (NULL_BRUSH));
    else
      m_brush.CreateSolidBrush (crColor);
    m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
  }
  ~CBrushDC (){
    ::SelectObject (m_hDC, m_hOldBrush);
  }

  void Color (COLORREF crColor){
    ::SelectObject (m_hDC, m_hOldBrush);
    m_brush.DeleteObject();
    if ( crColor == CLR_NONE )
      m_brush.Attach ((HBRUSH)::GetStockObject (NULL_BRUSH));
    else
      m_brush.CreateSolidBrush (crColor);
    m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
  }
  COLORREF Color () const{
    LOGBRUSH logBrush;
    ((CBrushDC*)this)->m_brush.GetLogBrush (&logBrush);
    return logBrush.lbColor;
  }
};

///////////////////////////////////////////////////////////////////////////////
class CBoldDC
{
protected:
  CFont m_fontBold;
  HDC m_hDC;
  HFONT m_hDefFont;

public:
  CBoldDC (HDC hDC, bool bBold): m_hDC (hDC), m_hDefFont (NULL)
  {
    LOGFONT lf;

    CFontHandle ((HFONT)::GetCurrentObject (m_hDC, OBJ_FONT)).GetLogFont (&lf);

    if ( ( bBold && lf.lfWeight != FW_BOLD) ||
      (!bBold && lf.lfWeight == FW_BOLD) )
    {
      lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

      m_fontBold.CreateFontIndirect (&lf);
      m_hDefFont = (HFONT)::SelectObject (m_hDC, m_fontBold);
    }
  }
  ~CBoldDC (){
    if ( m_hDefFont != NULL )
    {
      ::SelectObject (m_hDC, m_hDefFont);
    }
  }
};

#define KEYDOWN(Key) ((GetKeyState(Key)&0x8000)!=0)

class CClientRect : public CRect{
public:
  CClientRect (HWND hWnd){
    ::GetClientRect (hWnd, this);
  };
  CClientRect (const CWindow* pWnd){
    ::GetClientRect (pWnd->m_hWnd, this);
  };
};

class CWindowRect : public CRect{
public:
  CWindowRect (HWND hWnd){
    ::GetWindowRect (hWnd, this);
  };
  CWindowRect (const CWindow* pWnd){
    ::GetWindowRect (pWnd->m_hWnd, this);
  };
};


#endif