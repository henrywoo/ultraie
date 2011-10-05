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

#include "console.h"
#include "guard.h"
#include <iostream>

cmutex cconsole::m_lock;
bool cconsole::m_enabled = true;	
long cconsole::m_isn = 0;

cconsole::cconsole(const wchar_t* fmt, ...) 
{
	if(!m_enabled)
		return;
	m_id = m_isn++;

	va_list argList;
	//const wchar_t* _fmt = fmt.GetString();
	_crt_va_start(argList, fmt);
	m_name.FormatV(fmt, argList);
	_crt_va_end(argList);
	m_threadid = GetCurrentThreadId();
	Write(L"Entering %s", m_name);
};

cconsole::~cconsole()
{
	if(!m_enabled)
		return;
	Write(L"Leaving %s", m_name);
};


void cconsole::Write(wcstr fmt,...)
{
	if(!m_enabled)
		return;

	va_list argList;
	va_start(argList, fmt);
	WriteV(fmt,argList);
	va_end(argList);
}

void cconsole::WriteV(wcstr fmt,va_list argList)
{
	if(!m_enabled)
		return;

	cguard<cmutex> guard(m_lock);

    struct tm now;
    time_t long_time;
    time( &long_time );
    localtime_s(&now, &long_time);
	wchar_t timebuf[128];
	wcsftime( timebuf, 32, L"%Y-%m-%d %H:%M:%S ", &now);

	ATL::CString isn, msg;
  static unsigned long timeflag = clock();
	isn.Format(L"%s %04d %09lu %06lu %04d ", timebuf, m_id, clock(), clock()-timeflag, m_threadid);
  timeflag = clock();
	msg.FormatV(fmt, argList);
	std::wclog << isn.GetString() << msg.GetString() << std::endl;
}

std::wstreambuf* cconsole::Redirect(std::wostream& strm)
{
	if(!m_enabled)
		return strm.rdbuf();

	std::wstreambuf* strm_buffer = std::wclog.rdbuf();
	
	std::wclog.rdbuf (strm.rdbuf());

	return strm_buffer;
}
