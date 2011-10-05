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

#ifndef __UUFILE_TOOLBARCOM__
#define __UUFILE_TOOLBARCOM__
#pragma once

#include "resource.h"       // main symbols
#include "uufilebho.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



//COM
#include <comdef.h>//_bstr_t,...
//ATL
#include <atlbase.h>

#include <shlguid.h>
#include <shlobj.h>
#include <exdispid.h>
#include <comdefsp.h>

#include "reflectionwnd.h"
#include <6bees_iwebbrowser2.h>

using namespace std;

class ATL_NO_VTABLE Cuufilecom :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<Cuufilecom, &CLSID_uufilecom>,
  public IObjectWithSiteImpl<Cuufilecom>,
  public IDeskBand,
  public IInputObject,
  public IDispatchImpl<Iuufilecom, &IID_Iuufilecom, &LIBID_uufilebhoLib, 1, 0>
{
public:
  Cuufilecom();
  ~Cuufilecom();

  DECLARE_REGISTRY_RESOURCEID(IDR_TOOLBARCOM)

  BEGIN_CATEGORY_MAP(Cuufilecom)
  //  IMPLEMENTED_CATEGORY(CATID_InfoBand)
  //  IMPLEMENTED_CATEGORY(CATID_CommBand)
  //  IMPLEMENTED_CATEGORY(CATID_DeskBand)
  END_CATEGORY_MAP()

  BEGIN_COM_MAP(Cuufilecom)
    COM_INTERFACE_ENTRY(Iuufilecom)
    COM_INTERFACE_ENTRY_IID(IID_IInputObject, IInputObject)
    //COM_INTERFACE_ENTRY(IInputObject)
    //{&IID_IInputObject, offsetofclass(IInputObject, _ComMapClass), _ATL_SIMPLEMAPENTRY},//VC6

    COM_INTERFACE_ENTRY(IOleWindow)
    COM_INTERFACE_ENTRY_IID(IID_IDockingWindow, IDockingWindow)
    COM_INTERFACE_ENTRY(IObjectWithSite)
    COM_INTERFACE_ENTRY_IID(IID_IDeskBand, IDeskBand)

    COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()

  // IDeskBand
  STDMETHOD(GetBandInfo)(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi);

  // IOleWindow
  STDMETHOD(GetWindow)(HWND* phwnd);
  STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);

  // IDockingWindow
  STDMETHOD(CloseDW)(unsigned long dwReserved);
  STDMETHOD(ResizeBorderDW)(const RECT* prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);
  STDMETHOD(ShowDW)(BOOL fShow);

  // IObjectWithSiteImpl
  STDMETHOD(SetSite)(IUnknown* pUnkSite);
  STDMETHOD(GetSite)(REFIID riid, void **ppvSite);

  // IInputObject
  STDMETHOD(HasFocusIO)(void);
  STDMETHOD(TranslateAcceleratorIO)(LPMSG lpMsg);
  STDMETHOD(UIActivateIO)(BOOL fActivate, LPMSG lpMsg);

  //IDispatchImpl
  STDMETHOD(Invoke)(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);

  void FocusChange(BOOL bHaveFocus);
  BOOL CreateRefWnd();

  static bool iswebie;

private:
  // smart pointer of web browser
  _6bees_html::c6beewb2ptr spwb_;

  BOOL showit_;
  HWND m_hWnd;
  CComPtr<IInputObjectSite> sp_inputobjsite_;
  CComPtr<IConnectionPoint> sp_connectionpoint_;
  DWORD m_dwCookie;
  wstring last_url;
  DWORD tid;
  ReflectionWnd m_ReflectionWnd;

  static DWORD pid;
  // All thread ids' set.
  // If threadids.size()==1 means this is the last IE tab.Then we can call ProcessBasedTerm().
  // If threadids.size()==0 means this is the first IE tab.Then we can call ProcessBasedInit().
  static set<DWORD> threadids;
  // Or you can call a global init
  static bool ProcessBasedInit();
  static bool ProcessBasedTerm();
  static bool SystemBasedInit();// For the first IE Process
  static bool SystemBasedTerm();// For the last IE process

  // For every new thread(new tab), this function will be called
  bool ThreadBasedInit(IUnknown*);
  bool ThreadBasedTerm();

public:
  static map<DWORD,_6bees_html::c6beewb2ptr*> tid_wbptr;

};

OBJECT_ENTRY_AUTO(__uuidof(uufilecom), Cuufilecom)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




#endif

