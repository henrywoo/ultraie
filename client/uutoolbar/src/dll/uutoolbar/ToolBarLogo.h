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