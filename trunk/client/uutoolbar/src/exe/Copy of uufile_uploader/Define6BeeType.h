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

#ifndef INC_DEFINEMENTS_H
#define INC_DEFINEMENTS_H

#include <time.h>
#include <string>
#include <set>

namespace nsHelper{
	/// Class with static functions to get information about an error.
	class CError
	{
	public:
		static DWORD GetLastError(){
 return ::GetLastError();
    }

		static std::wstring GetErrorDescription(int iErrorCode = GetLastError()){
			LPVOID lpMsgBuf=NULL;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, iErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				reinterpret_cast<wchar_t*>(&lpMsgBuf), 0, NULL);
			std::wstring strErrorDescription;
			if( lpMsgBuf ){
				strErrorDescription = reinterpret_cast<wchar_t*>(lpMsgBuf);
				LocalFree(lpMsgBuf);
			}
			return strErrorDescription;
		}
	};

	/// Base class for implementing the notification stuff.
	/// @remarks Inherit public (instead of private) because it wouldn't compile under Dev-C++
	template <typename T, typename T2>
  class CObserverBase : public std::set<T>{
	public:
		typedef typename std::set<T> base_type;
		typedef typename std::set<T>::iterator iterator;

		~CObserverBase(){
			for( iterator it=base_type::begin(); it!=base_type::end(); it=base_type::begin() )
				(*it)->Detach(static_cast<T2>(this));
		}

		bool Attach(T p){
			if (find(p)!=base_type::end())
				return false;
			insert(p);
			p->Attach(static_cast<T2>(this));
			return true;
		}

		bool Detach(T p){
			if (find(p)==base_type::end())
				return false;
			erase(p);
			p->Detach(static_cast<T2>(this));
			return true;
		}
	};

	/// @brief Calculates elapsed CPU time.
	/// Is useful for calculating transferrates.
	class CTimer
	{
	public:
		CTimer():
			m_dfStart(clock()),
			m_sentbytes(0),
			m_bound(0),
			m_timepassed(0),
			m_timeremaining(0),
			m_size_long(-1),
			m_iniStart(0),
			m_finishedbytes(0){}

		/// Start the timer.
		void Start(){
			m_iniStart = clock()/CLOCKS_PER_SEC;
		}

		/// Restart the timer.
		void Restart(){
			m_dfStart = clock();
			m_sentbytes = 0;
		}

		// 在新的任务进来的时候调用
		void ReLaunch(){
			m_dfStart = clock();
			m_sentbytes = 0;
			m_bound = 0;
			m_timepassed = 0;
			m_timeremaining = 0;
			m_size_long = -1;
			m_iniStart = 0;
			m_finishedbytes = 0;
			m_sentbytes = 0;
		}

		void SetBound(short a){
			m_bound = a;
		}

		void addSentBytes(long a){
			m_sentbytes += a;
		}

		long getSpeed(){
			double a = GetElapsedTime();
			if ( a > m_bound && a!=0 ){
				m_curspeed=(long)(m_sentbytes/(a*1024));
				return m_curspeed==0 ? 1 : m_curspeed;
			}else{
				return -1;
			}
		}

		/// Get the elapsed time in seconds.
		double GetElapsedTime() const{
			return (static_cast<double>(clock()) - m_dfStart)/CLOCKS_PER_SEC;
		}

		// 获得已经过去的时间
		int GetTimepassed(){
			return (int)(clock()/CLOCKS_PER_SEC - m_iniStart);
		}

		// 获得剩余时间
		int GetRemainingTime(){
			if (m_curspeed!=0 && m_size_long!=-1 && m_size_long>m_finishedbytes){
				int a = (m_size_long - m_finishedbytes)/(m_curspeed*1024);
				return a;
			}else{
				return 1;
			}
		}

		void SetFileSize(long a){
			m_size_long = a;
		}

	private:
		double m_dfStart; //Elapsed CPU time for process (start of measurement)
		double m_iniStart;
		int    m_timepassed;
		int    m_timeremaining;
		short  m_bound;
		long   m_curspeed;
		long   m_size_long;
		long   m_sentbytes;    //要不断reset的
	public:
		long   m_finishedbytes;//总共发送出去的量
	};
}

#endif // INC_DEFINEMENTS_H
