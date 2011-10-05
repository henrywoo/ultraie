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

