#include "stdafx.h"
#include "6BeeCtrls.h"

C6BeeFileDialog::C6BeeFileDialog(
	BOOL bOpenFileDialog,
	_U_STRINGorID szDefExt,
	_U_STRINGorID szFileName,
	DWORD dwFlags,
	_U_STRINGorID szFilter,
	HWND hwndParent)
	:CFileDialogImpl<C6BeeFileDialog>(bOpenFileDialog, NULL, NULL, dwFlags, NULL, hwndParent),
    m_sDefExt(szDefExt.m_lpstr),
	m_sfilelocation(szFileName.m_lpstr),
    m_sFilter(szFilter.m_lpstr)
{
	m_ofn.lpstrDefExt = m_sDefExt;
	m_ofn.lpstrFilter = PrepFilterString ( m_sFilter );
 
	// setup initial file name
	if ( !m_sfilelocation.IsEmpty() )
		lstrcpyn ( m_szFileName, m_sfilelocation, _MAX_PATH );

}

LPCTSTR C6BeeFileDialog::PrepFilterString(ATL::CString& sFilter){
	wchar_t* psz = sFilter.GetBuffer(0);
	LPCTSTR pszRet = psz;
	while ( '\0' != *psz ){
		if ( '|' == *psz )
			*psz++ = '\0';
		else
			psz = CharNext ( psz );
    }
	return pszRet;
}