#include "ToolBarSELogo.h"
#include "RegIterator.h"
#include "6bees_util.h"
#include "6bees_const.h"
#include <imgcap_setting.h>

using namespace _6bees_util;

void SetDefaultSE(const wchar_t* s){
  CRegKey crk;
  long rk2=crk.Open(HKEY_LOCAL_MACHINE,_6bees_const::kregistryname,KEY_WRITE);
  if (rk2==ERROR_SUCCESS){
    rk2=crk.SetStringValue(_T("DefaultSE"),s);
  }
}

const USHORT offset_y = 5;
const USHORT offset_x = 4;

CToolBarSELogo::CToolBarSELogo():imgae_index(0),m_count(0){}

CToolBarSELogo::~CToolBarSELogo(){
  m_ImageList.Destroy();
  if (IsWindow()){
    DestroyWindow();
  }
}

LRESULT CToolBarSELogo::OnCreate(UINT,WPARAM,LPARAM,BOOL&){
  m_bmp = (HBITMAP)c6beeres::R().GetImg(SELOGO_BKGND,IMAGE_BITMAP,30,24);
  SetMyImageList((HBITMAP)c6beeres::R().GetImg(IDB_SEARCHMENU,IMAGE_BITMAP,16*5),
    m_ImageList,MASKGREY,16,16,5,GetSysColor(COLOR_3DFACE));

  CRegKey crk;
  long rk2=crk.Open(HKEY_LOCAL_MACHINE,_6bees_const::kregistryname,KEY_READ);
  if (rk2==ERROR_SUCCESS){
    wchar_t pszValue[100];
    ULONG len=100;
    rk2=crk.QueryStringValue(_T("DefaultSE"),pszValue,&len);
    if (wcscmp(pszValue,_T("Baidu"))==0){
      imgae_index=1;
    }else if (wcscmp(pszValue,_T("Yahoo"))==0){
      imgae_index=2;
    }else{
      imgae_index=0;
    }
  }
  return 0;
}

LRESULT CToolBarSELogo::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  CPaintDC mydc(m_hWnd);
  CDCHandle dc = mydc.m_hDC;
  CDC dcMem;
  dcMem.CreateCompatibleDC(dc);
  dc.SaveDC();
  dcMem.SaveDC();

  dcMem.SelectBitmap(m_bmp);
  dc.BitBlt(0,0,30,_6bees_const::ktbbuttonheight,dcMem,0,0,SRCCOPY);
  ::ImageList_Draw(m_ImageList, imgae_index, dc.m_hDC, offset_y, offset_x, ILD_TRANSPARENT);
  if(m_count==0){
    //initial status
  }else{
    // when mouse moves, m_count is not equal to 0,then blend the inner area, then it looks like animation effect
    static BLENDFUNCTION blendFunction={AC_SRC_OVER,0,50,AC_SRC_ALPHA};
    dc.AlphaBlend(offset_y,offset_x,16,16,dcMem,offset_y,offset_x,16,16,blendFunction);
  }  
  dcMem.RestoreDC(-1);
  dc.RestoreDC(-1);
  mydc.RestoreDC(-1);
  return 0;
}


LRESULT CToolBarSELogo::OnSetCursor(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  static HCURSOR hcur = LoadCursor(NULL,/*IDC_SIZEALL*/IDC_HAND);
  if(m_count<1){
    ++m_count;
    RECT rectcombo;
    this->GetClientRect(&rectcombo);
    InvalidateRect(&rectcombo,true);
  }  
  TRACKMOUSEEVENT  tme;
  tme.cbSize =sizeof(TRACKMOUSEEVENT);
  tme.dwFlags =TME_LEAVE;
  tme.dwHoverTime=0;
  tme.hwndTrack =m_hWnd;
  ::TrackMouseEvent(&tme);
  if (hcur!=NULL){
    SetCursor(hcur);
    return TRUE;
  }else{
    SetMsgHandled(false);
    return false;
  }
}


LRESULT CToolBarSELogo::OnMouseOut (UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  m_count = 0;
  RECT rectcombo;
  this->GetClientRect(&rectcombo);
  InvalidateRect(&rectcombo,true);
  return 0;
}

LRESULT CToolBarSELogo::OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  SetFocus();
  RECT rc={};
  GetClientRect(&rc);
  MapWindowPoints(HWND_DESKTOP, (POINT *)&rc, 2);
  // Cursor's horizontal and vertical position
  ////CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); 
  ////ClientToScreen(&pt);// Convert client coordinates to screen coordinates
  // Load the menu resource 
  /// @todo : Change to a uniform resource loading function!!!
  CMenu m_SELogomenu = c6beeres::R().GetMenu(IDR_SEARCHMENU);
  // TrackPopupMenu cannot display the menu bar so get a handle to the first shortcut menu. 
  CMenuHandle menuPopup = m_SELogomenu.GetSubMenu(0);
  if (!menuPopup.m_hMenu){return 0;}
  // Display the shortcut menu
  menuPopup.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON, rc.left ,rc.bottom, m_hWnd);
  // Destroy the menu and free any memory that the menu occupies
  menuPopup.DestroyMenu();
  m_SELogomenu.DestroyMenu();
  return 0;
}

LRESULT CToolBarSELogo::OnID_SE_GG   (WORD,WORD,HWND,BOOL&){
  imgae_index=0;
  ::SendMessage(this->GetParent(),_6bees_const::kWM_SE_GG,0,0);
  SetDefaultSE(_T("Google"));
  return 0;
}

LRESULT CToolBarSELogo::OnID_SE_BAIDU(WORD,WORD,HWND,BOOL&){
  imgae_index=1;
  ::SendMessage(this->GetParent(),_6bees_const::kWM_SE_BAIDU,0,0);
  SetDefaultSE(_T("Baidu"));
  return 0;
}

LRESULT CToolBarSELogo::OnID_SE_YAHOO(WORD,WORD,HWND,BOOL&){
  imgae_index=2;
  ::SendMessage(this->GetParent(),_6bees_const::kWM_SE_YAHOO,0,0);
  SetDefaultSE(_T("Yahoo"));
  return 0;
}

LRESULT CToolBarSELogo::OnID_SE_CLEAR(WORD,WORD,HWND,BOOL&){
  Clear6BeeSearchRegistryKey();
  return 0;
}

// This has been disabled temperarily.
LRESULT CToolBarSELogo::OnID_X_SETTING(WORD,WORD,HWND,BOOL&){
  ::SendMessage(this->GetParent(),_6bees_const::kWM_SE_SETTING,0,0);
  return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CToolBarSELogo::OnInitMenuPopup(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
  // Call CCoolContextMenu's implementation
  CCoolContextMenu<CToolBarSELogo>::OnInitMenuPopup(uMsg, wParam, lParam, bHandled);
  // Set the cursor shape to an arrow
  m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
  ::SetCursor(m_hCursor);
  return 0;
}

void CToolBarSELogo::AssociateImage(CMenuItemInfo& mii, coolMenuItemData * pMI)
{
  switch (mii.wID)
  {
  case ID_SE_GG:
    pMI->iImage = 0;
    break;
  case ID_SE_BAIDU:
    pMI->iImage = 1;
    break;
  case ID_SE_YAHOO:
    pMI->iImage = 2;
    break;
  case ID_SE_CLEAR:
    pMI->iImage = 3;
    break;
  case ID_X_SETTING:
    pMI->iImage = 4;
    break;
  default:
    pMI->iImage = -1;
    break;
  }
}
