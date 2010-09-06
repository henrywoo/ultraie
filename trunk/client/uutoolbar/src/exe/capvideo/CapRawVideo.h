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

