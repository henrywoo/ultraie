#ifndef _ATL_NO_PRAGMA_WARNINGS
#pragma warning (push)
#pragma warning(disable : 4668)  // is not defined as a preprocessor macro, replacing with '0' for '#if/#elif
#pragma warning(disable : 4820)  // padding added after member
#pragma warning(disable : 4571) //catch(...) blocks compiled with /EHs do NOT catch or re-throw Structured Exceptions
#endif //!_ATL_NO_PRAGMA_WARNINGS

#ifndef  __ATL6BEERESWIN_H__
#define __ATL6BEERESWIN_H__

#pragma once

#ifndef __cplusplus
#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
#error atlwin.h requires atlbase.h to be included first
#endif

#include <atlstdthunk.h>
#include <commctrl.h>
#include <atlsimpstr.h>
#include <atlstr.h>
#include <atlwin.h>

//protect template members from windowsx.h macros
#ifdef _INC_WINDOWSX
#undef SubclassWindow
#endif //_INC_WINDOWSX

#ifdef SetWindowLongPtrA
#undef SetWindowLongPtrA
inline LONG_PTR SetWindowLongPtrA( HWND hWnd, int nIndex, LONG_PTR dwNewLong )
{
  return( ::SetWindowLongA( hWnd, nIndex, LONG( dwNewLong ) ) );
}
#endif

#ifdef SetWindowLongPtrW
#undef SetWindowLongPtrW
inline LONG_PTR SetWindowLongPtrW( HWND hWnd, int nIndex, LONG_PTR dwNewLong )
{
  return( ::SetWindowLongW( hWnd, nIndex, LONG( dwNewLong ) ) );
}
#endif

#ifdef GetWindowLongPtrA
#undef GetWindowLongPtrA
inline LONG_PTR GetWindowLongPtrA( HWND hWnd, int nIndex )
{
  return( ::GetWindowLongA( hWnd, nIndex ) );
}
#endif

#ifdef GetWindowLongPtrW
#undef GetWindowLongPtrW
inline LONG_PTR GetWindowLongPtrW( HWND hWnd, int nIndex )
{
  return( ::GetWindowLongW( hWnd, nIndex ) );
}
#endif

#ifndef _ATL_NO_DEFAULT_LIBS
#pragma comment(lib, "gdi32.lib")
#endif  // !_ATL_NO_DEFAULT_LIBS

#pragma pack(push,_ATL_PACKING)

namespace ATL
{


  template <class T>
  class ATL_NO_VTABLE C6BeeResDialogImpl : public CDialogImplBaseT<CWindow>
  {
    HMODULE hModule;
  public:
#ifdef _DEBUG
    bool m_bModal;
    C6BeeResDialogImpl() : m_bModal(false) { }
#endif //_DEBUG
    // modal dialogs

    INT_PTR DoModal(const CString& respath, HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL)
    {
      BOOL result;

      ATLASSUME(m_hWnd == NULL);

      // Allocate the thunk structure here, where we can fail
      // gracefully.

      result = m_thunk.Init(NULL,NULL);
      if (result == FALSE) 
      {
        SetLastError(ERROR_OUTOFMEMORY);
        return -1;
      }

      _AtlWinModule.AddCreateWndData(&m_thunk.cd, (CDialogImplBaseT<CWindow>*)this);
#ifdef _DEBUG
      m_bModal = true;
#endif //_DEBUG
      hModule = ::LoadLibrary(respath.GetString());
      return ::DialogBoxParam(hModule, MAKEINTRESOURCE(static_cast<T*>(this)->IDD), hWndParent, T::StartDialogProc, dwInitParam);
    }
    BOOL EndDialog(int nRetCode)
    {
      ATLASSERT(::IsWindow(m_hWnd));
#ifdef _DEBUG
      ATLASSUME(m_bModal);  // must be a modal dialog
#endif //_DEBUG
      return ::EndDialog(m_hWnd, nRetCode);
    }
    // modeless dialogs
    HWND Create(HWND hWndParent, const CString& respath,LPARAM dwInitParam = NULL)
    {
      BOOL result;

      ATLASSUME(m_hWnd == NULL);

      // Allocate the thunk structure here, where we can fail
      // gracefully.

      result = m_thunk.Init(NULL,NULL);
      if (result == FALSE) 
      {
        SetLastError(ERROR_OUTOFMEMORY);
        return NULL;
      }

      _AtlWinModule.AddCreateWndData(&m_thunk.cd, (CDialogImplBaseT< TBase >*)this);
#ifdef _DEBUG
      m_bModal = false;
#endif //_DEBUG
      hModule = ::LoadLibrary(respath);
      HWND hWnd = ::CreateDialogParam(hModule, MAKEINTRESOURCE(static_cast<T*>(this)->IDD), hWndParent, T::StartDialogProc, dwInitParam);
      ATLASSUME(m_hWnd == hWnd);
      return hWnd;
    }
    // for CComControl
    HWND Create(HWND hWndParent, RECT&, const CString& respath,LPARAM dwInitParam = NULL)
    {
      return Create(hWndParent, respath, dwInitParam);
    }

    BOOL DestroyWindow()
    {
      ATLASSERT(::IsWindow(m_hWnd));
#ifdef _DEBUG
      ATLASSERT(!m_bModal);  // must not be a modal dialog
#endif //_DEBUG
      if(hModule) ::FreeLibrary(hModule);
      return ::DestroyWindow(m_hWnd);
    }
  };


  template <WORD t_wDlgTemplateID, BOOL t_bCenter = TRUE>
  class C6BeeResSimpleDialog : public CDialogImplBase
  {
    HMODULE hModule;
  public:
    INT_PTR DoModal(const ATL::CString& respath,HWND hWndParent = ::GetActiveWindow())
    {
      ATLASSUME(m_hWnd == NULL);
      hModule = ::LoadLibrary(respath);
      _AtlWinModule.AddCreateWndData(&m_thunk.cd, (CDialogImplBase*)this);
      INT_PTR nRet = ::DialogBox(hModule,  MAKEINTRESOURCE(t_wDlgTemplateID), hWndParent, StartDialogProc);
      m_hWnd = NULL;
      return nRet;
    }

    typedef CSimpleDialog<t_wDlgTemplateID, t_bCenter>  thisClass;
    BEGIN_MSG_MAP(thisClass)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_RANGE_HANDLER(IDOK, IDNO, OnCloseCmd)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
      // initialize controls in dialog with DLGINIT resource section
      ExecuteDlgInit(t_wDlgTemplateID);
      if(t_bCenter)
        CenterWindow(GetParent());
      return TRUE;
    }

    LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
      if(hModule) ::FreeLibrary(hModule);
      ::EndDialog(m_hWnd, wID);
      return 0;
    }
  };
}

#endif