#ifndef _MY_NEF_MODULE_H_
#define _MY_NEF_MODULE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <commctrl.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comctl32.lib")


class CShellMalloc
{
public:
   LPMALLOC m_pMalloc;
   HRESULT Init()
   {
      m_pMalloc = NULL;
      // It is safe to call ::SHGetMalloc()/::CoGetMalloc() without
      // first calling ::CoInitialize() according to MSDN.
      if( FAILED( ::SHGetMalloc(&m_pMalloc) ) ) 
	  {
         // TODO: TERMINATE
		  return E_FAIL;
      }
	  return S_OK;
   }
   void Term()
   {
      if( m_pMalloc!=NULL ) 
		  m_pMalloc->Release();
   }
   operator LPMALLOC() const
   {
      return m_pMalloc;
   }
   LPVOID Alloc(ULONG cb)
   {
      ATLASSERT(m_pMalloc!=NULL);
      ATLASSERT(cb>0);
      return m_pMalloc->Alloc(cb);
   }
   void Free(LPVOID p)
   {
      ATLASSERT(m_pMalloc!=NULL);
      ATLASSERT(p);
      m_pMalloc->Free(p);
   }
};

class CNWSModule : public CComModule
{
public:
	HRESULT Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE	h, const GUID* plibid =	NULL)
	{
		::OleInitialize(NULL);
#ifdef INITCOMMONCONTROLSEX
		INITCOMMONCONTROLSEX iccex;
		iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		iccex.dwICC =	ICC_LISTVIEW_CLASSES;
		::InitCommonControlsEx(&iccex);
#else
		::InitCommonControls();
#endif // INITCOMMONCONTROLSEX

#ifndef	_NO_CLIPFORMATS
	#define	CFSTR_OLECLIPBOARDPERSISTONFLUSH TEXT("OleClipboardPersistOnFlush")
	#ifndef	CFSTR_LOGICALPERFORMEDDROPEFFECT 
		#define CFSTR_LOGICALPERFORMEDDROPEFFECT	TEXT("Logical Performed	DropEffect")
	#endif // CFSTR_LOGICALPERFORMEDDROPEFFECT
	m_CFSTR_FILEDESCRIPTOR			 = (CLIPFORMAT)	::RegisterClipboardFormat(CFSTR_FILEDESCRIPTOR);
	m_CFSTR_FILECONTENTS				 = (CLIPFORMAT)	::RegisterClipboardFormat(CFSTR_FILECONTENTS);
	m_CFSTR_PASTESUCCEEDED			 = (CLIPFORMAT)	::RegisterClipboardFormat(CFSTR_PASTESUCCEEDED);
	m_CFSTR_LOGICALPERFORMEDDROPEFFECT = (CLIPFORMAT)	::RegisterClipboardFormat(CFSTR_LOGICALPERFORMEDDROPEFFECT);
	m_CFSTR_PERFORMEDDROPEFFECT		 = (CLIPFORMAT)	::RegisterClipboardFormat(CFSTR_PERFORMEDDROPEFFECT);
	m_CFSTR_PREFERREDDROPEFFECT		 = (CLIPFORMAT)	::RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
	m_CFSTR_SHELLIDLIST				 = (CLIPFORMAT)	::RegisterClipboardFormat(CFSTR_SHELLIDLIST);
	m_CFSTR_OLECLIPBOARDPERSISTONFLUSH = (CLIPFORMAT)	::RegisterClipboardFormat(CFSTR_OLECLIPBOARDPERSISTONFLUSH);
	#if	(_WIN32_WINNT >= 0x0500)
		m_CFSTR_TARGETCLSID				 = (CLIPFORMAT)	::RegisterClipboardFormat(CFSTR_TARGETCLSID);
	#endif // WIN32_WINNT
#endif // _NO_CLIPFORMATS

		// Get Shell allocator
		HRESULT Hr;
		Hr=m_Allocator.Init();
		if(FAILED(Hr))
			return Hr;

		// Get Win version
		OSVERSIONINFO	ovi	= {	0 };
		ovi.dwOSVersionInfoSize =	sizeof(ovi);
		::GetVersionEx(&ovi);
		m_wWinMajor =	(WORD) ovi.dwMajorVersion;
		m_wWinMinor =	(WORD) ovi.dwMinorVersion;

		// Register Self-defined clipformat
		m_CFSTR_NSEDRAGDROP = (CLIPFORMAT)::RegisterClipboardFormat(_T("NSEDRAGDROP"));

		return CComModule::Init(p, h,	plibid);
	}
	void	Term()
	{
		if(m_pidlNSFROOT)
			m_Allocator.Free(m_pidlNSFROOT);
		
		m_Allocator.Term();

		CComModule::Term();
	}

	// Shell	Allocator
	CShellMalloc m_Allocator;

	CShellImageLists m_ImageLists;

	LPITEMIDLIST m_pidlNSFROOT;

	CLIPFORMAT m_CFSTR_NSEDRAGDROP;

	USHORT m_usOSType;

	TCHAR m_szInstallPath[MAX_PATH];

	WORD	m_wWinMajor;
	WORD	m_wWinMinor;

#ifndef	_NO_CLIPFORMATS
	// Clipboard	formats
	CLIPFORMAT m_CFSTR_SHELLIDLIST;
	CLIPFORMAT m_CFSTR_FILECONTENTS;
	CLIPFORMAT m_CFSTR_PASTESUCCEEDED;
	CLIPFORMAT m_CFSTR_FILEDESCRIPTOR;
	CLIPFORMAT m_CFSTR_PERFORMEDDROPEFFECT;
	CLIPFORMAT m_CFSTR_PREFERREDDROPEFFECT;
	CLIPFORMAT m_CFSTR_LOGICALPERFORMEDDROPEFFECT;
	CLIPFORMAT m_CFSTR_OLECLIPBOARDPERSISTONFLUSH;
	#if	(_WIN32_WINNT >= 0x0500)
		CLIPFORMAT m_CFSTR_TARGETCLSID;
	#endif // _WIN32_WINNT
#endif // _NO_CLIPFORMATS
};


#endif // _MY_NEF_MODULE_H_