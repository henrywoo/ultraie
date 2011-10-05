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

#pragma once
#include "atlctrls.h"
#include "PlaySound.h"
#include "PlayMMSound.h"
#include "RecordSound.h"
#include "WriteSoundFile.h"

#include "../../dll/6beecommond/6beecommond.h"
#include "../../lib/6beebase/6bees_util.h"
#include <copydatamsg.h>
#include <atlstr.h>

const int NUM_BRUSHES=1;

CPlayMMSound* m_pPlayMMSound = new CPlayMMSound();
CMP3Creator* m_pWriteSound  = new CMP3Creator();

class CMainDlg :
  public CDialogImpl<CMainDlg>,
  public CUpdateUI<CMainDlg>,
  public CMessageFilter,
  public CIdleHandler
{
public:
  enum { IDD = IDD_MAINDLG };

  virtual BOOL PreTranslateMessage(MSG* pMsg)
  {
    return CWindow::IsDialogMessage(pMsg);
  }

  virtual BOOL OnIdle()
  {
    return FALSE;
  }

  BEGIN_UPDATE_UI_MAP(CMainDlg)
  END_UPDATE_UI_MAP()

  BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_TIMER, OnTimer)
    COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
    COMMAND_ID_HANDLER(IDOK, OnOK)
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
    COMMAND_HANDLER(IDC_START_STOP, BN_CLICKED, OnBnClickedStartStop)
    COMMAND_HANDLER(IDC_PLAY, BN_CLICKED, OnBnClickedPly)
    COMMAND_HANDLER(IDC_PAUSE, BN_CLICKED, OnBnClickedPause)
    COMMAND_HANDLER(IDC_UPLOAD, BN_CLICKED, OnBnClickedUpload)
    COMMAND_HANDLER(IDC_WAVEMODE, BN_CLICKED, OnBnClickedWaveMode)
  END_MSG_MAP()

  LRESULT OnInitDialog(UINT,WPARAM,LPARAM,BOOL&)
  {
    CenterWindow();
    HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),  IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    SetIcon(hIconSmall, FALSE);
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);
    UIAddChildWindowContainer(m_hWnd);

    m_pRecordSound = new CRecordSound();
    
    m_pPlaySound   = new CPlaySound();
    m_pPlayMMSound->m_pPlaySound = m_pPlaySound;

    m_Pause.Attach(GetDlgItem(IDC_PAUSE));
    m_Play.Attach(GetDlgItem(IDC_PLAY));
    m_Upload.Attach(GetDlgItem(IDC_UPLOAD));
    m_DrawWave.Attach(GetDlgItem(IDC_DRAWWAVE));
    m_ZoomCtrl.Attach(GetDlgItem(IDC_SLIDER));

    m_ZoomCtrl.SetRange(50,1000,TRUE);//除数在50-1000之间变动!
    m_ZoomCtrl.SetPageSize(10);
    m_ZoomCtrl.SetPos( 275);

    m_Pause.EnableWindow(FALSE);
    m_Play.EnableWindow(FALSE);
    m_Upload.EnableWindow(FALSE);
    
    m_StartStop.Attach(GetDlgItem(IDC_START_STOP));
    m_StartStop.SetFocus();
    
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::audiodir);
    _6bees_util::MakeSureDir(upath.c_str());
    ::_wchdir(upath.c_str());

    SetTimer(1,200);    
    for (int i=0;i<NUM_BRUSHES;i++){
      //gBrushes[i]=::CreateSolidBrush(RGB(rand()%10,rand()%25,rand()%255));
      gBrushes[i]=::CreateSolidBrush(RGB(10,125,210));
    }
    bLen = 0;
    wavemode    = TRUE;
    int width   = 1;
    COLORREF col= RGB(10,155,255);
    gPens = CreatePen(PS_SOLID,width,col);

    //增加一张图片!
    m_bmp.LoadBitmap(IDB_SINGER);
    
    return TRUE;
  }

  LRESULT OnDestroy(UINT,WPARAM,LPARAM,BOOL&)
  {
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);
    return 0;
  }

  LRESULT OnAppAbout(WORD,WORD,HWND,BOOL&)
  {
    CAboutDlg dlg;
    dlg.DoModal();
    return 0;
  }

  LRESULT OnOK(WORD,WORD wID,HWND,BOOL&)
  {
    CloseDialog(wID);
    return 0;
  }

  LRESULT OnCancel(WORD,WORD wID,HWND,BOOL&)
  {
    CloseDialog(wID);
    return 0;
  }

  LRESULT OnBnClickedStartStop(WORD,WORD wID,HWND,BOOL&)
  {
    if(!m_pRecordSound->m_bRecording){
      //_Error_MessageBox(_T("开始录音！"));
      m_StartStop.SetWindowText(_T("STOP"));
      m_Pause.EnableWindow(TRUE);
      m_Play.EnableWindow(FALSE);
      m_Upload.EnableWindow(FALSE);
      m_pRecordSound->OnPtrSoundWriter(m_pWriteSound);
      if (!m_pRecordSound->StartRecord()){
        _Error_MessageBox(_T("ERROR: Cannot start to record."));
        CloseDialog(wID);
      }
      m_pRecordSound->m_bRecording = TRUE;
    }else{
      //_Error_MessageBox(_T("停止录音！"));
      m_Pause.EnableWindow(FALSE);
      m_Play.EnableWindow(TRUE);
      m_Upload.EnableWindow(TRUE);
      m_StartStop.EnableWindow(TRUE);
      m_StartStop.SetWindowText(_T("START"));
      m_pRecordSound->StopRecord();
      m_pRecordSound->m_bRecording = FALSE;

      //应该用一张图片来代替!
      RECT rc;
      ::GetClientRect(m_hWnd,&rc);
      InvalidateRect(&rc,TRUE);
    }
    return 0;
  }

  LRESULT OnBnClickedPly(WORD,WORD wID,HWND,BOOL&){
    //m_Play.EnableWindow(FALSE);
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::audiodir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      wstring fp = upath + L"sound.wav"; 
      m_pPlayMMSound->pb = &m_Play;
      m_pPlayMMSound->OpenSoundFile(fp.c_str());
    }
    return 0;
  }

  LRESULT OnBnClickedPause(WORD,WORD wID,HWND,BOOL&){
    //_Error_MessageBox(_T("回放录音！"));
    if (!m_pRecordSound->m_Pause){
      m_Pause.SetWindowText(_T("Resume"));
      m_pRecordSound->m_Pause = TRUE;
    }else{
      m_Pause.SetWindowText(_T("Pause"));
      m_pRecordSound->m_Pause = FALSE;
    }
    return 0;
  }

  LRESULT OnBnClickedUpload(WORD,WORD wID,HWND,BOOL&){
    HWND hwndTo = ::FindWindow(0,_6bees_const::kUploader_WndTitle);
    using _6bees_util::ultrapath;
    const wstring& upath = ultrapath::R().get(ultrapath::audiodir);
    if(_6bees_util::MakeSureDir(upath.c_str())){
      ATL::CString audiopath = upath.c_str();
      audiopath.Append(_T("sound.mp3"));
      if(!hwndTo){
        using _6bees_util::ultrapath;
        const wstring& upath2 = ultrapath::R().get(ultrapath::uploader);
        ATL::CString UploaderCmd = upath2.c_str();
        UploaderCmd += _T(" -m upload -f \"") + audiopath + _T("\"");

        STARTUPINFO si = {sizeof(si)};
        PROCESS_INFORMATION pi;
        if (!CreateProcess(NULL,(LPWSTR)UploaderCmd.GetString(),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
          _Error_MessageBox(_T("ERROR"));
        }
      }else{
        USES_CONVERSION;
        _6beed_util::SendMsgToUploader(hwndTo,W2CA(audiopath),CPYDATA_UPLOAD_2_UPLOAD_DIRECT);
      }
      CloseDialog(wID);
    }
    return 0;
  }

  LRESULT OnBnClickedWaveMode(WORD,WORD wID,HWND,BOOL&){
    wavemode = !wavemode;
    return 0;
  }
  
  void CloseDialog(int nVal)
  {
    DestroyWindow();
    ::PostQuitMessage(nVal);
  }

  LRESULT OnTimer(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
  {
    if (wParam==1){
      if (m_pRecordSound->m_bRecording){
        int fzoom = m_ZoomCtrl.GetPos();
        //画波形...
        HWND gHwnd = m_DrawWave.m_hWnd;
        CDC cdc = m_DrawWave.GetDC();
        HDC hdc = cdc.m_hDC;
        RECT rct;
        ::GetClientRect(gHwnd,&rct);
        HBRUSH gBrushes_back = ::CreateSolidBrush(RGB(0,0,0));
        ::FillRect(hdc,&rct,gBrushes_back);
        //InvalidateRect(&rct,TRUE);
        RECT r={0,0,0,0};
        bLen = m_pWriteSound->bLen/2;
        if (bLen==0 || m_pWriteSound->pWAVBuffer==NULL) return 0;
        gWAVBuffer = new SHORT[bLen];
        memcpy(gWAVBuffer ,m_pWriteSound->pWAVBuffer ,bLen);

        int interval = bLen/(2*(rct.right-rct.left-10));
        if(interval<1) return 0;
        if(!wavemode){
          //用FillRect画图
          int j=0;
          r.bottom= rct.bottom-5;
          for (;r.right<rct.right-5;j++)
          {
            r.left  = j*2+5;
            r.right = r.left+2;

            r.top   = gWAVBuffer[j*interval]/fzoom ;
            r.top   = r.top>5 ? r.top : 5;
            if (r.top>r.bottom) r.top=r.bottom-1;
            ::FillRect(hdc,&r,gBrushes[j%NUM_BRUSHES]);
          }
        }else{
          //用Polyline画图
          POINT* gPoints;
          gPoints = new POINT[bLen];
          int j=0;
          for (;j<(rct.right-rct.left-10);j++)
          {
            gPoints[j].x=rct.left+5+j;
            gPoints[j].y=gWAVBuffer[j*interval]/fzoom;
            if (gPoints[j].y<5)
              gPoints[j].y=5;
            if (gPoints[j].y>rct.bottom-5)
              gPoints[j].y=rct.bottom-5;
          }
          HGDIOBJ old=SelectObject(hdc,gPens);
          Polyline(hdc,gPoints,j);
          SelectObject(hdc,old);
          delete [] gPoints;
        }
        delete [] gWAVBuffer;
      }else{
        //增加一个美女图片
        RECT rct;
        ::GetClientRect(m_DrawWave.m_hWnd,&rct);
        CDC dcMem;
        CDCHandle dc = m_DrawWave.GetDC();
        dcMem.CreateCompatibleDC(dc);
        dc.SaveDC();
        dcMem.SaveDC();
        dcMem.SelectBitmap(m_bmp);
        dc.BitBlt(5,5,rct.right-10,rct.bottom-10,dcMem,0,0,SRCCOPY);
        dc.RestoreDC(-1);
        dcMem.RestoreDC(-1);
      }
    }
    return 0;
  }

private:
  CRecordSound*    m_pRecordSound;
  CPlaySound*      m_pPlaySound;

  //GUI elements....
  CWindow m_StartStop;
  CWindow m_Pause;
  CWindow m_Play;
  CWindow m_Upload;
  CWindow m_DrawWave;
  CTrackBarCtrl m_ZoomCtrl;

  CBitmap m_bmp;

  HBRUSH gBrushes[NUM_BRUSHES];
  HPEN   gPens;
  PSHORT gWAVBuffer;
  int bLen;
  BOOL wavemode;
};