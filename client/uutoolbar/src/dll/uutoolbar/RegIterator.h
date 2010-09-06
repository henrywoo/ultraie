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