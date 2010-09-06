#ifndef __MY_MULTIPANE_H__
#define __MY_MULTIPANE_H__

//#include "stdafx.h"
//#include <atlctrlx.h>

class C6BeeMultiPaneStatusBar:
	public CMultiPaneStatusBarCtrlImpl<C6BeeMultiPaneStatusBar>
{	
public:
	DECLARE_FRAME_WND_CLASS(_T("C6BeeMultiPaneStatusBar"), IDR_MAINFRAME)

	C6BeeMultiPaneStatusBar(){}

	virtual ~C6BeeMultiPaneStatusBar(){
		if (IsWindow()){
			DestroyWindow();
		}
	}

public:
	BEGIN_MSG_MAP(C6BeeMultiPaneStatusBar)
		//MESSAGE_HANDLER(WM_CREATE, OnCreate)
		//CHAIN_MSG_MAP(CUpdateUI<C6BeeMultiPaneStatusBar>)
		//CHAIN_MSG_MAP(CFrameWindowImpl<C6BeeMultiPaneStatusBar>)
	END_MSG_MAP()

//protected:
	//CMultiPaneStatusBarCtrl m_wndStatusBar;


public:
	
	//LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	/*
	{
		//m_hWndStatusBar = m_wndStatusBar.Create(*this);
		m_hWndStatusBar=m_wndStatusBar.Create(m_hWnd,_T(""),WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|SBT_TOOLTIPS);
		int anPanes[] = { ID_DEFAULT_PANE, IDPANE_STATUS, IDPANE_CAPS_INDICATOR };
		m_wndStatusBar.SetPanes(anPanes,3,true);
		//UISetText(0, _T("Running"));
		return 0;
	}
	*/
};

#endif//__MY_NOTE_BHO_H__