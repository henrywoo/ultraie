#pragma once
#include "CapRawVideo.h"
#include "afxwin.h"
//#include <boost/shared_ptr.hpp>

class CCapVideoDlg : public CDialog{
public:
  CCapVideoDlg(CWnd* pParent = NULL);
  enum { IDD = IDD_CAPVIDEO_DIALOG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);

protected:
  HICON m_hIcon;
  UINT_PTR m_nTimer;

  virtual BOOL OnInitDialog();
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnSysCommand(UINT, LPARAM);
  DECLARE_MESSAGE_MAP()

public:
  //boost::shared_ptr<CCapRawVideo> m_caprawvideo;
  CCapRawVideo* m_caprawvideo;
  CStatic m_capdevinfo;
  CStatic m_cappreview;

public:
  void ShowDevInfo(int);
  CButton m_record2avi;
  afx_msg void OnBnClickedRecord2avi();
  afx_msg void OnBnClickedCapturepic();
  CButton m_shutdown;
  afx_msg void OnBnClickedShutdown();

  afx_msg void OnStartTimer();
  afx_msg void OnStopTimer();
  afx_msg void OnTimer(UINT nIDEvent);

  CStatic m_framerate;
  void ShowFrameRate();
};
