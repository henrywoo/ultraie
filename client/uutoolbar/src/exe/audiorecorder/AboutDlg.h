// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
  enum { IDD = IDD_ABOUTBOX };

  BEGIN_MSG_MAP(CAboutDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
    COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
  END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//  LRESULT MessageHandler(UINT,WPARAM,LPARAM,BOOL&)
//  LRESULT CommandHandler(WORD,WORD,HWND,BOOL&)
//  LRESULT NotifyHandler(int, LPNMHDR, BOOL& )

  LRESULT OnInitDialog(UINT,WPARAM,LPARAM,BOOL&)
  {
    CenterWindow(GetParent());
    return TRUE;
  }

  LRESULT OnCloseCmd(WORD,WORD wID,HWND,BOOL&)
  {
    EndDialog(wID);
    return 0;
  }
};
