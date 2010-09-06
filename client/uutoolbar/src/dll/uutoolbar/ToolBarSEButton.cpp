
#include "ToolBarSEButton.h"
#include "6bees_util.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "../../dll/6beeres/resource.h"
#include "6bees_str.h"
#include "6bees_const.h"

#pragma comment(lib,"6beecommond.lib")

using namespace _6bees_util;

#define CP_CHINESE 936

CToolBarSEButton::CToolBarSEButton():pwb_(NULL),BG2312SearchEngine(false){}

CToolBarSEButton::~CToolBarSEButton(){}

LRESULT CToolBarSEButton::OnCreate(UINT,WPARAM,LPARAM,BOOL&){
  m_bmp = (HBITMAP)c6beeres::R().GetImg(IDB_SEARCH,IMAGE_BITMAP,21,24);
  m_bmp_bkg = (HBITMAP)c6beeres::R().GetImg(IDB_SEARCH_BKG,IMAGE_BITMAP,21,24);

  CRegKey crk;
  long rk2=crk.Open(HKEY_LOCAL_MACHINE,_6bees_const::kregistryname,KEY_READ);
  if (rk2==ERROR_SUCCESS){
    wchar_t pszValue[100];
    ULONG len=100;
    rk2=crk.QueryStringValue(_T("DefaultSE"),pszValue,&len);
    if (wcscmp(pszValue,_T("Baidu"))==0){
      searchURL = "http://www.baidu.com/s?wd=";
      BG2312SearchEngine=true;
    }else if (wcscmp(pszValue,_T("Yahoo"))==0){
      searchURL=_6beed_util::isChineseOS() ? 
        "http://search.cn.yahoo.com/search?ei=gbk&fr=fp-tab-web-ycn&pid=ysearch&source=ysearch_www_hp_button&p=": 
      "http://search.yahoo.com/search?ei=utf-8&fr=fp-tab-web-ycn&pid=ysearch&source=ysearch_www_hp_button&p=";
      BG2312SearchEngine=_6beed_util::isChineseOS();
    }else{
      char GGurl[200]={};
      sprintf_s(GGurl,"http://www.google.com/search?ie=%s&q=",_6beed_util::getLangEncoding());
      searchURL = GGurl;
    }
  }
  
  return 0;
}

void CToolBarSEButton::SetBrowser(_6bees_html::c6beewb2ptr* _p){
  pwb_ = _p;
}

LRESULT CToolBarSEButton::OnSetCursor(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  static HCURSOR hcur = LoadCursor(NULL,/*IDC_SIZEALL*/IDC_HAND);
  if(m_count<1){
    m_count++;
    RECT rect;
    this->GetClientRect(&rect);
    InvalidateRect(&rect,true);
  }  
  TRACKMOUSEEVENT  tme;
  tme.cbSize =sizeof(TRACKMOUSEEVENT);
  tme.dwFlags =TME_LEAVE;
  tme.dwHoverTime=0;
  tme.hwndTrack =m_hWnd;
  ::TrackMouseEvent(&tme);
  if (hcur!=NULL){
    SetCursor(hcur);
    return TRUE;
  }else{
    SetMsgHandled(false);
    return false;
  }
}

LRESULT CToolBarSEButton::OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  VARIANT vEmpty;
  VariantInit(&vEmpty);
  _bstr_t finalURL;
  //If there is webpage loading then stop.
  if (pwb_){
    if (p_CToolBarCombo->GetWindowTextLength()){
      CComBSTR search_words;
      p_CToolBarCombo->GetWindowText(&search_words);
      p_CToolBarCombo->AddtoHistory(search_words.m_str);

      _bstr_t b(search_words);
      std::wstring szText = (LPCTSTR)b;

      static const unsigned int MAXLEN=1024;
      if (szText.size()>MAXLEN){
        return 0;
      }
      
      char ansistr[MAXLEN*2]={};
      if (_6beed_util::isChineseOS()){
        // you also can use CP_CHINESE
        std::string tmp = _6bees_str::stringmaker(CP_ACP) << szText;
        if(!_6beed_util::urlencode(ansistr,MAXLEN*2,tmp.c_str())){return 0;}
      }else{
        std::string tmp = _6bees_str::stringmaker(BG2312SearchEngine?CP_CHINESE:CP_UTF8) << szText;
        if(!_6beed_util::urlencode(ansistr,MAXLEN*2,tmp.c_str())){return 0;}
      }
      finalURL = searchURL + ansistr;
    }else{
      finalURL = _6bees_const::kUltraIEURLa;
    }
    pwb_->NavToURL(finalURL);
    }
  return true;
}

LRESULT CToolBarSEButton::OnSetGGURL(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  char GGurl[200]={};
  sprintf_s(GGurl,"http://www.google.com/search?ie=%s&q=",_6beed_util::getLangEncoding());
  searchURL = GGurl;
  BG2312SearchEngine=false;
  ::SendMessage(m_hWnd,WM_LBUTTONDOWN,0,0);
  return true;
}
LRESULT CToolBarSEButton::OnSetBaiduURL(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  searchURL = "http://www.baidu.com/s?wd=";
  BG2312SearchEngine=true;
  ::SendMessage(m_hWnd,WM_LBUTTONDOWN,0,0);
  return true;
}
LRESULT CToolBarSEButton::OnSetYahooURL(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  searchURL=_6beed_util::isChineseOS() ? 
    "http://search.cn.yahoo.com/search?ei=gbk&fr=fp-tab-web-ycn&pid=ysearch&source=ysearch_www_hp_button&p=": 
    "http://search.yahoo.com/search?ei=utf-8&fr=fp-tab-web-ycn&pid=ysearch&source=ysearch_www_hp_button&p=";
  BG2312SearchEngine=_6beed_util::isChineseOS();
  ::SendMessage(m_hWnd,WM_LBUTTONDOWN,0,0);
  return true;
}

LRESULT CToolBarSEButton::OnSearchSetting(UINT,WPARAM,LPARAM,BOOL&){
  pwb_->NavToURL(L"ultra://searchsetting");
  return 0;
}

LRESULT CToolBarSEButton::OnMouseOut (UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  // when the mouse moves out, reset m_count as 0
  m_count = 0;
  RECT rect;
  this->GetClientRect(&rect);
  InvalidateRect(&rect,true);
  return 0;
}

LRESULT CToolBarSEButton::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  CPaintDC hdc(m_hWnd);
  CDC dcMem;
  dcMem.CreateCompatibleDC(hdc);
  hdc.SaveDC();
  dcMem.SaveDC();
  //select and draw
  if(m_count==0){
    //initial status
    dcMem.SelectBitmap(m_bmp);
    hdc.BitBlt(0,0,21,_6bees_const::ktbbuttonheight,dcMem,0,0,SRCCOPY);
  }else{
    dcMem.SelectBitmap(m_bmp_bkg);
    hdc.BitBlt(0,0,21,_6bees_const::ktbbuttonheight,dcMem,0,0,SRCCOPY);
  }
  dcMem.RestoreDC(-1);
  hdc.RestoreDC(-1);
  return false;
}
