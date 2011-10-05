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

/*********************************************************************************/
// ËµÃ÷:
//
// ²Î¿¼: 
// 1. http://alax.info/blog/category/technology/wtl/page/2/  CColorComboBox: a fancy color combo box control in ATL and WTL
//
/*********************************************************************************/
#ifndef __UUTOOLBAR_TOOLBARCOMBO__
#define __UUTOOLBAR_TOOLBARCOMBO__

//COM
#include <comdef.h>//_bstr_t

//ATL
#include <atlbase.h>

//WTL
#include <atlapp.h>
#include <atlctrls.h>//CComboBox

const int kWM_TBCOMBOSEARCH_WEB = WM_USER+1223;

class CToolBarCOM;

typedef CWinTraits<WS_CHILD|WS_BORDER|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|CBS_DROPDOWN, 0> CComboBoxTraits;
class CToolBarCombo:
  public CWindowImpl< CToolBarCombo,CComboBox,CComboBoxTraits>,
  public IDropTarget
{
public:  
  DECLARE_WND_SUPERCLASS(TEXT("TOOLBARCOMBO"), TEXT("COMBOBOX"))

  BEGIN_MSG_MAP(CToolBarCombo)
    //MSG_WM_DROPDOWN(OnDropDown)
    MESSAGE_HANDLER(WM_PAINT,OnPaint)
    //MESSAGE_HANDLER(WM_CTLCOLOR,OnCtlColor)
    //COMMAND_CODE_HANDLER(CBN_SETFOCUS, OnSetFocus)
  ALT_MSG_MAP(1)
    COMMAND_CODE_HANDLER(CBN_SETFOCUS,  OnSetFocus)
    COMMAND_CODE_HANDLER(CBN_KILLFOCUS, OnLostFocus)
    COMMAND_CODE_HANDLER(CBN_EDITUPDATE,OnEditChange)
    COMMAND_CODE_HANDLER(CBN_SELENDOK,  OnSelChange)  
    //REFLECTED_NOTIFY_CODE_HANDLER(CBN_SETFOCUS, OnSetFocus)
    //REFLECTED_COMMAND_CODE_HANDLER(CBN_SETFOCUS, OnSetFocus)
    //CHAIN_MSG_MAP_ALT(COwnerDraw<CToolBarCombo>,1)
        
    DEFAULT_REFLECTION_HANDLER()
  END_MSG_MAP()

  // Handler prototypes:
  //  LRESULT MessageHandler(UINT,WPARAM,LPARAM,BOOL&);
  //  LRESULT CommandHandler(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
  //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
  
  LRESULT OnPaint(UINT,WPARAM,LPARAM,BOOL&);
  LRESULT OnUpdateBoxLen(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

  //LRESULT OnSetFocus(int wParam, LPNMHDR lParam, BOOL bHandled);
  //LRESULT OnSetFocus(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
  LRESULT OnSetFocus(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
  LRESULT OnLostFocus(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
  LRESULT OnEditChange(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
  LRESULT OnSelChange(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
  //LRESULT OnDropDown(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT AddtoHistory(const _bstr_t& varURL);

  STDMETHOD(TranslateAcceleratorIO)(LPMSG lpMsg);
  void SetBand(CToolBarCOM* p_CToolBarCOM);
  bool InsertHistory();

  //void DrawItem(LPDRAWITEMSTRUCT lpdis);

  // Support Drag and Drop
  //********************************************************************************
  STDMETHODIMP QueryInterface(REFIID, VOID**);
  STDMETHODIMP_(ULONG) AddRef(void);
  STDMETHODIMP_(ULONG) Release(void);
    STDMETHODIMP DragEnter(LPDATAOBJECT, DWORD, POINTL, LPDWORD);
  STDMETHODIMP DragOver(DWORD, POINTL, LPDWORD);
  STDMETHODIMP DragLeave(void);
  STDMETHODIMP Drop(LPDATAOBJECT, DWORD, POINTL, LPDWORD);
  //********************************************************************************

  CToolBarCombo();
  virtual ~CToolBarCombo();

  bool gotfocus;//TRUE if control has focus

  LPDATAOBJECT m_pIDataObject;

private:
  CToolBarCOM* p_CToolBarCOM;
  ULONG m_cRef;
  bool m_bAuto;

  void updateBoxLen(const wchar_t* txt=NULL);
};

#endif