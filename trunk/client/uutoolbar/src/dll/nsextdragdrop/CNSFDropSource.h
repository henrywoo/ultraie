#if !defined(AFX_DROPSOURCE_H__20041129_C21E_600E_0DDB_0080AD509054__INCLUDED_)
#define AFX_DROPSOURCE_H__20041129_C21E_600E_0DDB_0080AD509054__INCLUDED_

#pragma once


//////////////////////////////////////////////////////////////////////////////
// CDropSource

class ATL_NO_VTABLE CNSFDropSource : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDropSource
{
public:

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CNSFDropSource)
    COM_INTERFACE_ENTRY_IID(IID_IDropSource,IDropSource)
END_COM_MAP()

// IDropSource
public:
    STDMETHOD(QueryContinueDrag)(BOOL bEsc, DWORD dwKeyState)
    {
        if( bEsc ) 
        	return ResultFromScode(DRAGDROP_S_CANCEL);

        if( (dwKeyState & MK_LBUTTON)==0 ) 
        	return ResultFromScode(DRAGDROP_S_DROP);

        return S_OK;
    }

    STDMETHOD(GiveFeedback)(DWORD)
    {
        return ResultFromScode(DRAGDROP_S_USEDEFAULTCURSORS);
    }
};

#endif // !defined(AFX_DROPSOURCE_H__20041129_C21E_600E_0DDB_0080AD509054__INCLUDED_)

