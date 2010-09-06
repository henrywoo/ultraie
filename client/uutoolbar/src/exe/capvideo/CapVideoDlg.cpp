#include "stdafx.h"
#include "CapVideo.h"
#include "CapVideoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCapVideoDlg::CCapVideoDlg(CWnd* pParent):CDialog(CCapVideoDlg::IDD, pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_caprawvideo = NULL;
}

void CCapVideoDlg::DoDataExchange(CDataExchange* pDX){
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_CAPDEVINFO, m_capdevinfo);
  DDX_Control(pDX, IDC_PREVIEW,    m_cappreview);
  DDX_Control(pDX, IDC_RECORD2AVI, m_record2avi);
  DDX_Control(pDX, IDC_SHUTDOWN,   m_shutdown);
  DDX_Control(pDX, IDC_FRAMERATE, m_framerate);
}

BEGIN_MESSAGE_MAP(CCapVideoDlg, CDialog)
  ON_WM_PAINT()
  ON_WM_TIMER()
  ON_WM_SYSCOMMAND()
  ON_WM_QUERYDRAGICON()
  //}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_RECORD2AVI, OnBnClickedRecord2avi)
  ON_BN_CLICKED(IDC_CAPTUREPIC, OnBnClickedCapturepic)
  ON_BN_CLICKED(IDC_SHUTDOWN, OnBnClickedShutdown)
END_MESSAGE_MAP()

BOOL CCapVideoDlg::OnInitDialog(){
  CDialog::OnInitDialog();
  SetIcon(m_hIcon, TRUE);
  SetIcon(m_hIcon, FALSE);
  m_shutdown.EnableWindow(false);
  this->ShowDevInfo(0);
  this->ShowFrameRate();
  m_nTimer = SetTimer(1, 2000, 0);
  return TRUE;
}

void CCapVideoDlg::ShowDevInfo(int i){
  static wchar_t devinfo[300]={};
  wsprintf(devinfo,_T("%s"),m_caprawvideo->getdevinfo(i).c_str());
  m_capdevinfo.SetWindowText(devinfo);
}

void CCapVideoDlg::OnStopTimer(){
  KillTimer(m_nTimer);
}

void CCapVideoDlg::OnTimer(UINT nIDEvent){
  if(nIDEvent==1){
    ShowFrameRate();
  }
}

void CCapVideoDlg::ShowFrameRate(){
  static wchar_t frameinfo[20]={};
  wsprintf(frameinfo,_T("%dfps"),this->m_caprawvideo->getframerate());
  m_framerate.SetWindowText(frameinfo);
}

void CCapVideoDlg::OnPaint(){
  if (IsIconic()){
    CPaintDC dc(this);
    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;
    dc.DrawIcon(x, y, m_hIcon);
  }else{
    CDialog::OnPaint();
  }
}

void CCapVideoDlg::OnSysCommand(UINT i, LPARAM p){
  if(i == SC_CLOSE && strlen(m_caprawvideo->getavfilename())!=0)
    m_caprawvideo->PostThreadMessage(WM_SHUTDOWN,(WPARAM)0,(LPARAM)0);
  CWnd::OnSysCommand(i,p);
}

HCURSOR CCapVideoDlg::OnQueryDragIcon(){
  return static_cast<HCURSOR>(m_hIcon);
}

void CCapVideoDlg::OnBnClickedRecord2avi(){
  m_record2avi.EnableWindow(false);
  m_shutdown.EnableWindow(true);
  m_caprawvideo->PostThreadMessage(WM_RECORD2AVI,(WPARAM)0,(LPARAM)0);
}

void CCapVideoDlg::OnBnClickedCapturepic(){
  //AfxMessageBox(_T("take a picture!"));
  m_caprawvideo->PostThreadMessage(WM_CAPTUREPIC,(WPARAM)0,(LPARAM)0);
  
}

void CCapVideoDlg::OnBnClickedShutdown(){
  m_caprawvideo->PostThreadMessage(WM_SHUTDOWN,(WPARAM)0,(LPARAM)0);
  ::SendMessage(this->m_hWnd,WM_CLOSE,0,0);  
}