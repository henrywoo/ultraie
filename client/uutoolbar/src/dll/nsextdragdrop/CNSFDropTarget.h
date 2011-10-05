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


#if !defined(AFX_CNSFDROPTARGET_H__20041125_CE55_D769_D3CA_0080AD509054__INCLUDED_)
#define AFX_CNSFDROPTARGET_H__20041125_CE55_D769_D3CA_0080AD509054__INCLUDED_

#pragma once


//////////////////////////////////////////////////////////////////////////////
// CNSFDropTarget

class ATL_NO_VTABLE CNSFDropTarget :
  public CComObjectRootEx<CComSingleThreadModel>,
  public IDropTarget
{
public:

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  BEGIN_COM_MAP(CNSFDropTarget)
    COM_INTERFACE_ENTRY_IID(IID_IDropTarget,IDropTarget)
  END_COM_MAP()

public:

  //Data members
  CMyVirtualFolder *m_pFolder;  // parent folder
  UINT m_iAcceptFmt;    
  LPITEMIDLIST m_pidl;  //simple PIDL relative to parent folder
  CNWSPidlMgr  m_PidlMgr;


  HRESULT FinalConstruct()
  { 
    m_pFolder = NULL;
    m_iAcceptFmt = 0;

    return S_OK;
  }

  void FinalRelease()
  {
    ATLASSERT(m_pFolder);
    if(m_pFolder==NULL)
    {
      MessageBox(NULL,_T("CDropTarget::FinalRelease() m_pFolder==NULL"),_T("NSF"),MB_OK);
      return;
    }
    m_pFolder->Release();

    if( m_pidl != NULL)
    {
      m_PidlMgr.Delete(m_pidl);
      m_pidl=NULL;
    }

    m_iAcceptFmt = 0;
  }

  HRESULT _Init(CMyVirtualFolder *pFolder,/* HWND hWnd,*/LPCITEMIDLIST pidl)
  {
    ATLTRACE(_T("CDropTarget::_Init\n"));
    ATLASSERT(pFolder);
    if(pFolder==NULL)
    {
      MessageBox(NULL,_T("CDropTarget::_Init() pFolder==NULL"),_T("NSF"),MB_OK);
      return E_FAIL;
    }
    m_pFolder = pFolder;
    m_pFolder->AddRef();

    m_pidl=m_PidlMgr.Copy(pidl);
    return S_OK;
  }

  // Implementation
  DWORD _QueryDrop()
  {
    if( 0 == m_iAcceptFmt ) 
      return DROPEFFECT_NONE;

    if( FMT_HDROP_INDEX == m_iAcceptFmt)
      return DROPEFFECT_COPY;

    if( FMT_NSEDRAGDROP_INDEX == m_iAcceptFmt)
      return DROPEFFECT_MOVE;

    return DROPEFFECT_NONE;
  }

  // IDropTarget

  STDMETHOD(DragEnter)(LPDATAOBJECT pDataObj, 
    DWORD,// dwKeyState, 
    POINTL, 
    LPDWORD pdwEffect)
  {
    ATLTRACE(_T("CDropTarget::DragEnter\n"));
    ATLASSERT(pDataObj);

    // Does the drag source provide self-defined CF types?
    m_iAcceptFmt = 0;

    if( m_iAcceptFmt == 0 ) 
    {
      FORMATETC fe2 = { _Module.m_CFSTR_NSEDRAGDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
      if( S_OK == pDataObj->QueryGetData(&fe2) )
      {
        m_iAcceptFmt = FMT_NSEDRAGDROP_INDEX;
      }
      else
      {
        FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        if( S_OK == pDataObj->QueryGetData(&fe) )
        {
          m_iAcceptFmt = FMT_HDROP_INDEX;
        }
      }
    }
    *pdwEffect = _QueryDrop();
    return S_OK;
  }

  STDMETHOD(DragOver)(DWORD,// dwKeyState, 
    POINTL /*pt*/, LPDWORD pdwEffect)
  {
    *pdwEffect = _QueryDrop();
    return S_OK;
  }

  STDMETHOD(DragLeave)(VOID)
  {
    ATLTRACE(_T("CDropTarget::DragLeave\n"));
    m_iAcceptFmt = 0;
    return S_OK;
  }

  STDMETHOD(Drop)(LPDATAOBJECT pDataObj,
    DWORD,// dwKeyState,
    POINTL /*pt*/,
    LPDWORD pdwEffect)
  {
    ATLTRACE(_T("CDropTarget::Drop\n"));
    ATLASSERT(pDataObj);
    ATLASSERT(pdwEffect);
    ATLASSERT(m_pFolder);

    *pdwEffect = DROPEFFECT_NONE; // default to failed/cancelled

    // Determine drop effect...
    DWORD dwDropEffect = _QueryDrop();

    // Did we accept this drop effect?
    if( dwDropEffect == DROPEFFECT_NONE ) 
      return S_OK;

    // Drop data...
    HRESULT Hr;
    if( SUCCEEDED(Hr = m_pFolder->_DoDrop(pDataObj, dwDropEffect,m_pidl,m_iAcceptFmt) ) ) 
    {
      *pdwEffect = dwDropEffect;
      return  S_OK;
    }
    else
    {
      return Hr;
    }
  }
};

#endif // !defined(AFX_CDROPTARGET_H__20041125_CE55_D769_D3CA_0080AD509054__INCLUDED_)

