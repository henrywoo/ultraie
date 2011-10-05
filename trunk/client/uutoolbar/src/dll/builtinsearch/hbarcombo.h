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

#ifndef __UUTOOLBAR_HBARCOMBO__
#define __UUTOOLBAR_HBARCOMBO__

//COM
#include <comdef.h>//_bstr_t

//ATL
#include <atlbase.h>

//WTL
#include <atlapp.h>
#include <atlctrls.h>//CComboBox
#include <atlscrl.h>

#define HBAR_SHOWWINDOW       1000

class Chorizontalbar;

class CBandEditCtrl:
  public CWindowImpl<CBandEditCtrl, CEdit>,
  public IDropTarget
{
public:  
  CBandEditCtrl();
  virtual ~CBandEditCtrl();
  bool gotfocus;//TRUE if control has focus

public:
  DECLARE_WND_SUPERCLASS(NULL,CBandEditCtrl::GetWndClassName())

  STDMETHOD(TranslateAcceleratorIO)(LPMSG pMsg);
  // Support Drag and Drop
  STDMETHODIMP QueryInterface(REFIID, VOID**);
  STDMETHODIMP_(ULONG) AddRef(void);
  STDMETHODIMP_(ULONG) Release(void);

  STDMETHODIMP DragEnter(LPDATAOBJECT, DWORD, POINTL, LPDWORD);
  STDMETHODIMP DragOver(DWORD, POINTL, LPDWORD);
  STDMETHODIMP DragLeave(void);
  STDMETHODIMP Drop(LPDATAOBJECT, DWORD, POINTL, LPDWORD);

  void SetBand(Chorizontalbar* p_CHorizontalBar);

  BEGIN_MSG_MAP(CBandEditCtrl)
    ALT_MSG_MAP(1)
    COMMAND_CODE_HANDLER(EN_SETFOCUS,  OnSetFocus)
    COMMAND_CODE_HANDLER(EN_KILLFOCUS, OnLostFocus)
    COMMAND_CODE_HANDLER(EN_UPDATE,    OnEditChange)
    DEFAULT_REFLECTION_HANDLER()
  END_MSG_MAP()

  Chorizontalbar* m_pBand;
  LPDATAOBJECT m_pIDataObject;

protected:
  LRESULT OnEditChange(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
  LRESULT OnSetFocus(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
  LRESULT OnLostFocus(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);

  ULONG m_cRef;
  bool m_bAuto;
};

#endif