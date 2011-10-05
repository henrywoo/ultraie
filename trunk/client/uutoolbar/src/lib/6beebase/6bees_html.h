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