#if !defined(AFX_EXTRACTICON_H__20041104_57EC_56A4_0219_0080AD509054__INCLUDED_)
#define AFX_EXTRACTICON_H__20041104_57EC_56A4_0219_0080AD509054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CNWSPidlMgr.h"

//////////////////////////////////////////////////////////////////////////////
// CExtractIcon

class ATL_NO_VTABLE CExtractIcon : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public IExtractIcon
{
public:

BEGIN_COM_MAP(CExtractIcon)
    COM_INTERFACE_ENTRY_IID(IID_IExtractIcon,IExtractIcon)
END_COM_MAP()

	LPITEMIDLIST m_pidl;
	CNWSPidlMgr  m_PidlMgr;

    HRESULT _Init(LPCITEMIDLIST pidl)
    {
		m_pidl=m_PidlMgr.Copy(pidl);
        return S_OK;
    }

    // GetIconLocation:Retrieves the location and index of an icon. 
    STDMETHOD(GetIconLocation)(UINT uFlags, 
                               LPTSTR pszIconFile, 
                               UINT /*cchMax*/, 
                               LPINT piIndex, 
                               LPUINT puFlags)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("IExtractIcon::GetIconLocation\n"));
        ATLASSERT(piIndex);
        ATLASSERT(puFlags);

		if(m_pidl == NULL || m_pidl->mkid.cb == 0)
			return E_FAIL;

		ITEM_TYPE  type;
		type=m_PidlMgr.GetItemType(m_pidl);

        switch( type) 
		{
        case NWS_FOLDER:
            *piIndex = (uFlags & GIL_OPENICON)==0 ? ICON_INDEX_FOLDER : ICON_INDEX_FOLDER_OPEN;
            break;

		case NWS_FILE:
			*piIndex = ICON_INDEX_FILE;
			break;

        default:
			return E_FAIL;            
            break;
        }

        if( pszIconFile ) 
		{
            // HACK: Explorer needs *something*! It also must be unique...
            ::wsprintf(pszIconFile, _T("NSF%ld%ld"), *piIndex, (long) uFlags);
        }
        *puFlags = GIL_NOTFILENAME ;

        return S_OK;
    }

    STDMETHOD(Extract)(LPCTSTR /*pstrName*/, 
                             UINT nIconIndex, 
                             HICON *phiconLarge, 
                             HICON *phiconSmall, 
                             UINT /*nIconSize*/)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("IExtractIcon::Extract\n"));
        ATLASSERT(phiconLarge);
        ATLASSERT(phiconSmall);

        *phiconLarge = ImageList_ExtractIcon(NULL, _Module.m_ImageLists.m_hImageListLarge, nIconIndex);
        *phiconSmall = ImageList_ExtractIcon(NULL, _Module.m_ImageLists.m_hImageListSmall, nIconIndex);

        return S_OK;
    };

};


#endif // !defined(AFX_EXTRACTICON_H__20041104_57EC_56A4_0219_0080AD509054__INCLUDED_)