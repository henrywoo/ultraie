#pragma once
#include <commctrl.h>
#include "stdafx.h"

class CToolBarLogo:
	public CWindowImpl< CToolBarLogo >
{
public:
	BEGIN_MSG_MAP(CToolBarLogo)
		MESSAGE_HANDLER(WM_SETCURSOR,OnSetCursor)
		MESSAGE_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
		MESSAGE_HANDLER(WM_PAINT,OnPaint)
	END_MSG_MAP()

	LRESULT OnSetCursor(UINT,WPARAM,LPARAM,BOOL&);
	LRESULT OnLButtonDown(UINT,WPARAM,LPARAM,BOOL&);
	LRESULT OnPaint(UINT,WPARAM,LPARAM,BOOL&);

	CToolBarLogo();
	virtual ~CToolBarLogo();
	void SetBrowser(IWebBrowser2*);

	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR	 m_gdiplusToken;
	Gdiplus::Image *m_pngLogo;

private:
	IWebBrowser2 * p_IWebBrowser2;
};