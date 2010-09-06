/// Implementation of the CNotifyIconData class and the CTrayIconImpl template.
#pragma once

#include <atlmisc.h>

/// Wrapper class for the Win32 NOTIFYICONDATA structure
class CNotifyIconData : public NOTIFYICONDATA
{
public:  
  CNotifyIconData(){
    memset(this, 0, sizeof(NOTIFYICONDATA));
    cbSize = sizeof(NOTIFYICONDATA);
  }
};

/// @brief Template used to support adding an icon to the taskbar.
/// This class will maintain a taskbar icon and associated context menu.
template <class T>
class CTrayIconImpl
{
private:
  UINT WM_TRAYICON;
  CNotifyIconData m_nid;
  bool m_bInstalled;
  UINT m_nDefault;
  CMenu oMenu;
public:  
  CTrayIconImpl():
  m_bInstalled(false),
  m_nDefault(0),
  oMenu(NULL)
  {
    WM_TRAYICON = ::RegisterWindowMessage(_T("WM_TRAYICON"));
  }
  
  ~CTrayIconImpl(){
    RemoveIcon(); /// Remove the icon
    if(!oMenu.IsNull()){
      oMenu.DestroyMenu();
    }
  }

  /// Install a taskbar icon
  /// @param lpszToolTip   - The tooltip to display
  /// @param hIcon     - The icon to display
  /// @param nID    - The resource ID of the context menu
  /// @return true on success
  bool InstallIcon(LPCTSTR lpszToolTip, HICON hIcon, UINT nID){
    T* pT = static_cast<T*>(this);
    m_nid.hWnd = pT->m_hWnd;
    m_nid.uID = nID;
    m_nid.hIcon = hIcon;
    m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_nid.uCallbackMessage = WM_TRAYICON;
    wcscpy_s(m_nid.szTip,sizeof  m_nid.szTip, lpszToolTip);
    /// Install
    m_bInstalled = Shell_NotifyIcon(NIM_ADD, &m_nid) ? true : false;
    return m_bInstalled;
  }

  bool RemoveIcon(){
    if (!m_bInstalled)
      return false;
    m_nid.uFlags = 0;
    return Shell_NotifyIcon(NIM_DELETE, &m_nid) ? true : false;
  }

  /// Set the icon tooltip text
  /// returns true on success
  bool SetTooltipText(LPCTSTR pszTooltipText){
    if (pszTooltipText == NULL)
      return FALSE;
    m_nid.uFlags = NIF_TIP;
    _tcscpy(m_nid.szTip, pszTooltipText);
    return Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? true : false;
  }

  bool PopMsg(const wchar_t* msgTxt,LPCTSTR titleTxt){
    if (msgTxt == NULL)
      return FALSE;
    T* pT = static_cast<T*>(this);
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof( NOTIFYICONDATA );
    nid.hWnd = pT->m_hWnd; /// or whatever HWND you provided when you created it
    nid.uID = m_nid.uID; /// or whatever ID you provided when you created it
    nid.uFlags = NIF_INFO;
    nid.uTimeout = 1; /// In milliseconds. Min 10 seconds, max 30 seconds. If outside of range, it automatically takes closest limit
    nid.dwInfoFlags = NIIF_INFO;

    size_t infolen=_countof(nid.szInfo);
    size_t titlelen=_countof(nid.szInfoTitle);
    wstring txt(msgTxt),title(titleTxt);
    if (txt.size()>=infolen){
      txt.resize(infolen-1);
    }
    if (title.size()>=titlelen){
      title.resize(titlelen-1);
    }
    wcscpy_s( nid.szInfo, _countof(nid.szInfo),txt.c_str());
    wcscpy_s( nid.szInfoTitle, _countof(nid.szInfoTitle), title.c_str());
    return Shell_NotifyIcon(NIM_MODIFY, &nid) ? true : false;
  }

  /// Set the default menu item ID
  inline void SetDefaultItem(UINT nID) { m_nDefault = nID; }

  BEGIN_MSG_MAP(CTrayIcon)
    MESSAGE_HANDLER(WM_TRAYICON, OnTrayIcon)
  END_MSG_MAP()

  LRESULT OnTrayIcon(UINT , WPARAM wParam, LPARAM lParam, BOOL& ){
    /// Is this the ID we want?
    if (wParam != m_nid.uID)
      return 0;
    T* pT = static_cast<T*>(this);
    /// Was the right-button clicked?
    if (LOWORD(lParam) == WM_RBUTTONUP){
      SetMyMenu(oMenu);
      /// Get the sub-menu
      CMenuHandle oPopup(oMenu.GetSubMenu(0));
      /// Prepare
      pT->PrepareMenu(oPopup);
      /// Get the menu position
      CPoint pos;
      GetCursorPos(&pos);
      /// Make app the foreground
      SetForegroundWindow(pT->m_hWnd);
      /// Set the default menu item
      if (m_nDefault == 0)
        oPopup.SetMenuDefaultItem(0, TRUE);
      else
        oPopup.SetMenuDefaultItem(m_nDefault);
      /// Track
      oPopup.TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, pT->m_hWnd);
      /// BUGFIX: See "PRB: Menus for Notification Icons Don't Work Correctly"
      pT->PostMessage(WM_NULL);
    }else if (LOWORD(lParam) == WM_LBUTTONDBLCLK){
      /// Make app the foreground
      SetForegroundWindow(pT->m_hWnd);
      SetMyMenu(oMenu);
      /// Get the sub-menu
      CMenuHandle oPopup(oMenu.GetSubMenu(0));
      /// Get the item
      if (m_nDefault){
        pT->SendMessage(WM_COMMAND, m_nDefault, 0);
      }else{
        UINT nItem = oPopup.GetMenuItemID(0);
        pT->SendMessage(WM_COMMAND, nItem, 0);
      }
    }else if (LOWORD(lParam) == WM_LBUTTONUP){
      ///MessageBoxA(NULL,"Click!","Error",NULL);
    }
    return 0;
  }

  BOOL ChangeIcon(HICON hIcon){
    m_nid.uFlags = NIF_ICON;
    m_nid.hIcon = hIcon;
    return Shell_NotifyIcon(NIM_MODIFY,&m_nid); ///Call the Shell_NotifyIcon function
  }

  /// Allow the menu items to be enabled/checked/etc.
  virtual void PrepareMenu(HMENU hMenu)=0;
  /// Must call this to set menu resource
  virtual void SetMyMenu(CMenu& m)=0;
};
