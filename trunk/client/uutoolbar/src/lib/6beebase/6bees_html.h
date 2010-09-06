#ifndef __6BEES_HTML__
#define __6BEES_HTML__

#include <string>
#include "6bees_iwebbrowser2.h"
#include <atlstr.h>

namespace _6bees_html{
  using namespace std;

  struct ripthing{
    bool isbinary;
    string html;
    string relativepath;
    string newname;
  };

  void GetNavCode(const string& urlbase, const char* totalnum, int pnum,
    string& nav);

  bool ParseAction(const wchar_t* tbname, wstring& fname, string& _cmd,
    wstring& redirectpage);

  bool EntirePageCapture(
    HWND hwnd,
    c6beewb2ptr* _pwb,
    const wchar_t* imgpath,
    const wchar_t* imgpath_thumb,
    bool isthumbforwholepage,
    bool imghasborder,
    bool imghasshadow,
    COLORREF bcolor);

  LRESULT HighlightWebElement(
    IHTMLElement*& elementHit,
    c6beewb2ptr* _pwb,
    RECT& CaptureRect,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    BOOL& bHandled);

  void StopMouseHook(HWND);

  BOOL GeneratePicFromWebElement(
    c6beewb2ptr* _pwb,
    const RECT& caprect,
    const ATL::CString& AbsPicFilename,
    const ATL::CString& AbsPicFilename_thumb,
    bool imghasborder,
    bool imghasshadow,
    COLORREF bcolor);

  /*BOOL process_wholepage(int& progress,
    wstring& webpage,
    const wstring& wstrURL,
    const wstring& hostname,
    const ATL::CString& mstr);*/

  /*BOOL process_webpage_imgcssjs(wstring& webpage,
    const wstring& wstrURL,
    const wstring& hostname,
    int type,
    const ATL::CString& DirName);*/

  bool process_webpage_imgcssjs(
    string& webpage,
    const string& wstrURL,
    const string& hostname,
    const string& DirName,
    int type);

  bool process_wholepage(int& progress,
    string& webpage,
    const string& wstrURL,
    const string& hostname,
    const string& mstr);

  void getFavicon(IHTMLDocument2Ptr& pHtmlDoc2,const wchar_t* scheme,wstring& faviconURL);

  int isFaviconValid(const char* url);

  int getValidFavicon(const string& _url,string& realurl);

  bool getcurrenttitle2(IHTMLDocument2Ptr&,wstring&);

  bool GetHtmlDocument2(IWebBrowser2*,IHTMLDocument2Ptr&);

  void getBodyText(IHTMLDocument2Ptr& pHtmlDoc2,wstring& bodyText,int maxtextnumber=0);

};

#endif