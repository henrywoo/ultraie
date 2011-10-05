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

#ifndef  _6BEESBASE_IMGRENDER_H_
#define _6BEESBASE_IMGRENDER_H_

#pragma once
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

BOOL GetEncoderClsid(LPCWSTR format, CLSID* pClsid);

class ImageFormatFactory
{
public:
  static enum IMAGEFORMAT { JPEG, GIF, TIFF, BMP, PNG };
  static BOOL GetFormatCLSID(IMAGEFORMAT fmt,CLSID* CLSIDVal);
};


class ImageRender
{
private:
  Gdiplus::Bitmap* bmp;
  Gdiplus::Graphics* g;
  HDC bmpHdc;
protected:
  Gdiplus::Bitmap* GetBitmap();
  Gdiplus::Graphics* GetGraphics();
public:
  ImageRender();
  ImageRender(int cx, int cy);
  virtual ~ImageRender();
  void Destroy();
  BOOL CreateImage(int cx, int cy);
  HDC GetDC();
  void ReleaseDC();
  BOOL SaveToFile(LPCWSTR fileName,const CLSID* clsidVal);
};

#endif