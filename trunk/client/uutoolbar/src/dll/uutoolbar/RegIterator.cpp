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


#include "RegIterator.h"

RegIterator::RegIterator(void)
:m_nIndex(0){
    m_hKey.Create(Get6BeeAppRegistryKey(), _T("History"));
    m_nIndex = 0;
    operator++ (1);
}

RegIterator::~RegIterator(void){}

void RegIterator::operator++ (int){
    unsigned long size = 200;
    m_bValid = ((::RegEnumValue(m_hKey, m_nIndex++, m_szBuf, &size, 0, 0, 0, 0)) != ERROR_NO_MORE_ITEMS);
}


HKEY Get6BeeAppRegistryKey(){
    HKEY hAppKey = NULL;
    HKEY hCompanyKey = Get6BeeCompanyRegistryKey();
    DWORD dw;
    RegCreateKeyEx(hCompanyKey, sProfileName, 0, REG_NONE,REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL,&hAppKey, &dw);
    if (hCompanyKey != NULL)
        RegCloseKey(hCompanyKey);
    return hAppKey;
}

HKEY Get6BeeCompanyRegistryKey(){
    HKEY hSoftKey = NULL;
    HKEY hCompanyKey = NULL;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("software"), 0, KEY_WRITE | KEY_READ,&hSoftKey) == ERROR_SUCCESS)
    {
        DWORD dw;
        RegCreateKeyEx(hSoftKey, sRegistryKey, 0, REG_NONE,REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &hCompanyKey, &dw);
    }
    if (hSoftKey != NULL)
        RegCloseKey(hSoftKey);    
    return hCompanyKey;
}

// Delete ×¢²á±í: HKEY_CURRENT_USER\Software\UUToolBar\6BeeNetWorks\all\History
bool Clear6BeeSearchRegistryKey(){
  CRegKey m_hKey;
  m_hKey.Create(Get6BeeAppRegistryKey(), _T(""));
  if(m_hKey.RecurseDeleteKey(_T("History"))==ERROR_SUCCESS){
    return true;
  }else{
    ::MessageBox(NULL,_T("You have deleted the search history already!"),_T("6BeeToolBar!"),NULL);
    return false;
  }
}