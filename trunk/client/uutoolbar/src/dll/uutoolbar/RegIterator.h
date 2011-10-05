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

#ifndef __UUTOOLBAR_REGITERATOR__
#define __UUTOOLBAR_REGITERATOR__

#include <atlbase.h>//CRegKey,_T,BSTR

const BSTR sRegistryKey = _T("UUToolBar\\6BeeNetWorks\\");
const BSTR sProfileName = _T("all");
HKEY Get6BeeAppRegistryKey();
HKEY Get6BeeCompanyRegistryKey();
bool Clear6BeeSearchRegistryKey();

class RegIterator{
public:
  RegIterator();
    ~RegIterator();
    
    void operator++ (int);
    operator bool() {return m_bValid;}
    operator TCHAR* () { return m_szBuf;}
    operator int() { return m_nIndex - 1;}

protected:
    CRegKey m_hKey;
    TCHAR   m_szBuf[200];
    int     m_nIndex;
    bool    m_bValid;
};

#endif