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

#include "ggpralgorithm.hpp"
#include "..\Stub_Skeleton\6bees_net.h"
#include "..\Stub_Skeleton\uulogging.h"
#include <atlstr.h>

namespace nsplugin{
  using namespace std;

  //////////////////////////////////////////////////////////////////////////
  static const UINT GOOGLE_MAGIC = 0xE6359A60;
  void GooglePRHash(UINT& a, UINT& b, UINT& c){
    a -= b; a -= c; a ^= (c >> 13);
    b -= c; b -= a; b ^= (a << 8);
    c -= a; c -= b; c ^= (b >> 13);
    a -= b; a -= c; a ^= (c >> 12);
    b -= c; b -= a; b ^= (a << 16);
    c -= a; c -= b; c ^= (b >> 5);
    a -= b; a -= c; a ^= (c >> 3);
    b -= c; b -= a; b ^= (a << 10);
    c -= a; c -= b; c ^= (b >> 15);
  }

  ATL::CString GoogleCH(ATL::CString url){
    UINT length = url.GetLength();
    UINT a = 0x9E3779B9;
    UINT b = 0x9E3779B9;
    UINT c = GOOGLE_MAGIC;
    int k = 0;
    UINT len = length;
    while (len >= 12)	{
      a += (UINT)(url[k + 0] + (url[k + 1] << 8) + (url[k + 2] << 16) + (url[k + 3] << 24));
      b += (UINT)(url[k + 4] + (url[k + 5] << 8) + (url[k + 6] << 16) + (url[k + 7] << 24));
      c += (UINT)(url[k + 8] + (url[k + 9] << 8) + (url[k + 10] << 16) + (url[k + 11] << 24));
      GooglePRHash(a, b, c);
      k += 12;
      len -= 12;
    }
    c += length;
    switch (len){
  case 11:
    c += (UINT)(url[k + 10] << 24);
  case 10:
    c += (UINT)(url[k + 9] << 16);
  case 9:
    c += (UINT)(url[k + 8] << 8);
  case 8:
    b += (UINT)(url[k + 7] << 24);
  case 7:
    b += (UINT)(url[k + 6] << 16);
  case 6:
    b += (UINT)(url[k + 5] << 8);
  case 5:
    b += (UINT)(url[k + 4]);
  case 4:
    a += (UINT)(url[k + 3] << 24);
  case 3:
    a += (UINT)(url[k + 2] << 16);
  case 2:
    a += (UINT)(url[k + 1] << 8);
  case 1:
    a += (UINT)(url[k + 0]);
  default:
    break;
    }
    GooglePRHash(a, b, c);
    ATL::CString r;
    r.Format(_T("6%lu"), c);
    return r;
  }
  UINT GetGGPR(const wchar_t* url,const wchar_t* strIP){
    try{
      ATL::CString realurl = _T("info:");
      realurl = realurl + url;
      ATL::CString checksum = GoogleCH(realurl);
      ATL::CString query;
      query.Format(_T("http://%s/search?client=navclient-auto&ch=%s&features=Rank&q=%s"),
        strIP, checksum.GetString(), realurl);
      std::string html;
      int clen;
      char* tmp = _6bees_net::GetDataByHTTP(query,clen);
      if (tmp==NULL){
        wstring info = L"Cannot connect to URL: ";
        info += query;
        UUDEBUG((LOG_ERROR,info.c_str()));
        return 0;
      }else{
        html = tmp;
        delete [] tmp;
      }
      std::string::size_type st = html.find_last_of(':');
      string ggpr = html.substr(st+1,html.size()-st-1);
      return atoi(ggpr.c_str());
    }catch (...){
      return 0;
    }
    
  }

};