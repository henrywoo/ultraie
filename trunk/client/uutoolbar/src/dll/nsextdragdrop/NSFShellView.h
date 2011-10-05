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

// NSFShellView.h : Declaration of the CNSFShellView

#ifndef __NSFSHELLVIEW_H_
#define __NSFSHELLVIEW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"
#include <atlwin.h>
#include "MyVirtualFolder.h"
#include "CNWSPidlMgr.h"

typedef enum
{
  MI_DELETE_INDEX=0,
  MI_PROPERTIES_INDEX,
  MI_REFRESH_INDEX,
  MI_HELPTOPICS_INDEX,
}NSF_MI_INDEX;

#define ID_HELP_HELPTOPICS   (FCIDM_SHVIEWFIRST + 0x702)

/////////////////////////////////////////////////////////////////////////////
// CNSFShellView
class ATL_NO_VTABLE CNSFShellView : 
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<CNSFShellView, &CLSID_NSFShellView>,
  public IShellView
{
  enum { IDC_LISTVIEW = 123 };
  typedef enum {
    TBI_STD = 0,
    TBI_VIEW,
    TBI_LOCAL
  } TOOLBARITEM;

  typedef struct
  {
    TOOLBARITEM nType;
    TBBUTTON tb;
  } NS_TOOLBUTTONINFO, *PNS_TOOLBUTTONINFO;

public:
  HRESULT FinalConstruct();
  void FinalRelease();

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  BEGIN_COM_MAP(CNSFShellView)
    COM_INTERFACE_ENTRY_IID(IID_IShellView,IShellView)
    COM_INTERFACE_ENTRY(IOleWindow)
  END_COM_MAP()


  BEGIN_MSG_MAP(CNSFShellView)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
    MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
    MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
    NOTIFY_CODE_HANDLER(NM_SETFOCUS, OnNotifySetFocus)
    NOTIFY_CODE_HANDLER(NM_KILLFOCUS, OnNotifyKillFocus)
    NOTIFY_CODE_HANDLER(LVN_ITEMACTIVATE, OnItemActivated)
    NOTIFY_CODE_HANDLER(HDN_ITEMCLICK, OnHeaderClick)
    COMMAND_ID_HANDLER(ID_VIEW_REFRESH, OnRefresh)
    //Drag-and-drop
    NOTIFY_CODE_HANDLER(LVN_BEGINDRAG, OnBeginDrag)
    NOTIFY_CODE_HANDLER(LVN_BEGINRDRAG, OnBeginDrag)
  END_MSG_MAP()

public:

  static HRESULT WINAPI UpdateRegistry(BOOL bRegister){
    CComBSTR bstrDescription;

    CComBSTR bstrCLSID(CLSID_NSFShellView);
    bstrDescription.LoadString(IDS_DESCRIPTION);

    _ATL_REGMAP_ENTRY rm[] = { 
      { OLESTR("DESCRIPTION"), bstrDescription },
      { OLESTR("CLSID"), bstrCLSID },
      { NULL,NULL } };

      return _Module.UpdateRegistryFromResource(IDR_NSFSHELLVIEW, bRegister, rm);
  }

  //IOleWindow
  STDMETHOD(GetWindow)(HWND* phWnd);
  STDMETHOD(ContextSensitiveHelp)(BOOL);

  //IShellView
  STDMETHOD (TranslateAccelerator) (LPMSG);
  STDMETHOD (EnableModeless) (BOOL);
  STDMETHOD (UIActivate) (UINT);
  STDMETHOD (Refresh) (void);
  STDMETHOD (CreateViewWindow) (LPSHELLVIEW, LPCFOLDERSETTINGS, LPSHELLBROWSER, LPRECT, HWND*);
  STDMETHOD (DestroyViewWindow) (void);
  STDMETHOD (GetCurrentInfo) (LPFOLDERSETTINGS);
  STDMETHOD (AddPropertySheetPages) (DWORD, LPFNADDPROPSHEETPAGE, LPARAM);
  STDMETHOD (SaveViewState) (void);
  STDMETHOD (SelectItem) (LPCITEMIDLIST, UINT);
  STDMETHOD (GetItemObject) (UINT, REFIID, LPVOID*);

private:

  // Message handlers
  LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnSettingChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnNotifySetFocus(UINT /*CtlID*/, LPNMHDR /*lpnmh*/, BOOL &/*bHandled*/);
  LRESULT OnNotifyKillFocus(UINT /*CtlID*/, LPNMHDR /*lpnmh*/, BOOL &/*bHandled*/);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

  LRESULT OnHeaderClick(UINT CtlID, LPNMHDR lpnmh, BOOL &bHandled);
  LRESULT OnItemActivated(UINT CtlID, LPNMHDR lpnmh, BOOL &bHandled);
  LRESULT OnRefresh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

  //Drag-and-drop
  LRESULT OnBeginDrag(UINT CtlID, LPNMHDR lpnmh, BOOL &bHandled);

public:

  static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

  //////////////////////////////////////////////////////////////////////////////
  //    assistant functions

private:
  void    _GetShellSettings(SHELLFLAGSTATE &sfs, DWORD dwMask);
  LRESULT _ViewActivate(UINT uState);
  LRESULT _ViewDeactivate(void);
  void    _UpdateShellSettings(void);
  BOOL    _CreateListView(void);

public:	
  HRESULT _Init(CMyVirtualFolder *pFolder,LPCITEMIDLIST pidlRoot);	
  BOOL    _InitListView();
  HRESULT _FillListView();

  //////////////////////////////////////////////////////////////////////////////
  //    data members
public:

  const MSG* m_pCurrentMsg;
  HWND m_hWnd;//right panel window handler
  HWND m_hwndList;//ListView handler
  DWORD m_dwListViewStyle;//ListView type

  //Contains folder view information. 
  FOLDERSETTINGS m_FolderSettings;

  //Contains a set of flags that indicate the current shell settings.
  //This structure is used with the SHGetSettings function. 
  SHELLFLAGSTATE m_ShellFlags; 

  //keep the IShellBrowser interface pointer 
  //to allow communication with the Windows Explorer window. 
  LPSHELLBROWSER m_pShellBrowser;

  UINT m_uState;//current window state

  LPDROPTARGET m_pDropTarget; //保存RightPanel的DropTarget对象(以整个ListView为对象,而不针对其中单个的文件夹)

protected:

  CMyVirtualFolder *m_pFolder;    //pointer to current folder object
  LPITEMIDLIST  m_pidlRoot;
  HMENU m_hMenu;                //menu handler
  HACCEL m_hAccels;				//accel handler
  CNWSPidlMgr m_PidlMgr;
  BOOL       m_bForwardSort;   //current sort way
  USHORT     m_iSortedColumn;  //current sort column

};

#endif //__NSFSHELLVIEW_H_
