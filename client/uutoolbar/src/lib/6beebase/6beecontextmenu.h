#ifndef __UUTOOLBAR__COOLCONTEXTMENU__
#define __UUTOOLBAR__COOLCONTEXTMENU__

/////////////////////////////////////////////////////////////////
// CoolContextMenu.h - implementation of the CCoolContextMenu class
// From : http://www.codeproject.com/wtl/WTLOwnerDrawCtxtMenu.asp
/////////////////////////////////////////////////////////////////
//ATL
#include <atlbase.h>
//WTL
#include <atlapp.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include "6bees_gdi.h"

using namespace _6bees_gdi;

static const int MAX_MENU_ITEM_TEXT_LENGTH=100;
static const int IMGPADDING=6;
static const int TEXTPADDING=8;

struct coolMenuItemData{// menu item data
  LPTSTR lpstrText;
  UINT fType;
  UINT fState;
  int iImage;
};

template <class T>
class CCoolContextMenu
{
  private:
    SIZE m_szBitmap;
    SIZE m_szButton;
    CFont m_fontMenu;// used internally, only to measure text
    int m_cxExtraSpacing;
    bool m_bFlatMenus;
    COLORREF m_clrMask;
    CSimpleStack<HMENU> m_stackMenuHandle;

  public:
    CImageList m_ImageList;
    bool popped;

  protected:
    void coolMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct){
      coolMenuItemData* pmd = (coolMenuItemData*)lpMeasureItemStruct->itemData;
      if (pmd->fType & MFT_SEPARATOR){
        // separator - use half system height and zero width
        lpMeasureItemStruct->itemHeight = ::GetSystemMetrics(SM_CYMENU) / 2;
        lpMeasureItemStruct->itemWidth  = 0;
      }else{
        // Compute size of text - use DrawText with DT_CALCRECT
        CWindowDC dc(NULL);
        CFont fontBold;
        HFONT hOldFont = NULL;
        if (pmd->fState & MFS_DEFAULT){ // Need bold version of font
          LOGFONT lf={};
          m_fontMenu.GetLogFont(lf);
          lf.lfWeight += 200;
          fontBold.CreateFontIndirect(&lf);
          ATLASSERT(fontBold.m_hFont != NULL);
          hOldFont = dc.SelectFont(fontBold);
          fontBold.DeleteObject();
        }else{
          hOldFont = dc.SelectFont(m_fontMenu);
        }

        RECT rcText={};
        dc.DrawText(pmd->lpstrText, -1, &rcText,
            DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_CALCRECT);
        int cx = rcText.right - rcText.left;
        dc.SelectFont(hOldFont);
        LOGFONT lf = {};
        m_fontMenu.GetLogFont(lf);
        int cy = lf.lfHeight;
        if (cy < 0)
          cy = -cy;
        static const int cyMargin = 10;
        cy += cyMargin;

        lpMeasureItemStruct->itemHeight = cy;

        // Width is width of text plus some
        cx += 4;      // L/R margin for readability
        cx += 1;      // space between button and menu text
        cx += 2 * m_szButton.cx;  // button width
        cx += m_cxExtraSpacing;   // extra between item text and accelerator keys

        // Windows adds 1 to returned value
        cx -= ::GetSystemMetrics(SM_CXMENUCHECK) - 1;
        lpMeasureItemStruct->itemWidth = cx;   // we are done
      }
    }

    void DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct){
      coolMenuItemData* pmd = (coolMenuItemData*)lpDrawItemStruct->itemData;
      CDCHandle dc   = lpDrawItemStruct->hDC;
      RECT& rcItem   = lpDrawItemStruct->rcItem;
      LPCRECT pRect  = &rcItem;
      BOOL bDisabled = lpDrawItemStruct->itemState & ODS_GRAYED;
      BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
      BOOL bChecked  = lpDrawItemStruct->itemState & ODS_CHECKED;
      COLORREF crBackImg = CLR_NONE;
      CDCHandle* pDC = &dc;

      if (bSelected && !bDisabled){
        COLORREF crHighLight = ::GetSysColor(COLOR_HIGHLIGHT);
        CPenDC pen(*pDC, crHighLight);

        COLORREF c1 = HLS_TRANSFORM(::GetSysColor (COLOR_3DFACE), +73, 0);
        COLORREF c2 = HLS_TRANSFORM (crHighLight, +70, -57);
        COLORREF crBackImg = (bDisabled?c1:c2);
        CBrushDC brush(*pDC, crBackImg);
        pDC->Rectangle (pRect);
      }else{
        // Draw the menu item background
        CRect rc(pRect);
        //ǿ���ñ����߶�Ϊ21
        /*
           if ((pRect->bottom - pRect->top) < 20){
           rc.bottom = rc.top + 21;
           }
           */  
        //��������21,�����10,Ϊʲô?
        rc.right = m_szBitmap.cx + IMGPADDING;
        pDC->FillSolidRect(rc, crBackImg = HLS_TRANSFORM(
              ::GetSysColor(COLOR_3DFACE), 20, 0));
        rc.left = rc.right;
        rc.right = pRect->right;
        pDC->FillSolidRect(rc, HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE), 75, 0));
      }

      // Menu item is a separator
      if (pmd->fType & MFT_SEPARATOR){
        rcItem.top += (rcItem.bottom - rcItem.top) / 2;   // vertical center
        dc.DrawEdge(&rcItem, EDGE_ETCHED, BF_TOP);  // draw separator line
      }else{   
        // Draw the text
        CRect rc (pRect);
        /*
           if ((pRect->bottom - pRect->top) < 20){
           rc.bottom = rc.top + 21;
           }
           */
        ATL::CString sCaption = pmd->lpstrText;
        int nTab = sCaption.Find('\t');
        if (nTab >= 0){
          sCaption = sCaption.Left (nTab);
        }
        COLORREF c1=HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE), -18, 0);
        pDC->SetTextColor(bDisabled ? c1 : GetSysColor(COLOR_MENUTEXT));
        pDC->SetBkMode(TRANSPARENT);

        CBoldDC bold(*pDC, (lpDrawItemStruct->itemState & ODS_DEFAULT) != 0);
        rc.left = m_szBitmap.cx + IMGPADDING + TEXTPADDING;//д��
        pDC->DrawText(sCaption, sCaption.GetLength(), rc,
            DT_SINGLELINE|DT_VCENTER|DT_LEFT);
        if (nTab >= 0){  
          rc.right -= TEXTPADDING + 4;
          pDC->DrawText(pmd->lpstrText + nTab + 1,
              (int)_tcslen(pmd->lpstrText + nTab + 1),
              rc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
        }

        // Draw background and border around the check mark
        if (bChecked){
          COLORREF crHighLight = ::GetSysColor(COLOR_HIGHLIGHT);
          CPenDC pen(*pDC, crHighLight);

          COLORREF c1 = HLS_TRANSFORM(::GetSysColor (COLOR_3DFACE), +73, 0);
          COLORREF c2 = HLS_TRANSFORM(crHighLight, +50, -50);
          COLORREF c3 = HLS_TRANSFORM(crHighLight, +70, -57);
          COLORREF c4 = (bSelected ? c2 : c3);
          COLORREF crBackImg = (bDisabled?c1:c4);
          CBrushDC brush(*pDC, crBackImg);
          pDC->Rectangle(CRect(pRect->left + 1, pRect->top + 1,
                pRect->left + m_szButton.cx - 2, pRect->bottom - 1));
        }

        if (m_ImageList != NULL && pmd->iImage >= 0){
          bool bOver = !bDisabled && bSelected;
          if (bDisabled || (bSelected && !bChecked)){
            HICON hIcon = ImageList_ExtractIcon(NULL, m_ImageList, pmd->iImage);
            CBrush brush;
            brush.CreateSolidBrush(
                bOver?
                HLS_TRANSFORM(::GetSysColor(COLOR_HIGHLIGHT),+50,-66):
                HLS_TRANSFORM(::GetSysColor(COLOR_3DFACE),-27,0)
                );
            pDC->DrawState(CPoint(pRect->left + (bOver ? 4 : 3), rc.top + (bOver ? 5 : 4)),
                CSize(m_szBitmap.cx, m_szBitmap.cx), hIcon, DSS_MONO, brush);
            DestroyIcon(hIcon);
          }
          if (!bDisabled){
            ::ImageList_Draw(m_ImageList,
                pmd->iImage,
                pDC->m_hDC,
                pRect->left + ((bSelected && !bChecked) ? 2 : 3),
                rc.top + ((bSelected && !bChecked) ? 3 : 4),
                ILD_TRANSPARENT);
          }
        }else if(bChecked){ // Draw the check mark
          rc.left  = pRect->left + 5;
          rc.right = rc.left + m_szBitmap.cx + IMGPADDING;
          pDC->SetBkColor(crBackImg);
          HBITMAP hBmp = LoadBitmap(NULL, MAKEINTRESOURCE(OBM_CHECK));
          BOOL bRet = pDC->DrawState(CPoint(rc.left,rc.top + 3),
              CSize(rc.Size()), hBmp, DSS_NORMAL, (HBRUSH)NULL);
          DeleteObject(hBmp);
        }
      }
    }

    LRESULT InitMenuPopupHandler(UINT uMsg,WPARAM wParam,LPARAM lParam,
        BOOL& bHandled)
    { // System menu, do nothing
      if ((BOOL)HIWORD(lParam)){
        bHandled = FALSE;
        return 1;
      }
      CMenuHandle menuPopup = (HMENU)wParam;

      BOOL bRet = FALSE;
      popped = true;
      ::SetCursor(LoadCursor(NULL,IDC_ARROW));

      wchar_t szString[MAX_MENU_ITEM_TEXT_LENGTH]={};
      int menucount = menuPopup.GetMenuItemCount();
      for (int i = 0; i < menucount; i++){
        CMenuItemInfo mii;
        mii.cch = MAX_MENU_ITEM_TEXT_LENGTH;
        mii.fMask = MIIM_CHECKMARKS|MIIM_DATA|MIIM_ID|
          MIIM_STATE|MIIM_SUBMENU| MIIM_TYPE;
        mii.dwTypeData = szString;
        bRet = menuPopup.GetMenuItemInfo(i, TRUE, &mii);
        ATLASSERT(bRet);

        if (!(mii.fType & MFT_OWNERDRAW)){// not already an ownerdraw item
          coolMenuItemData* _pMI = new coolMenuItemData;
          if (_pMI){
            // Make this menu item an owner-drawn
            mii.fType |= MFT_OWNERDRAW;
            _pMI->fType = mii.fType;
            _pMI->fState = mii.fState;

            // Associate an image with a menu item
            _pMI->iImage = -1;
            static_cast<T*>(this)->AssociateImage(mii, _pMI);

            bool ignore = false;
            static_cast<T*>(this)->DynamicChangeMenuItem(mii,_pMI,szString,ignore);

            if (ignore){
              menuPopup.DeleteMenu(i,MF_BYPOSITION);
            }else{
              _pMI->lpstrText = new TCHAR[lstrlen(szString) + 1];
              if (_pMI->lpstrText != NULL)
                lstrcpy(_pMI->lpstrText, szString);
              mii.dwItemData = (ULONG_PTR)_pMI;
              bRet = menuPopup.SetMenuItemInfo(i, TRUE, &mii);
            }
          }
        }
      }

      // Add it to the list
      m_stackMenuHandle.Push(menuPopup.m_hMenu);

      return 0;
    }

    //OnMenuSelect��ʱ�����...
    LRESULT MenuSelectHandler(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
      // Check if a menu is closing, do a cleanup // Menu closing
      if (HIWORD(wParam) == 0xFFFF && lParam == NULL){
        // Restore the menu items to the previous state for all menus that were converted
        popped = false;
        HMENU hMenu = NULL;
        while ((hMenu = m_stackMenuHandle.Pop()) != NULL){
          CMenuHandle menuPopup = hMenu;
          // Restore state and delete menu item data
          BOOL bRet = FALSE;
          int menucount = menuPopup.GetMenuItemCount();
          for (int i = 0; i < menucount; i++){
            CMenuItemInfo mii;
            mii.fMask = MIIM_DATA | MIIM_TYPE;
            bRet = menuPopup.GetMenuItemInfo(i, TRUE, &mii);
            coolMenuItemData * pMI = (coolMenuItemData*)mii.dwItemData;
            if (pMI != NULL){
              mii.fMask = MIIM_DATA | MIIM_TYPE | MIIM_STATE;
              mii.fType = pMI->fType;
              mii.dwTypeData = pMI->lpstrText;
              mii.cch = lstrlen(pMI->lpstrText);
              mii.dwItemData = NULL;

              bRet = menuPopup.SetMenuItemInfo(i, TRUE, &mii);
              ATLASSERT(bRet);

              delete [] pMI->lpstrText;
              delete pMI;
            }
          }
        }
      }

      bHandled = FALSE;
      return 1;
    }

    LRESULT MeasureItemHandler(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
      LPMEASUREITEMSTRUCT lpMeasureItemStruct = (LPMEASUREITEMSTRUCT)lParam;
      coolMenuItemData * pmd = (coolMenuItemData*)lpMeasureItemStruct->itemData;

      if (lpMeasureItemStruct->CtlType == ODT_MENU && pmd != NULL)
        coolMeasureItem(lpMeasureItemStruct);
      else
        bHandled = FALSE;

      return (LRESULT)TRUE;
    }

    LRESULT DrawItemHandler(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
      LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;

      coolMenuItemData * pMI = (coolMenuItemData*)lpDrawItemStruct->itemData;
      if (lpDrawItemStruct->CtlType == ODT_MENU && pMI != NULL)// only owner-drawn menu item
        DrawItem(lpDrawItemStruct);
      else
        bHandled = FALSE;

      return TRUE;
    }

  public:
    CCoolContextMenu():
      popped(false),m_ImageList(NULL),m_clrMask(MASKGREY),m_cxExtraSpacing(0)
  {
    m_szBitmap.cx = 16;
    m_szBitmap.cy = 15;
    m_szButton.cx = m_szBitmap.cx + 6;
    m_szButton.cy = m_szBitmap.cy + 6;
    HFONT hf = ::CreateFont(16,0,0,0,FW_THIN,false,FALSE,FALSE,DEFAULT_CHARSET,
      OUT_STRING_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));
    m_fontMenu.Attach(hf);
  }

    ~CCoolContextMenu(){
      m_ImageList.Destroy();
      if (!m_fontMenu.IsNull()){
        m_fontMenu.DeleteObject();
      }
    }

    // Note: do not forget to put CHAIN_MSG_MAP in your message map.
    BEGIN_MSG_MAP(CCoolContextMenu)
      MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)
      MESSAGE_HANDLER(WM_MENUSELECT,  OnMenuSelect   )
      MESSAGE_HANDLER(WM_DRAWITEM,  OnDrawItem   )
      MESSAGE_HANDLER(WM_MEASUREITEM,   OnMeasureItem  )
    END_MSG_MAP()

    // Mostly copied from atlctrlw.h
    void GetSystemSettings(){
      // 1. Set up the font
      NONCLIENTMETRICS info = {};
      info.cbSize = sizeof(info);
      BOOL bRet = ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
      HWND handle = static_cast<T*>(this)->m_hWnd;
      if (handle==NULL){
        return;
      }
      if (bRet){
        LOGFONT logfont = {};
        if (m_fontMenu.m_hFont)
          m_fontMenu.GetLogFont(logfont);

        if (logfont.lfHeight != info.lfMenuFont.lfHeight ||
          logfont.lfWidth != info.lfMenuFont.lfWidth ||
          logfont.lfEscapement != info.lfMenuFont.lfEscapement ||
          logfont.lfOrientation != info.lfMenuFont.lfOrientation ||
          logfont.lfWeight != info.lfMenuFont.lfWeight ||
          logfont.lfItalic != info.lfMenuFont.lfItalic ||
          logfont.lfUnderline != info.lfMenuFont.lfUnderline ||
          logfont.lfStrikeOut != info.lfMenuFont.lfStrikeOut ||
          logfont.lfCharSet != info.lfMenuFont.lfCharSet ||
          logfont.lfOutPrecision != info.lfMenuFont.lfOutPrecision ||
          logfont.lfClipPrecision != info.lfMenuFont.lfClipPrecision ||
          logfont.lfQuality != info.lfMenuFont.lfQuality ||
          logfont.lfPitchAndFamily != info.lfMenuFont.lfPitchAndFamily ||
          lstrcmp(logfont.lfFaceName, info.lfMenuFont.lfFaceName) != 0)
        {
          HFONT hFontMenu = ::CreateFontIndirect(&info.lfMenuFont);
          if (hFontMenu){
            if (m_fontMenu.m_hFont)
              m_fontMenu.DeleteObject();
            m_fontMenu.Attach(hFontMenu);
            static_cast<T*>(this)->SetFont(m_fontMenu);
          }
        }
      }
      // 2.Check if we need extra spacing for menu item text
      CWindowDC dc(handle);
      HFONT hFontOld = dc.SelectFont(m_fontMenu);
      RECT rcText = {};
      dc.DrawText(_T("\t"), -1, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_CALCRECT);
      if(rcText.right > rcText.left && ((rcText.right-rcText.left)<4)){
        ::SetRectEmpty(&rcText);
        dc.DrawText(_T("x"), -1, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_CALCRECT);
        m_cxExtraSpacing = rcText.right - rcText.left;
      }else{
        m_cxExtraSpacing = 0;
      }
      dc.SelectFont(hFontOld);

      // 3. Get Windows version
      OSVERSIONINFO ovi = { sizeof(OSVERSIONINFO) };
      ::GetVersionEx(&ovi);

      // 4. Query flat menu mode (Windows XP or later)
      if ((ovi.dwMajorVersion == 5 && ovi.dwMinorVersion >= 1) || (ovi.dwMajorVersion > 5)){
#ifndef SPI_GETFLATMENU
        static const UINT SPI_GETFLATMENU = 0x1022;
#endif // !SPI_GETFLATMENU
        BOOL bRetVal = FALSE;
        bRet = ::SystemParametersInfo(SPI_GETFLATMENU, 0, &bRetVal, 0);
        m_bFlatMenus = (bRet && bRetVal);
      }
    }

    LRESULT OnInitMenuPopup(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
      return InitMenuPopupHandler(uMsg, wParam, lParam, bHandled);
    }

    LRESULT OnMenuSelect(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
      return MenuSelectHandler(uMsg, wParam, lParam, bHandled);
    }

    LRESULT OnMeasureItem(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
      return MeasureItemHandler(uMsg, wParam, lParam, bHandled);
    }

    LRESULT OnDrawItem(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
      return DrawItemHandler(uMsg, wParam, lParam, bHandled);
    }

    virtual void AssociateImage(CMenuItemInfo& mii, coolMenuItemData * pMI){
      // Default implementation, does not do anything
    }

    virtual void DynamicChangeMenuItem(CMenuItemInfo& mii, coolMenuItemData * pMI, TCHAR*,bool& ignore){
      // Default implementation, does not do anything
    }
};

#endif
