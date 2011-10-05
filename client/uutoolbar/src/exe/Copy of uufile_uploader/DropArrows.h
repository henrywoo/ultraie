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

//#include <atlmisc.h>

class CDropArrows : public CWindowImpl< CDropArrows >
{
public:
	CDropArrows()
	{
		m_bVertical = FALSE;
		m_nSpanLength = 0;
	}
	
	~CDropArrows()
	{
	}
	
	DECLARE_WND_CLASS( _T( "DropArrows" ) )

protected:
	CBrush m_bshArrowBrush;
	CRgn m_rgnArrowRegion;
	BOOL m_bVertical;
	int m_nSpanLength;
	
public:
	BOOL Create( HWND hWndParent, int nSpanLength, BOOL bVertical )
	{
		if ( !m_bshArrowBrush.CreateSolidBrush( RGB( 255, 0, 0 ) ) )
			return FALSE;
			
		m_bVertical = bVertical;
		m_nSpanLength = nSpanLength + 20;

		if ( CWindowImpl< CDropArrows >::Create( hWndParent, CRect( 0, 0, m_bVertical ? 12 : m_nSpanLength, m_bVertical ? m_nSpanLength : 12 ), NULL, WS_POPUP | WS_DISABLED, WS_EX_TOOLWINDOW ) == NULL )
			return FALSE;
		
		POINT ptArrow[ 7 ];

		ptArrow[0].x = bVertical ? 8 : 0;	ptArrow[0].y = bVertical ? 0 : 9;
		ptArrow[1].x = bVertical ? 8 : 4;	ptArrow[1].y = bVertical ? 4 : 9;
		ptArrow[2].x = bVertical ? 11 : 4;	ptArrow[2].y = bVertical ? 4 : 12;
		ptArrow[3].x = bVertical ? 6 : 10;	ptArrow[3].y = bVertical ? 9 : 6;
		ptArrow[4].x = bVertical ? 1 : 4;	ptArrow[4].y = bVertical ? 4 : 0;
		ptArrow[5].x = bVertical ? 4 : 4;	ptArrow[5].y = bVertical ? 4 : 4;
		ptArrow[6].x = bVertical ? 4 : 0;	ptArrow[6].y = bVertical ? 0 : 4;

		CRgn rgnFirstArrow;
		if ( !rgnFirstArrow.CreatePolygonRgn( ptArrow, 7, ALTERNATE ) )
			return FALSE;

		ptArrow[0].x = bVertical ? 4 : m_nSpanLength;		ptArrow[0].y = bVertical ? m_nSpanLength : 4;
		ptArrow[1].x = bVertical ? 4 : m_nSpanLength - 4;	ptArrow[1].y = bVertical ? m_nSpanLength - 4 : 4;
		ptArrow[2].x = bVertical ? 0 : m_nSpanLength - 4;	ptArrow[2].y = bVertical ? m_nSpanLength - 4 : 0;
		ptArrow[3].x = bVertical ? 6 : m_nSpanLength - 10;	ptArrow[3].y = bVertical ? m_nSpanLength - 10 : 6;
		ptArrow[4].x = bVertical ? 12 : m_nSpanLength - 4;	ptArrow[4].y = bVertical ? m_nSpanLength - 4 : 12;
		ptArrow[5].x = bVertical ? 8 : m_nSpanLength - 4;	ptArrow[5].y = bVertical ? m_nSpanLength - 4 : 9;
		ptArrow[6].x = bVertical ? 8 : m_nSpanLength;		ptArrow[6].y = bVertical ? m_nSpanLength : 9;

		CRgn rgnSecondArrow;
		if ( !rgnSecondArrow.CreatePolygonRgn( ptArrow, 7, ALTERNATE ) )
			return FALSE;

		if ( !m_rgnArrowRegion.CreateRectRgn( 0, 0, bVertical ? 12 : nSpanLength, bVertical ? nSpanLength : 12 ) )
			return FALSE;
		
		m_rgnArrowRegion.CombineRgn( rgnFirstArrow, rgnSecondArrow, RGN_OR );
		
		SetWindowRgn( m_rgnArrowRegion, FALSE );

		return TRUE;
	}
	
	BOOL Show( CPoint point )
	{
		return IsWindow() ? SetWindowPos( NULL, m_bVertical ? point.x - 7 : point.x - ( m_nSpanLength / 2 ), m_bVertical ? point.y - ( m_nSpanLength / 2 ) : point.y - 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE ) : FALSE;
	}
	
	BOOL Hide()
	{
		return IsWindow() ? ShowWindow( SW_HIDE ) : FALSE;
	}
	
	BEGIN_MSG_MAP_EX(CDropArrows)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
	END_MSG_MAP_EX()
	
	void OnDestroy()
	{
		m_bshArrowBrush.DeleteObject();
		m_rgnArrowRegion.DeleteObject();
	}
	
	BOOL OnEraseBkgnd( HDC dc ) 
	{
		CDCHandle dcErase( dc );
		dcErase.FillRect( CRect( 0, 0, m_bVertical ? 12 : m_nSpanLength, m_bVertical ? m_nSpanLength : 12 ), m_bshArrowBrush );
		return TRUE;
	}
};
