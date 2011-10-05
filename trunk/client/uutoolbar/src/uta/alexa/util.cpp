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