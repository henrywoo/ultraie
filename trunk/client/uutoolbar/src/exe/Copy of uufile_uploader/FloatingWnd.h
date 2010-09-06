#pragma once
#include "resource.h"
#include "CoolContextMenu.h"
#include "FTPClient.h"

#include <6BeeResDialog.hpp>

class CFloatingWnd:
	public C6BeeResDialogImpl<CFloatingWnd>,
	public CCoolContextMenu< CFloatingWnd >,
	public nsFTP::CFTPClient::CNotification
{
public:
    enum { IDD = IDD_FLOATINGWND };
	CFloatingWnd(HWND m_hWnd){
		Parent_hWnd = m_hWnd ;
	};

	BEGIN_MSG_MAP(CFloatingWnd)
		MESSAGE_HANDLER(WM_INITDIALOG,  OnInitDialog)
		
		MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)

		MESSAGE_HANDLER(WM_LBUTTONDOWN,  OnLButtonDown)
		MESSAGE_HANDLER(WM_RBUTTONDOWN,  OnRButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK,OnLButtonDBlick)
		MESSAGE_HANDLER(WM_LBUTTONUP,    OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE,    OnMouseMove)
		MESSAGE_HANDLER(WM_PAINT,        OnPaint)

		COMMAND_ID_HANDLER(IDCANCEL,     OnCancel)
		//COMMAND_ID_HANDLER(ID_A_EXIT,    OnID_A_EXIT)

		//CHAIN_MSG_MAP(CCoolContextMenu<CFloatingWnd>)
		//CHAIN_MSG_MAP_ALT(COwnerDraw<CFloatingWnd>,1)
        DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	//typedef BOOL (WINAPI *MYFUNC2)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);
	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);

	LRESULT OnInitDialog (UINT,WPARAM,LPARAM,BOOL&);
	LRESULT OnLButtonUp  (UINT,WPARAM,LPARAM,BOOL&);
	LRESULT OnLButtonDBlick(UINT,WPARAM,LPARAM,BOOL&);
	
	LRESULT OnLButtonDown(UINT,WPARAM,LPARAM,BOOL&);
	LRESULT OnRButtonDown(UINT,WPARAM,LPARAM,BOOL&);
	LRESULT OnPaint      (UINT,WPARAM,LPARAM,BOOL&);
	LRESULT OnMouseMove  (UINT,WPARAM,LPARAM,BOOL&);
	
	LRESULT OnCancel(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	LRESULT OnID_A_EXIT(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LRESULT OnInitMenuPopup(UINT,WPARAM,LPARAM,BOOL&);
    void AssociateImage(CMenuItemInfo& mii, coolMenuItemData * pMI);// Associates an image with a menu item
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	//MYFUNC2 UpdateLayeredWindow;
	MYFUNC SetLayeredWindowAttributes;
	bool m_bMouseDown;
	int xx;
	int yy;
	CBitmap m_bmpBackground;
	SIZE m_size;
	BLENDFUNCTION m_Blend;

	CMenu m_FloatingWndmenu;
	HCURSOR m_hCursor;
	HWND Parent_hWnd;
};