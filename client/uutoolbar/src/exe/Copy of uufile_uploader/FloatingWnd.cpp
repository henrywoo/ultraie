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

#include "stdafx.h"
#include "FloatingWnd.h"

#include <6BeeCommons.h>
#include <6beeres.h>

LRESULT CFloatingWnd::OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
	HIMAGELIST hImageList = CMyRes::R().GetImgList(IMGS_COMBO_MENU);
	m_ImageList.Attach(hImageList);

	m_bMouseDown = false;
	m_bmpBackground = (HBITMAP)CMyRes::R().GetImg(IDB_FLOATINGWND_BG,IMAGE_BITMAP,48,49);
	
	m_bmpBackground.GetSize(m_size);
	
	HINSTANCE hFuncInst = LoadLibrary(_T("User32.DLL"));
	if(hFuncInst){
		//UpdateLayeredWindow = (MYFUNC2) GetProcAddress(hFuncInst, "UpdateLayeredWindow");
		SetLayeredWindowAttributes = (MYFUNC) GetProcAddress(hFuncInst, "SetLayeredWindowAttributes");
	}
	else
	{
		// >= Windows 2000
	}

	SIZE sWin = {::GetSystemMetrics(SM_CXFULLSCREEN),::GetSystemMetrics(SM_CYFULLSCREEN)};
	if (::SetWindowPos(m_hWnd, HWND_TOPMOST,sWin.cx-100,100,48,48,SWP_NOSIZE|SWP_FRAMECHANGED)==0){
		::MessageBox(NULL,_T("失败0！"),_T("HELLO"),0);
	}
	

	POINT ptSrc={0,0};
	DWORD dwExStyle=::GetWindowLong(m_hWnd,GWL_EXSTYLE);
	if((dwExStyle&0x80000)!=0x80000)
		::SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle^0x80000);
	
	//BOOL bRet=FALSE;
	
	if (SetLayeredWindowAttributes(m_hWnd,0,180,2)==0){
		::MessageBox(NULL,_T("失败1！"),_T("HELLO"),0);
	}
	
	//bRet= UpdateLayeredWindow(m_hWnd,hdcScreen,&ptWinPos,&sizeWindow,m_hdcMemory,&ptSrc,0,&m_Blend,2);
	/*
	SIZE sizeWindow= {48,48};
	m_Blend.BlendOp=0; //theonlyBlendOpdefinedinWindows2000
	m_Blend.BlendFlags=0; //nothingelseisspecial...
	m_Blend.AlphaFormat=1; //...
	m_Blend.SourceConstantAlpha=125;//AC_SRC_ALPHA
	if(UpdateLayeredWindow(m_hWnd,NULL,NULL,&sizeWindow,NULL,&ptSrc,0,&m_Blend,2) == 0){
		::MessageBox(NULL,_T("失败2！"),_T("HELLO"),0);
	}
	*/


	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW );
	//ModifyStyle(WS_CAPTION,WS_MINIMIZEBOX,SWP_DRAWFRAME);

	FreeLibrary(hFuncInst);

	return true;
}


LRESULT CFloatingWnd::OnCancel(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
	this->DestroyWindow();
	return true;
}

LRESULT CFloatingWnd::OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
	//::MessageBox(NULL,_T("Simon Woo!"),_T("HELLO"),0);
	::SystemParametersInfo(SPI_SETDRAGFULLWINDOWS,TRUE,NULL,0);
	::SetCapture(this->m_hWnd);
	xx = GET_X_LPARAM(lParam);
	yy = GET_Y_LPARAM(lParam);
	m_bMouseDown = true;
	return 0;
}

LRESULT CFloatingWnd::OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
	//::MessageBox(NULL,_T("Simon Woo Release!!"),_T("HELLO"),0);
	::ReleaseCapture();
	m_bMouseDown = false;
	return 0;
}

//NOTE: WM_MOUSE的消息会一直触发!!
LRESULT CFloatingWnd::OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
	if (m_bMouseDown){
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		RECT wndRect;
		GetWindowRect(&wndRect);
		RECT newRect = {wndRect.left+xPos-xx, wndRect.top+yPos-yy, wndRect.right+xPos-xx, wndRect.bottom+yPos-yy};
		MoveWindow(&newRect, true);
	}
	return 0;
}

LRESULT CFloatingWnd::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
	CPaintDC hdc(m_hWnd);
	CDC dcMem;
	dcMem.CreateCompatibleDC(hdc);
	hdc.SaveDC();
	dcMem.SaveDC();
	//select and draw
	dcMem.SelectBitmap(m_bmpBackground);
	hdc.BitBlt(0,0,m_size.cx,m_size.cy,dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(-1);
	hdc.RestoreDC(-1);
	return 0;
}

LRESULT CFloatingWnd::OnLButtonDBlick(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
	return 0;
}

LRESULT CFloatingWnd::OnRButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
	if (m_FloatingWndmenu.m_hMenu != NULL)
		return 0;
	SetFocus();
	RECT rc;
	GetClientRect(&rc);
	MapWindowPoints(HWND_DESKTOP, (POINT *)&rc, 2);
	// Cursor's horizontal and vertical position
	CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); 
	// Convert client coordinates to screen coordinates 
	ClientToScreen(&pt);
	// Load the menu resource 
	if (!m_FloatingWndmenu.LoadMenu(IDR_POPUP))
	{
		ATLTRACE(_T("Menu resource was not loaded successfully!\n"));
		return 0;
	}
	// TrackPopupMenu cannot display the menu bar so get 
	// a handle to the first shortcut menu. 
 	CMenuHandle menuPopup = m_FloatingWndmenu.GetSubMenu(0);
	if (menuPopup.m_hMenu == NULL)
	{
		ATLTRACE(_T("Submenu was not retrieved successfully!\n"));
		return 0;
	}
	// Display the shortcut menu
 	//if (!menuPopup.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON, rc.left ,rc.bottom, m_hWnd))
	if (!menuPopup.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON, pt.x ,pt.y, m_hWnd))
	{
		ATLTRACE(_T("Context menu was not displayed successfully!\n"));
		return 0;
	}
	// Destroy the menu and free any memory that the menu occupies
	menuPopup.DestroyMenu();
	m_FloatingWndmenu.DestroyMenu();
	m_FloatingWndmenu.m_hMenu = NULL;
	return 0;
}

LRESULT CFloatingWnd::OnID_A_EXIT(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
	::SendMessage(Parent_hWnd,ID_A_EXIT,0,0);
	//DestroyWindow();
	return 0;
}

// 增加窗口弹出的代码开始
///////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CFloatingWnd::OnInitMenuPopup(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	int nPos;
	UINT uId;
	// Call CCoolContextMenu's implementation
	CCoolContextMenu<CFloatingWnd>::OnInitMenuPopup(uMsg, wParam, lParam, bHandled);
	// Set the cursor shape to an arrow
	m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
	ATLASSERT(m_hCursor);
	::SetCursor(m_hCursor);
	CMenuHandle menuPopup = (HMENU)wParam;
	ATLASSERT(menuPopup.m_hMenu != NULL);
	for (nPos = 0; nPos < menuPopup.GetMenuItemCount(); nPos++)
	{ 
		uId = GetMenuItemID(menuPopup, nPos);
		switch (uId) 
		{ 
		case ID_EDIT_SELECT_ALL:
			EnableMenuItem(menuPopup, uId, (GetWindowTextLength() > 0) ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_GRAYED);
			break;
		case ID_EDIT_CUT: 
		case ID_EDIT_COPY: 
		case ID_EDIT_PASTE: 
			EnableMenuItem(menuPopup, uId, IsClipboardFormatAvailable(CF_TEXT) ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_GRAYED);
			break;
		default:
			break;
		}
	}
	return 0;
}

void CFloatingWnd::AssociateImage(CMenuItemInfo& mii, coolMenuItemData * pMI)
{
	switch (mii.wID)
	{
	case ID_FILE_OPEN:
		pMI->iImage = 0;
		break;
	case ID_A_EXIT:
		pMI->iImage = 1;
		break;
	case ID_B_START:
		pMI->iImage = 2;
		break;
	case ID_B_SUSPENDED:
		pMI->iImage = 3;
		break;
	case ID_B_DELETE:
		pMI->iImage = 4;
		break;
	case ID_B_CHECKDETAILS:
		pMI->iImage = 5;
		break;
	default:
		pMI->iImage = -1;
		break;
	}
}
// 增加窗口弹出的代码结束
///////////////////////////////////////////////////////////////////////////////////////////////////////