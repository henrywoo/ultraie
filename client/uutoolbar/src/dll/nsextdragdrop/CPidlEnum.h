#ifndef _NEF_CPIDLENUM_H_
#define _NEF_CPIDLENUM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CNWSPidlMgr.h"

typedef struct tagENUMLIST
{
	struct tagENUMLIST   *pNext;
	LPITEMIDLIST         pidl;
}ENUMLIST, FAR *LPENUMLIST;

/////////////////////////////////////////////////////////////////////////////
// CPidlEnum

class ATL_NO_VTABLE CPidlEnum : 
   public CComObjectRootEx<CComSingleThreadModel>,
   public IEnumIDList
{
public:

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPidlEnum)
    COM_INTERFACE_ENTRY_IID(IID_IEnumIDList,IEnumIDList)
END_COM_MAP()

public:
   LPENUMLIST m_pFirst;
   LPENUMLIST m_pLast;
   LPENUMLIST m_pCurrent;
   CNWSPidlMgr  m_PidlMgr;

public:
	HRESULT FinalConstruct()
	{
		m_pFirst = m_pLast = m_pCurrent = NULL;
		return S_OK;
	}

	void FinalRelease()
	{
		DeleteList();
		m_pFirst = m_pLast = m_pCurrent = NULL;
	}

public:
	//IEnumIDList
	STDMETHOD (Next) (DWORD, LPITEMIDLIST*, LPDWORD);
	STDMETHOD (Skip) (DWORD);
	STDMETHOD (Reset) (void);
	STDMETHOD (Clone) (IEnumIDList **ppenum);

	//User Defined Funcs
	BOOL DeleteList(void);
	BOOL AddToEnumList(LPITEMIDLIST pidl);
    HRESULT _Init(LPCITEMIDLIST pidlRoot,DWORD dwFlags);
	HRESULT _AddPidls(ITEM_TYPE iItemType, LPTSTR pszPath);

};

#endif //_NEF_CPIDLENUM_H_