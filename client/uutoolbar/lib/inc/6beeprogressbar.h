#ifndef __MY_C6BEEPROGRESS_H__
#define __MY_C6BEEPROGRESS_H__

#include "../../dll/6beecommond/6beecommond.h"
#include <atlwin.h>
#include "6bees_str.h"

class C6BeeProgressCtrl:
  public CWindowImpl<C6BeeProgressCtrl>//public CProgressBarCtrl
{
public:
  DECLARE_WND_CLASS(NULL)

  BEGIN_MSG_MAP(C6BeeProgressCtrl)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
  END_MSG_MAP()

private:
  int   m_nLower;
  int   m_nUpper;
  int   m_nStep;
  int   m_nCurPos;
public:
  C6BeeProgressCtrl():m_nLower(0),m_nUpper(100),m_nCurPos(0),m_nStep(1){}

  ~C6BeeProgressCtrl(){}

  void SetRange(int nLower, int nUpper){
    m_nLower = nLower ;
    m_nUpper = nUpper ;
    m_nCurPos = nLower ;
  }
  void SetStep(int nStep){ m_nStep = nStep; }
  void SetPos(int nPos){ m_nCurPos = nPos; }

  LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    static RECT rectClient={};
    static COLORREF fontcolor = RGB(10,20,30);
    GetClientRect(&rectClient);

    PAINTSTRUCT m_ps;
    HDC m_hDC = ::BeginPaint(m_hWnd, &m_ps);
    ::SetTextColor(m_hDC, fontcolor);
    ::SetBkMode(m_hDC, TRANSPARENT);
    int nProgress=m_nCurPos>100?100:m_nCurPos;

    //1. border
    RECT rcProgress = rectClient;
    COLORREF m_rgbHeaderBackground=GetSysColor( COLOR_BTNFACE );
    COLORREF m_rgbBackground  = GetSysColor( COLOR_WINDOW );
    COLORREF m_rgbProgressTop = RGB( 170, 240, 170 );
    COLORREF m_rgbProgressBottom = RGB( 45, 210, 50 );
    _6beed_util::DrawRoundRect(m_hDC, &rcProgress, GetSysColor(COLOR_3DSHADOW), m_rgbHeaderBackground,m_rgbBackground);

    //2. progress
    ::InflateRect(&rcProgress, -1, -1);
    int tmp = rcProgress.right - rcProgress.left;
    rcProgress.right = rcProgress.left + (int)( (double)tmp*nProgress/100);
    _6beed_util::DrawGradient(m_hDC, &rcProgress, m_rgbProgressTop, m_rgbProgressBottom);

    //2.draw text
    HFONT myfont=::CreateFont(14,0,0,0,FW_THIN,false,FALSE,FALSE,DEFAULT_CHARSET,
      OUT_STRING_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));
    HFONT hOldFont = (HFONT)::SelectObject(m_hDC, myfont);
    std::wstring PercentageStr = _6bees_str::wstringmaker() << nProgress << L"%";
    int PercentageStr_size=(int)PercentageStr.size();
    SIZE fontsize;
    ::GetTextExtentPoint32(m_hDC,PercentageStr.c_str(),PercentageStr_size,&fontsize);
    RECT tmprect = rcProgress;
    tmprect.left  = rectClient.left + (rectClient.right - rectClient.left)/2 - fontsize.cx/2;
    tmprect.right= tmprect.left + fontsize.cx;
    ::DrawText(m_hDC,PercentageStr.c_str(),PercentageStr_size,&tmprect,
      DT_CENTER|DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER|DT_END_ELLIPSIS);
    ::SelectObject(m_hDC,(HFONT)hOldFont);
    ::DeleteObject(hOldFont);
    ::DeleteObject(myfont);
    return 0;
  }
};

#endif
