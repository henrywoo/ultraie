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

#include <string>
//#include <pcre++.h>
#include "6bees_net.h"
#include <map>
#include <6beefiledlg.h>
#include "FileProfile.h"
#include <iostream>
#include "MainDlg.h"
#include "6bees_util.h"
#include "6bees_str.h"
#include <vector>
using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;
#pragma comment(lib,"version.lib")//GetFileVersion

using namespace std;
extern CAppModule _Module;

void PopDlgtoSelectFiles(CMainDlg& dlgMain,ATL::CString& strFolderPath,ATL::CString& strFileName,int& mssize);

void SendMsgToUploader(HWND hwndTo,const char* filelist,int msg){
  int slen = (int)strlen(filelist);
  ::memset((void*)(filelist+slen),'\0',1);
  COPYDATASTRUCT cds;
  cds.dwData = msg;
  cds.lpData = (PVOID)filelist;
  cds.cbData = slen+1;
  ::SendMessage(hwndTo,WM_COPYDATA,(WPARAM)0,(LPARAM)&cds);
}

struct versionblk{
  unsigned short wMajor;
  unsigned short wMinor;
  unsigned short wRelease;
  unsigned short wBuild;
};

versionblk GetVersionBlk(const wchar_t* sFileName){
  versionblk ver;
  UINT jj = (UINT)sizeof(VS_FIXEDFILEINFO);
  wchar_t *pp,pp2[100];
  VS_FIXEDFILEINFO *lpBuffer2;
  void **pp1 = (void **)&lpBuffer2;
  pp2[0] = 0;
  int ii = ::GetFileVersionInfoSize(sFileName,NULL);
  if(ii>0){
    pp = new wchar_t[ii];
    if(::GetFileVersionInfo(sFileName,0,ii,pp)){
      if(::VerQueryValue(pp,_T("\\"),pp1,&jj)){
        ver.wMajor   = HIWORD(lpBuffer2->dwFileVersionMS);
        ver.wMinor   = LOWORD(lpBuffer2->dwFileVersionMS);
        ver.wRelease = HIWORD(lpBuffer2->dwFileVersionLS);
        ver.wBuild   = LOWORD(lpBuffer2->dwFileVersionLS);
      }
    }
    delete [] pp;
  }
  return ver;
}

UINT WINAPI CheckVersion_internal(PVOID p){
  try{
    string url=(const char*)p;
    int clen=0;
    char *data = _6bees_net::GetDataByHTTP(url.c_str(),clen);
    if (data!=NULL){
      string aurl = stringmaker()  << data;
      delete [] data;
      if(aurl!="0" && _6bees_str::startwith<string>(aurl,"http://")){
        string info = stringmaker() << "您好,"<< PRODUCTNAME <<"有更新的版本发布了,现在去下载升级新版本吗?\n\n旧版本可能无法正常工作.我们强烈建议您更新到最新版本!";
        if(::MessageBoxA(NULL,info.c_str(),"探测到新版本",MB_YESNO)==IDYES){
          ShellExecuteA(NULL,"open","iexplore.exe",aurl.c_str(),"",SW_SHOW);
        }
      }
    }
  }catch (...){}
  return 0;
}

string checkurl_url;
void CheckVersion(const wchar_t* filepath){
  wstring filename= PathFindFileNameW(filepath);
  LOWERWSTR(filename);
  versionblk verblk=GetVersionBlk(filepath);
  checkurl_url = _6bees_str::stringmaker()<< _6bees_const::kUltraIEURL << 
    "win32client/latestversion.php?filename=" << filename << "&version=" << verblk.wMajor <<
    "." << verblk.wMinor <<
    "." << verblk.wRelease <<
    "." << verblk.wBuild;
  ::_beginthreadex(NULL,0,CheckVersion_internal,(LPVOID)checkurl_url.c_str(),0,0);
}


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, wchar_t* lpstrCmdLine, int nCmdShow){
  HRESULT hRes = ::OleInitialize(NULL);
  ::DefWindowProc(NULL, 0, 0, 0L);
  AtlInitCommonControls(ICC_BAR_CLASSES | ICC_DATE_CLASSES);//Add flags to support other controls
  hRes = _Module.Init(NULL, hInstance);
  int nRet = 0;

  wchar_t pszLoader[MAX_PATH];
  ::GetModuleFileName(NULL, pszLoader, MAX_PATH);
  CheckVersion(pszLoader);

  _6bees_socket::ftpclient ftpClient;
  _6bees_timer::CTimer m_Timer;

  //UPLOADERMODE mode;
  vector<wstring> vec_files;
  //ParseCmdline(stringmaker(CP_ACP)<<lpstrCmdLine,mode,&vec_files);
  if (lpstrCmdLine==NULL){

  }else if (_6bees_str::startwith<wstring>(lpstrCmdLine,L"-m upload ")){
    wstring cmdline(lpstrCmdLine);
    cmdline = cmdline.substr(10);
    vec_files = _6bees_util::Split(cmdline,L"|");
  }

  // 1. If there is an uploader already, then open it and upload
  HWND hwndTo = ::FindWindow(0,_6bees_const::kUploader_WndTitle);
  if(hwndTo){
    wstring filelist;//f will be like:XXX YYY ZZZ
    for(int i=0;i<(int)vec_files.size();i++){
      filelist.append(vec_files[i] + ((i==(int)vec_files.size()-1)?_T(""):_T("\t")));
    }
    SendMsgToUploader(hwndTo,(stringmaker()<<filelist).c_str(),CPYDATA_UPLOAD_2_UPLOAD);
  }else{
    // 2. open a new uploader
    CMainDlg dlgMain;
    int mssize=0;
    if(vec_files.empty()){
      CString strFolderPath,strFileName;
      dlgMain.PopDlgtoSelectFiles(strFolderPath,strFileName,mssize);
    }else{
      for(vector<wstring>::const_iterator i=vec_files.begin();i!=vec_files.end();++i){
        if(!PathIsDirectory(i->c_str()) && PathFileExists(i->c_str()) && GetFileSizeW(i->c_str(),mssize))
        {
          CFileProfile fileProfile(0,i->c_str(),mssize);
          dlgMain.AddUploadFile(fileProfile);
        }
      }
    }
    if(!dlgMain.IsEmpty())
      nRet = (int)dlgMain.DoModal();
  }
                     
  _Module.Term();
  ::OleUninitialize();
  return nRet;
}