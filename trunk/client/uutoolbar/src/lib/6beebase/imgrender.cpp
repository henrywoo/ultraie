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

#include "ImageRender.h"

BOOL GetEncoderClsid(LPCWSTR format, CLSID* pClsid){
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes
   Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
   Gdiplus::GetImageEncodersSize(&num, &size);
   if(size == 0){
      return FALSE;
   }
   pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL){
      return FALSE;
   }
   Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
   for(UINT j = 0; j < num; ++j){
      if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0){
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return TRUE;  // Success
      }    
   }
   free(pImageCodecInfo);
   return FALSE;  // Failure
}

///////////////////////////////////////////////////
BOOL ImageFormatFactory::GetFormatCLSID(ImageFormatFactory::IMAGEFORMAT fmt,  CLSID* CLSIDVal)
{
  // for my own project and for the sake of demonstration, we
  // only support 4 types for now.
  BOOL retVal = FALSE;
  switch(fmt)
  {
  case JPEG:
    retVal = GetEncoderClsid(L"image/jpeg", CLSIDVal);
    break;
  case GIF:
    retVal = GetEncoderClsid(L"image/gif", CLSIDVal);
    break;
  case TIFF:
    retVal = GetEncoderClsid(L"image/tiff", CLSIDVal);
    break;
  case BMP:
    retVal = GetEncoderClsid(L"image/bmp", CLSIDVal);
    break;
  case PNG:
    retVal = GetEncoderClsid(L"image/png", CLSIDVal);
    break;
  default:
    retVal = FALSE;
    break;
  }

  return retVal;
}

///////////////////////////////////////////////////

ImageRender::ImageRender():
bmp(NULL),
g(NULL),
bmpHdc(NULL)
{}

ImageRender::ImageRender(int cx, int cy):
bmp(new Gdiplus::Bitmap(cx, cy)),
g(new Gdiplus::Graphics(bmp)),
bmpHdc(NULL)
{}

ImageRender::~ImageRender(){
  Destroy();
}

void ImageRender::Destroy()
{
  ReleaseDC();
  if (bmp != NULL){
    delete bmp;
    bmp = NULL;
  }
  if (g != NULL){
    delete g;
    g = NULL;
  }
}

BOOL ImageRender::CreateImage(int cx, int cy)
{
  if (bmp == NULL && g == NULL){
    bmp = new Gdiplus::Bitmap(cx, cy);
    g = new Gdiplus::Graphics(bmp);
    return TRUE;
  }
  return FALSE;
}

HDC ImageRender::GetDC()
{
  if (g == NULL || bmp == NULL){
    return NULL;
  }
  bmpHdc = g->GetHDC();
  return bmpHdc; 
}

void ImageRender::ReleaseDC()
{
  if (bmpHdc == NULL || g == NULL || bmp == NULL){
    return;
  }
  g->ReleaseHDC(bmpHdc);
  bmpHdc = NULL;
}

BOOL ImageRender::SaveToFile(LPCWSTR fileName, const CLSID* clsidVal)
{
  if(bmp == NULL || clsidVal==NULL) return FALSE;
  ReleaseDC();
  Gdiplus::Status retVal = bmp->Save(fileName, clsidVal, NULL);
  return (retVal == Gdiplus::Ok);
}
