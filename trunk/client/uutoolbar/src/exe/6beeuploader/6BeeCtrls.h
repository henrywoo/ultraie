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