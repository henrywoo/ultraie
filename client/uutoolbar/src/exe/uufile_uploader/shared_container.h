///////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
//
#include <list>
#include <algorithm>
#include <functional>

#include "FileProfile.h"

/*
 * functor is a big shit!
template<typename T1>
struct debug_print_SerialSet:public binary_function<T1, int, void>
{
	void operator()(const T1 & b,const int c) const{
		int d = c;
		CString a;a.Format(_T("( %d. ) %s"),++d, b->m_filelocation.GetString());
		OutputDebugString( a.GetString() );
	}
};
*/

template <typename T>
class SerialSet:public set<T>
{
	private:
		HANDLE m_Mutex;
	public:
		SerialSet(){
			m_Mutex = ::CreateMutex(NULL,false,NULL);
		}
		~SerialSet(){
			::CloseHandle(m_Mutex);
		}
		bool shared_erase(const T &a){
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				bool should_erase_it = false;
				for(iterator i=begin();i!=end();i++){
					if(i->Compare(a)==0){
						should_erase_it = true;
					}
				}
				if(should_erase_it)
					erase(a);
				::ReleaseMutex(m_Mutex);
				return true;
			}else{
				OutputDebugStringA("Cannot acquire mutex 1");
				return false;
			}
		}

		bool shared_insert(const T &a){
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				bool should_insert_it = true;
				for(const_iterator i=begin();i!=end();i++){
					if(i->Compare(a)==0){
						should_insert_it = false;
					}
				}
				if(should_insert_it)
					insert(a);
				::ReleaseMutex(m_Mutex);
				return true;
			}else{
				OutputDebugStringA("Cannot acquire mutex 2");
				return false;
			}
		}

		bool shared_print(){
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				OutputDebugStringA( "-------------- 当前活动(active)的线程对应的filepath --------------" );
				int tmp=0;
				for(const_iterator i=begin();i!=end();i++){
					CString a;
					a.Format(_T("( %d. ) %s"),++tmp, i->GetString());
					OutputDebugString( a.GetString() );
				}
				OutputDebugStringA( "------------------------------------------------------------------" );
				::ReleaseMutex(m_Mutex);
				return true;
			}else{
				OutputDebugStringA("Cannot acquire mutex 3");
				return false;
			}
		}

		bool shared_empty(/*in*/bool& result) const{
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				result = empty();
				::ReleaseMutex(m_Mutex);
				return true;
			}else{
				OutputDebugStringA("Cannot acquire mutex 4");
				return false;
			}
		}

		bool shared_size(/*in*/int& result) const{
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				result = size();
				::ReleaseMutex(m_Mutex);
				return true;
			}else{
				OutputDebugStringA("Cannot acquire mutex 5");
				return false;
			}
		}

};

template <typename T>
class SerialList:public list<T>{

	private:
		HANDLE m_Mutex;
	public:
		SerialList(){
			m_Mutex = ::CreateMutex(NULL,false,NULL);
		}
		~SerialList(){
			::CloseHandle(m_Mutex);
		}
		bool shared_empty(/*in*/bool& result) const{
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				result = empty();
				::ReleaseMutex(m_Mutex);
				return true;
			}else{
				OutputDebugStringA("Cannot acquire mutex 4");
				return false;
			}
		}

		bool shared_push_back(const T &a){
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				push_back(a);
				::ReleaseMutex(m_Mutex);
				return true;
			}else{
				OutputDebugStringA("Cannot acquire mutex 2");
				return false;
			}
		}

		bool shared_pop_front(){
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				pop_front();
				::ReleaseMutex(m_Mutex);
				return true;
			}else{
				OutputDebugStringA("Cannot acquire mutex 2");
				return false;
			}
		}


		bool shared_has_item(const T &a,/*out*/bool & r){
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				for(const_iterator i=begin();i!=end();i++){
					if((*i)->m_filelocation.Compare(a->m_filelocation)==0){
						r=true;
						break;
					}
				}
				//r = std::find(begin(),end(),a)==end();
				::ReleaseMutex(m_Mutex);
				return true;
			}else{
				OutputDebugStringA("Cannot acquire mutex 2");
				return false;
			}
		}

		bool shared_remove(const T &a){
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				remove(a);
				::ReleaseMutex(m_Mutex);
				return true;
			}else{
				OutputDebugStringA("Cannot acquire mutex 2");
				return false;
			}
		}


};




template <typename Key,typename Value>
class SerialMap:public map<Key,Value>{

	private:
		HANDLE m_Mutex;
	public:
		SerialMap(){
			m_Mutex = ::CreateMutex(NULL,false,NULL);
		}
		~SerialMap(){
			::CloseHandle(m_Mutex);
		}

		bool shared_insert(const Key &a,const Value &b){// CString -> HANDLE
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				bool should_insert_it = true;
				for(const_iterator i=begin();i!=end();i++){
					if(i->first.Compare(a)==0){
						should_insert_it = false;
						break;
					}
				}
				if(should_insert_it)
					insert(make_pair(a,b));
				::ReleaseMutex(m_Mutex);
				return true;
			}else
				return false;
		}

		bool shared_erase(const Key &a){// CString -> HANDLE
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				erase(a);
				::ReleaseMutex(m_Mutex);
				return true;
			}else
				return false;
		}

		// subscript
		Value shared_get_Value_from_Key(const Key &a){
			DWORD dwResult = WaitForSingleObject(m_Mutex,INFINITE);
			if(dwResult==WAIT_OBJECT_0){
				Value tmp = (*this)[a];
				::ReleaseMutex(m_Mutex);
				return tmp;
			}else
				return NULL;
		}

};
