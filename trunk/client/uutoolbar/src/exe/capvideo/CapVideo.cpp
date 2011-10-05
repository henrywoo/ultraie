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

#include "stdafx.h"
#include "CapVideo.h"
#include "CapVideoDlg.h"
#include "../../dll/6beecommond/6beecommond.h"
#include <copydatamsg.h>

#pragma comment(lib,"6beecommond.lib")

CCapVideoApp theApp;

Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
ULONG_PTR m_gdiplusToken;

BEGIN_MESSAGE_MAP(CCapVideoApp, CWinApp)
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CCapVideoApp::CCapVideoApp(){
  m_caprawvideo = new CCapRawVideo;
  GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
}

CCapVideoApp::~CCapVideoApp(){
  delete m_caprawvideo;
}

int CCapVideoApp::ExitInstance(){
  Gdiplus::GdiplusShutdown(m_gdiplusToken);
  return 0;
}

BOOL CCapVideoApp::InitInstance(){
  InitCommonControls();
  CWinApp::InitInstance();
  SetRegistryKey(_T("6BEE NETWORKS PTE LTD"));
  CWinThread::InitInstance();
  CCapVideoDlg dlg;//main thread start dialog frame
  if(!InitCapture()){
    return FALSE;
  }
  dlg.m_caprawvideo = m_caprawvideo;
  if (m_caprawvideo){//derive a thread to capture video
    m_caprawvideo->PostThreadMessage(WM_STARTPREVIEW,(WPARAM)&(dlg.m_cappreview),(LPARAM)0);
  }
  INT_PTR nResponse = dlg.DoModal();
  if (nResponse == IDCANCEL && strlen(m_caprawvideo->getavfilename())!=0){
    /// @todo temp comment out this -  can use a function in 6beecommond.dll to replace it.
    ///////////////////////////// start to upload /////////////////////////////////
    /*
    USES_CONVERSION;
    CString videopath;
    videopath.Format(_T("%s%s"),_6beed_util::Get6BEEPath(_6bees_const::kVideoDir,true),A2CW(m_caprawvideo->getavfilename()));
    HWND hwndTo = ::FindWindow(0,_6bees_const::kUploader_WndTitle);
    if(!hwndTo){
      CString UploaderCmd = _6beed_util::Get6BEEPath(_6bees_const::kUploaderName);
      UploaderCmd += _T(" -m upload -f \"") + videopath + _T("\"");
      STARTUPINFO si = {sizeof(si)};
      PROCESS_INFORMATION pi;
      if (!CreateProcessW(NULL,const_cast<LPWSTR>(UploaderCmd.GetString()),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
        ::AfxMessageBox(_T("ERROR: Cannot Start 6BeeUpLoader!"));
      }
    }else{
      _6beed_util::SendMsgToUploader(hwndTo,W2CA(videopath),CPYDATA_UPLOAD_2_UPLOAD_DIRECT);
    }
    */
    /////////////////////////////////////////////////////////////////////////////
  }
  return FALSE;
}

BOOL CCapVideoApp::InitCapture(){
  if(m_caprawvideo->getdevinfo(0)==_T("NODEVICE")){
    return false;
  }
  m_caprawvideo->CreateThread();
  return true;
}
