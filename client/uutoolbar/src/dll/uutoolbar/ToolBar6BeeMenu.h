#ifndef __UUTOOLBAR_TOOLBAR6BEEMENU__
#define __UUTOOLBAR_TOOLBAR6BEEMENU__

#include "resource.h"
#include <6BeeContextMenu.h>

class C6BeeMenu:
  public CWindowImpl< C6BeeMenu,CWindow >,
  public CMenu,
  public CCoolContextMenu< C6BeeMenu >
{
public:
  BEGIN_MSG_MAP(C6BeeMenu)
    CHAIN_MSG_MAP(CCoolContextMenu< C6BeeMenu >)
  END_MSG_MAP()

  void AssociateImage(CMenuItemInfo& mii, coolMenuItemData * pMI){
        switch (mii.wID){
case ID_X_MY_6BEE_SPACE:
  pMI->iImage = 4;
  break;
case ID_X_CONFIG:
  pMI->iImage = 30;
  break;
case ID_Y_UPLOADFOLDER:
  pMI->iImage = 18;
  break;
  /*case ID_Y_UPLOADFILES:
  pMI->iImage = 32;
  break;*/
case ID_Z_AREACAP:
  pMI->iImage = 33;
  break;
case ID_Z_WINDOWCAP:
  pMI->iImage = 34;
  break;
case ID_Z_WEBELEMENTSCAPTURE:
  pMI->iImage = 35;
  break;
case ID_Z_WHOLEWEBPAGECAPTURE:
  pMI->iImage = 36;
  break;
case ID_M_SAVESELECTEDTEXTANDIMAGES:
  pMI->iImage = 38;
  break;
case ID_M_SAVEENTIREWEBPAGE:
  pMI->iImage = 39;
  break;
case ID_M_SAVEWEBELEMENTASHTML:
  pMI->iImage = 40;
  break;
case ID_M_SAVEWEBELEMENTASTEXT:
  pMI->iImage = 41;
  break;
case ID_Z_CAPTURESETTING:
  pMI->iImage = 18;
  break;
case ID_O_SAVETHISPAGE:
  pMI->iImage = 15;
  break;
case ID_O_SYNCHRONIZE_UP:
  pMI->iImage = 51;
  break;
case ID_O_SYNCHRONIZE_DOWN:
  pMI->iImage = 50;
  break;
case ID_O_MANAGEOLBM:
  pMI->iImage = 53;
  break;
default:
  pMI->iImage = -1;
  break;
        }
    }

  LRESULT OnInitMenuPopup(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
        CCoolContextMenu<C6BeeMenu>::OnInitMenuPopup(uMsg, wParam, lParam, bHandled);
    HCURSOR m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
    ::SetCursor(m_hCursor);
    ::DestroyCursor(m_hCursor);
    /*
    int nPos;
    UINT uId;
    CMenuHandle menuPopup = (HMENU)wParam;
        for (nPos = 0; nPos < menuPopup.GetMenuItemCount(); nPos++)
    { 
      uId = ::GetMenuItemID(menuPopup, nPos);
      switch (uId) 
      { 
      case ID_EDIT_PASTE: 
        ::EnableMenuItem(menuPopup, uId, IsClipboardFormatAvailable(CF_TEXT) ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_GRAYED);
        break;
      default:
        break;
      }
    }
    */
        return 0;
    }

//private:
  //HCURSOR m_hCursor;
};

#endif