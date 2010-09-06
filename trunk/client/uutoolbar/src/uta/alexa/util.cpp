#include "util.h"
#include <msxml2.h>
#include <string>

namespace nsplugin{

  int GetAlexaRank(const wchar_t* url){
    std::wstring realurl(L"http://data.alexa.com/data?cli=10&dat=snbamz&url=");
    realurl.append(url);

    CoInitialize(NULL);
    IXMLDOMDocument *pxmldoc = NULL;
    HRESULT h = CoCreateInstance(__uuidof(DOMDocument30),NULL,CLSCTX_INPROC_SERVER,
      __uuidof(IXMLDOMDocument),(void**)&pxmldoc);
    if (SUCCEEDED(h)){
      pxmldoc->put_async(VARIANT_FALSE);
      pxmldoc->put_validateOnParse(VARIANT_FALSE);
      pxmldoc->put_resolveExternals(VARIANT_FALSE);

      VARIANT_BOOL status;
      CComVariant comvar_ = realurl.c_str();//html.c_str();
      pxmldoc->load(comvar_,&status);

      IXMLDOMNode* pNode=NULL;
      pxmldoc->selectSingleNode(L"/ALEXA/SD/POPULARITY", &pNode);
      if (pNode){
        IXMLDOMNamedNodeMap* p=NULL;
        if(SUCCEEDED(pNode->get_attributes(&p))){
          IXMLDOMNode* alexarank=NULL;
          p->getNamedItem(L"TEXT",&alexarank);
          CComVariant alexa_;
          alexarank->get_nodeValue(&alexa_);
          ATL::CString bstr_=alexa_;
          return _wtoi(bstr_.GetString());
        }
      }
      pxmldoc->Release();
      CoUninitialize();
    }
    return 0;
  }

};