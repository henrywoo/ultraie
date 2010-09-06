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
//�������˵��������ҽ�Explorer.exe����Cmd.exe���������е�CreateProcessA/W,  
//һ��˵��������Windows�߼�����ϵ�����Ӧ����û����ģ�����ʱ��Explorer.exe  
//��CreateProcessA/W���������Ѿ�����Ľ��̹ҽ���(������M$�Լ������ֽ�),��������  
//��Explorer.exe�ĺ���������и����޷��ҵ�CreateProcessA/W����ڣ��Ӷ�������ҽ�  
//ʧ�ܣ��ɴ˿ɼ����޸ĺ��������ҽ�API�ķ����޷��γ���ʽ�ҽ�(��������ĳ�����̹ҽ�  
//�󣬾Ͳ����ٱ���Ľ��̹ҽ�)��  
//����취��  
//��WinNT/2000�¿������޸�API��������ķ����ҽ�API,��Ϊÿ���������ں�DLL���Ƕ����ģ�  
//���ҿ����ú���VirtualProtectEx�޸�DLL��ҳ�汣������������д����Win9X�¸÷����в�ͨ,  
//��Ϊ�ں�DLL���ǹ���ģ�����ҳ�治��д
//�޸ķ������޸�CreateProcess��ǰ7���ֽ�Ϊ
//MOV   EAX,XXXX
//JMP   EAX
//XXXX   Ϊ   ��ĺ����ĵ�ַ
//X86�¶�������Ϊ0B8H,X,X,X,X,0FFH,0E0H
//
//��Ȼ�����и��õĽ���취,��9X/NT��ͨ�ã������ܸ��ӣ�����˼���.  
//ʹ��Debug   APIs   ������Ҫ�ҽӵĽ���(��Ȼ�������Ľ��̽����ˣ������Խ���Ҳ�����)  
//�ڵ��Ժ������ñ����Խ��̵���ִ�С�Ȼ����ú���GetThreadContextȡ���̵߳�  
//EIP(��ǰָ��ָ��)������EIP���Ƿ����ҳ�CreateProcessA/W���ҽӵ�THUNK��  
//�Ӷ�ȡ�ú����ĵ�����ڡ���ֻ��һ���뷨����δʵ�֡�   
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