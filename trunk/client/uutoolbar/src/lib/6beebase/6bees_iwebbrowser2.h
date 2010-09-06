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