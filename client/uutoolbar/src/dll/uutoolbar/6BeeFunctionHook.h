#pragma once
#include <mmsystem.h>//HWAVEOUT
#include <dsound.h>//LPDIRECTSOUNDBUFFER
#pragma comment(lib,"dsound.lib")

#include <wininet.h>
#pragma comment(lib,"wininet.lib")

#include <set>
//////////////////////////////////////////////////////////////////////////
void HookwaveOutWrite();
void UnHookwaveOutWrite();
void HookLock();
void UnHookLock();
void HookPlay();
void UnHookPlay();

MMRESULT WINAPI NewwaveOutWrite(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
HRESULT WINAPI NewLock(LPDIRECTSOUNDBUFFER lpDsb,DWORD dwWriteCursor,DWORD dwWriteBytes,LPVOID *lplpvAudioPtr1,
  LPDWORD lpdwAudioBytes1,LPVOID *lplpvAudioPtr2,LPDWORD lpdwAudioBytes2,DWORD dwFlags);
HRESULT WINAPI NewPlay(LPDIRECTSOUNDBUFFER lpDsb, DWORD d1, DWORD d2, DWORD d3);

//////////////////////////////////////////////////////////////////////////
//http://topic.csdn.net/t/20050213/17/3782626.html
//按照你的说法，是想挂接Explorer.exe或者Cmd.exe两个进程中的CreateProcessA/W,  
//一般说来，按照Windows高级编程上的做法应该是没问题的，但有时候，Explorer.exe  
//的CreateProcessA/W函数可能已经被别的进程挂接了(或者是M$自己做的手脚),这样，你  
//在Explorer.exe的函数导入表中根本无法找到CreateProcessA/W的入口，从而导致你挂接  
//失败，由此可见，修改函数导入表挂接API的方法无法形成链式挂接(即函数被某个进程挂接  
//后，就不能再被别的进程挂接)。  
//解决办法：  
//在WinNT/2000下可以用修改API函数代码的方法挂接API,因为每个进程中内核DLL都是独立的，  
//而且可以用函数VirtualProtectEx修改DLL的页面保护属性让它可写，在Win9X下该方法行不通,  
//因为内核DLL都是共享的，而且页面不可写
//修改方法，修改CreateProcess的前7个字节为
//MOV   EAX,XXXX
//JMP   EAX
//XXXX   为   你的函数的地址
//X86下二进制码为0B8H,X,X,X,X,0FFH,0E0H
//
//当然，还有更好的解决办法,在9X/NT下通用，不过很复杂，给出思想吧.  
//使用Debug   APIs   调试你要挂接的进程(当然，如果你的进程结束了，被调试进程也会结束)  
//在调试函数中让被调试进程单步执行。然后调用函数GetThreadContext取得线程的  
//EIP(当前指令指针)，利用EIP看是否能找出CreateProcessA/W被挂接的THUNK，  
//从而取得函数的调用入口。这只是一个想法，尚未实现。   
//////////////////////////////////////////////////////////////////////////
BOOL WINAPI _6BEECreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation);
BOOL WINAPI _6BEECreateProcessW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, 
		LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, 
		LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
LONG WINAPI _6BEERegSetValueExA(HKEY hKey,LPCSTR lpValueName,DWORD Reserved,DWORD dwType,const BYTE* lpData,DWORD cbData);//Advapi32.dll
LONG WINAPI _6BEERegSetValueExW(HKEY hKey,LPCWSTR lpValueName,DWORD Reserved,DWORD dwType,const BYTE* lpData,DWORD cbData);//Advapi32.dll

/*
STDAPI _6BEEURLOpenStreamA(LPUNKNOWN pCaller,LPCSTR szURL,DWORD dwReserved,LPBINDSTATUSCALLBACK lpfnCB);
STDAPI _6BEEURLOpenStreamW(LPUNKNOWN pCaller,LPCWSTR szURL,DWORD dwReserved,LPBINDSTATUSCALLBACK lpfnCB);
HINTERNET _6BEEHttpOpenRequestA(HINTERNET hConnect,LPCSTR lpszVerb,LPCSTR lpszObjectName,LPCSTR lpszVersion,LPCSTR lpszReferer,LPCSTR* lplpszAcceptTypes,DWORD dwFlags,DWORD_PTR dwContext);
BOOL _6BEEHttpSendRequestA(HINTERNET hRequest,LPCSTR lpszHeaders,DWORD dwHeadersLength,LPVOID lpOptional,DWORD dwOptionalLength);
*/

void   HookFunctionTemp(const char * dllname,const char * functionname, DWORD* btOldBytes, BYTE* _btNewBytes,
  std::set<DWORD> &Hooked_PIDs, DWORD FuncAddress);
void UnHookFunctionTemp(const char * dllname,const char * functionname, DWORD* btOldBytes, std::set<DWORD> &Hooked_PIDs);

void HookFuncOperation(bool willhook,std::set<DWORD>& a,std::set<DWORD>& b,DWORD pid,DWORD a1[],DWORD b1[],
  const char* dl,const char* fa,const char* fb,BYTE newBytes[]=NULL,DWORD newfa=0,DWORD newfb=0);

BOOL WINAPI Wrap6BEECreateProcessW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, 
		LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, 
		LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation,bool iswebie=true);
BOOL WINAPI Wrap6BEERegSetValueExA(HKEY hKey,LPCSTR lpValueName,DWORD Reserved,DWORD dwType,const BYTE* lpData,DWORD cbData);


#include <vector>

class SoundThunk{
	std::vector<char> thunkData;
public:
	SoundThunk();
	~SoundThunk();
	void addData(char* pData,int len);
	unsigned long moveoutData();
};