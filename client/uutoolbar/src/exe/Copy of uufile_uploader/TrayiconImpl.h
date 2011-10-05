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
public:  
  CTrayIconImpl() : m_bInstalled(false), m_nDefault(0)
  {
    WM_TRAYICON = ::RegisterWindowMessage(_T("WM_TRAYICON"));
  }
  
  ~CTrayIconImpl()
  {
    /// Remove the icon
    RemoveIcon();
  }

  /// Install a taskbar icon
  ///   lpszToolTip   - The tooltip to display
  //  hIcon     - The icon to display
  ///   nID    - The resource ID of the context menu
  //// returns true on success
  bool InstallIcon(LPCTSTR lpszToolTip, HICON hIcon, UINT nID)
  {
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

  /// Remove taskbar icon
  /// returns true on success
  bool RemoveIcon()
  {
    if (!m_bInstalled)
      return false;
    /// Remove
    m_nid.uFlags = 0;
    return Shell_NotifyIcon(NIM_DELETE, &m_nid) ? true : false;
  }

  /// Set the icon tooltip text
  /// returns true on success
  bool SetTooltipText(LPCTSTR pszTooltipText)
  {
    if (pszTooltipText == NULL)
      return FALSE;
    /// Fill the structure
    m_nid.uFlags = NIF_TIP;
    _tcscpy(m_nid.szTip, pszTooltipText);
    /// Set
    return Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? true : false;
  }

  bool PopMsg(LPCTSTR msgTxt,LPCTSTR title){
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
    _tcscpy( nid.szInfo, msgTxt);
    _tcscpy( nid.szInfoTitle, title);

    return Shell_NotifyIcon(NIM_MODIFY, &nid) ? true : false;
  }

  /// Set the default menu item ID
  inline void SetDefaultItem(UINT nID) { m_nDefault = nID; }

  BEGIN_MSG_MAP(CTrayIcon)
    MESSAGE_HANDLER(WM_TRAYICON, OnTrayIcon)
  END_MSG_MAP()

  LRESULT OnTrayIcon(UINT , WPARAM wParam, LPARAM lParam, BOOL& )
  {
    /// Is this the ID we want?
    if (wParam != m_nid.uID)
      return 0;
    T* pT = static_cast<T*>(this);
    /// Was the right-button clicked?
    if (LOWORD(lParam) == WM_RBUTTONUP)
    {
      /// Load the menu
      CMenu oMenu;
      if (!oMenu.LoadMenu(m_nid.uID))
        return 0;
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
      /// Done
      oMenu.DestroyMenu();
    }
    else if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
    {
      /// Make app the foreground
      SetForegroundWindow(pT->m_hWnd);
      /// Load the menu
      CMenu oMenu;
      if (!oMenu.LoadMenu(m_nid.uID))
        return 0;
      /// Get the sub-menu
      CMenuHandle oPopup(oMenu.GetSubMenu(0));      
      /// Get the item
      if (m_nDefault)
      {
        /// Send
        pT->SendMessage(WM_COMMAND, m_nDefault, 0);
      }
      else
      {
        UINT nItem = oPopup.GetMenuItemID(0);
        /// Send
        pT->SendMessage(WM_COMMAND, nItem, 0);
      }
      /// Done
      oMenu.DestroyMenu();
    }
    return 0;
  }

  /// Allow the menu items to be enabled/checked/etc.
  virtual void PrepareMenu(HMENU hMenu)
  {
    /// Stub
  }
};
