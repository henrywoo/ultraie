#include "uulogging_i.h"
#include <time.h>
#include <atlbase.h>
#include <strsafe.h>
#include "trace.h"
#include "6bees_util.h"
#include "6bees_const.h"
#include "6bees_str.h"
using namespace _6bees_str;

bool uulogging::initialized = false;
uulogging* uulogging::pinstance = NULL;
cmutex uulogging::lock_;

uulogging& uulogging::R(){
  if(!initialized){
    cguard g(lock_);
    if(pinstance == NULL){ 
      static uulogging_i p_;
      pinstance = &p_;
      if(pinstance){ //make sure compiler cannot reorder 
        initialized = true; // only set to true after creation 
      }
    }
  }
  return *pinstance;
}

uulogging_i::uulogging_i():
lock_(),
m_bopen(false),
m_line(0),
m_file(NULL){
  wchar_t pszLoader[MAX_PATH];
  GetModuleFileName(NULL, pszLoader, MAX_PATH);
  std::wstring logfile = PathFindFileName(pszLoader);
  logfile.append(L"_debug.log");
  wstring fp = _6bees_util::GetUltraPath(logfile.c_str(),false);
  Initialize(fp.c_str());
}

uulogging_i::~uulogging_i(){}

bool uulogging_i::Initialize(const wchar_t* _fname){
  if(m_bopen)
    return true;
  wchar_t tmpdir[MAX_PATH];
  wcscpy_s(tmpdir,MAX_PATH,_fname);
  PathRemoveFileSpec(tmpdir);
  //_6beed_util::MakeSureDir(tmpdir);

  string fnamea_=stringmaker()<<_fname;
  destFile.open(fnamea_.c_str(), std::ios::out|std::ios::ate|std::ios::app);    
  if((m_bopen = destFile.is_open())){      
    destFile <<
      "Day Time    Duration Thread Code Message\n"
      "----------- -------- ------ ---- ---------------------------------------\n"
      << std::endl;
  }
  return m_bopen;
}

void uulogging_i::Destroy(){
  if (m_bopen == false)
    return;
  m_bopen = false;
  destFile.close();
}

void uulogging_i::Set(const char* file, int line){
  m_file  = file;
  m_line  = line;
}

void uulogging_i::Write(RmLevel level, const char* format_ptr, ...){
  string fptr(format_ptr);
  
  /// @note Or it will fail for: file:///hello%20world
  if (fptr.find('%')!=string::npos){
    fptr.erase(remove_if(fptr.begin(),fptr.end(),bind2nd(equal_to<wchar_t>(), ('%'))),fptr.end());
  }
  const char* p = fptr.c_str();

  va_list vp;
  va_start(vp, p);
  WriteImpl(level, p, vp);
  va_end(vp);
}

void uulogging_i::Write(RmLevel level, const wchar_t* format_ptr, ...){
  string fptr=stringmaker(CP_ACP)<<format_ptr;
  
  /// @note Or it will fail for: file:///hello%20world
  if (fptr.find('%')!=string::npos){
    fptr.erase(remove_if(fptr.begin(),fptr.end(),bind2nd(equal_to<wchar_t>(), (L'%'))),fptr.end());
  }

  const char* tmp = fptr.c_str();
  va_list vp;
  va_start(vp, tmp);
  WriteImpl(level, tmp, vp);
  va_end(vp);
}

void uulogging_i::Write_v(RmLevel level,const char* format_ptr, va_list& vp){
  WriteImpl(level, format_ptr, vp);
}

/// All writes lead here.
void uulogging_i::WriteImpl(RmLevel level, const char* format_ptr, va_list& vp){
  static const int MAX_MESSAGE_SIZE = 1024;
  char message[MAX_MESSAGE_SIZE+1];
  HRESULT hr = StringCbVPrintfA(message, MAX_MESSAGE_SIZE, format_ptr, vp);
  if(S_OK != hr){
    cguard write_guard(lock_);
    destFile << "*** Error "<<hr<<" creating buffer for message "<<format_ptr<<std::endl;
    return;
  }

  // create the log entry
  LogRecord record(level);
  if(LOG_ERROR == level){
    record.message = stringmaker(CP_ACP) << message << " at (" << m_file << "," << m_line << ")";
  }else{
    record.message = message;
  }

  cguard write_guard(lock_);
  record.write(destFile);
}