#include "stdafx.h"
#include "HTMLEdit.h"
#include "HTMLEditDoc.h"
#include "HTMLEditView.h"
#include <atlimage.h>
#include <imgcap_setting.h>

static const short IMAGE_BITCOUNT = 24;
extern MYMODE mode;
extern const wchar_t* absThumbPath;
extern void NotifyWnd();

IMPLEMENT_DYNCREATE(CHTMLEditView, CHtmlEditView)
BEGIN_MESSAGE_MAP(CHTMLEditView, CHtmlEditView)
  ON_UPDATE_COMMAND_UI(ID_MODE_EDIT, OnUpdateModeEdit)
  ON_UPDATE_COMMAND_UI(ID_MODE_VIEW, OnUpdateModeView)
  // You may want to add command and UI handlers such as:
  //   ON_COMMAND(ID_UNDERLINE,           OnUnderline)
  //   ON_UPDATE_COMMAND_UI(ID_UNDERLINE, OnUpdateUnderline)
  // instead of using the DHTMLEDITING_CMD_ENTRY mechanism
  // below, because the DHTMLEDITING_CMD_ENTRY mechanism allows 
  // the commands in the toolbar to still be active even when in view-mode only.
END_MESSAGE_MAP()

BEGIN_DHTMLEDITING_CMDMAP(CHTMLEditView)
  DHTMLEDITING_CMD_ENTRY(ID_EDIT_COPY, IDM_COPY)
  DHTMLEDITING_CMD_ENTRY(ID_EDIT_CUT,  IDM_CUT)
  DHTMLEDITING_CMD_ENTRY(ID_EDIT_PASTE,IDM_PASTE)
  DHTMLEDITING_CMD_ENTRY(ID_EDIT_UNDO, IDM_UNDO)

  DHTMLEDITING_CMD_ENTRY(ID_UNDERLINE, IDM_UNDERLINE)
  DHTMLEDITING_CMD_ENTRY(ID_ITALIC,    IDM_ITALIC)
  DHTMLEDITING_CMD_ENTRY(ID_BOLD,      IDM_BOLD)
  DHTMLEDITING_CMD_ENTRY(ID_LEFT,      IDM_JUSTIFYLEFT)
  DHTMLEDITING_CMD_ENTRY(ID_CENTER,    IDM_JUSTIFYCENTER)
  DHTMLEDITING_CMD_ENTRY(ID_RIGHT,     IDM_JUSTIFYRIGHT)

  DHTMLEDITING_CMD_ENTRY(ID_HYPERLINK,   IDM_HYPERLINK)
  DHTMLEDITING_CMD_ENTRY(ID_IMAGE,       IDM_IMAGE)
  DHTMLEDITING_CMD_ENTRY(ID_FONT,        IDM_FONT)
  DHTMLEDITING_CMD_ENTRY(ID_INDENT,      IDM_INDENT)
  DHTMLEDITING_CMD_ENTRY(ID_OUTDENT,     IDM_OUTDENT)
  DHTMLEDITING_CMD_ENTRY(ID_ORDERLIST,   IDM_ORDERLIST )
  DHTMLEDITING_CMD_ENTRY(ID_UNORDERLIST, IDM_UNORDERLIST)

  DHTMLEDITING_CMD_ENTRY(ID_FILE_PRINTPREVIEW, IDM_PRINTPREVIEW)
  DHTMLEDITING_CMD_ENTRY(ID_FILE_PRINT, IDM_PRINT)
END_DHTMLEDITING_CMDMAP()

CHTMLEditView::CHTMLEditView(){}

CHTMLEditView::~CHTMLEditView(){}

BOOL CHTMLEditView::PreCreateWindow(CREATESTRUCT& cs){
  return CHtmlEditView::PreCreateWindow(cs);
}

void CHTMLEditView::OnInitialUpdate(void){
  CHtmlEditView::OnInitialUpdate();
  SetOffline(TRUE);
  SetSilent(TRUE);
  time_begin = time(NULL);
  m_bDesignMode = TRUE;
}

void CHTMLEditView::OnUpdateModeEdit(CCmdUI *pCmdUI){
  pCmdUI->SetCheck(m_bDesignMode);
}

void CHTMLEditView::OnUpdateModeView(CCmdUI *pCmdUI){
  pCmdUI->SetCheck(!m_bDesignMode);
}

void CHTMLEditView::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL){
  HRESULT hr = S_OK;
  CComPtr<IDispatch> spDispDoc;
  CComPtr<IHTMLDocument2> spHTMLDoc2;
  // Convert the IDispatch into IWebBrowser2
  CComQIPtr<IWebBrowser2> spTempWebBrowser = pDisp;
  if (spTempWebBrowser && m_pBrowserApp && m_pBrowserApp.IsEqualObject(spTempWebBrowser)){// Is this event associated with the top-level browser?
    if (SUCCEEDED(m_pBrowserApp->get_Document(&spDispDoc))){
      hr = spDispDoc->QueryInterface(IID_IHTMLDocument2,(void**)&spHTMLDoc2);//...and verifies that it is an HTML document
      if (SUCCEEDED(hr)){
        //CString cost;
        //cost.Format(_T("time cost:%lu"),(unsigned long)(time(NULL) - time_begin));
        //OutputDebugString(cost);
        //MessageBox(cost);
        if (mode==HIDE_WINDOW_TO_THUMB){
          CreateSnapshotThumb_NotifyWnd();
          ::PostQuitMessage(0);
        }
      }
    }
  }
}

void CHTMLEditView::OnDownloadBegin(){
  static unsigned long timediff = 5000;
  if (time(NULL)-time_begin>timediff){
    time_begin = time(NULL);
    if (mode==HIDE_WINDOW_TO_THUMB){
      CreateSnapshotThumb_NotifyWnd();
      timediff=5000*10000;
    }
  }
}

BOOL CHTMLEditView::CreateSnapshotThumb_NotifyWnd(){
  RECT ClientAreaRect={};
  HWND hWnd_HTMLClientArea=NULL;//the class of window is TabWindowClass
  CComQIPtr<IServiceProvider,&IID_IServiceProvider> pServiceProvider(m_pBrowserApp);
  if (pServiceProvider.p==NULL){return FALSE;}
  IOleWindow* pWindow = NULL;
  if (SUCCEEDED(pServiceProvider->QueryService(SID_SShellBrowser,IID_IOleWindow,(void**)&pWindow))){
    if (SUCCEEDED(pWindow->GetWindow(&hWnd_HTMLClientArea)) && hWnd_HTMLClientArea!=NULL){
      ::GetWindowRect(hWnd_HTMLClientArea,&ClientAreaRect);// hWnd_HTMLClientArea is the handle of TabWindowClass
    }
    pWindow->Release();
  }
  long width = ClientAreaRect.right - ClientAreaRect.left;
  long height = ClientAreaRect.bottom - ClientAreaRect.top;

  IViewObject2* pViewObject  = NULL;
  if (FAILED(m_pBrowserApp->QueryInterface(IID_IViewObject2, (void**)&pViewObject)))
    return true;
  BITMAPINFOHEADER bih={};
  BITMAPINFO bi={};
  RGBQUAD rgbquad={};
  bih.biSize          = sizeof(BITMAPINFOHEADER);
  bih.biWidth         = width;
  bih.biHeight        = height;
  bih.biPlanes        = 1;
  bih.biBitCount      = IMAGE_BITCOUNT;
  bih.biClrUsed       = 0;
  bih.biSizeImage     = 0;
  bih.biCompression   = BI_RGB;
  bih.biXPelsPerMeter = 0;
  bih.biYPelsPerMeter = 0;
  bi.bmiHeader = bih;
  bi.bmiColors[0] = rgbquad;
  HDC hdcMain = ::GetDC(m_hWnd);
  if (!hdcMain)
    return true;
  HDC hdcMem = CreateCompatibleDC(hdcMain);
  if (!hdcMem)
    return true;
  char* bitmapData = NULL;
  HBITMAP hBitmap = CreateDIBSection(hdcMain, &bi, DIB_RGB_COLORS, (void**)&bitmapData, NULL, 0);
  if (!hBitmap) {
    return true;
  }
  SelectObject(hdcMem, hBitmap);
  RECTL rcBounds = { 0, 0, width+25, height+25 };
  if (SUCCEEDED(pViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, hdcMain,hdcMem, &rcBounds, NULL, NULL, 0))){
    const short MARGIN = 2;
    //CImage image;
    //image.Create(width-MARGIN*2, height-MARGIN*2, IMAGE_BITCOUNT);
    //CImageDC imageDC(image);
    ////::BitBlt(imageDC, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
    //::BitBlt(imageDC, 0, 0, width-MARGIN*2, height-MARGIN*2, hdcMem, MARGIN, MARGIN, SRCCOPY);
    //image.Save(L"C:\\a.png");

    CImage image_thumb;
    image_thumb.Create(Thumb_Width,Thumb_Height,IMAGE_BITCOUNT);
    CImageDC image_thumbDC(image_thumb);
    SetStretchBltMode(image_thumbDC,HALFTONE);
    ::StretchBlt(image_thumbDC, 0, 0, Thumb_Width, Thumb_Height, hdcMem, MARGIN, 
      MARGIN,width-2*MARGIN,height-2*MARGIN,SRCCOPY);
    image_thumb.Save(absThumbPath);
  }
  DeleteObject(hBitmap);
  DeleteDC(hdcMem);
  pViewObject->Release();

  NotifyWnd();
  return true;
}