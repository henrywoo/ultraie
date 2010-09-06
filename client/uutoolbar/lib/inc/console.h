#ifndef __UULOGGING__
#define __UULOGGING__

#include "guard.h"
#include <string>
#include <atlstr.h>
#include <time.h>
typedef const wchar_t*  wcstr;

class cconsole{
public:
  cconsole(const wchar_t* name, ...);
  ~cconsole();

  void Write(wcstr fmt,...);
  void WriteV(wcstr fmt, va_list argList);

  static std::wstreambuf* Redirect(std::wostream& strm);
  static bool m_enabled;	

private:
  static cmutex m_lock;
  static long m_isn;
  ATL::CString m_name;
  DWORD m_threadid;
  long m_id;
};

#endif


