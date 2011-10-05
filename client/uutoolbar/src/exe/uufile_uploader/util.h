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

#ifndef __6BEEUPLOADER_UTIL_H__
#define __6BEEUPLOADER_UTIL_H__

#include <comdef.h>
#include "FileProfile.h"
#include "6bees_timer.h"

////////////////////////////////////////////////////////////////////
typedef CListArray<CFileProfile> FList;//File List


/// @brief packet info transfered to a new thread
struct tpacket{
  CListWindowFileData* t_pWndFD;
  CFileProfile*  t_pfp;
  FList*    t_up_flist;
  _6bees_timer::CTimer*  t_timer;
  _6bees_socket::ftpclient* t_ftpclient;
  LPVOID   t_pmaindlg;
  int  t_ID;
} threadpacket[ATSMAXCOUNT];


struct ppacket{
  CListWindowFileData* cfd;
  CString  fpath;
  _6bees_socket::ftpclient* _6bees_socket;
  __int64* psbytes;
  int status;
};

_6bees_timer::CTimer m_Timer[ATSMAXCOUNT];

///////////////////////////////////////////////////////////////////////////////////
list<CFileProfile*> sfp_waiting_list;//can we use queue/deque

set<CString>    sfp_active;// abs path set

CFileProfile WaitingFileArray500[500];//maximum 500 waiting status files

CFileProfile active_fp[ATSMAXCOUNT];

CRITICAL_SECTION csection;

wstring runIE_url;

UINT WINAPI runIE_internal(PVOID p){
  try{
    _variant_t varURL = (wchar_t*)p;
    CoInitialize(NULL);
    IWebBrowser2Ptr pIE(0);
    if (SUCCEEDED(CoCreateInstance(CLSID_InternetExplorer,NULL,CLSCTX_LOCAL_SERVER,
      IID_IWebBrowser2,(void**)&pIE)))
    {
      pIE->put_Visible(VARIANT_TRUE);
      _variant_t varEmpty;
      pIE->Navigate2(&varURL,&varEmpty,&varEmpty,&varEmpty,&varEmpty);
    }
    CoUninitialize();
  }catch (...){}
  return 0;
}

void runIE(const wchar_t* url){
  runIE_url = url;
  ::_beginthreadex(NULL,0,runIE_internal,(LPVOID)runIE_url.c_str(),0,0);
}

bool GetFileSizeW(const wchar_t* fpath,int& fsize){
  WIN32_FILE_ATTRIBUTE_DATA  fileattr;
  if (::GetFileAttributesExW((LPCTSTR)fpath,::GetFileExInfoStandard,&fileattr)){
    fsize = (fileattr.nFileSizeHigh * (MAXWORD+1)) + fileattr.nFileSizeLow;
    return true;
  }
  return false;
}

bool urlencode(char* buf,int bufsize,const char* str){
  static const char special[] = "!\"#$%&()*+,/:;<=>?@[\\]^`{|}~%";
  char tmp[10];/* temp buffer */ 
  buf[0] = '\0';
  int j=(int)strlen(str);
  if (j>1024*2){
    return false;
  }
  for(int i=0; i<j; i++){ 
    char c = str[i]; 
    if(strchr(special,c)){ /* special character */ 
      sprintf_s(tmp,"%%%02X",(unsigned char)c); 
      strcat_s(buf,bufsize,tmp); 
    }else if(c == ' '){ /* blank */ 
      strcat_s(buf,bufsize,"+");
    }else{ 
      if(c < 0){ /* none ASCII character */
        sprintf_s(tmp,"%%%02X%%%02X",(unsigned char)str[i],(unsigned char)str[i+1]); 
        strcat_s(buf,bufsize,tmp); 
        ++i; 
      }else{ /* ASCII character */ 
        sprintf_s(tmp,"%c",c); 
        strcat_s(buf,bufsize,tmp); 
      } 
    } 
  }
  return true;
}

void DrawRoundRect(HDC hdc,LPRECT rcRect,COLORREF rgbOuter,COLORREF rgbInner ,COLORREF m_rgbBackground){
  CDCHandle cdchandle(hdc);
  RECT tmprect;
  ::CopyRect(&tmprect,rcRect);

  CPen penBorder;
  penBorder.CreatePen(PS_SOLID,1,rgbOuter);
  CBrush bshInterior;
  bshInterior.CreateSolidBrush(m_rgbBackground);

  SelectObject(hdc,(HGDIOBJ)penBorder);
  SelectObject(hdc,(HGDIOBJ)bshInterior);

  POINT p={};
  cdchandle.RoundRect(&tmprect,p);
  ::InflateRect(&tmprect,-1,-1);

  CPen penInnerBorder;
  penInnerBorder.CreatePen(PS_SOLID,1,rgbInner);
  SelectObject(hdc,(HGDIOBJ)penInnerBorder);

  cdchandle.RoundRect(&tmprect,p);
}

void DrawGradient(HDC hdc,LPRECT lpRect,COLORREF rgbTop,COLORREF rgbBottom){
  GRADIENT_RECT grdRect = { 0,1 };
  TRIVERTEX triVertext[ 2 ] = {
    lpRect->left,
    lpRect->top,
    GetRValue(rgbTop) << 8,
    GetGValue(rgbTop) << 8,
    GetBValue(rgbTop) << 8,
    0x0000,
    lpRect->right,
    lpRect->bottom,
    GetRValue(rgbBottom) << 8,
    GetGValue(rgbBottom) << 8,
    GetBValue(rgbBottom) << 8,
    0x0000
  };
  ::GradientFill(hdc,triVertext,2,&grdRect,1,GRADIENT_FILL_RECT_V);
}

#endif