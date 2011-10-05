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

#include "vfw.h"

#include <vector>
#include <string>
using std::vector;//必须加这个
using std::string;
using std::wstring;

#define WM_STARTPREVIEW WM_USER+1500
#define WM_SHUTDOWN     WM_USER+1501
#define WM_RECORD2AVI   WM_USER+1502
#define WM_CAPTUREPIC   WM_USER+1503


class CCapRawVideo : public CWinThread
{
  DECLARE_DYNCREATE(CCapRawVideo)

public:
  CCapRawVideo();
  virtual ~CCapRawVideo();

public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();

protected:
  DECLARE_MESSAGE_MAP()

protected:
  afx_msg void StartPreview(WPARAM wParam, LPARAM lParam);
  afx_msg void StopCapApp(WPARAM wParam, LPARAM lParam);
  afx_msg void Record2avi(WPARAM wParam, LPARAM lParam);
  afx_msg void CapturePic(WPARAM wParam, LPARAM lParam);

public:
  wstring getdevinfo(unsigned int);
  HWND ghWndCap;
  wstring m_framerate;
  int getframerate();
  const char* getavfilename();
private:
  CAPTUREPARMS * m_captureParms;
  BITMAPINFO   * m_bitmapinfo;
  CAPDRIVERCAPS* m_capdrivercaps;
  CStatic* dlg;

  //should be a list of driver structure
  vector<wstring> capdevlist;
  int preview_area_width;
  int preview_area_height;
  char *av_extension;
  char *filename;
  CStringA final_filename;
};

