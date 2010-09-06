#include "UUFilter.h"
#include <6bees_iwebbrowser2.h>
#include "../../dll/6beecommond/6beecommond.h"

#include "ToolBarCom.h"

CUUFilter::CUUFilter(){

}

STDMETHODIMP CUUFilter::SetSite(IUnknown* pUnk){
  if (pUnk != NULL){
    IWebBrowser2 *pBrowser = NULL;
    HRESULT hr = pUnk->QueryInterface(IID_IWebBrowser2, (void **) &pBrowser);
    // Ensure that our site is an browser window
    if (SUCCEEDED(hr)){
      const OLECHAR* toolbar_clsid = OLESTR("{0E1F40A1-4D93-435A-A8C5-273FE2ECF99C}");
      VARIANT vtBandGUID, vtShow;// status bar
      vtBandGUID.vt = VT_BSTR;
      vtBandGUID.bstrVal = SysAllocString(toolbar_clsid);
      vtShow.vt = VT_BOOL;
      vtShow.boolVal = true;
      pBrowser->ShowBrowserBar(&vtBandGUID, &vtShow, 0);
      SysFreeString(vtBandGUID.bstrVal);

      switch(_6beed_share::u_getieversion()){
        case 7:
        case 6:
          {// Display the band object (the Search bar in this case)
            const OLECHAR* hbar_clsid = OLESTR("{92268371-AE67-4B85-BF60-157E57009ECC}");
            VARIANT vtBandGUID2, vtShow2;
            vtBandGUID2.vt = VT_BSTR;
            vtBandGUID2.bstrVal = SysAllocString(hbar_clsid);
            vtShow2.vt = VT_BOOL;
            vtShow2.boolVal = false;//search bar
            if(FAILED(pBrowser->ShowBrowserBar(&vtBandGUID2, &vtShow2, 0))){
              MessageBox(NULL,_T("ERROR TO LOAD EXPLORER BAR!"),_T("ERROR"),NULL);
            }
            SysFreeString(vtBandGUID2.bstrVal);
          }break;
        case 8:break;
      }
      pBrowser->Release();
    }
  }
  return __super::SetSite(pUnk);
}

HRESULT CUUFilter::FinalConstruct(){
  return S_OK;
}

void CUUFilter::FinalRelease(){

}


