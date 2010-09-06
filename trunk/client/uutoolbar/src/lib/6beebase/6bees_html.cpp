#include "6bees_html.h"

#include <math.h>//ceil
#include <map>
#include <pcre++.h>

#include "../../dll/mousehook/mousehook.h"
#include "../../dll/6beeres/resource.h"
#include "imgcap_setting.h" //shadow_size
#include "6bees_const.h"
#include "6bees_util.h"//inttostr,Split,WriteCharToFile_UTF8
#include "6bees_window.h"
#include "imagerender.h"
#include <6bees_gdi.h>
#include <6bees_net.h>
#include <6bees_httpclient.h>
#include <6bees_str.h>
#include <6bees_lang.h>
#include <direct.h>

namespace _6bees_html{
  using namespace std;
  using namespace _6bees_str;
  using namespace _6bees_util;

  using _6bees_gdi::THUMBIMGW;
  using _6bees_gdi::THUMBIMGH;

  _COM_SMARTPTR_TYPEDEF(IHTMLRect, __uuidof(IHTMLRect));
  _COM_SMARTPTR_TYPEDEF(IHTMLElement2, __uuidof(IHTMLElement2));

  RECT INFO_RECT_WEBELE_RIGHT={0,INFO_OFFSET,0,INFO_OFFSET + INFO_HEIGHT_WEBELE};

  void GetNavCode(const string& urlbase, const char* totalnum, int pnum, string& nav){
    int hnum = atoi(totalnum)/100;
    nav=  "<div class='pages'>";
    if(pnum!=0)
      nav +=  "<a href='"+urlbase+"0/' style='font-weight: bold;'> << </a>...";
    if(pnum-6>0)  nav +=  stringmaker() << "<a href='" << urlbase << (pnum-6) << "/'> " << (pnum-5) << " </a>";
    if(pnum-5>0)  nav +=  "<a href='"+urlbase + int2str(pnum-4) + "/'> " + int2str(pnum-4) + " </a>";
    if(pnum-4>0)  nav +=  "<a href='"+urlbase + int2str(pnum-4) + "/'> " + int2str(pnum-3) + " </a>";
    if(pnum-3>0)  nav +=  "<a href='"+urlbase + int2str(pnum-3) + "/'> " + int2str(pnum-2) + " </a>";
    if(pnum-2>0)  nav +=  "<a href='"+urlbase + int2str(pnum-2) + "/'> " + int2str(pnum-1) + " </a>";
    if(pnum-1>0)  nav +=  "<a href='"+urlbase + int2str(pnum-1) + "/'> " + int2str(pnum) + " </a>";
    nav +=  "&nbsp;<font color=red size=4><b>";
    nav +=  int2str(pnum+1);
    nav +=  "</b></font>&nbsp;";
    if(hnum>=pnum+1)  nav +=  "<a href='"+urlbase + int2str(pnum+2-1) + "/'> " + int2str(pnum+2) + " </a>";
    if(hnum>=pnum+2)  nav +=  "<a href='"+urlbase + int2str(pnum+3-1) + "/'> " + int2str(pnum+3) + " </a>";
    if(hnum>=pnum+3)  nav +=  "<a href='"+urlbase + int2str(pnum+4-1) + "/'> " + int2str(pnum+4) + " </a>";
    if(hnum>=pnum+4)  nav +=  "<a href='"+urlbase + int2str(pnum+5-1) + "/'> " + int2str(pnum+5) + " </a>";
    if(hnum>=pnum+5)  nav +=  "<a href='"+urlbase + int2str(pnum+6-1) + "/'> " + int2str(pnum+6) + " </a>";
    if(hnum>=pnum+6)  nav +=  "<a href='"+urlbase + int2str(pnum+7-1) + "/'> " + int2str(pnum+7) + " </a>";
    if(pnum!=hnum)    nav +=  "...<a href='"+urlbase + int2str(hnum) +"/' style='font-weight: bold;'> >> </a>";
#if __CHINESE__
    nav +=  stringmaker(CP_UTF8) << L"&nbsp;&nbsp;(页:  " << (pnum+1) << "/" << (hnum+1) << L" 总页数 ; 总记录数: ";
#else
    nav +=  "&nbsp;&nbsp;( Pages:  " + int2str(pnum+1) + "/"+int2str(hnum+1) +" total ; Records: ";
#endif
    nav +=  totalnum;
    nav +=  ")</div>";
  }

  /// Data Format: action@[DELETE|INSERT]&table@[MediaResources|SurfHistory|...]&condition@id=123||title="Google"
  bool ParseAction(const wchar_t* tbname, wstring& fname, string& _cmd,
    wstring& redirectpage)
  {
    vector<wstring> vs = Split(fname,_T("&"));
    wstring action,table,condition,setval;

    for(vector<wstring>::iterator a=vs.begin();a!=vs.end();++a){
      vector<wstring> vs2 = Split(*a,_T("@"));
      if(vs2.size()==2){
        if(vs2[0].compare(_T("action"))==0){
          action = vs2[1];
        }else if(vs2[0].compare(_T("table"))==0){
          table=vs2[1];
          if (table != tbname){
            return false;
          }
        }else if(vs2[0].compare(_T("condition"))==0){
          condition = vs2[1];
          strreplace(condition,_T("||"),_T(" and "));
        }else if(vs2[0].compare(_T("set"))==0){
          setval=vs2[1];
          strreplace(setval,_T("||"),_T(" , "));
        }else if(vs2[0].compare(_T("redirectpage"))==0){
          redirectpage=vs2[1];
        }
      }
    }
    if(!action.empty() && !table.empty()){
      ATL::CString cmd;
      bool isupdate=false;
      if(action.compare(_T("delete"))==0){
        cmd.Format(_T("delete from %s where %s"),table.c_str(),condition.c_str());
      }else if(action.compare(_T("update"))==0){
        cmd.Format(_T("update %s set %s"),table.c_str(),setval.c_str());
        if (!condition.empty()){
          cmd.Append(_T(" where "));
          cmd.Append(condition.c_str());
        }
        isupdate=true;
      }
      _cmd = stringmaker()<<cmd.GetString();
    }
    return true;
  }

  bool EntirePageCapture(
    HWND hwnd,
    c6beewb2ptr* _pwb,
    const wchar_t* imgpath,
    const wchar_t* imgpath_thumb,
    bool isthumbforwholepage,
    bool imghasborder,
    bool imghasshadow,
    COLORREF bcolor)
  {
    bool result=true;
    IHTMLDocument2Ptr& pHtmlDoc2 = _pwb->GetHTMLDoc2();
    if (!pHtmlDoc2){
      return false;
    }

    // todo - This function still has some problem (2008.09.10)
    IHTMLWindow2Ptr pHtmlw2(0);//To execute scroll!
    if (FAILED(pHtmlDoc2->get_parentWindow(&pHtmlw2))){
      return FALSE;
    }
    IHTMLElementPtr  pElementBody(0);
    if (FAILED(pHtmlDoc2->get_body(&pElementBody))){
      return FALSE;
    }
    if (pElementBody.GetInterfacePtr()==NULL){
      return FALSE;
    }
    IHTMLElement* par_html = NULL;
    pElementBody->get_parentElement(&par_html);// the <html> element

    CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> apHTMLElement2(par_html);
    CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> apBodyElement2(pElementBody);
    // function get_scrollTop is not relible, test case: http://www.cplusplus.com/reference/clibrary/cmath/,
    // refer to document here: http://www.ureader.com/msg/15171001.aspx
    // document.compatMode property, the property indicates whether the browser works in standards compliant or in quirks mode,
    // possible values are "CSS1Compat" and "BackCompat", the latter value means quirks mode.
    bool isCompat=true;
    CComQIPtr<IHTMLDocument5> pDoc5 = pHtmlDoc2;
    if (!pDoc5.p)
      isCompat = true;
    BSTR bstrCompatMode;
    if (FAILED(pDoc5->get_compatMode(&bstrCompatMode)))
      isCompat = true;
    if (wcscmp(bstrCompatMode, _T("CSS1Compat")) == NULL)
      isCompat = false;

    long sw=0,sh=0,  cw=0,ch=0,oh=0,ow=0;
    if(FAILED(apBodyElement2->get_scrollWidth(&sw))||FAILED(pElementBody->get_offsetWidth(&ow))){
      return FALSE;
    }
    CComQIPtr<IViewObject,&IID_IViewObject> pRender(pHtmlDoc2);
    RECT cltrect={};
    HWND hcurrentwnd=_pwb->GetCurrentTab();//the class of window is TabWindowClass
    ::GetWindowRect(hcurrentwnd,&cltrect);// hcurrentwnd is the handle of TabWindowClass
    long ClientAreaHeight = cltrect.bottom-cltrect.top;
    long ClientAreaWidth  = cltrect.right -cltrect.left;
    //change to full screen - discard this now //if (sw>ow)
    //if (sw>ClientAreaWidth){
    //  if(FAILED(_pwb->put_FullScreen(VARIANT_TRUE))){return FALSE;}
    //}

    int statusbarheight(0),shuipinbarheight(0);
    _6bees_window::IE7::GetBarHeight(hcurrentwnd,statusbarheight,shuipinbarheight);

    //Release version  - lock the mouse when scrolling web page
    //InstallMouseHook(m_hWnd,g_hCursorSearchWindow,CAPWHOLEPAGE);
    InstallMouseHook(hwnd,NULL,CAPWHOLEPAGE);

    if(FAILED(pHtmlw2->scroll(0,0))){
      return false;
    }
    //scrollHeight/scrollWidth are the actual height and width of web page contents.
    if( FAILED(apBodyElement2->get_scrollWidth(&sw)) ||  FAILED(apBodyElement2->get_scrollHeight(&sh))
      ||FAILED(apBodyElement2->get_clientWidth(&cw)) ||  FAILED(apBodyElement2->get_clientHeight(&ch))
      ||FAILED(pElementBody->get_offsetHeight(&oh))   ||  FAILED(pElementBody->get_offsetWidth(&ow))
      ){
        return false;
    }
/*
#if 0
    wstring STRM;
    STRM = boost::lexical_cast<wstring>(oh);
    STRM = _T("Offset Height: ") + STRM;
    OutputDebugStringW(STRM.c_str());
    STRM = boost::lexical_cast<wstring>(ClientAreaHeight);
    STRM = _T("ClientArea Height: ") + STRM;
    OutputDebugStringW(STRM.c_str());
    STRM = boost::lexical_cast<wstring>(ch);
    STRM = _T("clientHeight : ") + STRM;
    OutputDebugStringW(STRM.c_str());
    STRM = boost::lexical_cast<wstring>(sh);
    STRM = _T("scrollHeight : ") + STRM;
    OutputDebugStringW(STRM.c_str());
#endif
*/
    //For this web page :http://www.cplusplus.com/reference/clibrary/cmath/
    //992>682 -> This means Offset_Height is NOT reliable, we can not use it!!!!
#if 0
    if (oh>ClientAreaHeight){
    //OutputDebugStringA("ERROR2: oh>ClientAreaHeight");
    return false;
    }
#endif

    // some fix and adjustment
    if (cw>sw){
      sw=cw;
    }
    if (ch>sh){
      sh=ch;
    }
    //sw -= 3;
    //sh -= 1;
    //when there are other plugins, this method maybe invalid. Why?
    static const int margin = 2;//You can find this in MS's document. There is an offset of 2.
    int sys_vcxscroll = GetSystemMetrics(SM_CXVSCROLL);// width of vertical right scrolling bar of IE, equal to 16 under XP classic, equal to 17 under windows XP xp style
    int horizontal_scroll_bar_height = sw>ow?sys_vcxscroll:0;
    // fix a bug - then we can capture gmail web page
    if (sw==ow){
      sw=ClientAreaWidth-sys_vcxscroll-2*margin;
    }
    oh = ClientAreaHeight- horizontal_scroll_bar_height - shuipinbarheight - statusbarheight;
    ow = ClientAreaWidth - sys_vcxscroll - 2*margin;

    ImageRender ir2(sw,sh);
    CDC renderDC2 = ir2.GetDC();
    long curposh=0,curposw=0;
    int tmpimgaddingh=sys_vcxscroll;
    int tmpimgaddingw=2*margin;
    if (pRender.p){
      long tmpb=0,tmpb2=0;
      for(int testval=0;;){
        long tmpsl=0,tmpst=0,tmpsl2=0,tmpst2=0;
        for(curposh=0;;){
          ImageRender ir(ow+tmpimgaddingw,oh+tmpimgaddingh);//this temp img buffer should be wider and higher!!!
          HDC renderDC = ir.GetDC();
          RECTL sourceDrawRect = {0,0,ow+sys_vcxscroll+2*margin,oh+2*margin+horizontal_scroll_bar_height};
          if(FAILED(pRender->Draw(DVASPECT_CONTENT,-1,NULL,NULL,NULL,renderDC,&sourceDrawRect,NULL,NULL,0))){return false;}
          tmpsl=0,tmpst=0;
          {
            isCompat ? apBodyElement2->get_scrollLeft(&tmpsl) : apHTMLElement2->get_scrollLeft(&tmpsl);
            isCompat ? apBodyElement2->get_scrollTop(&tmpst) : apHTMLElement2->get_scrollTop(&tmpst);
          }
          ///@todo bug here renderDC2.m_hDC can be null! 2009-05-19
          renderDC2.BitBlt(tmpsl,tmpst,ow+tmpimgaddingw,oh+tmpimgaddingh,renderDC,margin,margin,SRCCOPY);
          if (imgpath==NULL){// For thumb, only capture one time
            break;
          }
#ifdef _TESTING_
          ATL::CString tmpimg;
          tmpimg.Format(_T("C:\\tmp\\%d%s"),++testval,picture_extension);
          ir.SaveToFile(tmpimg,&clsid);
#endif
          int scroll_distance = tmpst+oh-sys_vcxscroll;
          if(FAILED(pHtmlw2->scrollTo(tmpsl,scroll_distance))){
            return false;
          }
          tmpsl2=0,tmpst2=0;
          {
            isCompat ? apBodyElement2->get_scrollLeft(&tmpsl2) : apHTMLElement2->get_scrollLeft(&tmpsl2);
            isCompat ? apBodyElement2->get_scrollTop(&tmpst2) : apHTMLElement2->get_scrollTop(&tmpst2);
          }
          if((tmpst2==tmpst+sys_vcxscroll||tmpst2==tmpst) && tmpsl2==tmpsl){
            break;
          }
          if (curposh==scroll_distance){
            break;
          }
          curposh=scroll_distance;
        }
        tmpb=0,tmpb2=0;
        if(isCompat ? FAILED(apBodyElement2->get_scrollLeft(&tmpb)) : FAILED(apHTMLElement2->get_scrollLeft(&tmpb))){return false;}
        if(FAILED(pHtmlw2->scroll(ow+tmpb-sys_vcxscroll, 0))){return false;}
        if(isCompat ? FAILED(apBodyElement2->get_scrollLeft(&tmpb2)) : FAILED(apHTMLElement2->get_scrollLeft(&tmpb2))){return false;}
        if (tmpb==tmpb2){break;}
        if (curposw==ow+tmpb-sys_vcxscroll){break;}
        curposw=ow+tmpb-sys_vcxscroll;
      }
    }
    int shadow_offset = (imghasshadow?shadow_size:0);
    int border_offset  = (imghasborder?border_size:0);
    int imgw= sw+shadow_offset+border_offset*2;
    int imgh= sh+shadow_offset+border_offset*2;

    if (imgpath!=NULL){
      ImageRender imgIR(imgw,imgh);
      CDC imgDC = imgIR.GetDC();
      imgDC.BitBlt(border_offset,border_offset,sw,sh,renderDC2.m_hDC,0,0,SRCCOPY);
      if(imghasshadow){
        _6bees_gdi::_6BeeAddShadow(
          (HBITMAP)c6beeres::R().GetImg(IDB_S1,IMAGE_BITMAP,shadow_size,shadow_size),
          (HBITMAP)c6beeres::R().GetImg(IDB_S2,IMAGE_BITMAP,shadow_size,shadow_size),
          (HBITMAP)c6beeres::R().GetImg(IDB_S3,IMAGE_BITMAP,shadow_size,shadow_size),
          (HBITMAP)c6beeres::R().GetImg(IDB_S4,IMAGE_BITMAP,shadow_size,shadow_size),
          (HBITMAP)c6beeres::R().GetImg(IDB_S5,IMAGE_BITMAP,shadow_size,shadow_size),
          imgDC,sw,sh,border_offset,shadow_size
          );
      }
      if(imghasborder){
        RECT r={0,0,sw+border_offset,sh+border_offset};
        _6bees_gdi::_6BeeDrawRect(imgDC,r,false,bcolor);
      }
      CLSID clsid;
      GetEncoderClsid(_6bees_gdi::_6BeeGetImgFormat(L".png"), &clsid);
      imgIR.SaveToFile(imgpath, &clsid);
    }

    // Generate Thumb Image
    if(imgpath_thumb){
      if (isthumbforwholepage){
        RECT hdcRect={0,0,sw,sh};
        _6bees_gdi::GenerateThumb(imgpath_thumb,renderDC2.m_hDC,hdcRect);
      }else{
        double rate =min(sw*1.0/THUMBIMGW,sh*1.0/THUMBIMGH);
        RECT hdcRect={0,0,(int)(THUMBIMGW*rate),(int)(THUMBIMGH*rate)};
        _6bees_gdi::GenerateThumb(imgpath_thumb,renderDC2.m_hDC,hdcRect);
      }
    }

    //_pwb->put_FullScreen(VARIANT_FALSE);
    //release hook
    if(!UnInstallMouseHook()){
      //OutputDebugStringA("ERROR: hook release failed!");
    }
    return result;
  }








  // When capture image and clip web element, call this to highlight the rectangle of web element
  LRESULT HighlightWebElement(
    IHTMLElement*& elementHit,
    c6beewb2ptr* _pwb,
    RECT& CaptureRect,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    BOOL& bHandled
    )
  {
    IHTMLDocument2Ptr& pHtmlDoc2 = _pwb->GetHTMLDoc2();
    if (!pHtmlDoc2){
      return false;
    }
    //To execute javascript!
    IHTMLWindow2Ptr pHtmlw2(NULL);
    if (FAILED(pHtmlDoc2->get_parentWindow(&pHtmlw2))){
      return 0;
    }
    long _tempID=-1;
    RECT cltrect={};
    HWND hcurrentwnd=_pwb->GetCurrentTab();//the class of window is TabWindowClass
    ::GetWindowRect(hcurrentwnd,&cltrect);// hcurrentwnd is the handle of TabWindowClass

    int statusbarheight(0),shuipinbarheight(0);
    _6bees_window::IE7::GetBarHeight(hcurrentwnd,statusbarheight,shuipinbarheight);
    int x = (int)(wParam),y = (int)(lParam);
    POINT mouse_point={x,y};
    if(!::PtInRect(&cltrect,mouse_point)){
      //RefreshWindow (hcurrentwnd);
      return 0;
    }
    mouse_point.x -= cltrect.left;
    mouse_point.y -= cltrect.top;

    //move mouse and get the IHTMLElementPtr and then we can get innerhtml!
    //IHTMLElement* elementHit;
    if(FAILED(pHtmlDoc2->elementFromPoint(x-cltrect.left,y-cltrect.top,&elementHit))){
      return 0;
    }
    if(elementHit && FAILED(elementHit->get_sourceIndex(&_tempID))){
      return 0;
    }

    static int elementsrcid = -1;
    if(elementsrcid!=_tempID && _tempID>0){
      /*CComBSTR tmp;
      if(elementHit && SUCCEEDED(elementHit->get_innerHTML(&tmp))){
        elementHTML = tmp;
      }*/

      elementsrcid = _tempID;
      int sys_vcxscroll = GetSystemMetrics(SM_CXVSCROLL);//unser XP style it is 17, classic 16!
      int margin(2);
#if 0
      ATL::CString debugStr;
      debugStr.Format(_T("%d"),_tempID);
      OutputDebugString(debugStr);
#endif

      //IHTMLElement2Ptr apBodyElement2;
      //elementHit->QueryInterface(IID_IHTMLElement2,(void**)&apBodyElement2);
      CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> apBodyElement2(elementHit);
      long sw=0,sh=0;
      if(FAILED(apBodyElement2->get_scrollWidth(&sw))||FAILED(apBodyElement2->get_scrollHeight(&sh))){return FALSE;}
      IHTMLRectPtr pRect;
      //This method retrieves an object that exposes the left, top, right, and bottom coordinates of the union of rectangles
      //relative to the client's upper-left corner. In Microsoft Internet Explorer 5, the window's upper-left is at 2,2 (pixels)
      //with respect to the true client.
      apBodyElement2->getBoundingClientRect(&pRect);
      long num_top=0,num_left=0,num_bottom=0,num_right=0;
      if (FAILED(pRect->get_left(&num_left))||
        FAILED(pRect->get_top(&num_top))||
        FAILED(pRect->get_right(&num_right))||
        FAILED(pRect->get_bottom(&num_bottom))
      ){return 0;}
      ATL::CString capinfo;
      capinfo.Format(tsize,num_right-num_left,num_bottom-num_top);
      long ow = cltrect.right-cltrect.left- sys_vcxscroll - 2*margin;
      long oh = cltrect.bottom-cltrect.top- statusbarheight - (sw>ow?sys_vcxscroll:0) - shuipinbarheight;
      if(num_left==0&&num_top==0){
        num_right -= sys_vcxscroll;
        if(sw>ow){
          num_bottom -= sys_vcxscroll;
        }
      }
      // save the rect as CaptureRect, then we can use it when we capture image in the future (relative coordinate)
      CaptureRect.left   = num_left;
      CaptureRect.top    = num_top;
      CaptureRect.right  = num_right;
      CaptureRect.bottom = num_bottom;

      // 1. draw webelement rectanle
      HDC hWindowDC=::GetWindowDC(hcurrentwnd);// The DC of the found window.
      _6bees_window::RefreshWindow (hcurrentwnd);
      HPEN gpen = CreatePen (PS_SOLID,2,SELECT_RECTCOLOR);
      HGDIOBJ  hPrevPen   = SelectObject (hWindowDC, gpen);
      HGDIOBJ  hPrevBrush = SelectObject (hWindowDC, GetStockObject(HOLLOW_BRUSH));
      Rectangle(hWindowDC,num_left,num_top,num_right,num_bottom);

      // 2. draw information pane
      HWND hdesktop =::GetDesktopWindow();
      HDC hDesktopDC=::GetWindowDC(hdesktop);// The DC of Desktop.

      // show the info of captured pictures
      static HBRUSH hbrush=::CreatePatternBrush(
        (HBITMAP)c6beeres::R().GetImg(IDB_BITMAP_INFOBK,IMAGE_BITMAP,INFO_WIDTH,INFO_HEIGHT));
      CDC deskCDC(hDesktopDC);

      RECT info_rect=INFO_RECT_WEBELE_LEFT;
      if(PtInRect(&INFO_RECT_WEBELE_LEFT,mouse_point)){
        if (INFO_RECT_WEBELE_RIGHT.left==0){
          SIZE s={};
          s.cx = (LONG)::GetSystemMetrics( SM_CXFULLSCREEN );
          s.cy = (LONG)::GetSystemMetrics( SM_CYFULLSCREEN );
          INFO_RECT_WEBELE_RIGHT.left  = s.cx-INFO_WIDTH-INFO_OFFSET;
          INFO_RECT_WEBELE_RIGHT.right = s.cx-INFO_OFFSET;
        }
        info_rect=INFO_RECT_WEBELE_RIGHT;
      }
      FillRect(hDesktopDC,&info_rect,hbrush);//draw bmp background
      _6bees_gdi::_6BeeDrawRect(deskCDC,info_rect,true,0);// draw border - todo
      SetBkMode(hDesktopDC,TRANSPARENT);

      HFONT boldfont = ::CreateFont(14,0,0,0,FW_BOLD,false,FALSE,FALSE,DEFAULT_CHARSET,OUT_STRING_PRECIS,
        CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));
      SelectObject(hDesktopDC, boldfont);
      SelectObject(hDesktopDC, hPrevPen);
      SelectObject(hDesktopDC, hPrevBrush);

      const static int FONTSIZE=14;
      TextOut(hDesktopDC,info_rect.left+TEXT_OFFSET,TEXT_OFFSET,webele_cap,(int)wcslen(webele_cap));
      TextOut(hDesktopDC,info_rect.left+TEXT_OFFSET,TEXT_OFFSET+FONTSIZE,info_exit,(int)wcslen(info_exit));
      TextOut(hDesktopDC,info_rect.left+TEXT_OFFSET,TEXT_OFFSET+FONTSIZE*2,capinfo,capinfo.GetLength());

      ::DeleteObject(boldfont);
      ::DeleteObject(hPrevBrush);
      ::DeleteObject(hPrevPen);
      ::DeleteObject(gpen);
      ::ReleaseDC (hdesktop, hDesktopDC);
      ::ReleaseDC (hcurrentwnd, hWindowDC);
    }
    return 0;
  }


  void StopMouseHook(HWND h){
    _6bees_window::RefreshWindow(GetDesktopWindow());// how to invalidate the desktop?
    _6bees_window::RefreshWindow(h);
    if(!UnInstallMouseHook()){
    }
  }

  BOOL GeneratePicFromWebElement(
    c6beewb2ptr* _pwb,
    const RECT& caprect,
    const ATL::CString& AbsPicFilename,
    const ATL::CString& AbsPicFilename_thumb,
    bool imghasborder,
    bool imghasshadow,
    COLORREF bcolor)
  {
    static const short margin = 2, IMAGE_BITCOUNT = 24;
    IHTMLDocument2Ptr& pHtmlDoc2 = _pwb->GetHTMLDoc2();
    if (!pHtmlDoc2){
      return FALSE;
    }
    BOOL result = true;
    int sys_vcxscroll = GetSystemMetrics(SM_CXVSCROLL);
    int img_width = caprect.right - caprect.left;
    int img_height= caprect.bottom - caprect.top;

    // IViewObject2 is only supported by activex control??
    IHTMLWindow2Ptr pHtmlw2(NULL);//To execute scroll!
    if (FAILED(pHtmlDoc2->get_parentWindow(&pHtmlw2))){
      return FALSE;
    }
    CComQIPtr<IViewObject,&IID_IViewObject> pRender(pHtmlDoc2);
    if (pRender==NULL){ return FALSE; }

    IHTMLElementPtr  pElementBody = (IHTMLElementPtr) NULL;
    if (FAILED(pHtmlDoc2->get_body(&pElementBody))){return FALSE;}
    if (!pElementBody) {return FALSE;}

    IHTMLElement* par_html = NULL;
    pElementBody->get_parentElement(&par_html);// the <html> element

    CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> apHTMLElement2(par_html);
    CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> apBodyElement2(pElementBody);

    bool isCompat=true;
    CComQIPtr<IHTMLDocument5> pDoc5 = pHtmlDoc2;
    if (!pDoc5)  isCompat = true;
    BSTR bstrCompatMode;
    if (FAILED(pDoc5->get_compatMode(&bstrCompatMode)))  isCompat = true;
    if (wcscmp(bstrCompatMode, _T("CSS1Compat")) == NULL)  isCompat = false;
    //method 3: scrolling and capture(sigh!)
    // todo:
    // hide 3D border
    /*
    CComQIPtr<IHTMLBodyElement,&IID_IHTMLBodyElement> pBody(pElementBody);
    if(pBody==NULL) return FALSE;
    IHTMLStyle* pStyle=NULL;
    HRESULT hr = pElementBody->get_style(&pStyle);
    if (SUCCEEDED(hr)) {
    pStyle->put_borderStyle(CComBSTR("none"));
    pStyle->Release();
    }
    // hide scrollbars
    // testing this has problem: http://auto.163.com/photoset/008V0008/10666.html
    // put_scroll may not work for BHO, so discard this.
    hr = pBody->put_scroll(CComBSTR("no"));
    if (FAILED(hr)){
    //OutputDebugStringA("put scroll failed.");
    ::MessageBoxA(NULL,"put scroll failed.","",NULL);
    return FALSE;
    }
    */

    RECT ClientAreaRect={};
    HWND hWnd_HTMLClientArea=_pwb->GetCurrentTab();
    ::GetWindowRect(hWnd_HTMLClientArea,&ClientAreaRect);

    RECT tmpret={};
    int statusbarheight(0),shuipinbarheight(0);
    _6bees_window::IE7::GetBarHeight(hWnd_HTMLClientArea,statusbarheight,shuipinbarheight);

    long sw=0,ow=0,sh=0;
    if(FAILED(apBodyElement2->get_scrollWidth(&sw))||
      FAILED(pElementBody->get_offsetWidth(&ow))||
      FAILED(apBodyElement2->get_scrollHeight(&sh)))
    {return FALSE;}

    long ClientAreaHeight = ClientAreaRect.bottom-ClientAreaRect.top;
    long ClientAreaWidth  = ClientAreaRect.right -ClientAreaRect.left;

    ImageRender ir2(img_width,img_height);
    CDC renderDC2 = ir2.GetDC();

    long oh = ClientAreaHeight- statusbarheight - (sw>ow?sys_vcxscroll:0) - shuipinbarheight;
    ow = ClientAreaWidth - sys_vcxscroll - 2*margin;

    long tmpsl=0,tmpst=0;
    if (isCompat){
      if (FAILED(apBodyElement2->get_scrollLeft(&tmpsl))){return false;}
      if (FAILED(apBodyElement2->get_scrollTop(&tmpst))){return false;}
    }else{
      if (FAILED(apHTMLElement2->get_scrollLeft(&tmpsl))){return false;}
      if (FAILED(apHTMLElement2->get_scrollTop(&tmpst))){return false;}
    }
    // A
    //////////////////////////////////////////////////////////////////////////
    if(img_width<=ow && img_height<=oh){
      if(caprect.right>=ow && caprect.top<0){//1
        pHtmlw2->scrollTo((caprect.right-ow)+tmpsl,caprect.top+tmpst-margin);
        if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect,
          0,0,img_width,img_height,caprect.left-(caprect.right-ow),margin)){ return false; }
      }
      else if(caprect.left>=0 && caprect.right<=ow && caprect.top<0){//2
        pHtmlw2->scrollTo(tmpsl,caprect.top+tmpst-margin);
        if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect,
          0,0,img_width,img_height,caprect.left,margin)){ return false; }
      }
      else if(caprect.left<0 && caprect.top<0){//3
        pHtmlw2->scrollTo(tmpsl+caprect.left-margin,tmpst+caprect.top-margin);
        if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect,
          0,0,img_width,img_height,margin,margin)){ return false; }
      }
      else if(caprect.bottom>oh && caprect.left<=0){//4
        pHtmlw2->scrollTo(tmpsl+caprect.left-margin,(caprect.bottom-oh)+tmpst+margin);
        if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect,
          0,0,img_width,img_height,margin,caprect.top-(caprect.bottom-oh)-margin)){ return false; }
      }
      else if(caprect.left>=0 && caprect.right<=ow && caprect.bottom>oh){//5
        pHtmlw2->scrollTo(tmpsl,tmpst+(caprect.bottom-oh)+margin);
        if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect,
          0,0,img_width,img_height,caprect.left,caprect.top-(caprect.bottom-oh)-margin)){ return false; }
      }
      else if(caprect.right>ow && caprect.bottom>oh){//6
        pHtmlw2->scrollTo(tmpsl+(caprect.right-ow),tmpst+(caprect.bottom-oh)+margin);
        if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect,
          0,0,img_width,img_height,caprect.left-(caprect.right-ow),caprect.top-(caprect.bottom-oh)-margin)){ return false; }
      }
      else if(caprect.left<0 && caprect.bottom<=oh && caprect.top>=0){//7
        pHtmlw2->scrollTo(tmpsl+caprect.left-margin,tmpst);
        if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect,
          0,0,img_width,img_height,margin,caprect.top)){ return false; }
      }
      else if(caprect.right>ow && caprect.bottom<=oh && caprect.top>=0){//9
        pHtmlw2->scrollTo(tmpsl+(caprect.right-ow),tmpst);
        if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect,
          0,0,img_width,img_height,caprect.left-(caprect.right-ow),caprect.top)){ return false; }
      }else{//8
        if(!_6bees_gdi::_6BeeDraw(renderDC2,ow,oh,sw,sys_vcxscroll,margin,pRender,caprect,0,0,img_width,img_height,caprect.left,caprect.top)){ return false; }
      }

    }else if(img_width<ow && img_height>oh){
      // B
      //////////////////////////////////////////////////////////////////////////
      int t = 5;//must less than oh!
      int i=(int)ceil((img_height*1.0)/(oh-t));
      if( caprect.right>ow){//1==9==6
        for (int j=0;j<i;j++){
          int a=tmpsl+(caprect.right-ow);
          int b=tmpst+caprect.top+j*(oh-t)-margin;
          pHtmlw2->scrollTo(a,b);
          int stop=j*(oh-t);
          if(b+oh>sh){stop=stop+sh-(b+oh)+margin*2;}
          if(!_6bees_gdi::_6BeeDraw(renderDC2,ow,oh,sw,sys_vcxscroll,margin,pRender,caprect,0,stop,ow,oh,caprect.left-(caprect.right-ow),margin)){ return false; }
        }
      }
      else if(caprect.right<=ow && caprect.left>=0){//2==8==5
        for (int j=0;j<i;j++){
          int a=tmpsl;
          int b=tmpst+caprect.top-margin+j*(oh-t);
          pHtmlw2->scrollTo(a,b);
          int stop=j*(oh-t);
          if(b+oh>sh){stop=stop+sh-(b+oh)+margin*2;}
          if(!_6bees_gdi::_6BeeDraw(renderDC2,ow,oh,sw,sys_vcxscroll,margin,pRender,caprect,0,stop,ow,oh,caprect.left,margin)){ return false; }
        }
      }
      else if(caprect.left<=0 && caprect.right<=ow){//3==7==4
        for (int j=0;j<i;j++){
          int a=tmpsl+caprect.left-margin;
          int b=tmpst+caprect.top-margin+j*(oh-t);
          pHtmlw2->scrollTo(a,b);
          int stop=j*(oh-t);
          if(b+oh>sh){stop=stop+sh-(b+oh)+margin*2;}
          if(!_6bees_gdi::_6BeeDraw(renderDC2,ow,oh,sw,sys_vcxscroll,margin,pRender,caprect,0,stop,ow,oh,margin,margin)){ return false; }
        }
      }
    }else if(img_width>=ow && img_height<=oh){
      // C
      //////////////////////////////////////////////////////////////////////////
      int t = 5;//must less than ow!!!!
      int i=(int)ceil((img_width*1.0)/(ow-t));
      if(caprect.top<=0 && caprect.bottom<=oh){//1==4==7
        for (int j=0;j<i;j++){
          int toleft=tmpsl+caprect.left-margin+j*(ow-t);
          pHtmlw2->scrollTo(toleft,tmpst+caprect.top-margin);
          int sleft=j*(ow-t);
          if(toleft + ow >sw){ sleft = sleft+sw-(toleft+ow); }
          if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect, sleft,0,ow,oh,margin,margin)){ return false; }
        }
      }
      else if(caprect.top>0 && caprect.bottom<=oh){//2==5==8
        for (int j=0;j<i;j++){
          int toleft=tmpsl+caprect.left-margin+j*(ow-t);
          pHtmlw2->scrollTo( toleft,tmpst );
          if (isCompat){
            if (FAILED(apBodyElement2->put_scrollLeft(toleft))){return false;}
          }else{
            if (FAILED(apHTMLElement2->put_scrollLeft(toleft))){return false;}
          }
          int sleft=j*(ow-t);
          if(toleft + ow >sw){ sleft = sleft+sw-(toleft+ow); }
          if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect, sleft,0,ow,oh,margin,caprect.top)){ return false; }
        }
      }
      else if(caprect.bottom>=oh){//3==6==9
        for (int j=0;j<i;j++){
          int a = tmpsl+caprect.left-margin+j*(ow-t);
          int b = (caprect.bottom-oh)+tmpst+margin;
          pHtmlw2->scrollTo(a,b);
          int sleft=j*(ow-t);
          if(a + ow >sw){ sleft = sleft+sw-(a+ow); }
          if(!_6bees_gdi::_6BeeDraw(renderDC2,ow,oh,sw,sys_vcxscroll,margin,pRender,caprect,
            sleft,0,ow,oh,margin,caprect.top-(caprect.bottom-oh)-margin)){ return false; }
        }
      }
    }else if(img_width>=ow && img_height>=oh){
      int t=10;//must less than ow!!!!
      int ii=(int)ceil((img_width *1.0)/(ow-t));
      int jj=(int)ceil((img_height*1.0)/(oh-t));
      for(int w=0;w<ii;w++){//width
        for (int h=0;h<jj;h++){//height
          int a = tmpsl+ caprect.left+ (caprect.left>0?margin:(-1*margin))+ w*(ow-t);
          int b = tmpst+ caprect.top + (caprect.top >0?margin:(-1*margin))+ h*(oh-t);
          pHtmlw2->scrollTo(a,b);
          int sleft=w*(ow-t),stop=h*(oh-t);
          if(a+ow>sw){ sleft=sleft+sw-a-ow; }
          if(b+oh>sh){ stop =stop +sh-b-oh+margin*2; }//i donot know why need to +margin*2, but this can work OK....
          if(!_6bees_gdi::_6BeeDraw(renderDC2, ow, oh, sw, sys_vcxscroll, margin, pRender, caprect,
            sleft,stop,ow,oh,margin,margin)){ return false; }
        }
      }
    }
    //pHtmlw2->scrollTo(tmpsl,tmpst);
    //pBody->put_scroll(CComBSTR("auto"));

    // 1.Generate Thumb
    RECT hdcRect={0,0,img_width,img_height};
    _6bees_gdi::GenerateThumb(AbsPicFilename_thumb,renderDC2.m_hDC,hdcRect);

    // 2.Generate Real Image
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

    CLSID clsid;
    GetEncoderClsid(_6bees_gdi::_6BeeGetImgFormat(L".png"), &clsid);
    imgIR.SaveToFile(AbsPicFilename.GetString(), &clsid);
    return result;
  }


  static const int RIPOFF_BIN=0,RIPOFF_TXT=1,RIPOFF_IMGINJSCSS=2;
  static const char* reArray[4]={
    "(<img\\s+.*?src=[\"|']?([^>\"'\\s]+)[\"|']?.*?\\s*/?>)", // image in HTML
    "(<link\\s+.*?href=[\"|']?([^>\"'\\s]+)[\"|']?.*?\\s*/?>)",// css,ico,js in HTML
    "(url\\(['|\"]?(.*?)['|\"]?\\))"// image in js, css, HTML(style script also can in HTML)
  };

  bool process_wholepage(int& progress,
    string& webpage,
    const string& wstrURL,
    const string& hostname, /// http://www.ultraie.com
    const string& mstr)
  {
    using namespace _6bees_util;
    using namespace pcrepp;
    progress = 5;
    if(!process_webpage_imgcssjs(webpage,wstrURL,hostname,mstr,RIPOFF_TXT))
      return false;//rip off CSS,JS
    progress = 10;
    if(!process_webpage_imgcssjs(webpage,wstrURL,hostname,mstr,RIPOFF_BIN))
      return false;//rip off pictures
    progress = 80;
    if(!process_webpage_imgcssjs(webpage,wstrURL,hostname,mstr,RIPOFF_IMGINJSCSS))
      return false;//rip off pictures
    Pcre reg4("<a\\s+.*?href=([\"|']?)([^>\"'\\shttp]+)([\"|']?.*?\\s*/?)>",PCRE_CASELESS);
    string replace_str = stringmaker() << "<a href=$1" << hostname << "/$2$3>";
    webpage = reg4.replace(webpage, replace_str);
    progress = 100;

    /// @todo  - force all pages are UTF-8 encoding
    wstring fp = ultrapath::R().get(ultrapath::webcontentsdir);
    MakeSureDir(fp.c_str());
    string chfname = stringmaker() << fp << mstr << ".html";
    if (!webpage.empty() && !PathFileExistsA(chfname.c_str())){
      wstring wp = wstringmaker() << webpage;
      wstring fn = wstringmaker() << chfname;
      if (!WriteCharToFile_UTF8(wp.c_str(),fn.c_str(),true)){
        string info = "Cannot write to file:\n" + chfname;
        ::MessageBoxA(NULL,info.c_str(),"ERROR",NULL);
      }
    }
    return true;
  }

  bool process_webpage_imgcssjs(
    string& webpage,
    const string& wstrURL,
    const string& hostname,
    const string& DirName,
    int type)
  {
#ifndef __6BEEUPLOADER__
    using namespace _6bees_util;
    using namespace pcrepp;
    if (type>2 || !startwith<string>(wstrURL,"http://")){//explicity instantiation
      return false;
    }

    wstring fp = ultrapath::R().get(ultrapath::webcontentsdir);
    MakeSureDir(fp.c_str());

    string ResDir = stringmaker() << fp << DirName;
    map<string,ripthing> ripinfo_map;

    Pcre re(reArray[type],PCRE_CASELESS);
    int pos = 0;
    int html_size = (int)webpage.size();
    while((pos<=html_size) && re.search(webpage,pos) ){
      !PathFileExistsA(ResDir.c_str()) && _mkdir(ResDir.c_str());
      pos = re.get_match_end(0);
      string imgmark_ = re.get_match(0);
      string picurl_ = re.get_match(1);
      string ori_picurl(picurl_),picurl(picurl_);
      if (!startwith(picurl_,string("http://"))){/// if use relative path
        if (picurl_[0]=='/'){
          picurl = hostname + picurl;
        }else{
          _6bees_net::apr_uri_t t;
          if(_6bees_net::_6beeparseuri(wstrURL.c_str(),t)){
            string p = t.path;
            p = p.substr(0,p.find_last_of('/')+1);
            if (p=="/"){
              picurl = stringmaker() << "http://" << t.hostinfo << "/" << picurl;
            }else{
              picurl = stringmaker() << "http://" << t.hostinfo << "/" << p << picurl;
            }
            ProcessURLA(picurl);
          }else{
            return false;
          }
          /*string tmpurl;
          int pos2=0;
          Pcre re2("(http://[^\\?|#]+)[\\?|#]?.*?",PCRE_CASELESS);
          if(re2.search(wstrURL,pos2)){
            string _tmpurl = re2.get_match(0);
            size_t tmp = _tmpurl.rfind("/");
            if (tmp!=6 && tmp!=_tmpurl.size() && tmp!=string::npos){
              _tmpurl = _tmpurl.substr(0,tmp);
            }
            tmpurl = _tmpurl;
          }
          picurl = tmpurl + "/" + picurl;
          ProcessURLA(picurl);*/
        }
      }
      /// if it is a new url
      if(ripinfo_map.find(picurl)==ripinfo_map.end()){
        char ws_layer[10]={};
        sprintf_s(ws_layer,10,"%lu",(unsigned long)picurl.rfind("/"));
        string picname = ws_layer + picurl.substr(picurl.rfind("/")+1,picurl.size());
        /// replace picture name like abc.png?x=1&y=2#&c=^
        static const char* const human_format = "_";
        Pcre re3("[^\\w\\.]+");
        string realpicname = re3.replace(picname,human_format);
        string ext = GetFileExt(picurl);
        bool isbinary = (ext!="css" && ext!="js");
        ripthing rps={
          isbinary,
          imgmark_,    //html
          ori_picurl,  //relative path
          realpicname};//new name
        ripinfo_map[picurl] = rps;
      }
    }

    for (map<string,ripthing>::iterator i=ripinfo_map.begin();i!=ripinfo_map.end();++i){
      const string& picurl     = i->first;
      const string& imgmark    = i->second.html;
      const string& ori_picurl = i->second.relativepath;
      const string& picname    = i->second.newname;
      bool isbinary      = i->second.isbinary;

      string picpath = ResDir + "\\" +picname;
      string picpath_relative = DirName + "/" + picname;

#if 0
      bool result=false;
      /// @todo - download all images or only use image links
      if (false){
        if (isbinary){
          result = (DownloadURLToFile(picurl.c_str(),picpath.c_str()) >= 0);
        }else{
          char* buffer=NULL;
          if(result=(DownloadURLToFile(picurl.c_str(),picpath.c_str(),buffer) >= 0)){
            wstring wjs_css_html = wstringmaker() << buffer;
            delete [] buffer;
            process_webpage_imgcssjs(wjs_css_html,picurl,hostname,RIPOFF_IMGINJSCSS,DirName);
          }
        }
      }
      if(result){
        wstring y = strreplace(imgmark,ori_picurl.c_str(),picpath_relative.c_str());
        webpage = strreplace(webpage,imgmark.c_str(),y.c_str());
      }else{
        wstring y = strreplace(imgmark,ori_picurl.c_str(),picurl.c_str());
        webpage = strreplace(webpage,imgmark.c_str(),y.c_str());
      }
#endif

      string y = imgmark;
      strreplace(y,ori_picurl.c_str(),picurl.c_str());
      strreplace(webpage,imgmark.c_str(),y.c_str());
    }
#endif
    return true;
  }


  void getFavicon(IHTMLDocument2Ptr& pHtmlDoc2,const wchar_t* scheme,wstring& faviconURL){
    // 1. get favicon from HTML source
    IHTMLElementCollectionPtr pElemColl(NULL);
    if(SUCCEEDED(pHtmlDoc2->get_all(&pElemColl)) ){
      // Obtained element collection.
      long count = 0;
      HRESULT hr = pElemColl->get_length( & count );
      if (FAILED(hr))
        return;
      for (int i=0;i<count;i++){
        CComVariant varName(i);   
        varName.ChangeType(VT_UINT);   
        CComVariant varIndex;
        CComPtr<IDispatch>pDisp;
        if(SUCCEEDED(pElemColl->item(varName,varIndex,&pDisp))){
          CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElement(pDisp);
          if(pElement.p){
            CComBSTR bstrTag;
            pElement->get_tagName(&bstrTag);
            wstring tagName(bstrTag);
            LOWERWSTR(tagName);
            if (tagName == L"link"){
              CComVariant comrel;
              pElement->getAttribute(_T("rel"),0,&comrel);
              wstring rel = ComVar2wstr(comrel);
              LOWERWSTR(rel);
              if ((rel == L"shortcut icon") || (rel == L"icon")){
                CComVariant faviconhref;
                if(SUCCEEDED(pElement->getAttribute(_T("href"),0,&faviconhref))){
                  wstring favicon=ComVar2wstr(faviconhref);
                  if (favicon.substr(0,7)==scheme){
                    faviconURL = favicon;
                  }else{
                    CComBSTR turl;
                    pHtmlDoc2->get_URL(&turl);
                    wstring tmpURL=turl;
                    size_t a=tmpURL.find_last_of(L'/');
                    tmpURL=tmpURL.substr(0,a+1);
                    tmpURL += favicon;
                    ProcessURL(tmpURL);
                    faviconURL=tmpURL;
                  }                
                  return;
                }
              }
            }else if(tagName==L"body"){
              break;
            }
          }
        }
      }
    }
    if (faviconURL.empty()){
      // 2. get favicon from http://domain/favicon.ico
      CComBSTR tdomain;
      pHtmlDoc2->get_domain(&tdomain);
      if (tdomain.m_str!=NULL){//when the url cannot be display, this value is NULL.
        faviconURL = wstringmaker() << scheme << L"://" << tdomain.m_str << L"/favicon.ico";
      }
    }
  }

  int isFaviconValid(const char* url){
    using namespace _6bees_str;
    _6bees_net::apr_uri_t t;
    if(_6bees_net::_6beeparseuri(url,t)){
      _6bees_socket::httpclient _httpclient;
      _httpclient.ConnectHttpServer(t.hostname,t.port);
      long a=0;
      string pObj = t.path;
      if (t.query){
        pObj.append("?").append(t.query);
      }
      _httpclient.FormatRequestHeader(t.hostname,pObj.c_str(),a,NULL,url,0,0);
      _httpclient.SendReqAndGetRespHeader();
      int status=_httpclient.GetServerState();
      if (status==200||status==304){
        char ct[100]={};
        int result=_httpclient.GetField("Content-Type",ct,100);
        if (result != -1){
          std::string contentT=ct;
          if((contentT.size()>6 && startwith<string>(contentT,"image/")) ||
            (contentT.size()>10 && startwith<string>(contentT,"text/plain")) ||
            (contentT.size()>24 && startwith<string>(contentT,"application/octet-stream"))
            )
          {
            char iconsize[20]={};
            if(_httpclient.GetField("Content-Length",iconsize,20) != -1){
              return atoi(iconsize);
            }
          }
        }
      }
    }
    return 0;
  }

  int getValidFavicon(const string& _url,string& realurl){
    _6bees_net::apr_uri_t t;
    if(_6bees_net::_6beeparseuri(_url.c_str(),t)){
      int iconsize=0;
      if (_url.substr(4) == ".ico"){
        if ((iconsize = isFaviconValid(_url.c_str()))>0){
          realurl = _url;
          return iconsize;
        }
      }
      string url2;
      url2 = stringmaker()<<t.scheme<<"://"<<t.hostname<<":"<< t.port_str<<"/favicon.ico";
      if ((iconsize=isFaviconValid(url2.c_str()))>0){
        realurl = url2;
      }
      return iconsize;
    }
    return -1;
  }

  bool getcurrenttitle2(IHTMLDocument2Ptr& pHtmlDoc2,wstring& title_ACP){
    if(pHtmlDoc2.GetInterfacePtr()==NULL) return false;
    CComBSTR t;//Passing the address of an initialized CComBSTR to a function as an [out] parameter causes a memory leak!!!
    if (FAILED(pHtmlDoc2->get_title(&t))){
      return NULL;
    }
    title_ACP  = (wchar_t*)t;
    return true;
  }

  bool GetHtmlDocument2(IWebBrowser2* pwb_,IHTMLDocument2Ptr& pHtmlDoc2){
    if(pwb_==NULL){return false;}
    LPDISPATCH pDispatch = 0;
    if (FAILED(pwb_->get_Document(&pDispatch))){
      return false;
    }
    pHtmlDoc2=pDispatch;
    if(pDispatch)
      pDispatch->Release();
    return true;
  }

  //TO Be Deleted - IHTMLElement* c6beewb2ptr::GetElementByTagName(const char* _name)
  void getBodyText(IHTMLDocument2Ptr& pHtmlDoc2,wstring& bodyText,int maxtextnumber){
    IHTMLElementCollectionPtr pElemColl(NULL);
    if(SUCCEEDED(pHtmlDoc2->get_all(&pElemColl)) ){
      long count = 0;
      HRESULT hr = pElemColl->get_length( & count );
      if (FAILED(hr))
        return;
      for (int i=0;i<count;i++){
        CComVariant varName(i);   
        varName.ChangeType(VT_UINT);   
        CComVariant varIndex;
        CComPtr<IDispatch>pDisp;
        if(SUCCEEDED(pElemColl->item(varName,varIndex,&pDisp))){
          CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElement(pDisp);
          if(pElement.p){
            CComBSTR bstrTag;
            pElement->get_tagName(&bstrTag);
            bstrTag.ToLower();
            if (bstrTag=="body"){
              CComBSTR bt;
              pElement->get_innerText(&bt);
              bodyText = bt;
              if (maxtextnumber>0 && bodyText.size()>(size_t)maxtextnumber){
                bodyText = bodyText.substr(0,maxtextnumber-3) + L"...";
              }
              return;
            }
          }
        }
      }
    }
  }

};