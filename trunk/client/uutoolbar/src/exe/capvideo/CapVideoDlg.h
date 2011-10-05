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
