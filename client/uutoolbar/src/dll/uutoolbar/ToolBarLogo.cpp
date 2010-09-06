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
	//方法二: 直接读取png文件并显示!
	Gdiplus::Graphics graphics(dc.m_hDC);
	graphics.DrawImage(m_pngLogo,Gdiplus::Point(0,0));
	return 0;
}

//鼠标移动到控件上面的时候显示一个小手!
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