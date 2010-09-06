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