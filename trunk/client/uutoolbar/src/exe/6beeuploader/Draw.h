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

///////////////////////////////////////////////////////////////////////////////
//
// Draw.h : header file
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

///////////////////////////////////////////////////////////////////////////////
typedef DWORD HLSCOLOR;
#define HLS(h,l,s) ((HLSCOLOR)(((BYTE)(h)|((WORD)((BYTE)(l))<<8))|(((DWORD)(BYTE)(s))<<16)))

///////////////////////////////////////////////////////////////////////////////
#define HLS_H(hls) ((BYTE)(hls))
#define HLS_L(hls) ((BYTE)(((WORD)(hls)) >> 8))
#define HLS_S(hls) ((BYTE)((hls)>>16))

///////////////////////////////////////////////////////////////////////////////
HLSCOLOR RGB2HLS (COLORREF rgb);
COLORREF HLS2RGB (HLSCOLOR hls);

///////////////////////////////////////////////////////////////////////////////
// Performs some modifications on the specified color : luminance and saturation
COLORREF HLS_TRANSFORM (COLORREF rgb, int percent_L, int percent_S);


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CBufferDC : public CDCHandle 
{
    HDC     m_hDestDC;
    CBitmap m_bitmap;     // Bitmap in Memory DC
    CRect   m_rect;
    HGDIOBJ m_hOldBitmap; // Previous Bitmap

public:
    CBufferDC (HDC hDestDC, const CRect& rcPaint = CRect(0,0,0,0));
   ~CBufferDC ();
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CPenDC
{
protected:
    CPen m_pen;
    HDC m_hDC;
    HPEN m_hOldPen;

public:
    CPenDC (HDC hDC, COLORREF crColor = CLR_NONE);
   ~CPenDC ();

    void Color (COLORREF crColor);
    COLORREF Color () const;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CBrushDC
{
protected:
    CBrush m_brush;
    HDC m_hDC;
    HBRUSH m_hOldBrush;

public:
    CBrushDC (HDC hDC, COLORREF crColor = CLR_NONE);
   ~CBrushDC ();

    void Color (COLORREF crColor);
    COLORREF Color () const;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CBoldDC
{
protected:
    CFont m_fontBold;
    HDC m_hDC;
    HFONT m_hDefFont;

public:
    CBoldDC (HDC hDC, bool bBold);
   ~CBoldDC ();
};
