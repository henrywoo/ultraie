#include "6bees_gdi.h"
#include "ImageRender.h"
#include "6bees_window.h"
#include "../../dll/6beeres/resource.h"
#include "imgcap_setting.h"
#include "guard.h"

namespace _6bees_gdi{

  bool SetMyImageList(HBITMAP hbmp,CImageList& m_ImageList,COLORREF maskcolor,int ix, int iy, int imgnum,COLORREF bgcolor){
    if (m_ImageList.IsNull()){
      if (!m_ImageList.Create(ix,iy,ILC_COLOR16|ILC_MASK,imgnum,1)){
        return false;
      }
      m_ImageList.Add(hbmp,maskcolor);
      ::DeleteObject(hbmp);
      if(bgcolor)
        m_ImageList.SetBkColor(bgcolor);
    }
    return true;
  }


  std::string DectoHex(int dec){
    char outStr[7]={};
    sprintf_s(outStr,7,"%06X",dec);
    std::string tmp(outStr);
    std::string r=tmp.substr(0,2);
    std::string g=tmp.substr(2,2);
    std::string b=tmp.substr(4,2);
    std::string result="#"+b+g+r;
    return result;
  }

  void GenerateThumb(const ATL::CString& absthumb,HDC hdc,const RECT& hdcRect){
    int thumb_w,thumb_h;
    int ori_rect_w=hdcRect.right-hdcRect.left,ori_rect_h=hdcRect.bottom-hdcRect.top;
    double ratew = 1.0*ori_rect_w/THUMBIMGW;
    double rateh = 1.0*ori_rect_h/THUMBIMGH;
    if (ratew>rateh){
      thumb_w=THUMBIMGW;
      thumb_h=(int)(ori_rect_h/ratew);
    }else if(rateh>ratew){
      thumb_h=THUMBIMGH;
      thumb_w=(int)(ori_rect_w/rateh);
    }else{
      thumb_w = ori_rect_w;
      thumb_h = ori_rect_h;
    }
    ImageRender thumbimg(thumb_w,thumb_h);
    CDC renderDC2_thumb = thumbimg.GetDC();
    renderDC2_thumb.SetStretchBltMode(HALFTONE);
    renderDC2_thumb.StretchBlt(0,0,thumb_w,thumb_h,hdc,
      hdcRect.left,hdcRect.top,hdcRect.right-hdcRect.left,hdcRect.bottom-hdcRect.top,SRCCOPY);
    CLSID thumb_clsid;
    GetEncoderClsid(_T("image/jpeg"), &thumb_clsid);
    thumbimg.SaveToFile(absthumb,&thumb_clsid);
  }

  void _6BeeAddShadow(HBITMAP S1,HBITMAP S2,HBITMAP S3,HBITMAP S4,HBITMAP S5,
    CDC& dcSave,int rect_w,int rect_h,int border_offset,int shadow_size)
  {
    CDC dcMem;
    dcMem.CreateCompatibleDC(NULL);
    dcMem.SelectBitmap(S1);
    dcSave.BitBlt(0,rect_h+border_offset*2,shadow_size,shadow_size, dcMem.m_hDC,0,0,SRCCOPY);
    dcMem.SelectBitmap(S2);
    dcSave.StretchBlt(shadow_size,rect_h+border_offset*2,rect_w+border_offset*2-shadow_size,shadow_size,dcMem.m_hDC,0,0,shadow_size,shadow_size,SRCCOPY);
    dcMem.SelectBitmap(S3);
    dcSave.BitBlt(rect_w+border_offset*2,rect_h+border_offset*2,shadow_size,shadow_size,dcMem.m_hDC,0,0,SRCCOPY);
    dcMem.SelectBitmap(S4);
    dcSave.StretchBlt(rect_w+border_offset*2,shadow_size,shadow_size,rect_h+border_offset*2-shadow_size,dcMem.m_hDC,0,0,shadow_size,shadow_size,SRCCOPY);
    dcMem.SelectBitmap(S5);
    dcSave.BitBlt(rect_w+border_offset*2,0,shadow_size,shadow_size,dcMem.m_hDC,0,0,SRCCOPY);
    DeleteObject(S1);
    DeleteObject(S2);
    DeleteObject(S3);
    DeleteObject(S4);
    DeleteObject(S5);
  }

  void _6BeeDrawRect(CDC& dcMem,const RECT& r1,bool haspenalready,COLORREF bcolor){
    HPEN hpen;
    static const int BORDERWIDTH=1;
    if(!haspenalready){
      hpen=::CreatePen(PS_SOLID,BORDERWIDTH,bcolor);
      HPEN hOldPen=dcMem.SelectPen(hpen);
    }
    HGDIOBJ  hPrevBrush = SelectObject (dcMem, GetStockObject(HOLLOW_BRUSH));
    Rectangle(dcMem,r1.left,r1.top,r1.right+BORDERWIDTH,r1.bottom+BORDERWIDTH);
    if (!haspenalready){
      DeleteObject(hpen);
    }
    DeleteObject(hPrevBrush);
  }

  ATL::CString _6BeeGetImgFormat(const ATL::CString& extension){
    if(extension.CompareNoCase(_T(".png"))==0)
      return _T("image/png");
    else if(extension.CompareNoCase(_T(".jpg"))==0)
      return _T("image/jpeg");
    else if(extension.CompareNoCase(_T(".gif"))==0)
      return _T("image/gif");
    else if(extension.CompareNoCase(_T(".bmp"))==0)
      return _T("image/bmp");
    else
      return _T("");
  }

  bool _6BeeDraw(CDC& renderDC2,
    long ow, long oh, long sw, long sys_vcxscroll, int margin,
    CComQIPtr<IViewObject,&IID_IViewObject> &pRender,
    const RECT &caprect,int desleft,int destop,int width, int height, int srcleft,int srctop)
  {
    ImageRender ir(ow, oh);
    HDC renderDC = ir.GetDC();
    RECTL sourceDrawRect = {0,0,ow+sys_vcxscroll+2*margin,oh+2*margin+(sw>ow?sys_vcxscroll:0)};
    if(FAILED(pRender->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, renderDC, &sourceDrawRect, NULL, NULL, 0))){
      return false;
    }
    renderDC2.BitBlt(desleft,destop,width,height,renderDC,srcleft,srctop,SRCCOPY);
    return true;
  }

  /*BOOL _6BeeGenPicFromWnd(HWND g_hwndFoundWindow,
    const ATL::CString& AbsPicFilename,
    const ATL::CString& AbsPicFilename_thumb,
    bool imghasborder,
    bool imghasshadow,
    COLORREF bcolor
    )
  {//To be seperated
    BOOL result = true;
    RECT caprect={};
    ::GetWindowRect(g_hwndFoundWindow, &caprect);
    _6bees_window::RefreshWindow(g_hwndFoundWindow);//delete the border line
    CDC dcScreen;
    dcScreen.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    Sleep(500);
    int img_width = caprect.right - caprect.left;
    int img_height= caprect.bottom - caprect.top;
    CLSID clsid;
    GetEncoderClsid(_6bees_gdi::_6BeeGetImgFormat(L".png"), &clsid);
    ImageRender ir2(img_width,img_height);
    CDC renderDC2 = ir2.GetDC();
    renderDC2.BitBlt(0,0,caprect.right-caprect.left,caprect.bottom-caprect.top,dcScreen.m_hDC,caprect.left,caprect.top,SRCCOPY);

    // Generate Thumb Image
    //////////////////////////////////////////////////////////////////////////
    GenerateThumb(AbsPicFilename_thumb,dcScreen.m_hDC,caprect);

    // Generate Real Image (width border, shadow...)
    //////////////////////////////////////////////////////////////////////////
    int shadow_offset=(imghasshadow?shadow_size:0);
    int border_offset=(imghasborder?border_size:0);
    int imgw= img_width+shadow_offset+border_offset*2;
    int imgh= img_height+shadow_offset+border_offset*2;

    ImageRender imgIR(imgw,imgh);
    CDC imgDC = imgIR.GetDC();
    imgDC.BitBlt(border_offset,border_offset,img_width,img_height,renderDC2.m_hDC,0,0,SRCCOPY);
    if(imghasshadow){
      _6bees_gdi::_6BeeAddShadow(
        (HBITMAP)c6beeres::R().GetImg(IDB_S1,IMAGE_BITMAP,shadow_size,shadow_size),
        (HBITMAP)c6beeres::R().GetImg(IDB_S2,IMAGE_BITMAP,shadow_size,shadow_size),
        (HBITMAP)c6beeres::R().GetImg(IDB_S3,IMAGE_BITMAP,shadow_size,shadow_size),
        (HBITMAP)c6beeres::R().GetImg(IDB_S4,IMAGE_BITMAP,shadow_size,shadow_size),
        (HBITMAP)c6beeres::R().GetImg(IDB_S5,IMAGE_BITMAP,shadow_size,shadow_size),
        imgDC,img_width,img_height,border_offset,shadow_size
        );
    }
    if(imghasborder){
      RECT r={0,0,img_width+border_offset,img_height+border_offset};
      _6bees_gdi::_6BeeDrawRect(imgDC,r,false,bcolor);
    }
    imgIR.SaveToFile(AbsPicFilename.GetString(), &clsid);
    return result;
  }*/

  HBITMAP icon2bitmap(HICON hicon){
    ICONINFO iconinfo={};
    if(::GetIconInfo(hicon,&iconinfo)){
      return iconinfo.hbmColor;
    }else{
      return NULL;
    }
  }

  BOOL CreateBitmapInfoStruct(HBITMAP hBmp,PBITMAPINFO& pbmi){ 
    BITMAP bmp; 
    //PBITMAPINFO pbmi; 
    WORD    cClrBits; 

    // Retrieve the bitmap color format, width, and height. 
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
      return FALSE;

    // Convert the color format to a count of bits. 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
      cClrBits = 1; 
    else if (cClrBits <= 4) 
      cClrBits = 4; 
    else if (cClrBits <= 8) 
      cClrBits = 8; 
    else if (cClrBits <= 16) 
      cClrBits = 16; 
    else if (cClrBits <= 24) 
      cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 

    if (cClrBits != 24) 
      pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits)); 
    // There is no RGBQUAD array for the 24-bit-per-pixel format. 
    else 
      pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 

    // Initialize the fields in the BITMAPINFO structure. 

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
      pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
      * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
    pbmi->bmiHeader.biClrImportant = 0; 
    return TRUE; 
  }

  BOOL CreateBMPFile(LPCTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC){ 
    HANDLE hf;                 // file handle 
    BITMAPFILEHEADER hdr;       // bitmap file-header 
    PBITMAPINFOHEADER pbih;     // bitmap info-header 
    LPBYTE lpBits;              // memory pointer 
    DWORD dwTotal;              // total count of bytes 
    DWORD cb;                   // incremental count of bytes 
    BYTE *hp;                   // byte pointer 
    DWORD dwTmp; 

    pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits) 
      return FALSE;

    // Retrieve the color table (RGBQUAD array) and the bits 
    // (array of palette indices) from the DIB. 
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS)) {
      return FALSE;
    }
    // Create the .BMP file. 
    hf = CreateFile(pszFile, GENERIC_READ | GENERIC_WRITE, (DWORD) 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL); 
    if (hf == INVALID_HANDLE_VALUE) 
      return FALSE;
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
    // Compute the size of the entire file. 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed   * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Compute the offset to the array of color indices. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +   pbih->biSize + pbih->biClrUsed   * sizeof (RGBQUAD); 

    // Copy the BITMAPFILEHEADER into the .BMP file. 
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),   (LPDWORD) &dwTmp,  NULL)) {
      return FALSE;
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD),   (LPDWORD) &dwTmp,(NULL))) 
      return FALSE;

    // Copy the array of color indices into the .BMP file. 
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
      return FALSE;
    // Close the .BMP file. 
    if (!CloseHandle(hf)) 
      return FALSE;

    // Free memory. 
    GlobalFree((HGLOBAL)lpBits);
    return TRUE;
  }

  BOOL GetExt(int cs,ATL::CString &ext,GUID & guidFileType){
    switch (cs){
  case 0:
    ext=_T("png");
    guidFileType = Gdiplus::ImageFormatPNG;
    break;
  case 1:
    ext=_T("jpg");
    guidFileType = Gdiplus::ImageFormatJPEG;
    break;
  case 2:
    ext=_T("gif");
    guidFileType = Gdiplus::ImageFormatGIF;
    break;
  default:
    ext=_T("bmp");
    guidFileType = Gdiplus::ImageFormatBMP;
    break;
    }
    return TRUE;
  }

  void CreateThumbnail(const std::wstring& str_file,std::wstring& newname, int width, int height){
    using namespace Gdiplus;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    Image* src_image = new Image(str_file.c_str());
    if (src_image->GetWidth()>(UINT)width && src_image->GetHeight()>(UINT)height){
      Gdiplus::Image* dst_image = src_image->GetThumbnailImage(width, height);
      if (dst_image){
        CLSID thumb_clsid;
        GetEncoderClsid(_T("image/png"), &thumb_clsid);
        std::wstring tmpname = PathFindFileName(str_file.c_str());
        std::wstring foldername = str_file.substr(0,str_file.size()-tmpname.size());
        newname = foldername + L"tn-" + tmpname+ L".png";
        dst_image->Save(newname.c_str(),&thumb_clsid);
      }
      delete dst_image;
    }
    delete src_image;
    GdiplusShutdown(gdiplusToken);
  }

  static BYTE _ToRGB (float rm1, float rm2, float rh)
  {
    if      (rh > 360.0f) rh -= 360.0f;
    else if (rh <   0.0f) rh += 360.0f;

    if      (rh <  60.0f) rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
    else if (rh < 180.0f) rm1 = rm2;
    else if (rh < 240.0f) rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      

    return (BYTE)(rm1 * 255);
  }

  HLSCOLOR RGB2HLS(COLORREF rgb){
    unsigned char minval = min(GetRValue(rgb), min(GetGValue(rgb), GetBValue(rgb)));
    unsigned char maxval = max(GetRValue(rgb), max(GetGValue(rgb), GetBValue(rgb)));
    float mdiff  = float(maxval) - float(minval);
    float msum   = float(maxval) + float(minval);

    float luminance = msum / 510.0f;
    float saturation = 0.0f;
    float hue = 0.0f; 

    if(maxval != minval )
    { 
      float rnorm = (maxval - GetRValue(rgb)  ) / mdiff;      
      float gnorm = (maxval - GetGValue(rgb)) / mdiff;
      float bnorm = (maxval - GetBValue(rgb) ) / mdiff;   

      saturation = (luminance <= 0.5f) ? (mdiff / msum) : (mdiff / (510.0f - msum));

      if (GetRValue(rgb) == maxval) hue = 60.0f * (6.0f + bnorm - gnorm);
      if (GetGValue(rgb) == maxval) hue = 60.0f * (2.0f + rnorm - bnorm);
      if (GetBValue(rgb) == maxval) hue = 60.0f * (4.0f + gnorm - rnorm);
      if (hue > 360.0f) hue = hue - 360.0f;
    }
    return HLS ((hue*255)/360, luminance*255, saturation*255);
  }

  COLORREF HLS2RGB (HLSCOLOR hls){
    float hue        = ((int)HLS_H(hls)*360)/255.0f;
    float luminance  = HLS_L(hls)/255.0f;
    float saturation = HLS_S(hls)/255.0f;

    if(saturation == 0.0f )
    {
      return RGB (HLS_L(hls), HLS_L(hls), HLS_L(hls));
    }
    float rm1, rm2;

    if(luminance <= 0.5f ) rm2 = luminance + luminance * saturation;  
    else                     rm2 = luminance + saturation - luminance * saturation;
    rm1 = 2.0f * luminance - rm2;   
    BYTE red   = _ToRGB (rm1, rm2, hue + 120.0f);   
    BYTE green = _ToRGB (rm1, rm2, hue);
    BYTE blue  = _ToRGB (rm1, rm2, hue - 120.0f);

    return RGB (red, green, blue);
  }

  ///////////////////////////////////////////////////////////////////////////////
  // Performs some modifications on the specified color : luminance and saturation
  COLORREF HLS_TRANSFORM (COLORREF rgb, int percent_L, int percent_S){
    HLSCOLOR hls = RGB2HLS (rgb);
    BYTE h = HLS_H(hls);
    BYTE l = HLS_L(hls);
    BYTE s = HLS_S(hls);

    if(percent_L > 0 )
    {
      l = BYTE(l + ((255 - l) * percent_L) / 100);
    }
    else if(percent_L < 0 )
    {
      l = BYTE((l * (100+percent_L)) / 100);
    }
    if(percent_S > 0 )
    {
      s = BYTE(s + ((255 - s) * percent_S) / 100);
    }
    else if(percent_S < 0 )
    {
      s = BYTE((s * (100+percent_S)) / 100);
    }
    return HLS2RGB (HLS(h, l, s));
  }

  ///////////////////////////////////////////////////////////////////////////////
  CBufferDC::CBufferDC (HDC hDestDC, const CRect& rcPaint) : m_hDestDC (hDestDC)
  {
    if(rcPaint.IsRectEmpty() )
    {
      ::GetClipBox (m_hDestDC, m_rect);
    }
    else
    {
      m_rect = rcPaint;
    }
    Attach (::CreateCompatibleDC (m_hDestDC));
    m_bitmap.Attach (::CreateCompatibleBitmap (m_hDestDC, m_rect.right, m_rect.bottom));
    m_hOldBitmap = ::SelectObject (m_hDC, m_bitmap);

    if(m_rect.top > 0 )
    {
      ExcludeClipRect (0, 0, m_rect.right, m_rect.top);
    }
    if(m_rect.left > 0 )
    {
      ExcludeClipRect (0, m_rect.top, m_rect.left, m_rect.bottom);
    }
  }

  ///////////////////////////////////////////////////////////////////////////////
  CBufferDC::~CBufferDC ()
  {
    ::BitBlt (m_hDestDC, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), m_hDC, m_rect.left, m_rect.top, SRCCOPY);
    ::SelectObject (m_hDC, m_hOldBitmap);
  }


  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  CPenDC::CPenDC (HDC hDC, COLORREF crColor) : m_hDC (hDC)
  {
    m_pen.CreatePen (PS_SOLID, 1, crColor);
    m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
  }

  ///////////////////////////////////////////////////////////////////////////////
  CPenDC::~CPenDC ()
  {
    ::SelectObject (m_hDC, m_hOldPen);
  }

  ///////////////////////////////////////////////////////////////////////////////
  void CPenDC::Color (COLORREF crColor)
  {
    ::SelectObject (m_hDC, m_hOldPen);
    m_pen.DeleteObject();
    m_pen.CreatePen (PS_SOLID, 1, crColor);
    m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
  }

  ///////////////////////////////////////////////////////////////////////////////
  COLORREF CPenDC::Color () const
  {
    LOGPEN logPen;

    ((CPenDC*)this)->m_pen.GetLogPen (&logPen);

    return logPen.lopnColor;
  }


  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  CBrushDC::CBrushDC (HDC hDC, COLORREF crColor) : m_hDC (hDC)
  {
    if(crColor == CLR_NONE ) m_brush.Attach ((HBRUSH)::GetStockObject (NULL_BRUSH));
    else                       m_brush.CreateSolidBrush (crColor);
    m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
  }

  ///////////////////////////////////////////////////////////////////////////////
  CBrushDC::~CBrushDC ()
  {
    ::SelectObject (m_hDC, m_hOldBrush);
  }

  ///////////////////////////////////////////////////////////////////////////////
  void CBrushDC::Color (COLORREF crColor)
  {
    ::SelectObject (m_hDC, m_hOldBrush);
    m_brush.DeleteObject();
    if(crColor == CLR_NONE ) m_brush.Attach ((HBRUSH)::GetStockObject (NULL_BRUSH));
    else                       m_brush.CreateSolidBrush (crColor);
    m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
  }

  ///////////////////////////////////////////////////////////////////////////////
  COLORREF CBrushDC::Color () const
  {
    LOGBRUSH logBrush;

    ((CBrushDC*)this)->m_brush.GetLogBrush (&logBrush);

    return logBrush.lbColor;
  }


  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  CBoldDC::CBoldDC (HDC hDC, bool bBold) : m_hDC (hDC), m_hDefFont (NULL)
  {
    LOGFONT lf;

    CFontHandle ((HFONT)::GetCurrentObject (m_hDC, OBJ_FONT)).GetLogFont (&lf);

    if(( bBold && lf.lfWeight != FW_BOLD) ||
      (!bBold && lf.lfWeight == FW_BOLD) )
    {
      lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

      m_fontBold.CreateFontIndirect (&lf);
      m_hDefFont = (HFONT)::SelectObject (m_hDC, m_fontBold);
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  CBoldDC::~CBoldDC ()
  {
    if(m_hDefFont != NULL )
    {
      ::SelectObject (m_hDC, m_hDefFont);
    }
  }

};