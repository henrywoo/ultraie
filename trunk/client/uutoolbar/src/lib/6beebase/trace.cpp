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
