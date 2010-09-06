#include "resource.h"
#include "H_HBarWnd.h"
#include <MsHTML.h>
#include <string>
#include <6bees_util.h>
#include <6bees_str.h>
#include <6bees_const.h>
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#include <atltypes.h>//CRect

using namespace _6bees_util;
using namespace _6bees_const;
using namespace _6bees_str;

#define HBAR_CLOSEHIGHLIGHT   1001
#define HBAR_SEARCHNEXTFAILED 1002
#define HBAR_SEARCHPREVFAILED 1003

#define HBAR_ICASE 1111

#define kGreen RGB(240,255,240)
#define kRed    RGB(255,90,90)

const int IDM_EDITBOX = 19792;

CH_HBarWnd::CH_HBarWnd():pwb_(0),isHighLight(true),totalfound(0),bodyRange(0),body(0),
currentRange(0),currentRange2(0),newpage(TRUE),m_hImageList(0){
  bkcolor = kGreen;
  HLstyle = kHighLightStyle;
  casestr_ = _T("Match Case");
}

CH_HBarWnd::~CH_HBarWnd(){
  if(hi_HL)::DestroyIcon(hi_HL);
  if(m_hBrush)::DeleteObject(m_hBrush);
  ::DeleteObject(font_edit);
  ::DeleteObject(font_case);
  SetBrowser(NULL);
  if (IsWindow()){
    DestroyWindow();
  }
}

LRESULT CH_HBarWnd::OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  // Draw Search Buttons
  SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_MIXEDBUTTONS);
  SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
  SendMessage(m_hWnd, TB_SETMAXTEXTROWS, 1, 0L);
  SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

  HICON hi_down   = (HICON)c6beeres::R().GetImg(IDI_DOWN);
  HICON hi_up        = (HICON)c6beeres::R().GetImg(IDI_UP);
  HICON hi_search  = (HICON)c6beeres::R().GetImg(IDI_HSEARCH);
  hi_HL = (HICON)c6beeres::R().GetImg(IDI_HLIGHT);
  
  m_hImageList = ImageList_Create(16,16,ILC_COLOR16|ILC_MASK,8,8);
  ::SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)m_hImageList);

  SecureZeroMemory((void*)Buttons,(sizeof(Buttons)));

  int bcount=0;
  Buttons[bcount].iBitmap = ImageList_AddIcon(m_hImageList,hi_search);
  Buttons[bcount].fsState = TBSTATE_ENABLED;
  Buttons[bcount].fsStyle = BTNS_BUTTON | BTNS_SHOWTEXT |BTNS_AUTOSIZE ;
  Buttons[bcount].iString = (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0,(LPARAM)_T(""));
  Buttons[bcount].idCommand = IDM_H_LOGO;

  ++bcount;// SEP
  Buttons[bcount].fsStyle = TBSTYLE_SEP;

  ++bcount;// SEP
  Buttons[bcount].fsStyle = TBSTYLE_SEP;
  Buttons[bcount].idCommand = IDM_EDITBOX;

  ++bcount;// SEP
  Buttons[bcount].fsStyle = TBSTYLE_SEP;

  ++bcount;
  Buttons[bcount].iBitmap = ImageList_AddIcon(m_hImageList,hi_down);
  Buttons[bcount].fsState = TBSTATE_ENABLED /*| TBSTATE_ELLIPSES*/;
  Buttons[bcount].fsStyle = BTNS_BUTTON | BTNS_SHOWTEXT |BTNS_AUTOSIZE ;
  Buttons[bcount].iString = (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0,(LPARAM)_T("Next"));
  Buttons[bcount].idCommand = IDM_H_NEXT;

  ++bcount;
  Buttons[bcount].iBitmap = ImageList_AddIcon(m_hImageList,hi_up);
  Buttons[bcount].fsState = TBSTATE_ENABLED;
  Buttons[bcount].fsStyle = BTNS_BUTTON | BTNS_SHOWTEXT |BTNS_AUTOSIZE;
  Buttons[bcount].iString = (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0,(LPARAM)_T("Previous"));
  Buttons[bcount].idCommand = IDM_H_PREV;

  ++bcount;
  Buttons[bcount].iBitmap = ImageList_AddIcon(m_hImageList,hi_HL);
  Buttons[bcount].fsState = TBSTATE_ENABLED|TBSTATE_PRESSED;
  Buttons[bcount].fsStyle = BTNS_BUTTON | BTNS_SHOWTEXT |BTNS_AUTOSIZE ;
  Buttons[bcount].iString = (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0,(LPARAM)_T("HighLight All"));
  Buttons[bcount].idCommand = IDM_H_HIGHTLIGHT;

  DestroyIcon(hi_down);
  DestroyIcon(hi_up);
  DestroyIcon(hi_HL);
  DestroyIcon(hi_search);

  ::SendMessage(m_hWnd, TB_SETPADDING, 0, MAKELONG(8,6));
  ::SendMessage(m_hWnd, TB_ADDBUTTONS, kbtnnum, (LPARAM)&Buttons);
  
  unsigned short TBLogoWidth = (_6beed_util::IsXPTheme() ? 18+10 : 18+5);
  {
    TBBUTTONINFO tbi2;
    tbi2.cbSize = sizeof(TBBUTTONINFO);
    tbi2.dwMask = TBIF_SIZE | TBIF_STYLE;
    tbi2.fsStyle = Buttons[0].fsStyle;
    tbi2.cx = TBLogoWidth;
    SendMessage(m_hWnd, TB_SETBUTTONINFO, IDM_H_LOGO, (LPARAM)&tbi2);
  }
  {
    TBBUTTONINFO tbi;
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_SIZE | TBIF_STYLE;
    tbi.fsStyle = TBSTYLE_SEP;
    tbi.cx = 300;
    SendMessage(m_hWnd, TB_SETBUTTONINFO, IDM_EDITBOX, (LPARAM)&tbi);
  }

  CRect rect;
  GetItemRect(2, &rect);
  m_BandEditCtrl.Create(m_hWnd, rect, NULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_WANTRETURN|ES_AUTOHSCROLL,WS_EX_CLIENTEDGE);
  font_edit = ::CreateFont(16,0,0,0,FW_BOLD,false,FALSE,FALSE,DEFAULT_CHARSET,OUT_STRING_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));
  m_BandEditCtrl.SetFont(font_edit);
  RegisterDragDrop(m_BandEditCtrl.m_hWnd,  (LPDROPTARGET)&m_BandEditCtrl);

  rect.left  = rect.right + 250;
  rect.right = rect.left + 300;

  font_case = ::CreateFont(14,0,0,0,FW_THIN,false,FALSE,FALSE,DEFAULT_CHARSET,OUT_STRING_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("arial"));
  m_case.Create(m_hWnd,rect,casestr_.c_str(),WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX);
  m_case.SetFont(font_case);
  return 0;
}

void    CH_HBarWnd::SetBrowser(IWebBrowser2* pBrowser){
  if (pwb_)
    pwb_->Release();
  pwb_ = pBrowser;
  if (pwb_)
    pwb_->AddRef();
}

LRESULT CH_HBarWnd::OnCommand(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  if (!HIWORD(wParam)){
    long lSite = LOWORD(wParam);
    switch (lSite){
    case IDM_H_NEXT:
      ::PostMessage(m_hWnd, WM_HBAR_SEARCH_NEXT, 0, 0);
    	break;
    case IDM_H_PREV:
      ::PostMessage(m_hWnd, WM_HBAR_SEARCH_PREV, 0, 0);
      break;
    case IDM_H_HIGHTLIGHT:
      if (!isHighLight){
        ::SendMessage(m_hWnd, TB_SETSTATE, IDM_H_HIGHTLIGHT, (LPARAM)MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED,0));
        ::SendMessage(m_hWnd, TB_CHANGEBITMAP, IDM_H_HIGHTLIGHT, (LPARAM)MAKELONG(ImageList_AddIcon(m_hImageList,hi_HL),0));
        isHighLight = true;
        newpage = TRUE;
        ::PostMessage(m_hWnd, kWM_HBarCOMBOSEARCH, 0, 0);
      }else{
        //trun off hightlight
        HICON ico_NOHL=(HICON)c6beeres::R().GetImg(IDI_IS_HL);
        ::SendMessage(m_hWnd, TB_CHANGEBITMAP, IDM_H_HIGHTLIGHT, (LPARAM)MAKELONG(ImageList_AddIcon(m_hImageList,ico_NOHL),0));
        ::SendMessage(m_hWnd, TB_SETSTATE, IDM_H_HIGHTLIGHT, (LPARAM)MAKELONG(TBSTATE_ENABLED,0));
        ::PostMessage(m_hWnd, kWM_HBarCOMBOSEARCH, HBAR_CLOSEHIGHLIGHT, HBAR_CLOSEHIGHLIGHT);
        isHighLight = false;
        DestroyIcon(ico_NOHL);
      }
      break;
    default:
      if (uMsg==273){//BN_CLICKED
        bool icase = (m_case.GetCheck()==BST_UNCHECKED);
        ::PostMessage(m_hWnd, kWM_HBarCOMBOSEARCH, HBAR_ICASE, (icase?0:1));
      }
      break;
    }
  }
  return -1;
}

LRESULT CH_HBarWnd::OnWM_SHOWWINDOW(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  return 0;
}

void CH_HBarWnd::SetInfo(int count ,const wchar_t* sw){
  if(count>0)
    casestr_ = wstringmaker() << L"Match Case        (Found "<< count << L" \"" << sw << L"\" in this page.)";
  else
    casestr_ = L"Match Case";
  m_case.SetWindowText(casestr_.c_str());
}

LRESULT CH_HBarWnd::OnHBarSearch(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  newpage = FALSE;
  CComBSTR search_words;
  if (!::IsWindow(m_BandEditCtrl.m_hWnd)){
    return 0;
  }
  m_BandEditCtrl.GetWindowText(&search_words);
  wstring sw = (search_words.m_str==NULL) ? L"" : search_words.m_str;

  IHTMLDocument2Ptr pHtmlDoc2 = this->GetHtmlDocument();
  if (pHtmlDoc2==NULL || FAILED(pHtmlDoc2->get_body(&body))){
    return 0;
  }
  CComQIPtr<IHTMLBodyElement,&IID_IHTMLBodyElement> bodyElement(body);
  if (bodyElement.p==NULL){
    return 0;
  }

  bool icase;
  if (wParam==HBAR_ICASE){
    icase = lParam==0;
  }else{
    icase = (m_case.GetCheck()==BST_UNCHECKED);
  }

  if(sw.empty()){
    if(!prev_search_words.empty())
      highLight(pHtmlDoc2, prev_search_words, DELSTYLE, HLstyle, isHighLight,icase);
    prev_search_words = sw;
    SetInfo(0);
    return 0;
  }
  if(lParam==HBAR_SHOWWINDOW && wParam==HBAR_SHOWWINDOW){//close window
    if(!prev_search_words.empty()){
      highLight(pHtmlDoc2, prev_search_words, DELSTYLE, HLstyle, isHighLight, icase);
      prev_search_words = _T("");
      return 0;
    }
    if(!sw.empty()){
      highLight(pHtmlDoc2, sw, DELSTYLE, HLstyle, isHighLight,icase);
      prev_search_words = _T("");
      return 0;
    }
  }else if(lParam==HBAR_CLOSEHIGHLIGHT && wParam==HBAR_CLOSEHIGHLIGHT){//turn off highlight
    if(!prev_search_words.empty()){
      highLight(pHtmlDoc2, prev_search_words, DELSTYLE, HLstyle, TRUE ,icase);
      prev_search_words = _T("");
      return 0;
    }
    if(!sw.empty()){
      highLight(pHtmlDoc2, sw, DELSTYLE, HLstyle, true,icase);
      prev_search_words = _T("");
      return 0;
    }
  }else if(!prev_search_words.empty()){
    if (wParam==HBAR_ICASE){
      highLight(pHtmlDoc2, sw, DELSTYLE, HLstyle, isHighLight,!icase);
      totalfound = highLight(pHtmlDoc2, sw, ADDSTYLE, HLstyle, isHighLight,icase);
      SetInfo(totalfound,sw.c_str());
      return true;
    }else{
      if (prev_search_words != sw){
        totalfound = highLight(pHtmlDoc2, prev_search_words, DELSTYLE, HLstyle, isHighLight,icase);
      }
    }
  }

  if(!sw.empty()){
    VARIANT_BOOL bSuccess;
    long t=0;
    styleaction bHighLight=DELSTYLE;
    if(FAILED(bodyElement->createTextRange(&bodyRange)) && bodyRange==NULL){
      return 0;
    }
    DWORD flag = icase?0:FINDTEXT_MATCHCASE;
    if(SUCCEEDED(bodyRange->findText(search_words,0, flag,&bSuccess))){
      if( bSuccess==VARIANT_TRUE ){
        bodyRange->select();
        bodyRange->scrollIntoView(TRUE);
        bodyRange->moveStart((BSTR)CComBSTR("Character"),search_words.Length(),&t);
        bodyRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
        currentRange = bodyRange;
        bHighLight=ADDSTYLE;
      }
    }
    if(lParam!=HBAR_SEARCHNEXTFAILED || wParam!=HBAR_SEARCHNEXTFAILED){
      totalfound = highLight(pHtmlDoc2,sw,bHighLight,HLstyle,isHighLight,icase);
      SetInfo(totalfound,sw.c_str());
      if(totalfound==0){
        bkcolor = kRed;
        m_hBrush = ::CreateSolidBrush(bkcolor);
        RECT rc;
        m_BandEditCtrl.GetClientRect(&rc);
        m_BandEditCtrl.InvalidateRect(&rc,TRUE);
      }else{
        bkcolor = kGreen;
        m_hBrush = ::CreateSolidBrush(bkcolor);
        RECT rc;
        m_BandEditCtrl.GetClientRect(&rc);
        m_BandEditCtrl.InvalidateRect(&rc,TRUE);
      }
    }
    prev_search_words = sw;
  }
  return true;
}

LRESULT CH_HBarWnd::OnHBarSearchNext(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  if(newpage){
    ::PostMessage(m_hWnd, kWM_HBarCOMBOSEARCH, 0, 0);
    return 0;
  }
  if(totalfound==1) return 0;
  CComBSTR search_words = NULL;
  m_BandEditCtrl.GetWindowText(&search_words);
  VARIANT_BOOL bSuccess;
  long t=0;
  DWORD flag = (m_case.GetCheck()==BST_UNCHECKED?0:FINDTEXT_MATCHCASE);
  if(currentRange!=NULL && SUCCEEDED(currentRange->findText(search_words,0,flag,&bSuccess)) && bSuccess==VARIANT_TRUE){
    if (FAILED(currentRange->select())){
      ::PostMessage(m_hWnd, kWM_HBarCOMBOSEARCH, HBAR_SEARCHNEXTFAILED, HBAR_SEARCHNEXTFAILED);
    }
    currentRange->scrollIntoView(TRUE);
    currentRange->moveStart((BSTR)CComBSTR("Character"),search_words.Length(),&t);
    currentRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
  }else{
    ::PostMessage(m_hWnd, kWM_HBarCOMBOSEARCH, HBAR_SEARCHNEXTFAILED, HBAR_SEARCHNEXTFAILED);
  }
  return 0;
}

LRESULT CH_HBarWnd::OnHBarSearchPrev(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  if(newpage){
    return 0;
  }
  CComBSTR search_words = NULL;
  m_BandEditCtrl.GetWindowText(&search_words);
  ATL::CString sw = search_words.m_str;
  if(sw.IsEmpty()){
    return 0;
  }
  newpage = FALSE;
  if(currentRange2==NULL){
    CComQIPtr<IHTMLBodyElement,&IID_IHTMLBodyElement> bodyElement(body);
    if(FAILED(bodyElement->createTextRange(&currentRange2)) && currentRange2==NULL){return 0;}
  }
  VARIANT_BOOL bSuccess;
  long t=0;
  if(wParam == HBAR_SEARCHPREVFAILED && lParam==HBAR_SEARCHPREVFAILED){//at the beginning
    CComQIPtr<IHTMLBodyElement,&IID_IHTMLBodyElement> bodyElement(body);
    if(FAILED(bodyElement->createTextRange(&currentRange2)) && currentRange2==NULL){return 0;}
  }else{
    currentRange2->setEndPoint(_T("EndToStart"),currentRange);
    currentRange2->moveEnd(_T("Character"),-1*search_words.Length(),&t);
  }
  DWORD flag = (m_case.GetCheck()==BST_UNCHECKED?0:FINDTEXT_MATCHCASE);
  if(currentRange2!=NULL && SUCCEEDED(currentRange2->findText(search_words,-1,flag,&bSuccess)) && bSuccess==VARIANT_TRUE){
    int a=0;
    if(FAILED(currentRange2->select())){a=1;}
    currentRange->setEndPoint(_T("StartToEnd"),currentRange2);
    if(a==1){::PostMessage(m_hWnd, WM_HBAR_SEARCH_PREV, 0, 0);}
  }else{
    ::PostMessage(m_hWnd, WM_HBAR_SEARCH_PREV, HBAR_SEARCHPREVFAILED, HBAR_SEARCHPREVFAILED);
  }
  return 0;
}

//return the times the string found
UINT CH_HBarWnd::highLight(IHTMLDocument2Ptr& doc,
                              const wstring& highlightText,
                              styleaction action,//Add or Delete Style!
                              const wstring& highlightAttr,
                              bool isHighLight,// Is highligh button checked?
                              bool icase)// ignore case
{
  if(highlightText.empty()) return 0;
  int count=0;
  CComQIPtr<IMarkupServices> pMS=doc;
  CComQIPtr<IMarkupContainer> pMC=doc;
  if(pMS.p && pMC.p){
    CComPtr<IMarkupPointer> ptrBegin, ptrEnd;
    _bstr_t attr(highlightAttr.c_str());
    _bstr_t textToFind(highlightText.c_str());

    if(FAILED(pMS->CreateMarkupPointer(&ptrBegin)) ||
      FAILED(pMS->CreateMarkupPointer(&ptrEnd)) ||
      FAILED(ptrBegin->SetGravity(POINTER_GRAVITY_Right)) ||
      FAILED(ptrEnd->SetGravity(POINTER_GRAVITY_Left)) ||
      FAILED(ptrBegin->MoveToContainer(pMC, TRUE)) ||
      FAILED(ptrEnd->MoveToContainer(pMC, FALSE)))
    {
      return 0;
    }
    DWORD flag = (icase?0:FINDTEXT_MATCHCASE);
    while(S_FALSE!=ptrBegin->FindText(textToFind, flag, ptrEnd, NULL)){
      count++;
      if(isHighLight){
        IHTMLElementPtr pFontEl;
        if(action==ADDSTYLE){
          // create FONT element with attributes for selection // Insert created element to context
          if(S_FALSE==pMS->CreateElement(TAGID_FONT, attr, &pFontEl) || S_FALSE==pMS->InsertElement( pFontEl, ptrBegin, ptrEnd)){break;}
        }else{
          // Remove last created element in the context
          if(S_FALSE==ptrBegin->CurrentScope( &pFontEl) || S_FALSE==pMS->RemoveElement(pFontEl)){break;}
        }
      }
      ptrBegin->MoveToPointer(ptrEnd);
    }
  }
  return count;
}



IHTMLDocument2Ptr CH_HBarWnd::GetHtmlDocument(){
  LPDISPATCH pDispatch = 0;
  if(pwb_){
    if (FAILED(pwb_->get_Document(&pDispatch))){
      return NULL;
    }
  }
  IHTMLDocument2Ptr pHtmlDoc2 = pDispatch;
  if(pDispatch)
    pDispatch->Release();
  return pHtmlDoc2;
}

LRESULT CH_HBarWnd::OnCtlColorEdit(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  HDC hdcEdit   = (HDC)  wParam; // handle to display context 
  HWND hwndEdit = (HWND) lParam; // handle to static control 
  if (hwndEdit==m_BandEditCtrl.m_hWnd)
  {
    CComBSTR search_words = NULL;
    m_BandEditCtrl.GetWindowText(&search_words);
    ATL::CString sw = search_words.m_str;
    if(sw.IsEmpty() && bkcolor!=kGreen){
      bkcolor = kGreen;
      m_hBrush = ::CreateSolidBrush(bkcolor);
      RECT rc;
      m_BandEditCtrl.GetClientRect(&rc);
      m_BandEditCtrl.InvalidateRect(&rc,TRUE);
    }
    if (m_hBrush == NULL)
      m_hBrush = ::CreateSolidBrush(bkcolor);
    SetBkColor(hdcEdit,bkcolor);
    SetTextColor(hdcEdit, RGB(0,20,245));
    return ((LRESULT) m_hBrush);
  }
  return 0;
}