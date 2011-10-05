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

#ifndef __UUTOOLBAR_WEBBROWSERPTR__
#define __UUTOOLBAR_WEBBROWSERPTR__

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <exdisp.h>
#include <comdef.h>
#include <comdefsp.h>
#include <shlguid.h>

#include <string>

_COM_SMARTPTR_TYPEDEF(IHTMLDocument2, __uuidof(IHTMLDocument2));

_COM_SMARTPTR_TYPEDEF(IHTMLWindow3, __uuidof(IHTMLWindow3));
_COM_SMARTPTR_TYPEDEF(IHTMLElement2, __uuidof(IHTMLElement2));
_COM_SMARTPTR_TYPEDEF(IHTMLDocument3, __uuidof(IHTMLDocument3));
_COM_SMARTPTR_TYPEDEF(IHTMLRect, __uuidof(IHTMLRect));

 namespace  _6bees_html{
   using namespace std;

   typedef CComQIPtr<IWebBrowser2, &IID_IWebBrowser2> WBTYPE;

   class c6beewb2ptr{
     WBTYPE spWebBrowser_;

   public:
     WBTYPE& GetPtr();

     IHTMLDocument2Ptr GetHTMLDoc2();

     wstring GetURL();
     wstring GetTitle();

     HWND GetCurrentTab();
     bool NavToURL(const wchar_t* url,bool isnewtab=false);
     bool NavToURL(const char* url,bool isnewtab=false);
     bool NavToURL(const _bstr_t& url,bool isnewtab=false);

     void PostNav(const string&);
     bool ShowHBar(const wchar_t*);
     bool ShowStatusBar(bool);

     /// caller responsible for releasing
     /// @return 
     /// (1.)NULL - cannot find the node with the specific name
     /// (2.)the pointer to IHTMLElement
     IHTMLElement* GetElementByTagName(const char* _name);
     IHTMLElement* GetElementByIDName(const char* _name);

   };
 };

#endif