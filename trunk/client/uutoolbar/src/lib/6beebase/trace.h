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

#ifndef __UULOGGING_TRACE__
#define __UULOGGING_TRACE__

#include "uulogging.h"
#include "guard.h"
#include <iostream>

struct LogTime{
  SYSTEMTIME m_Time;
  LogTime(){
    GetLocalTime(&m_Time);
  }
};

std::ostream &operator << (std::ostream &str,const LogTime &log_time);

/**
* This class represents one entry in the log
*/
struct LogRecord{
  explicit LogRecord(RmLevel _level=LOG_ERROR):
  thread_id(GetCurrentThreadId()), level(_level), message() {}

  // Write this record to the file
  void write(std::ostream& out);

  LogTime ltime; // time of event
  DWORD thread_id; // thread generating event
  RmLevel level; // event level
  std::string message; // message
};

/**
* This class represents a collection of LogRecords
*/ 
class LogBuffer {
public:
  // Construct 
  LogBuffer(int cap)
  :arraySize_(cap),index_(0),records(new LogRecord[cap]),lock_(){}

  virtual ~LogBuffer(){
    delete [] records;
  }

  // Add a record
  void addRecord(const LogRecord& r) {
    cguard g(lock_);
    if(index_ > arraySize_) // in case we never make it into clear
      index_ = 0;
    records[index_++] = r;
  }

  // Clear the buffer
  void clear() {
    cguard g(lock_);
    index_ = 0;
  }

  // How many elements have been used?
  int size() const {
    cguard g(const_cast<cmutex&>(lock_));
    return index_;
  }

  // How many elements does this support
  int capacity() const {return arraySize_;}

  // Dump the records to a file
  void write(std::ostream& fp);

private:
  // this is the size of the buffer
  int arraySize_;
  // this is the next element to be used
  int index_;
  // this is the array (memory leaks and lockup when using std::vector)
  LogRecord* records;
  // protect the array and index_
  cmutex lock_;
  // not implemented
  LogBuffer(const LogBuffer& o);
  // not implemented
  LogBuffer& operator=(const LogBuffer& o);
};

#endif
