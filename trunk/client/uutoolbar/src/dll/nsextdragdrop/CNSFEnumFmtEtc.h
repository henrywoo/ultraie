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

#if !defined(AFX_ENUMFORMATETC_H__20041129_075F_904E_7D1B_0080AD509054__INCLUDED_)
#define AFX_ENUMFORMATETC_H__20041129_075F_904E_7D1B_0080AD509054__INCLUDED_

#pragma once


/////////////////////////////////////////////////////////////////////////////
// CEnumFORMATETC

class ATL_NO_VTABLE CNSFEnumFmtEtc : 
   public CComObjectRootEx<CComSingleThreadModel>, 
   public IEnumFORMATETC
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CNSFEnumFmtEtc)
   COM_INTERFACE_ENTRY(IEnumFORMATETC)
END_COM_MAP()

	ULONG m_dwEnumPos;

    HRESULT FinalConstruct()
    { 
		m_dwEnumPos=0;
		return S_OK;
	}

	void FinalRelease()
	{
	}  

   // IEnumFORMATETC

   STDMETHOD(Next)(ULONG, LPFORMATETC pFormatetc, ULONG *pdwCopied)
   {
      VALIDATE_POINTER(pFormatetc);

      if( pdwCopied!=NULL ) 
		  *pdwCopied = 1L;
      m_dwEnumPos++;

      switch( m_dwEnumPos ) 
	  {
      case 1:
         pFormatetc->cfFormat = _Module.m_CFSTR_NSEDRAGDROP;
         pFormatetc->ptd = NULL;
         pFormatetc->dwAspect = DVASPECT_CONTENT;
         pFormatetc->lindex = -1;
         pFormatetc->tymed = TYMED_HGLOBAL;
         break;
      case 2:
         pFormatetc->cfFormat = CF_HDROP;
         pFormatetc->ptd = NULL;
         pFormatetc->dwAspect = DVASPECT_CONTENT;
         pFormatetc->lindex = -1;
         pFormatetc->tymed = TYMED_HGLOBAL;
         break;
      default:
         if( pdwCopied!=NULL ) 
			 *pdwCopied = 0L;
         return S_FALSE;
      }      
      return S_OK;
   }

   STDMETHOD(Skip)(ULONG n)
   {
      m_dwEnumPos += n;
      return S_OK;
   }

   STDMETHOD(Reset)(void)
   {
      m_dwEnumPos = 0L;
      return S_OK;
   }

   STDMETHOD(Clone)(LPENUMFORMATETC*)
   {
      ATLTRACENOTIMPL(_T("CNSFEnumFmtEtc::Clone"));
   }
   
};


#endif // !defined(AFX_ENUMFORMATETC_H__20041129_075F_904E_7D1B_0080AD509054__INCLUDED_)

