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

#include "6bees_iwebbrowser2.h"
#include "6bees_const.h"
#include "uulogging.h"
#include "../../dll/6beecommond/6beecommond.h"
#pragma comment(lib,"6beecommond.lib")
#include <6bees_lang.h>

namespace _6bees_html{

  WBTYPE& c6beewb2ptr::GetPtr(){
    return spWebBrowser_;
  }

  // IHTMLDocument2Ptr is a _com_ptr_t class.
  IHTMLDocument2Ptr c6beewb2ptr::GetHTMLDoc2(){
    if(spWebBrowser_==NULL){
      return NULL;
    }
    LPDISPATCH pDispatch = NULL;
    if (FAILED(spWebBrowser_->get_Document(&pDispatch))){
      return NULL;
    }
    IHTMLDocument2Ptr pHtmlDoc2(pDispatch);//_com_ptr supports constructor with a pointer
    if(pDispatch)
      pDispatch->Release();

    return pHtmlDoc2;//_com_ptr supports copy constructor
  }

  // Get current tab's URL. For more information, please see:
  // http://support.microsoft.com/kb/272095
  wstring c6beewb2ptr::GetURL(){
    CComBSTR url;
    if (FAILED(spWebBrowser_->get_LocationURL(&url))){
      return L"";
    }
    return static_cast<wchar_t*>(url);
  }

  // todo - this function sometimes can not get the title string
  wstring c6beewb2ptr::GetTitle(){
    IHTMLDocument2Ptr pHtmlDoc2=GetHTMLDoc2();
    CComBSTR title;
    if (!pHtmlDoc2 || FAILED(pHtmlDoc2->get_title(&title))){
      return L"";
    }
    return static_cast<wchar_t*>(title);
  }

  // hwndBrowser is the handle of TabWindowClass
  HWND c6beewb2ptr::GetCurrentTab(){
    HWND hwndBrowser = NULL;
    IServiceProvider* pServiceProvider = NULL;
    if (SUCCEEDED(spWebBrowser_->QueryInterface(IID_IServiceProvider, (void**)&pServiceProvider))){
      IOleWindow* pWindow = NULL;
      if (SUCCEEDED(pServiceProvider->QueryService(SID_SShellBrowser, IID_IOleWindow, (void**)&pWindow))){
        pWindow->GetWindow(&hwndBrowser);
        pWindow->Release();
      }
      pServiceProvider->Release();
    } 
    return hwndBrowser;
  }

  bool c6beewb2ptr::NavToURL(const wchar_t* url,bool isnewtab){
    CComVariant vEmpty;
    CComVariant vURL(url);
    CComVariant flag=isnewtab ? 0x0800 : vEmpty;
    spWebBrowser_->Navigate2(&vURL, &flag, &vEmpty, &vEmpty, &vEmpty);
    return true;
  }

  bool c6beewb2ptr::NavToURL(const char* url,bool isnewtab){
    CComVariant vEmpty;
    CComVariant vURL(url);
    CComVariant flag=isnewtab ? 0x0800 : vEmpty;
    spWebBrowser_->Navigate2(&vURL, &flag, &vEmpty, &vEmpty, &vEmpty);
    return true;
  }

  bool c6beewb2ptr::NavToURL(const _bstr_t& url,bool isnewtab){
    CComVariant vEmpty;
    CComVariant flag=isnewtab ? 0x0800 : vEmpty;
    spWebBrowser_->Navigate(url, &flag, &vEmpty, &vEmpty, &vEmpty);
    return true;
  }

  void c6beewb2ptr::PostNav(const string& redirecturl){
    string email(_6beed_share::u_getEmail());
    string pass(_6beed_share::u_getPassword());
    if (email.empty()||pass.empty()){
      NavToURL(_6bees_const::kUltraIEURL);
    }
    string szPostData = "e=" + email + "&p=" + pass + "&r=" + redirecturl;
    //szPostData.append("e=").append(_6beed_share::u_getEmail()).
    //  append("&p=").append(_6beed_share::u_getPassword()).
    //  append("&r=").append(redirecturl);

    CComVariant vURL(floginurl);

    size_t nSize = szPostData.size();
    SAFEARRAY *psa = SafeArrayCreateVector(VT_UI1, 0, (ULONG)nSize);

    LPSTR pPostData;
    SafeArrayAccessData(psa, (LPVOID*) &pPostData);
    memcpy(pPostData, szPostData.c_str(), nSize);
    SafeArrayUnaccessData(psa);

    CComVariant vPostData;
    vPostData.vt = VT_ARRAY | VT_UI1;
    vPostData.parray = psa;

    CComVariant vHeaders(L"Content-Type: application/x-www-form-urlencoded\r\n");

    spWebBrowser_->Navigate2(
      &vURL,
      NULL, // Flags
      NULL, // Target
      &vPostData,
      &vHeaders  // Headers
      );
  }

  //"{92268371-AE67-4B85-BF60-157E57009ECC}"
  bool c6beewb2ptr::ShowHBar(const wchar_t* clsid){
    VARIANT vtBandGUID2, vtShow2;
    vtBandGUID2.vt = VT_BSTR;
    vtBandGUID2.bstrVal = SysAllocString(clsid);//this is your toolbar's CLSID
    vtShow2.vt = VT_BOOL;
    vtShow2.boolVal = true;
    if(FAILED(spWebBrowser_->ShowBrowserBar(&vtBandGUID2,&vtShow2,0))){
      std::wstring info = L"Cannot load tool bar with CLSID:";
      info += clsid;
      UUDEBUG((LOG_ERROR,info.c_str()));
    }
    SysFreeString(vtBandGUID2.bstrVal);
    return true;
  }

  bool c6beewb2ptr::ShowStatusBar(bool b){
    return SUCCEEDED(spWebBrowser_->put_StatusBar(b?VARIANT_TRUE:VARIANT_FALSE));
  }

  IHTMLElement* c6beewb2ptr::GetElementByTagName(const char* _name){
    IHTMLDocument2Ptr& pHtmlDoc2 = GetHTMLDoc2();
    IHTMLElementCollectionPtr pElemColl(NULL);
    if (pHtmlDoc2 && SUCCEEDED(pHtmlDoc2->get_all(&pElemColl))){
      long count = 0;
      HRESULT hr = pElemColl->get_length(&count);
      if (FAILED(hr))
        return NULL;
      for (int i=0;i<count;i++){
        CComVariant varName(i);   
        varName.ChangeType(VT_UINT);   
        CComVariant varIndex;
        CComPtr<IDispatch>pDisp;
        if(SUCCEEDED(pElemColl->item(varName,varIndex,&pDisp))){
          IHTMLElement* pElement = NULL;
          if(SUCCEEDED(pDisp->QueryInterface(IID_IHTMLElement,(LPVOID*)&pElement))){
            CComBSTR bstrTag;
            pElement->get_tagName(&bstrTag);
            bstrTag.ToLower();
            if (bstrTag==_name){
              return pElement;
            }
          }
        }
      }
    }
    return NULL;
  }

  IHTMLElement* c6beewb2ptr::GetElementByIDName(const char* _name){
    IHTMLDocument2Ptr& pHtmlDoc2 = GetHTMLDoc2();
    IHTMLElementCollectionPtr pElemColl(NULL);
    if (pHtmlDoc2 && SUCCEEDED(pHtmlDoc2->get_all(&pElemColl))){
      long count = 0;
      HRESULT hr = pElemColl->get_length(&count);
      if (FAILED(hr))
        return NULL;
      for (int i=0;i<count;i++){
        CComVariant varName(i);   
        varName.ChangeType(VT_UINT);   
        CComVariant varIndex;
        CComPtr<IDispatch>pDisp;
        if(SUCCEEDED(pElemColl->item(varName,varIndex,&pDisp))){
          IHTMLElement* pElement = NULL;
          if(SUCCEEDED(pDisp->QueryInterface(IID_IHTMLElement,(LPVOID*)&pElement))){
            CComBSTR bstrID;
            pElement->get_id(&bstrID);
            bstrID.ToLower();
            if (bstrID==_name){
              return pElement;
            }
          }
        }
      }
    }
    return NULL;
  }


};