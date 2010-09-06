#ifndef __UUTOOLBAR_TOOLBARSELOGO__
#define __UUTOOLBAR_TOOLBARSELOGO__

#include <6BeeContextMenu.h>

#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"

class CToolBarCOM;

class CToolBarSELogo:
  public CWindowImpl< CToolBarSELogo,CButton >,
  public CCoolContextMenu< CToolBarSELogo >
{
public:
  DECLARE_WND_SUPERCLASS(TEXT("TOOLBARSELOGO"), TEXT("BUTTON"))
  BEGIN_MSG_MAP(CToolBarSELogo)
    MESSAGE_HANDLER(WM_CREATE,OnCreate)
    MESSAGE_HANDLER(WM_PAINT,OnPaint)
    MESSAGE_HANDLER(WM_SETCURSOR,  OnSetCursor  )
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseOut   )
    MESSAGE_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
    COMMAND_ID_HANDLER(ID_SE_GG,    OnID_SE_GG)
    COMMAND_ID_HANDLER(ID_SE_BAIDU, OnID_SE_BAIDU)
    COMMAND_ID_HANDLER(ID_SE_YAHOO, OnID_SE_YAHOO)
    COMMAND_ID_HANDLER(ID_SE_CLEAR, OnID_SE_CLEAR)
    COMMAND_ID_HANDLER(ID_X_SETTING, OnID_X_SETTING)
    MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)
    CHAIN_MSG_MAP(CCoolContextMenu<CToolBarSELogo>)
  END_MSG_MAP()
  
  LRESULT OnPaint(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnCreate   (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnSetCursor  (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnMouseOut   (UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnLButtonDown(UINT,WPARAM,LPARAM,BOOL&);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LRESULT OnInitMenuPopup(UINT,WPARAM,LPARAM,BOOL&);
    void AssociateImage(CMenuItemInfo& mii, coolMenuItemData * pMI);// Associates an image with a menu item
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  LRESULT OnID_SE_GG   (WORD,WORD,HWND,BOOL&);
  LRESULT OnID_SE_BAIDU(WORD,WORD,HWND,BOOL&);
  LRESULT OnID_SE_YAHOO(WORD,WORD,HWND,BOOL&);
  LRESULT OnID_SE_CLEAR(WORD,WORD,HWND,BOOL&);
  LRESULT OnID_X_SETTING(WORD,WORD,HWND,BOOL&);
  CToolBarSELogo();
  virtual ~CToolBarSELogo();

protected:
  CBitmap m_bmp;
  HCURSOR m_hCursor;
  BOOL m_bOver;
private:
  short m_count;
  int imgae_index;
};

#endif