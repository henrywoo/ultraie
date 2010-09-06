#ifndef __6BEES_GDI__
#define __6BEES_GDI__

#include <atlbase.h>
#include <atltypes.h>
#include <atlstr.h>
#include <atlapp.h>
#include <atlgdi.h>
#include <atlctrls.h>
#include <string>

namespace _6bees_gdi{
  #define OBM_CHECK 32760 // From <winuser.h>
  #define HLS(h,l,s) ((HLSCOLOR)(((BYTE)(h)|((WORD)((BYTE)(l))<<8))|(((DWORD)(BYTE)(s))<<16)))
  #define HLS_H(hls) ((BYTE)(hls))
  #define HLS_L(hls) ((BYTE)(((WORD)(hls)) >> 8))
  #define HLS_S(hls) ((BYTE)((hls)>>16))
  #define KEYDOWN(Key) ((GetKeyState(Key)&0x8000)!=0)

  typedef DWORD HLSCOLOR;

  const UINT THUMBIMGW = 182;
  const UINT THUMBIMGH = 137;

  bool SetMyImageList(HBITMAP hbmp,
    CImageList& m_ImageList,
    COLORREF maskcolor,
    int ix,
    int iy,
    int imgnum,
    COLORREF bgcolor=0);

  std::string DectoHex(int dec);

  void GenerateThumb(const ATL::CString& absthumb,HDC hdc,const RECT& hdcRect);

  void _6BeeAddShadow(HBITMAP S1,HBITMAP S2,HBITMAP S3,HBITMAP S4,HBITMAP S5,
    CDC& dcSave,int rect_w,int rect_h,int border_offset,int shadow_size);

  void _6BeeDrawRect(CDC& dcMem,const RECT& r1,bool haspenalready,COLORREF bcolor);

  ATL::CString _6BeeGetImgFormat(const ATL::CString& extension);

  bool _6BeeDraw(CDC& renderDC2,
    long ow, long oh, long sw, long sys_vcxscroll, int margin,
    CComQIPtr<IViewObject,&IID_IViewObject> &pRender,
    const RECT &caprect,int desleft,int destop,int width, int height, int srcleft,int srctop);

  /*BOOL _6BeeGenPicFromWnd(HWND g_hwndFoundWindow,
    const ATL::CString& AbsPicFilename,
    const ATL::CString& AbsPicFilename_thumb
    );*/

  /// @return FAIL - NULL
  HBITMAP icon2bitmap(HICON hicon);

  BOOL CreateBitmapInfoStruct(HBITMAP hBmp,PBITMAPINFO& pbmi);

  BOOL CreateBMPFile(LPCTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

  void CreateThumbnail(const std::wstring& str_file,std::wstring& newname, int width, int height);

  BOOL GetExt(int cs,ATL::CString &ext,GUID & guidFileType);

  HLSCOLOR RGB2HLS(COLORREF rgb);
  COLORREF HLS2RGB(HLSCOLOR hls);

  // Performs some modifications on the specified color : luminance and saturation
  COLORREF HLS_TRANSFORM (COLORREF rgb, int percent_L, int percent_S);


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


};


#endif
