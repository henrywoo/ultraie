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

#include "stdafx.h"
#include "ToolBarLogo.h"
#include "VisualStylesXP.h"
#include <6beeres.h>
#include <6BeeCommons.h>
#include "util.h"

CToolBarLogo::CToolBarLogo():
p_IWebBrowser2(NULL){
	if(GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL)==0){
		GetImageFromIDResource(IDR_PNG_LOGO,_T("PNG"),m_pngLogo);
	}
}

CToolBarLogo::~CToolBarLogo(){
	if(m_gdiplusToken)
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	if (IsWindow()){
		DestroyWindow();
	}
}

LRESULT CToolBarLogo::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
	CPaintDC dc(m_hWnd);
	#ifdef _VISUALSTYLE_XP_H_
	if (g_xpStyle.IsAppThemed())
	{
		RECT rc = {};
		::GetClientRect(m_hWnd,&rc);
		HTHEME hTheme = g_xpStyle.OpenThemeData(m_hWnd, L"TAB");
		g_xpStyle.DrawThemeBackground(hTheme, dc.m_hDC, TABP_BODY, TIS_NORMAL, &rc, 0);
		g_xpStyle.CloseThemeData(hTheme);
	}
	#endif
	//������: ֱ�Ӷ�ȡpng�ļ�����ʾ!
	Gdiplus::Graphics graphics(dc.m_hDC);
	graphics.DrawImage(m_pngLogo,Gdiplus::Point(0,0));
	return 0;
}

//����ƶ����ؼ������ʱ����ʾһ��С��!
LRESULT CToolBarLogo::OnSetCursor(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
	static HCURSOR hcur = LoadCursor(NULL,/*IDC_SIZEALL*/IDC_HAND);
	if (hcur!=NULL){
		SetCursor(hcur);
		return TRUE;
	}else{
		SetMsgHandled(false);
		return false;
	}
}

void CToolBarLogo::SetBrowser(IWebBrowser2* pBrowser){
	if (p_IWebBrowser2) p_IWebBrowser2->Release();
	p_IWebBrowser2 = pBrowser;
	if (p_IWebBrowser2) p_IWebBrowser2->AddRef();
}

LRESULT CToolBarLogo::OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
	if (p_IWebBrowser2){
		CComVariant vEmpty;
		CComVariant vURL(UltraIEURL);
		p_IWebBrowser2->Navigate2(&vURL, &vEmpty, &vEmpty, &vEmpty, &vEmpty);
	}
	return 0;
}