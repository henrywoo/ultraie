#pragma once

#include <atldlgs.h>

class C6BeeFileDialog : public CFileDialogImpl<C6BeeFileDialog>
{
public:
  // Construction
  C6BeeFileDialog( BOOL bOpenFileDialog,
                  _U_STRINGorID szDefExt = 0U,
                  _U_STRINGorID szFileName = 0U, 
                  DWORD dwFlags = OFN_HIDEREADONLY/*| OFN_OVERWRITEPROMPT*/ | OFN_ALLOWMULTISELECT,
                  _U_STRINGorID szFilter = 0U,
                  HWND hwndParent = NULL );
  // Maps
  BEGIN_MSG_MAP(C6BeeFileDialog)
    CHAIN_MSG_MAP(CFileDialogImpl<C6BeeFileDialog>)
  END_MSG_MAP()
 
  // Overrides
  void OnInitDone ( LPOFNOTIFY lpon )
  {
    GetFileDialogWindow().CenterWindow(lpon->lpOFN->hwndOwner);
  }

 
protected:
  LPCTSTR PrepFilterString( CString& sFilter );
  CString m_sDefExt, m_sfilelocation, m_sFilter;
};