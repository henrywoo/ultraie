#ifndef __UUTOOLBAR_WEBBROWSERPTR__
#define __UUTOOLBAR_WEBBROWSERPTR__

#include "stdafx.h"
#include <string>

typedef CComQIPtr<IWebBrowser2, &IID_IWebBrowser2> WBTYPE;

 namespace  _6bees_html{

   using namespace std;   

   class C6BeeWB2Ptr{     
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
     bool ShowHBar(const wchar_t*);
     bool ShowStatusBar(bool);

     // caller responsible for releasing
     // return value: 
     // (1.)NULL - cannot find the node with the specific name
     // (2.)the pointer to IHTMLElement
     IHTMLElement* GetElementByTagName(const char* _name);
     IHTMLElement* GetElementByIDName(const char* _name);

   };
 };

#endif