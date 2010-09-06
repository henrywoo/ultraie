#include "trace.h"

#include <time.h>
#include <iomanip>
#include <strsafe.h>

static long time_count = clock();

std::ostream &operator << (std::ostream &str,const LogTime &log_time){
  char    timebuf[40]={};
  const SYSTEMTIME  &tm=log_time.m_Time;
  long tmp=clock();
  StringCbPrintfA(timebuf,sizeof(timebuf),"%02d %02d:%02d:%02d.%03d %6lu",
    tm.wDay,tm.wHour,tm.wMinute,tm.wSecond,tm.wMilliseconds,tmp-time_count);
  time_count = tmp;
  return str << timebuf;
}

void LogBuffer::write(std::ostream& out){
  cguard g(lock_);
  for(int i = 0; i < index_; ++i){
    records[i].write(out);
  }
  clear();
}

void LogRecord::write(std::ostream& out){
  out
    << ltime
    << std::setw(7) << thread_id 
    << std::setw(4) << level 
    << " " << message.c_str() << std::endl;
}
