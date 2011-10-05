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