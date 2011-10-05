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

#define WINVER      0x0500
//#define _WIN32_WINNT  0x0501
#define _WIN32_IE    0x0501
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define _WTL_NO_CSTRING

#include <atlbase.h>
#include <atlstr.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlddx.h>
#include <process.h>//_beginthreadex
#include <atlframe.h>
#include <atlctrls.h>
#include "resource.h"
#include "UpDownLoader.h"
#include <6beefiledlg.h>
#include "FileProfile.h"
#include <iostream>
#include "../../dll/6beecommond/6beecommond.h"
#include "MainDlg.h"
#include "6bees_util.h"
#include "6bees_str.h"
using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;

#pragma comment(lib,"6beecommond.lib")
#pragma comment(lib,"6beemousehook.lib")

using namespace std;
CAppModule _Module;

void PopDlgtoSelectFiles(CMainDlg& dlgMain,ATL::CString& strFolderPath,ATL::CString& strFileName,int& mssize);

void termin_func(){
  UUDEBUG((LOG_ERROR,"CPYDATA_DATATRANSFER_TERM - termination comes, oops...:-("));
  exit(0);
}



int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, wchar_t* lpstrCmdLine, int nCmdShow){
  set_terminate(termin_func);
  HRESULT hRes = ::OleInitialize(NULL);
  ::DefWindowProc(NULL, 0, 0, 0L);
  AtlInitCommonControls(ICC_BAR_CLASSES | ICC_DATE_CLASSES);//Add flags to support other controls
  hRes = _Module.Init(NULL, hInstance);
  int nRet = 0;

  _6bees_socket::ftpclient ftpClient;
  _6bees_timer::CTimer m_Timer;

  UPLOADERMODE mode;
  vector<wstring> vec_files;
  ParseCmdline(stringmaker(CP_ACP)<<lpstrCmdLine,mode,&vec_files);

  switch (mode){
  case LOGIN_MODE:{
    CMainDlg dlgMain;
    nRet = (int)dlgMain.DoModal();
  }break;
  case UPLOAD_MODE:{
    // 1. If there is an uploader already, then open it and upload
    HWND hwndTo = ::FindWindow(0,_6bees_const::kUploader_WndTitle);
    if(hwndTo){
      wstring filelist;//f will be like:XXX YYY ZZZ
      for(int i=0;i<(int)vec_files.size();i++){
        filelist.append(vec_files[i] + ((i==(int)vec_files.size()-1)?_T(""):_T("\t")));
      }
      _6beed_util::SendMsgToUploader(hwndTo,(stringmaker()<<filelist).c_str(),CPYDATA_UPLOAD_2_UPLOAD);
      break;
    }
    // 2. open a new uploader
    CMainDlg dlgMain;
    int mssize=0;
    if(vec_files.empty()){
      CString strFolderPath,strFileName;
      dlgMain.PopDlgtoSelectFiles(strFolderPath,strFileName,mssize);
    }else{
      for(vector<wstring>::const_iterator i=vec_files.begin();i!=vec_files.end();++i){
        if(!PathIsDirectory(i->c_str()) &&
          PathFileExists(i->c_str()) &&
          _6beed_util::GetFileSizeW(i->c_str(),mssize))
        {
          CFileProfile fileProfile(0,i->c_str(),mssize);
          dlgMain.AddUploadFile(fileProfile);
        }
      }
    }
    if(!dlgMain.IsEmpty())
      nRet = (int)dlgMain.DoModal();
    }break;
  case UPLOAD_ICON_MODE:
  case UPLOAD_THUMB_MODE:
    if (vec_files.size()!=3){break;  }
    _6beed_util::QuietUploadFile(vec_files[0].c_str(),vec_files[1].c_str(),vec_files[2].c_str(),mode);
    break;
  case DIRECTUPLOAD_MODE:{
    if (vec_files.size()!=1){break;  }
    const wchar_t * filepath = vec_files[0].c_str();
    // 1. If there is an uploader already, then open it and upload
    HWND hwndTo = ::FindWindow(0,_6bees_const::kUploader_WndTitle);
    if(hwndTo){
      USES_CONVERSION;
      _6beed_util::SendMsgToUploader(hwndTo,W2CA(filepath),CPYDATA_UPLOAD_2_UPLOAD_DIRECT);
      wstring msg= wstringmaker() << L"log debug: direct upload - " << filepath;
      UUDEBUG((LOG_DBG,msg.c_str()));
      break;
    }

    // open new uploader
    CMainDlg dlgMain;
    int mssize=0;
    if(PathFileExists(filepath) && _6beed_util::GetFileSizeW(filepath,mssize)){
      CFileProfile fileProfile(0,filepath,mssize,true);
      if(dlgMain.AddImediateUploadFile(fileProfile))
        nRet = (int)dlgMain.DoModal();
    }  
    }break;
  default:
    break;
  }
  _Module.Term();
  ::OleUninitialize();
  return nRet;
}

bool ParseCmdline(string cmdline,
                  UPLOADERMODE& mode,
                  vector<wstring>* r)
{
  if(r==NULL)
    return false;
  Pcre re1("-m\\s+(\\w+)\\s*.*",PCRE_CASELESS);
  Pcre re2("\"([^\"]+)\"",PCRE_CASELESS);

  if(re1.search(cmdline)){
    string tmp= re1.get_match(0);

    if(tmp=="upload"){
      mode = UPLOAD_MODE;
    }else if(tmp=="thumbupload"){
      mode = UPLOAD_THUMB_MODE;
    }else if(tmp=="iconupload"){
      mode = UPLOAD_ICON_MODE;
    }else if(tmp=="directupload"){
      mode = DIRECTUPLOAD_MODE;
    }else if(tmp=="login"){
      mode = LOGIN_MODE;
      return true;
    }else{
      return false;
    }

    int pos = 0;
    int siz = (int)cmdline.size();
    switch (mode){
      case UPLOAD_MODE:
        while((pos<=siz) && re2.search(cmdline,pos)){
          pos = re2.get_match_end(0);
          string fn = re2.get_match(0);
          r->push_back(wstringmaker(CP_ACP)<<fn);
        }
        break;
      case UPLOAD_ICON_MODE:
      case UPLOAD_THUMB_MODE:
      case DIRECTUPLOAD_MODE:
        {
          int i=0;
          while((pos<=siz) && re2.search(cmdline,pos) && i<=2){
            pos = re2.get_match_end(0);
            string fn = re2.get_match(0);
            r->push_back(wstringmaker(CP_ACP)<<fn);
            i++;
          }
        }break;
      default:
        return false;
    }
  }
  return true;
}