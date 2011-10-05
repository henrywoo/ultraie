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

#include "stdafx.h"
#include "NSFShellView.h"
#include "MyVirtualFolder.h"

//add for drag drop
#include "CNSFDataObject.h"
#include "CNSFDropSource.h"

TCHAR g_szMenuItemCaptions_English[][20]={ _T("&Delete"),_T("P&roperties"),_T("R&efresh"),_T("&About NSF")};

#define MENU_OFFSET 1 
#define MENU_MAX 100 

/////////////////////////////////////////////////////////////////////////////
// CNSFShellView

HRESULT CNSFShellView::FinalConstruct()
{
  ATLTRACE(_T("CNSFShellView::FinalConstruct\n"));

  m_pShellBrowser=NULL;
  m_hWnd=NULL;
  m_hwndList=NULL;
  m_uState=SVUIA_DEACTIVATE; 

  ::ZeroMemory(&m_ShellFlags,sizeof(m_ShellFlags));

  m_pFolder = NULL;
  m_hMenu = NULL;
  m_dwListViewStyle = WS_TABSTOP | 
    WS_VISIBLE |
    WS_CHILD | 
    LVS_REPORT |
    LVS_EDITLABELS |
    LVS_SHOWSELALWAYS |
    LVS_SHAREIMAGELISTS |
    LVS_SINGLESEL;

  m_iSortedColumn = 0;
  m_bForwardSort = TRUE;

  return S_OK;
}

void CNSFShellView::FinalRelease()
{

  ATLASSERT(m_pFolder);
  m_pFolder->Release();

  m_pShellBrowser=NULL;
  m_hWnd=NULL;
  m_hwndList=NULL;
  m_hMenu = NULL;

  if(m_pidlRoot != NULL)
  {
    m_PidlMgr.Delete(m_pidlRoot);
    m_pidlRoot = NULL;
  }
}

/////////////////////////////////////////////////////////////////////////////
// IOleWindow

HRESULT CNSFShellView::GetWindow(HWND* phWnd)
{
  ATLTRACE2(atlTraceCOM, 0, _T("IOleWindow::GetWindow\n"));
  ATLASSERT(phWnd);

  if(::IsWindow(m_hWnd)==0)
    return E_FAIL;

  ATLASSERT(::IsWindow(m_hWnd));
  *phWnd = m_hWnd;
  return S_OK;
}

HRESULT CNSFShellView::ContextSensitiveHelp(BOOL)
{
  ATLTRACENOTIMPL(_T("IOleWindow::ContextSesitiveHelp"));
}

/////////////////////////////////////////////////////////////////////////////
// IShellView

HRESULT CNSFShellView::AddPropertySheetPages(
  DWORD /*dwReserved*/,
  LPFNADDPROPSHEETPAGE /*lpfn*/, 
  LPARAM /*lParam*/)
{
  ATLTRACENOTIMPL(_T("IShellView::AddPropertySheetPages"));
}

STDMETHODIMP CNSFShellView::CreateViewWindow(
  LPSHELLVIEW lpPrevView,
  LPCFOLDERSETTINGS lpFS, 
  LPSHELLBROWSER pSB,
  LPRECT prcView, 
  HWND* phWnd)
{

  ATLTRACE("CNSFShellView::CreateViewWindow\n");
  ATLASSERT(m_pFolder);
  if( m_pFolder==NULL ) 
  {
    MessageBox(NULL,_T("CNSFShellView::CreateViewWindow() m_pFolder==NULL"),_T("NSF"),MB_OK);
    return E_FAIL;
  }
  ATLASSERT(prcView);
  ATLASSERT(pSB);
  ATLASSERT(lpFS);
  ATLASSERT(phWnd);

  HRESULT Hr;

  // Register the ClassName.
  // The ClassName comes from the string resource IDS_CLASSNAME!
  TCHAR szClassName[64] = { 0 };
  ::LoadString(_Module.GetResourceInstance(), IDS_CLASSNAME, szClassName, (sizeof(szClassName) / sizeof(TCHAR)) - 1);
  WNDCLASS wc = { 0 };
  *phWnd = NULL;

  // If our window class has not been registered, then do so
  if( !::GetClassInfo(_Module.GetModuleInstance(), szClassName, &wc) ) 
  {
    //wc.style = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _Module.GetModuleInstance();
    wc.hIcon = NULL;
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szClassName; 

    if( !::RegisterClass(&wc) ) 
      return HRESULT_FROM_WIN32(::GetLastError());
  }

  // Set up the member variables
  m_pShellBrowser = pSB;
  m_FolderSettings = *lpFS;

  m_ShellFlags.fWin95Classic = TRUE; //Classic Style option is enabled 
  m_ShellFlags.fShowAttribCol = TRUE; //Show File Attributes in Detail View option is enabled 
  m_ShellFlags.fShowAllObjects = TRUE; //Show All Files option is enabled 

  // Load the ListView settings from the stream
  // FIX: This fixes a problem where XP doesn't relay this attribute
  CComPtr<IStream> spStream;
  if( SUCCEEDED( m_pShellBrowser->GetViewStateStream(STGM_READ, &spStream) ) ) 
  {
    long lVersion = 0;
    DWORD dwRead = 0;
    spStream->Read(&lVersion, sizeof(lVersion), &dwRead);
    spStream->Read(&m_FolderSettings.ViewMode, sizeof(m_FolderSettings.ViewMode), &dwRead);
  }

  // Get our parent window
  HWND hwndShell = NULL;
  m_pShellBrowser->GetWindow(&hwndShell);

  // Create host window
  *phWnd = ::CreateWindowEx(WS_EX_CONTROLPARENT,
    szClassName,
    NULL,
    WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP,
    prcView->left,
    prcView->top,
    prcView->right - prcView->left,
    prcView->bottom - prcView->top,
    hwndShell,
    NULL,
    _Module.GetModuleInstance(),
    (LPVOID) this);
  if( *phWnd == NULL ) 
    return HRESULT_FROM_WIN32(::GetLastError());

  m_pShellBrowser->AddRef();

  //add for enable the drop target
  HR( m_pFolder->CreateViewObject(m_hWnd, IID_IDropTarget, (LPVOID*) &m_pDropTarget) );
  if(m_pDropTarget==NULL)
  {
    MessageBox(NULL,_T("CNSFShellView::CreateViewWindow() m_pDropTarget==NULL"),_T("NSF"),MB_OK);
    return E_FAIL;
  }
  HR( ::RegisterDragDrop(m_hwndList, m_pDropTarget) );

  return S_OK;
}

STDMETHODIMP CNSFShellView::DestroyViewWindow(void)
{
  ATLTRACE("CNSFShellView::DestroyViewWindow\n");
  if(::IsWindow(m_hwndList)==0)
    return E_FAIL;

  ATLTRACE2(atlTraceCOM, 0, _T("IShellView::DestroyViewWindow\n"));
  ATLASSERT(m_pShellBrowser);

  //Unregister the drag drop
  ::RevokeDragDrop(m_hwndList);
  m_pDropTarget->Release();

  // Make absolutely sure all our UI is cleaned up.
  UIActivate(SVUIA_DEACTIVATE);

  ::DestroyWindow(m_hWnd);

  // Release the shell browser object
  m_pShellBrowser->Release();
  return S_OK;
}

HRESULT CNSFShellView::EnableModeless(BOOL /*fEnable*/)
{
  ATLTRACENOTIMPL(_T("IShellView::EnableModeless"));
}

//GetCurrentInfo(): Retrieves the current folder settings. 
HRESULT CNSFShellView::GetCurrentInfo(LPFOLDERSETTINGS lpFS)
{
  ATLTRACE2(atlTraceCOM, 0, _T("IShellView::GetCurrentInfo\n"));
  ATLASSERT(lpFS);
  *lpFS = m_FolderSettings;
  return S_OK;
}
HRESULT CNSFShellView::Refresh(void)
{
  ATLTRACE2(atlTraceCOM, 0, _T("IShellView::Refresh\n"));
  ATLASSERT(::IsWindow(m_hwndList));
  if(::IsWindow(m_hwndList)==0)
    return E_FAIL;

  // Empty the list
  ListView_DeleteAllItems(m_hwndList);

  // Refill the list
  _FillListView();

  return S_OK;
}

HRESULT CNSFShellView::SaveViewState(void)
{
  ATLASSERT(m_pShellBrowser);
  CComPtr<IStream> spStream;

  if( FAILED( m_pShellBrowser->GetViewStateStream(STGM_WRITE, &spStream) ) ) 
    return S_OK;
  long lVersion = 1;
  DWORD dwWritten = 0;
  spStream->Write(&lVersion, sizeof(lVersion), &dwWritten);
  spStream->Write(&m_FolderSettings.ViewMode, sizeof(m_FolderSettings.ViewMode), &dwWritten);
  return S_OK;
}

HRESULT CNSFShellView::SelectItem(LPCITEMIDLIST /*pidlItem*/, UINT /*uFlags*/)
{
  ATLTRACENOTIMPL(_T("IShellView::SelectItem"));
}


STDMETHODIMP CNSFShellView::GetItemObject(UINT uItem, REFIID riid, LPVOID* ppRetVal)
{
  return E_NOINTERFACE;
}

//TranslateAccelerator():Translates accelerator key strokes 
// when a namespace extension's view has the focus. 
STDMETHODIMP CNSFShellView::TranslateAccelerator(LPMSG pmsg)
{
  return S_FALSE;
}

HRESULT CNSFShellView::UIActivate(UINT uState)
{
  ATLTRACE2(atlTraceCOM, 0, _T("IShellView::UIActivate (%d)\n"), uState);
  ATLASSERT(m_pShellBrowser);

  // don't do anything if the state isn't really changing
  if( m_uState == uState ) 
    return S_OK; 

  _ViewActivate(uState);

  if( uState != SVUIA_DEACTIVATE) 
  {
    // Update the status bar: set 'parts' and change text
    LRESULT lResult;
    INT nPartArray[1] = { -1 };

    m_pShellBrowser->SendControlMsg(FCW_STATUS, SB_SETPARTS, 1, (LPARAM)nPartArray, &lResult);

    // Set the statusbar text to the default description.
    // The string resource IDS_DESCRIPTION must be defined!
    TCHAR szName[128] = { 0 };
    ::LoadString(_Module.GetResourceInstance(), IDS_DESCRIPTION, szName, (sizeof(szName) / sizeof(TCHAR)) - 1);

    m_pShellBrowser->SendControlMsg(FCW_STATUS, SB_SETTEXT, 0, (LPARAM)szName, &lResult);
  }

  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

HRESULT CNSFShellView::_Init(CMyVirtualFolder* pFolder,LPCITEMIDLIST pidlRoot)
{
  ATLTRACE("CNSFShellView::_Init\n");
  ATLASSERT(pFolder);
  if( pFolder==NULL ) 
  {
    MessageBox(NULL,_T("CNSFShellView::_Init() pFolder==NULL"),_T("NSF"),MB_OK);
    return E_FAIL;
  }
  m_pFolder = pFolder;
  m_pFolder->AddRef();

  m_pidlRoot=m_PidlMgr.Copy(pidlRoot);

  return S_OK;
}

// Setup ListView control
// Adds column headers and clears all items.
// Set each column's property include imagelist
BOOL CNSFShellView::_InitListView()
{
  ATLTRACE("CNSFShellView::_InitListView\n");

  // Empty the list
  ListView_DeleteAllItems(m_hwndList);

  // Initialize the columns
  LV_COLUMN lvColumn;

  TCHAR szString[3][20] = {_T("文件名"), _T("类型"),_T("大小")}; //empty the list

  int iWidth[3] = { 240 , 120 , 120 };

  lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
  lvColumn.fmt = LVCFMT_LEFT;

  for(int i = 0; i < 3; i++) 
  { 
    lvColumn.pszText = szString[i]; 
    lvColumn.cx = iWidth[i];
    ListView_InsertColumn(m_hwndList, i, &lvColumn); 
  } 

  ListView_SetImageList(m_hwndList, _Module.m_ImageLists.m_hImageListLarge, LVSIL_NORMAL);
  ListView_SetImageList(m_hwndList, _Module.m_ImageLists.m_hImageListSmall, LVSIL_SMALL);

  return TRUE;
}

HRESULT CNSFShellView::_FillListView()
{
  ATLTRACE(_T("CNSFShellView::_FillListView\n"));
  DWORD dwFlags = SHCONTF_NONFOLDERS | SHCONTF_FOLDERS;
  LPENUMIDLIST pEnumIDList;
  HRESULT Hr;
  HR(m_pFolder->EnumObjects(m_hWnd, dwFlags, &pEnumIDList));
  {
    // Turn the listview's redrawing off
    ::SendMessage(m_hwndList, WM_SETREDRAW, FALSE, 0L);

    DWORD dwFetched = 0;
    LPITEMIDLIST pidl = NULL;

    while( (S_OK == pEnumIDList->Next(1, &pidl, &dwFetched)) && (dwFetched!=0) )
    {
      LV_ITEM lvi = { 0 };
      //set the mask
      lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
      //add the item to the end of the list
      lvi.iItem = ListView_GetItemCount(m_hwndList);
      lvi.lParam = (LPARAM)m_PidlMgr.Copy(pidl);
      //get text on a callback basis
      TCHAR szName[MAX_PATH]={};
      m_PidlMgr.GetName(pidl,szName);
      lvi.pszText = szName;

      if( m_PidlMgr.GetItemType(pidl) == NWS_FOLDER){
        lvi.iImage = ICON_INDEX_FOLDER;
      }else{
        lvi.iImage = ICON_INDEX_FILE;
      }
      ListView_InsertItem(m_hwndList, &lvi);
      TCHAR szTemp[MAX_PATH]={};
      _tcsnset(szTemp,0,MAX_PATH);
      HR(m_PidlMgr.GetItemAttributes(pidl,ATTR_TYPE,szTemp));
      ListView_SetItemText(m_hwndList,lvi.iItem, COL_TYPE,szTemp); 
    }
    pEnumIDList->Release();
    CListSortInfo sort = { m_pFolder, COL_NAME, TRUE };
    ListView_SortItems(m_hwndList, ListViewSortFuncCB, (LPARAM) &sort);
    // Turn the listview's redrawing back on and force it to draw
    ::SendMessage(m_hwndList, WM_SETREDRAW, TRUE, 0L);
    ::InvalidateRect(m_hwndList, NULL, TRUE);
    ::UpdateWindow(m_hwndList);
  }

  return S_OK;
}


LRESULT CALLBACK CNSFShellView::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
  CNSFShellView *pThis = (CNSFShellView *)::GetWindowLong(hWnd, GWL_USERDATA);

  if( uMessage == WM_NCCREATE ) 
  {
    LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
    pThis = (CNSFShellView *) lpcs->lpCreateParams;
    ::SetWindowLong(hWnd, GWL_USERDATA, (LONG)pThis);
    // Set the window handle
    pThis->m_hWnd = hWnd;
    return 1;
  }


  ATLASSERT(pThis);
  if(pThis==NULL)
  {
    MessageBox(NULL,_T("NSFShellView::WndProc() pThis=NULL"),_T("NSF"),MB_OK);
    return E_FAIL;
  }

  MSG msg = { pThis->m_hWnd, uMessage, wParam, lParam, 0, { 0, 0 } };

  const MSG* pOldMsg = pThis->m_pCurrentMsg;
  pThis->m_pCurrentMsg = &msg;

  // pass to the message map to process
  LRESULT lRes=0;
  BOOL bRet = pThis->ProcessWindowMessage(pThis->m_hWnd, uMessage, wParam, lParam, lRes, 0);

  // restore saved value for the current message
  ATLASSERT(pThis->m_pCurrentMsg==&msg);
  if(pThis->m_pCurrentMsg!=&msg)
  {
    MessageBox(NULL,_T("NSFShellView::WndProc() pThis->m_pCurrentMsg!=&msg"),_T("NSF"),MB_OK);
    return E_FAIL;
  }
  pThis->m_pCurrentMsg = pOldMsg;

  if( !bRet ) 
    lRes = ::DefWindowProc(hWnd,uMessage, wParam, lParam);

  return lRes;
}


///////////////////////////////////////////////////////////////////////////////////
// user defined helper functions
// Since ::SHGetSettings() is not implemented in all versions of the shell, get the 
// function address manually at run time. This allows the extension to run on all 
// platforms.
void CNSFShellView::_GetShellSettings(SHELLFLAGSTATE &sfs, DWORD dwMask)
{
  typedef void (WINAPI *PFNSHGETSETTINGSPROC)(LPSHELLFLAGSTATE lpsfs, DWORD dwMask);
  HINSTANCE hinstShell32;
  hinstShell32 = ::LoadLibrary(_T("shell32.dll"));
  if( hinstShell32 != NULL )
  {
    PFNSHGETSETTINGSPROC pfnSHGetSettings = (PFNSHGETSETTINGSPROC) ::GetProcAddress(hinstShell32, "SHGetSettings");
    if( pfnSHGetSettings ) (*pfnSHGetSettings)(&sfs, dwMask);
    ::FreeLibrary(hinstShell32);
  }
}

LRESULT CNSFShellView::_ViewActivate(UINT uState)
{
  ATLTRACE2(atlTraceCOM, 0, _T("IShellView::_ViewActivate %d\n"), uState);
  // Don't do anything if the state isn't really changing
  if( m_uState == uState ) 
    return S_OK;

  _ViewDeactivate();

  if( uState!=SVUIA_DEACTIVATE ) 
  {
    //Create the menu
    m_hMenu=::CreateMenu(); 

    ATLASSERT(m_hMenu != NULL);	

    if( m_hMenu ) 
    {	 		
      // Let the browser insert its standard items first.
      OLEMENUGROUPWIDTHS omw = { 0, 0, 0, 0, 0, 0 };
      m_pShellBrowser->InsertMenusSB(m_hMenu, &omw );			

      MENUITEMINFO mii;
      //get the Help menu so we can merge with it
      ZeroMemory(&mii, sizeof(mii));
      mii.cbSize = sizeof(mii);
      mii.fMask = MIIM_SUBMENU;

      if(::GetMenuItemInfo(m_hMenu, FCIDM_MENU_HELP, FALSE, &mii))
      {
        ::InsertMenu(mii.hSubMenu, 0xFFFFFFFF, MF_BYPOSITION,
          ID_HELP_HELPTOPICS,g_szMenuItemCaptions_English[MI_HELPTOPICS_INDEX]);
      }

      ZeroMemory(&mii, sizeof(mii));
      mii.cbSize = sizeof(mii);
      mii.fMask = MIIM_SUBMENU;
      //get the file menu so we can merge with it 
      if(::GetMenuItemInfo(m_hMenu, FCIDM_MENU_FILE, FALSE, &mii))
      {				 
        if(SVUIA_ACTIVATE_FOCUS == uState)
        {
          /*
          ::InsertMenu(mii.hSubMenu,0,MF_BYPOSITION|MF_SEPARATOR,
          0, NULL );
          ::InsertMenu(mii.hSubMenu,0,MF_BYPOSITION,
          ID_EDIT_PROPERTIES,g_szMenuItemCaptions_English[MI_PROPERTIES_INDEX]);

          ::InsertMenu(mii.hSubMenu,0,MF_BYPOSITION,
          ID_EDIT_DELETE,g_szMenuItemCaptions_English[MI_DELETE_INDEX]);
          */
          //You can add your own menu into file menu here
        }
      }			

      //add the items that should only be added if we have the focus
      if(SVUIA_ACTIVATE_FOCUS == uState)
      {			
        // The Edit menu created by Explorer is empty, so we can 
        // nuke it.
        ::DeleteMenu(m_hMenu, FCIDM_MENU_EDIT, MF_BYCOMMAND );	
      }

      m_pShellBrowser->SetMenuSB(m_hMenu, NULL, m_hWnd);
    }
  }
  m_uState = uState;

  return 0;
}

LRESULT CNSFShellView::_ViewDeactivate(void)
{
  ATLTRACE2(atlTraceCOM, 0, _T("IShellView::_ViewDeactivate\n"));

  if(m_uState != SVUIA_DEACTIVATE)
  {
    if(m_hMenu)
    {
      m_pShellBrowser->SetMenuSB(NULL, NULL, NULL);

      m_pShellBrowser->RemoveMenusSB(m_hMenu);

      DestroyMenu(m_hMenu);

      m_hMenu = NULL;
    }

    m_uState = SVUIA_DEACTIVATE;
  }

  return 0;
}

// A helper function which will take care of some of
// the fancy new Win98 settings...
void CNSFShellView::_UpdateShellSettings(void)
{
  // Get the m_ShellFlags state
  _GetShellSettings(m_ShellFlags, 
#if 0
    SSF_DESKTOPHTML | 
    SSF_NOCONFIRMRECYCLE | 
    SSF_SHOWALLOBJECTS | 
    SSF_SHOWATTRIBCOL | 
    SSF_SHOWCOMPCOLOR |
#endif
    SSF_DOUBLECLICKINWEBVIEW | 
    SSF_WIN95CLASSIC);

  // Update the ListView control accordingly
  DWORD dwExStyles = 0;
  if( !m_ShellFlags.fWin95Classic && !m_ShellFlags.fDoubleClickInWebView ) {
    dwExStyles |= LVS_EX_ONECLICKACTIVATE | 
      LVS_EX_TRACKSELECT | 
      LVS_EX_UNDERLINEHOT;
  }
  ListView_SetExtendedListViewStyle(m_hwndList, dwExStyles);
}


BOOL CNSFShellView::_CreateListView(void)
{
  // Initialize and create the actual List View control
  ATLASSERT((m_dwListViewStyle & (WS_VISIBLE|WS_CHILD))==(WS_VISIBLE|WS_CHILD));

  m_dwListViewStyle &= ~LVS_TYPEMASK; 
  m_dwListViewStyle |= LVS_REPORT;
  if( (FWF_ALIGNLEFT & m_FolderSettings.fFlags) != 0 ) 
    m_dwListViewStyle |= LVS_ALIGNLEFT;
  if( (FWF_AUTOARRANGE & m_FolderSettings.fFlags) != 0 ) 
    m_dwListViewStyle |= LVS_AUTOARRANGE;

#if (_WIN32_IE >= 0x0500)
  if( (FWF_SHOWSELALWAYS & m_FolderSettings.fFlags) != 0 ) 
    m_dwListViewStyle |= LVS_SHOWSELALWAYS;
#endif

  if(_Module.m_wWinMajor == 5 && _Module.m_wWinMinor ==1){ //XP
    m_FolderSettings.fFlags |= FWF_NOCLIENTEDGE;          
  }
  // TODO
  m_hwndList = ::CreateWindowEx( (FWF_NOCLIENTEDGE & m_FolderSettings.fFlags) != 0 ? 0 : WS_EX_CLIENTEDGE,
    WC_LISTVIEW,
    NULL,
    m_dwListViewStyle,
    0,0,0,0,
    m_hWnd,
    (HMENU) IDC_LISTVIEW,
    _Module.GetModuleInstance(),
    NULL);
  if( m_hwndList == NULL ) 
    return FALSE;

  _UpdateShellSettings();
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Message Handlers

LRESULT CNSFShellView::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  ATLTRACE2(atlTraceWindowing, 0, _T("IShellView::OnSetFocus\n"));
  return OnNotifySetFocus(NULL,NULL,bHandled);

  ::SetFocus(m_hwndList);		
  return 0;
}

LRESULT CNSFShellView::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  ATLTRACE2(atlTraceWindowing, 0, _T("IShellView::OnKillFocus\n"));

  return OnNotifyKillFocus(NULL,NULL,bHandled);

  return 0;
}

LRESULT CNSFShellView::OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  m_pShellBrowser->OnViewWindowActive(this);
  _ViewActivate(SVUIA_ACTIVATE_FOCUS);
  return 0;
}

LRESULT CNSFShellView::OnSettingChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{ 
  _UpdateShellSettings();
  return 0;
}

//NM_SETFOCUS
LRESULT CNSFShellView::OnNotifySetFocus(UINT /*CtlID*/, LPNMHDR /*lpnmh*/, BOOL &/*bHandled*/)
{
  ATLTRACE2(atlTraceWindowing, 0, _T("IShellView::OnNotifySetFocus\n"));
  ATLASSERT(m_pShellBrowser);

  m_pShellBrowser->OnViewWindowActive(this);

  _ViewActivate(SVUIA_ACTIVATE_FOCUS);

  return 0;
}

LRESULT CNSFShellView::OnNotifyKillFocus(UINT /*CtlID*/, LPNMHDR /*lpnmh*/, BOOL &/*bHandled*/)
{
  ATLTRACE2(atlTraceWindowing, 0, _T("IShellView::OnNotifyKillFocus\n"));
  _ViewActivate(SVUIA_ACTIVATE_NOFOCUS);
  return 0;
}

LRESULT CNSFShellView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
  // Resize the ListView to fit window
  if(m_hwndList) 
    ::MoveWindow(m_hwndList, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
  return 0;
}

LRESULT CNSFShellView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  if( _CreateListView() )
    if( _InitListView() )
      _FillListView();
  return 0;
}

//when user click the head of listview, sort the column
LRESULT CNSFShellView::OnHeaderClick(UINT CtlID, LPNMHDR lpnmh, BOOL& bHandled)
{
  ATLTRACE(_T("CNSFShellView::OnHeaderClick\n"));

  NMHEADER* pnmh = (NMHEADER*) lpnmh;
  // Handle left-click only 
  if( pnmh->iButton==0 )
  {
    MessageBox(NULL,_T("点击Header"),_T("NSF"),MB_OK);

    int nClickedItem = pnmh->iItem;
    // Set the sorted column to the column that was just clicked. If we're
    // already sorting on that column, reverse the sort order.
    if(nClickedItem == m_iSortedColumn )
      m_bForwardSort = !m_bForwardSort;
    else
      m_bForwardSort = TRUE;
    m_iSortedColumn = (USHORT)nClickedItem;
    // Set up a CListSortInfo for the sort function to use.
    const USHORT aCols[] = { COL_NAME, COL_TYPE };
    CListSortInfo sort = { m_pFolder, aCols[m_iSortedColumn], m_bForwardSort }; 
    ListView_SortItems(m_hwndList,ListViewSortFuncCB,(LPARAM)&sort);
  }
  return 0;

}

//handle dbclick events in NSE's listview
LRESULT CNSFShellView::OnItemActivated(UINT CtlID, LPNMHDR lpnmh, BOOL& bHandled)
{
  // ATLTRACE(_T("CNSFShellView::OnItemActivated\n"));
  LV_ITEM lvItem;
  ZeroMemory(&lvItem, sizeof(lvItem));

  lvItem.mask = LVIF_PARAM;

  LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)lpnmh;
  lvItem.iItem = lpnmlv->iItem;

  if(ListView_GetItem(m_hwndList, &lvItem))
  {
    //folders to be activated
    if(NWS_FOLDER == m_PidlMgr.GetItemType((LPITEMIDLIST)lvItem.lParam))
    {
      //Tells Windows Explorer to browse to another folder
      m_pShellBrowser->BrowseObject( (LPITEMIDLIST)lvItem.lParam, 
        SBSP_DEFBROWSER | SBSP_RELATIVE);
    }
    else
    {
      //do nothing when dbclick file object
      //You can add code to open file here
    }
  }
  return 0;
}

LRESULT CNSFShellView::OnRefresh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  Refresh();
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Drag-and-drop functions
LRESULT CNSFShellView::OnBeginDrag(UINT ClitID, LPNMHDR lpnmh, BOOL& bHandled)
{
  ATLTRACE(_T("CNSFShellView::OnBeginDrag\n"));

  // Get selected ListView item
  LPITEMIDLIST pidlSelected=NULL;

  UINT nCount = ListView_GetSelectedCount(m_hwndList);
  if( 0 == nCount)
  {
    pidlSelected = NULL;
    return E_FAIL;
  }
  else
  {
    LV_ITEM lvItem;
    int nSelItem = ListView_GetNextItem( m_hwndList, -1, LVIS_SELECTED );

    ZeroMemory(&lvItem, sizeof(lvItem));
    lvItem.mask = LVIF_PARAM;
    lvItem.iItem = nSelItem;

    if(ListView_GetItem(m_hwndList, &lvItem))
    {			
      pidlSelected=(LPITEMIDLIST)(lvItem.lParam);

      //Drag a folder from right panel is not allowed
      if(NWS_FOLDER == m_PidlMgr.GetItemType(pidlSelected))
      {
        return E_FAIL;
      }
    }
  }

  // Create a IDataObject and let the system handle operation...
  HRESULT Hr;
  CComObject<CNSFDataObject>* pDataObject;
  HR( CComObject<CNSFDataObject>::CreateInstance(&pDataObject) );
  pDataObject->AddRef();

  //Called by our ShellView, pidl should be a complex pidl
  LPITEMIDLIST pidlComplex;
  pidlComplex = m_PidlMgr.Concatenate(m_pFolder->m_pidlPath,pidlSelected);
  ATLASSERT(pidlComplex);

  HR( pDataObject->_Init(m_pFolder, pidlComplex) );

  if( SUCCEEDED(Hr) && (pDataObject!=NULL) ) 
  {
    // If we're dragging then call Window's DoDragDrop()
    // method with a freshly created IDropSource object
    CComObject<CNSFDropSource>* pDropSource;
    HR( CComObject<CNSFDropSource>::CreateInstance(&pDropSource) );
    pDropSource->AddRef();
    DWORD dwEffect = 0;
    Hr = ::DoDragDrop( pDataObject, 
      pDropSource, 
      DROPEFFECT_MOVE, 
      &dwEffect);
    pDropSource->Release();

    if(Hr)
    {
      // if drag-and-drop succ, deleted the dragged object from NSE
      DeleteItemInCfgFile(pidlComplex);
      Refresh();
      RefreshShellViewWndsExcept(m_hWnd);
    }
  }

  pDataObject->Release();

  m_PidlMgr.Delete(pidlComplex);
  return S_OK;
}
