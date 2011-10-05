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

#if !defined(AFX_DATAOBJECT_H__20041129_075F_904E_7D1B_0080AD509054__INCLUDED_)
#define AFX_DATAOBJECT_H__20041129_075F_904E_7D1B_0080AD509054__INCLUDED_

#pragma once

#include "CNSFEnumFmtEtc.h"
#include "Util.h"
#include <dinput.h>

/////////////////////////////////////////////////////////////////////////////
// CNSFDataObject

class ATL_NO_VTABLE CNSFDataObject : 
    public CComObjectRootEx<CComSingleThreadModel>, 
    public IDataObject
{
public:

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CNSFDataObject)
    COM_INTERFACE_ENTRY(IDataObject)
END_COM_MAP()

    CMyVirtualFolder* m_pFolder;   // current folder object
    LPITEMIDLIST m_pidl;   // The complex PIDL of selected file
    //HWND m_hWnd;           // The source window's HWND
	CNWSPidlMgr m_PidlMgr;

    HRESULT FinalConstruct()
    {
        m_pFolder = NULL;
        //m_hWnd = NULL;
        return S_OK;
    }

    void FinalRelease()
    {
		//HRESULT Hr;
		ATLASSERT(m_pFolder);
		if(m_pFolder==NULL)
		{
			MessageBox(NULL,_T("CNSFDataObject()::FinalRelease() m_pFolder==NULL"),_T("NSF"),MB_OK);
			return;
		}

        m_pFolder->Release();

		if(	m_pidl != NULL)
		{
			m_PidlMgr.Delete(m_pidl);
			m_pidl=NULL;
		}
		//m_hWnd = NULL;
    }

    HRESULT _Init(CMyVirtualFolder *pFolder, /*HWND hWnd,*/ LPCITEMIDLIST pidl)
    {
        ATLTRACE(_T("CNSFDataObject::_Init\n"));
        ATLASSERT(pFolder);
		if(pFolder==NULL)
		{
			MessageBox(NULL,_T("CDataObject()::_Init() pFolder==NULL"),_T("NSF"),MB_OK);
			return E_FAIL;
		}

        ATLASSERT(pidl);
        m_pFolder = pFolder;
        m_pFolder->AddRef();

        //m_hWnd = hWnd;
		m_pidl=m_PidlMgr.Copy(pidl);

        return S_OK;
    }
	HRESULT _GetLMouseStatus(BOOL *bM_LButtonUp)
	{
		HRESULT Hr;
		// Mouse Device handle
		LPDIRECTINPUT           pDirectInput=NULL; 
		LPDIRECTINPUTDEVICE     pMouse=NULL; 
		BOOL Mouse_LButtonDown;

		*bM_LButtonUp =FALSE;

		// Get Mouse Divice Handler
		if(FAILED( Hr= DirectInputCreate(GetModuleHandle(NULL), DIRECTINPUT_VERSION, &pDirectInput, NULL) ) )
			goto END_GETLMOUSESTATUS;

		if(FAILED( Hr= pDirectInput->CreateDevice(GUID_SysMouse, &pMouse, NULL) ) )
			goto END_GETLMOUSESTATUS;

		if(FAILED( Hr= pMouse->SetDataFormat( &c_dfDIMouse ) ) )
			goto END_GETLMOUSESTATUS;

		if(FAILED( Hr= pMouse->Acquire() ) )
			goto END_GETLMOUSESTATUS;

		//获取目前鼠标状态
		DIMOUSESTATE dims;      // DirectInput mouse state structure
		ZeroMemory( &dims, sizeof(dims) );
		Hr = pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims );
		if( FAILED(Hr) ) 
		{
			Hr = pMouse->Acquire();

			while( Hr == DIERR_INPUTLOST ) 
				Hr = pMouse->Acquire();

			//Hr!=DIERR_INPUTLOST
			if(FAILED(Hr))
				goto END_GETLMOUSESTATUS;
		}

		//1:按下 0:弹起
		Mouse_LButtonDown = (dims.rgbButtons[0] & 0x80) ? 1 : 0;

		if( Mouse_LButtonDown == 0)
		{
			*bM_LButtonUp =TRUE;
		}
		if( Mouse_LButtonDown == 1)
		{
			*bM_LButtonUp =FALSE;
		}

	END_GETLMOUSESTATUS:

		//Release Mouse Device Handle
		if(pMouse)
			pMouse->Unacquire();

		if(pMouse)
		{
			pMouse->Release();
			pMouse = NULL;
		}
		if(pDirectInput)
		{
			pDirectInput->Release();
			pDirectInput = NULL;
		}
		return Hr;
	}

private:
// IDataObject
public:

	//Called by a data consumer(Shell or NSE itself) to obtain data from a source data object. 
    STDMETHOD(GetData)(FORMATETC *pFormatetc, STGMEDIUM *pMedium)
    {
        ATLTRACE(_T("CNSFDataObject::GetData\n"));
        ATLASSERT(m_pFolder);
		if(m_pFolder==NULL)
		{
			MessageBox(NULL,_T("CDataObject()::GetData() m_pFolder==NULL"),_T("NSF"),MB_OK);
			return E_FAIL;
		}
        VALIDATE_POINTER(pFormatetc);
        VALIDATE_POINTER(pMedium);
        HRESULT Hr;
        ::ZeroMemory(pMedium, sizeof(STGMEDIUM));

		if( pFormatetc->cfFormat == _Module.m_CFSTR_NSEDRAGDROP ) //Drag-and-Drop inside NSE
		{
			//Transfer the PIDL
			UINT dwSize = m_PidlMgr.GetByteSize(m_pidl);

			HGLOBAL hMem = ::GlobalAlloc(GMEM_ZEROINIT| GMEM_MOVEABLE | GMEM_SHARE | GMEM_DISCARDABLE, dwSize);
			if( hMem==NULL ) 
				return E_OUTOFMEMORY;

			LPTSTR  psz = (LPTSTR)::GlobalLock(hMem);
			ATLASSERT(psz);
			if(NULL == psz )
			{
				::GlobalFree(hMem );
				return E_UNEXPECTED;
			}

			::CopyMemory(psz,m_pidl,dwSize);

			::GlobalUnlock(hMem);            
			pMedium->tymed = TYMED_HGLOBAL;
			pMedium->hGlobal = hMem;
			pMedium->pUnkForRelease = NULL;

			return S_OK;
		}
		else if( pFormatetc->cfFormat == CF_HDROP ) //从NSF到Explorer
		{
			//Transfer the temporary create file name
			//CFSTR_FILEDESCRIPTORA
			//bM_LButtonUp= (FALSE:按下 TRUE:弹起)
			BOOL bM_LButtonUp = FALSE;
			HR(_GetLMouseStatus(&bM_LButtonUp));

			TCHAR szTmpFileName[MAX_PATH]={};
			//1 create a temporary file use the selected object's name

			if( bM_LButtonUp == TRUE )
			{
				DWORD dwSize=MAX_PATH;

				//1.1 get selected object's name
				TCHAR szFileName[MAX_PATH]={};
				LPITEMIDLIST  pidlTemp = m_PidlMgr.GetLastItem(m_pidl);
				HR(m_PidlMgr.GetName(pidlTemp,szFileName));
				if(dwSize == 0)
					return E_FAIL;

				//1.2 create a temporary file use the name
				dwSize=MAX_PATH;
				::GetTempPath(dwSize,szTmpFileName);
				//_tcscat(szTmpFileName,_T("\\"));
				_tcscat(szTmpFileName,szFileName);

				FILE* fpTmp = _tfopen(szTmpFileName,_T("a"));
				if(fpTmp == NULL)
					return E_FAIL;
				fclose(fpTmp);
			}

	        //Alloc memory for data: DropFile Struct + FileName string
			//the data that follows is a double null-terminated list of file names. 
			HGLOBAL hgDrop = ::GlobalAlloc(GPTR, sizeof(DROPFILES) + sizeof(TCHAR) * (_tcslen(szTmpFileName)+1) );
			if(NULL == hgDrop )
				return E_OUTOFMEMORY;

			DROPFILES* pDrop = (DROPFILES*) ::GlobalLock(hgDrop );
			ATLASSERT(pDrop);
			if(NULL == pDrop )
			{
				::GlobalFree(hgDrop );
				return E_UNEXPECTED;
			}
			
	        // Fill in the DROPFILES struct.
		    pDrop->pFiles = sizeof(DROPFILES);
#ifdef _UNICODE
			// If we're compiling for Unicode, set the Unicode flag in the struct to
			// indicate it contains Unicode strings.
			pDrop->fWide = TRUE;
#endif
			// Append FileName data after DROPFILES Structure
			LPTSTR pszBuff = (LPTSTR) (LPBYTE(pDrop) + sizeof(DROPFILES));
			_tcscpy( pszBuff, (LPCTSTR) szTmpFileName );

			::GlobalUnlock(hgDrop );

			pMedium->tymed = TYMED_HGLOBAL;
			pMedium->hGlobal = hgDrop;
			pMedium->pUnkForRelease = NULL;

			return S_OK;
		}

        return DV_E_FORMATETC;
    }

    STDMETHOD(GetDataHere)(FORMATETC* /*pFormatetc*/, STGMEDIUM* /*pMedium*/)
    {
        ATLTRACENOTIMPL(_T("CNSFDataObject::GetDataHere"));
    }

    STDMETHOD(QueryGetData)(FORMATETC* pFormatetc)
    {
        ATLASSERT(pFormatetc);
		if(pFormatetc == NULL)
		{
			MessageBox(NULL,_T("CDataObject::QueryGetData() pFormatetc == NULL"),_T("NSF"),MB_OK);
			return E_INVALIDARG;
		}
		
        if( pFormatetc->cfFormat != _Module.m_CFSTR_NSEDRAGDROP &&
             pFormatetc->cfFormat != CF_HDROP )
             return DV_E_FORMATETC;

		if(pFormatetc->ptd!=NULL)
			return E_INVALIDARG;
		
        if( (pFormatetc->dwAspect & DVASPECT_CONTENT)==0 ) 
            return DV_E_DVASPECT;
		
		if(pFormatetc->lindex !=-1)
			return DV_E_LINDEX;

		if((pFormatetc->tymed & TYMED_HGLOBAL) ==0)
			return DV_E_TYMED;

        return S_OK;
    }

    STDMETHOD(EnumFormatEtc)(DWORD /*dwDirection*/, IEnumFORMATETC** ppenumFormatEtc)
    {
        ATLTRACE(_T("CNSFDataObject::EnumFormatEtc\n"));
        VALIDATE_POINTER(ppenumFormatEtc);
        HRESULT Hr;

        CComObject<CNSFEnumFmtEtc> *obj;
        HR( CComObject<CNSFEnumFmtEtc>::CreateInstance(&obj) );
		obj->AddRef();

        Hr=obj->QueryInterface(IID_IEnumFORMATETC, (LPVOID *)ppenumFormatEtc);
		obj->Release();
		return Hr;
    }
	//Called by an object containing a data source(例如CNSFShellView创建了一个IDataObject对象)
	//to transfer data (保存在pMedium中)to
	//the object that implements this method
	//fRelease:约定由谁释放pMedium所占资源(true:IDataObject对象负责;false:pMedium的创建者)
    STDMETHOD(SetData)(FORMATETC*,// pFormatEtc,
      STGMEDIUM*,// pMedium,
      BOOL// fRelease
      )
    {
        ATLTRACENOTIMPL(_T("CNSFDataObject::SetData"));
    }

    STDMETHOD(GetCanonicalFormatEtc)(FORMATETC*, FORMATETC*)
    {
        ATLTRACENOTIMPL(_T("CNSFDataObject::GetCanonicalFormatEtc"));
    }

    STDMETHOD(DAdvise)(FORMATETC *, DWORD, IAdviseSink *, DWORD *)
    {
        ATLTRACENOTIMPL(_T("CNSFDataObject::DAdvise"));
    }

    STDMETHOD(DUnadvise)(DWORD)
    {
        ATLTRACENOTIMPL(_T("CNSFDataObject::DUnadvise"));
    }

    STDMETHOD(EnumDAdvise)(IEnumSTATDATA **)
    {
        ATLTRACENOTIMPL(_T("CNSFDataObject::EnumDAdvise"));
    }

};

#endif // !defined(AFX_DATAOBJECT_H__20041129_075F_904E_7D1B_0080AD509054__INCLUDED_)

