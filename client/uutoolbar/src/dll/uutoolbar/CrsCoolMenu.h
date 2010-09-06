//////////////////////////////////////////////////////////////////
//
//说明: 本文件参考价值，并未在项目中直接使用
//巫富珩 2007.05.20
//
//////////////////////////////////////////////////////////////////
// Depends on ctrlw.h
// == Begin, Ramon Smits 22-04-2000
// Side bar styles
#define SIDEBAR_FONT	"LCD"
#define SIDEBAR_ID	666
#define SIDEBAR_WIDTH	21
// == End

class CCoolMenuBar:
	public CCommandBarCtrlImpl<CCoolMenuBar>
{
public:
	DECLARE_WND_SUPERCLASS(_T("_CoolMenuBar_"), GetWndClassName())
		
	typedef CCommandBarCtrlImpl<CCoolMenuBar> BASE;
	
	BEGIN_MSG_MAP(CCoolMenuBar)
		CHAIN_MSG_MAP(BASE)
		ALT_MSG_MAP(1)
		CHAIN_MSG_MAP_ALT(BASE, 1)
		ALT_MSG_MAP(2)
		CHAIN_MSG_MAP_ALT(BASE, 2)
		ALT_MSG_MAP(3)
		CHAIN_MSG_MAP_ALT(BASE, 3)
	END_MSG_MAP()

	// == Begin, Ramon Smits 22-04-2000
	// This version takes 2 colors, one for the text and one for the shortcut. This way
	// both can have a different color.
	void DrawMenuText(CDCHandle& dc, RECT& rc, LPCTSTR lpstrText, COLORREF colorText, COLORREF colorAccellerator)
	{
		int nTab = -1;
		for(int i = 0; i < lstrlen(lpstrText); i++)
		{
			if(lpstrText[i] == '\t')
			{
				nTab = i;
				break;
			}
		}
		dc.SetTextColor(colorText);
		dc.DrawText(lpstrText, nTab, &rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		dc.SetTextColor( colorAccellerator );
		if(nTab != -1)
			dc.DrawText(&lpstrText[nTab + 1], -1, &rc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
		dc.SetTextColor(colorText);
	}
	// == End

	// == Begin, Ramon Smits 22-04-2000
	// This renders a gradient bar from top to botten in the width that is specified in the 
	// item structure.
	void DrawSideBar (CDCHandle& dc, RECT& grrect, LPCTSTR lpstrSidebarText)
	{
		RECT rct;
		dc.GetClipBox(&rct);
		int iWidth = grrect.right - grrect.left;
		int iHeight = grrect.top - grrect.bottom;
		int iSideBarHeight = rct.bottom-rct.top;
		int iSideBarWidth = rct.right-rct.left;

		COLORREF right	= GetSysColor(COLOR_ACTIVECAPTION);
		COLORREF left	= GetSysColor(27); // COLOR_GRADIENTACTIVECAPTION

		COLOR16 r = (COLOR16) ((left & 0x000000FF)<<8);
		COLOR16 g = (COLOR16) (left & 0x0000FF00);
		COLOR16 b = (COLOR16) ((left & 0x00FF0000)>>8);

		TRIVERTEX        vert[2] ;
		GRADIENT_RECT    gRect;
		vert [0] .x      = 0;
		vert [0] .y      = 0;
		vert [0] .Red    = r;
		vert [0] .Green  = g;
		vert [0] .Blue   = b;
		vert [0] .Alpha  = 0x0000;

		r = (COLOR16) ((right & 0x000000FF)<<8);
		g = (COLOR16) (right & 0x0000FF00);
		b = (COLOR16) ((right & 0x00FF0000)>>8);

		vert [1] .x      = iWidth;
		vert [1] .y      = iSideBarHeight;
		vert [1] .Red    = r;
		vert [1] .Green  = g;
		vert [1] .Blue   = b;
		vert [1] .Alpha  = 0x0000;

		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;
		
		GradientFill(dc.m_hDC,vert,2,&gRect,1,GRADIENT_FILL_RECT_V);

		HFONT hFont;

		hFont = CreateFont(iWidth, 0, 900,900,0,FALSE,FALSE,FALSE,0,OUT_DEFAULT_PRECIS,CLIP_MASK, PROOF_QUALITY, FF_DONTCARE, _T(SIDEBAR_FONT));

		if (lpstrSidebarText)
		{
			dc.SetBkMode(TRANSPARENT);
			HFONT fontold = dc.SelectFont( hFont );

			RECT dims;

			dims.left = dims.top = 1;
			dims.right = iWidth;
			dims.bottom = iSideBarHeight;

			dc.SetTextColor( 0x0 );
			dc.DrawText(lpstrSidebarText,
				(int)strlen(reinterpret_cast<const char *>(lpstrSidebarText)),
				&dims,
				DT_SINGLELINE|DT_BOTTOM);
			dims.top -= 1;
			dims.left -= 1;
			dims.right -= 1;
			dims.bottom -= 1;

			dc.SetTextColor( GetSysColor(COLOR_CAPTIONTEXT) );
			dc.DrawText(lpstrSidebarText,
				(int)strlen(reinterpret_cast<const char *>(lpstrSidebarText)),
				&dims,
				DT_SINGLELINE|DT_BOTTOM);
			dc.SelectFont( fontold );
		}
	}
	// == End
	//===================================================================================================================
	// Implementation - ownerdraw overrideables and helpers
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		
		_MenuItemData* pmd = (_MenuItemData*)lpDrawItemStruct->itemData;
		CDCHandle dc = lpDrawItemStruct->hDC;
		const RECT& rcItem = lpDrawItemStruct->rcItem;

		if(pmd->fType & MFT_SEPARATOR)
		{
			// draw separator
			RECT rc = rcItem;
			rc.top += (rc.bottom - rc.top) / 2;	// vertical center
			dc.DrawEdge(&rc, EDGE_ETCHED, BF_TOP);	// draw separator line
		}
		else		// not a separator
		{
			//== Begin, Ramon Smits 22 april 2000
			if(lpDrawItemStruct->itemID == SIDEBAR_ID)
			{
				DrawSideBar(dc,(RECT)rcItem, pmd->lpstrText);
				return;
			}
			//== End

			BOOL bDisabled = lpDrawItemStruct->itemState & ODS_GRAYED;
			BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
			BOOL bChecked  = lpDrawItemStruct->itemState & ODS_CHECKED;
			BOOL bHasImage = FALSE;

			if(LOWORD(lpDrawItemStruct->itemID) == (WORD)-1)
				bSelected = FALSE;
			RECT rcButn = { rcItem.left, rcItem.top, rcItem.left + m_szButton.cx, rcItem.top + m_szButton.cy };			// button rect
			::OffsetRect(&rcButn, 0, ((rcItem.bottom - rcItem.top) - (rcButn.bottom - rcButn.top)) / 2);	// center vertically

			int iButton = pmd->iButton;
			if(iButton >= 0)
			{
				bHasImage = TRUE;

				// calc drawing point
				SIZE sz = { rcButn.right - rcButn.left - m_szBitmap.cx, rcButn.bottom - rcButn.top - m_szBitmap.cy };
				sz.cx /= 2;
				sz.cy /= 2;
				POINT point = { rcButn.left + sz.cx, rcButn.top + sz.cy };

				// draw disabled or normal
				if(!bDisabled)
				{
					// normal - fill background depending on state
					if(!bChecked || bSelected)
					{
						dc.FillRect(&rcButn, (HBRUSH)LongToPtr((bChecked && !bSelected) ? (COLOR_3DLIGHT + 1) : (COLOR_MENU + 1)));
					}
					else
					{
						COLORREF crTxt = dc.SetTextColor(::GetSysColor(COLOR_BTNFACE));
						COLORREF crBk = dc.SetBkColor(::GetSysColor(COLOR_BTNHILIGHT));
						CBrush hbr(CDCHandle::GetHalftoneBrush());
						dc.SetBrushOrg(rcButn.left, rcButn.top);
						dc.FillRect(&rcButn, hbr);
						dc.SetTextColor(crTxt);
						dc.SetBkColor(crBk);
					}
					// draw pushed-in or popped-out edge
					if(bSelected || bChecked)
					{
						RECT rc2 = rcButn;
						dc.DrawEdge(&rc2, bChecked ? BDR_SUNKENOUTER : BDR_RAISEDINNER, BF_RECT);
					}
					// draw the image
					::ImageList_Draw(m_hImageList, iButton, dc, point.x, point.y, ILD_TRANSPARENT);
				}
				else
				{
					DrawBitmapDisabled(dc, iButton, point);
				}
			}
			else
			{
				// no image - look for custom checked/unchecked bitmaps
				CMenuItemInfo info;
				info.fMask = MIIM_CHECKMARKS | MIIM_TYPE;
				::GetMenuItemInfo((HMENU)lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, MF_BYCOMMAND, &info);
				if(bChecked || info.hbmpUnchecked != NULL)
				{
					BOOL bRadio = ((info.fType & MFT_RADIOCHECK) != 0);
					bHasImage = Draw3DCheckmark(dc, rcButn, bSelected, bDisabled, bRadio, bChecked ? info.hbmpChecked : info.hbmpUnchecked);
				}
			}

			// draw item text
			int cxButn = m_szButton.cx;
			COLORREF colorBG = ::GetSysColor(bSelected ? COLOR_HIGHLIGHT : COLOR_MENU);
			if(bSelected || lpDrawItemStruct->itemAction == ODA_SELECT)
			{
				RECT rcBG = rcItem;
				if(bHasImage)
					rcBG.left += cxButn + s_kcxGap;
				dc.FillRect(&rcBG, (HBRUSH)LongToPtr(bSelected ? (COLOR_HIGHLIGHT + 1) : (COLOR_MENU + 1)));

				// == Begin, Ramon Smits 22-04-2000
				if(bSelected)
					dc.Draw3dRect(&rcBG, GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_3DHILIGHT));
				// == End
			}

			// calc text rectangle and colors
			RECT rcText = rcItem;
			rcText.left += cxButn + s_kcxGap + s_kcxTextMargin;
			rcText.right -= cxButn;
			dc.SetBkMode(TRANSPARENT);
			COLORREF colorText = ::GetSysColor(bDisabled ?  (bSelected ? COLOR_GRAYTEXT : COLOR_3DSHADOW) : (bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT));
			// == Begin (altered), Ramon Smits 22-04-2000
			COLORREF colorAccellerator = ::GetSysColor(bDisabled ?  COLOR_GRAYTEXT : (bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_HIGHLIGHT)); //RS
			// == End

			// font already selected by Windows
			if(bDisabled && (!bSelected || colorText == colorBG))
			{
				// disabled - draw shadow text shifted down and right 1 pixel (unles selected)
				RECT rcDisabled = rcText;
				::OffsetRect(&rcDisabled, 1, 1);
				BASE::DrawMenuText(dc, rcDisabled, pmd->lpstrText, ::GetSysColor(COLOR_3DHILIGHT));
			}

			// == Begin (altered), Ramon Smits 22-04-2000
			DrawMenuText(dc, rcText, pmd->lpstrText, colorText, colorAccellerator);
			// == End, Original Begin
			// DrawMenuText(dc, rcText, pmd->lpstrText, colorText); // finally!
			// == End
		}
		
	}
	


	/**/
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{
		_MenuItemData* pmd = (_MenuItemData*)lpMeasureItemStruct->itemData;

		if(pmd->fType & MFT_SEPARATOR)	// separator - use half system height and zero width
		{
			lpMeasureItemStruct->itemHeight = ::GetSystemMetrics(SM_CYMENU) / 2;
			lpMeasureItemStruct->itemWidth  = 0;
		}
		else
		{
			// == Begin, Ramon Smits 22-04-2000
			if(lpMeasureItemStruct->itemID == SIDEBAR_ID)
			{
				// We only need to return the width of the sidebar to let windows
				// know the width of sidebar. It doesn't need to reserve height
				// because we determine the height ourselfs in the DrawItem method
				lpMeasureItemStruct->itemWidth = SIDEBAR_WIDTH - GetSystemMetrics(SM_CXMENUCHECK);
				lpMeasureItemStruct->itemHeight= 0;
				return;
			}
			// == End

			// compute size of text - use DrawText with DT_CALCRECT
			CWindowDC dc(NULL);
			HFONT hOldFont;
			if(pmd->fState & MFS_DEFAULT)
			{
				// need bold version of font
				LOGFONT lf;
				m_fontMenu.GetLogFont(lf);
				lf.lfWeight += 200;
				CFont fontBold;
				fontBold.CreateFontIndirect(&lf);
				ATLASSERT(fontBold.m_hFont != NULL);
				hOldFont = dc.SelectFont(fontBold);
			}
			else
			{
				hOldFont = dc.SelectFont(m_fontMenu);
			}

			RECT rcText = { 0, 0, 0, 0 };
			dc.DrawText(pmd->lpstrText, -1, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_CALCRECT);
			int cx = rcText.right - rcText.left;
			dc.SelectFont(hOldFont);

			LOGFONT lf;
			m_fontMenu.GetLogFont(lf);
			int cy = lf.lfHeight;
			if(cy < 0)
				cy = -cy;
			cy += 8;

			// height of item is the bigger of these two
			lpMeasureItemStruct->itemHeight = max(cy, (int)m_szButton.cy);

			// width is width of text plus a bunch of stuff
			cx += 2 * s_kcxTextMargin;	// L/R margin for readability
			cx += s_kcxGap;			// space between button and menu text
			cx += 2 * m_szButton.cx;	// button width (L=button; R=empty margin)

			// Windows adds 1 to returned value
			cx -= ::GetSystemMetrics(SM_CXMENUCHECK) - 1;
			lpMeasureItemStruct->itemWidth = cx;		// done deal
		}
	}/**/


};
