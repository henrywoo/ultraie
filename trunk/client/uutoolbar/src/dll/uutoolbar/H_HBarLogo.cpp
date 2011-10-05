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

#include "H_HBarLogo.h"

#include "6bees_util.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#pragma comment(lib,"6beecommond.lib")

using namespace _6bees_util;

bool GetImageFromIDResource(UINT nID, LPCTSTR sTR,Gdiplus::Image* &pImg){
  using _6bees_util::ultrapath;
  static wstring dllpath(ultrapath::R().get(ultrapath::uploader));
  static HINSTANCE hInst = ::LoadLibrary(dllpath.c_str());
  HRSRC hRsrc = ::FindResource(hInst,MAKEINTRESOURCE(nID),sTR); /// type
  if (!hRsrc)
    return false;
  /// load resource into memory
  DWORD len = SizeofResource(hInst, hRsrc);
  BYTE* lpRsrc = (BYTE*)::LoadResource(hInst, hRsrc);
  if (!lpRsrc)
    return false;
  /// Allocate global memory on which to create stream
  HGLOBAL m_hMem = ::GlobalAlloc(GMEM_FIXED, len);
  BYTE* pmem = (BYTE*)::GlobalLock(m_hMem);
  ::memcpy(pmem,lpRsrc,len);
  IStream* pstm;
  ::CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);  
  /// load from stream
  pImg=Gdiplus::Image::FromStream(pstm);
  /// free/release stuff
  ::GlobalUnlock(m_hMem);
  pstm->Release();
  ::FreeResource(lpRsrc);
  return true;
}

CH_HBarLogo::CH_HBarLogo():m_bmp((HBITMAP)c6beeres::R().GetImg(IDB_SEARCH,IMAGE_BITMAP,21,24)),myfont2(NULL){
  m_gdiplus_started = FALSE;
  if(GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL)==0){
    m_gdiplus_started = TRUE;
  }
  m_pngLogo = NULL;
  SetPNG(IDR_HBAR_LOGO);
}

CH_HBarLogo::~CH_HBarLogo(){
  if(m_gdiplus_started && m_gdiplusToken){
    m_gdiplus_started = FALSE;
    Gdiplus::GdiplusShutdown(m_gdiplusToken);
  }
  DeleteObject((HGDIOBJ)(HFONT)(myfont2));
  m_bmp.DeleteObject();
  if (IsWindow()){
    DestroyWindow();
  }
}

BOOL CH_HBarLogo::SetPNG(UINT PNG_UINT){
  if(m_gdiplus_started)
    return GetImageFromIDResource(PNG_UINT,_T("PNG"),m_pngLogo);
  else
    return FALSE;
}

LRESULT CH_HBarLogo::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  if (!::IsWindow(m_hWnd)){
    return S_OK;
  }
  CPaintDC mydc(m_hWnd);
  SetBkMode(mydc.m_hDC,TRANSPARENT);

  Gdiplus::Graphics graphics(mydc.m_hDC);
  graphics.DrawImage(m_pngLogo,Gdiplus::Point(2,2));

  RECT rect = {26,4,62,_6bees_const::ktbbuttonheight-4};
  if(myfont2==NULL){
    myfont2 = ::CreateFont(16,0,0,0,FW_THIN,false,FALSE,FALSE,DEFAULT_CHARSET,OUT_STRING_PRECIS,
      CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));
  }
  DrawText(mydc.m_hDC,_T("Find:"),-1,&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
  mydc.RestoreDC(-1);

  return 0;
}