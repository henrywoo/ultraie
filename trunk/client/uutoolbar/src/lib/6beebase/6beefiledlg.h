#ifndef __WTL_6BEEFILEDLG__
#define __WTL_6BEEFILEDLG__

#include <atlstr.h>
#define _CSTRING_NS ATL
//WTL
#include <atlapp.h>

#include <atldlgs.h>

class C6BeeFileDialog:
  public CFileDialogImpl<C6BeeFileDialog>
{
public:
  // Construction
  C6BeeFileDialog( BOOL bOpenFileDialog,
    _U_STRINGorID szDefExt = 0U,
    _U_STRINGorID szFileName = 0U, 
    DWORD dwFlags = OFN_HIDEREADONLY/*| OFN_OVERWRITEPROMPT*/ | OFN_ALLOWMULTISELECT,
    _U_STRINGorID szFilter = 0U,
    HWND hwndParent = NULL ):
  CFileDialogImpl<C6BeeFileDialog>(bOpenFileDialog, NULL, NULL, dwFlags, NULL, hwndParent),
    m_sDefExt(szDefExt.m_lpstr),
    m_sFileName(szFileName.m_lpstr),
    m_sFilter(szFilter.m_lpstr)
  {
    m_ofn.lpstrDefExt = m_sDefExt;
    m_ofn.lpstrFilter = PrepFilterString(m_sFilter);

    // setup initial file name
    if(!m_sFileName.IsEmpty() )
      lstrcpyn(m_szFileName, m_sFileName, _MAX_PATH );

  }
  // Maps
  BEGIN_MSG_MAP(C6BeeFileDialog)
    CHAIN_MSG_MAP(CFileDialogImpl<C6BeeFileDialog>)
  END_MSG_MAP()

  // Overrides
  void OnInitDone(LPOFNOTIFY lpon ) {
    GetFileDialogWindow().CenterWindow(lpon->lpOFN->hwndOwner);
  }

protected:
  LPCTSTR PrepFilterString( ATL::CString& sFilter ){
    LPTSTR psz = sFilter.GetBuffer(0);
    LPCTSTR pszRet = psz;
    while('\0' != *psz ) {
      if('|' == *psz ){
        *psz++ = '\0';
      }else{
        psz = CharNext(psz );
      }
    }
    return pszRet;
  }

  ATL::CString m_sDefExt, m_sFileName, m_sFilter;
};

#endif
