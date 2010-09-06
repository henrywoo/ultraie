#include "webaudio.h"
#include "resource.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "6bees_util.h"
#include "6bees_window.h"
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dsound.lib")

#include "webaudiorec.h"

#pragma warning(disable: 4311)
#pragma warning(disable: 4312)

namespace nsplugin{
  using namespace _6bees_util;

#ifdef __CHINESE__
#define ktip_wm L"ÍøÒ³¾²ÒôÆ÷"
#define kcap_wm L"ÍøÒ³¾²ÒôÆ÷"
#else
#define ktip_wm L"Web Muter"
#define kcap_wm L"Web Muter"
#endif

  //Processed Based Variables initialization
  /*jumpbytes actually is:
  * mov  eax, 0x400000
  * jmp   eax
  *
  * here 0x400000 is the address of new function*/
  // NOTE: must be locked by the same lock
  BYTE webaudio::jumpbytes[8] = {
    0x00B8,0x0000,0x0000,0x0040,0x0000,0x00FF,0x00E0,0x0000
  };
  DWORD webaudio::ori_waveOutWriteaddrbuffer[2] ={};
#if 0
  DWORD webaudio::ori_lockaddressbuffer[2]={0,0};
  DWORD webaudio::ori_playaddrbuffer[2]={0,0};

  // original volume
  // initialize as DSBVOLUME_MIN
  // NOTE: After unhook, set it as DSBVOLUME_MIN again!!!

  LONG webaudio::gDSoundVolume=DSBVOLUME_MIN;
#endif
  bool webaudio::ishooked_=false;
  CRITICAL_SECTION webaudio::gcri_={};
  const UINT MUTE_WEBPAGE = 4096;/// @todo 
  const UINT UNMUTE_WEBPAGE = 4097;


  //////////////////////////////////////////////////////////////////////////
  webaudio& webaudio::R(){
    static webaudio _webaudio;
    return _webaudio;
  }

  webaudio::webaudio(){
    // should read from DB or other places
    enabled_ = true;
    len_ = 30;
    caption_= kcap_wm;
    tip_ = ktip_wm;
    menuwID_=1314;
    /*
    HIMAGELIST himglst=ImageList_LoadImage(hmodule_,MAKEINTRESOURCE(IDB_WEBAUDIO),
      16,1,CLR_DEFAULT,IMAGE_BITMAP,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_SHARED);
    HICON hi1=ImageList_GetIcon(himglst,0,ILD_NORMAL);
    HICON hi2=ImageList_GetIcon(himglst,1,ILD_NORMAL);
    if(hi1==NULL){
      MessageBoxA(NULL,"E","hi1",NULL);
    }
    if(hi2==NULL){
      MessageBoxA(NULL,"E","hi2",NULL);
    }
    if(hi1!=NULL)icons_.push_back(hi1);
    if(hi2!=NULL)icons_.push_back(hi2);*/
    
    //icons_ init finish

    ProcessBasedInit();
    ThreadBasedInit();
  }

  webaudio::~webaudio(){
    ThreadBasedTerm();
    ProcessBasedTerm();
  }

  bool webaudio::init(){
    initgurad();
    if(enabled_ && audioparam::R().ismuting){
      webaudio::StartHook();
    }
    return true;
  }

  bool webaudio::initgurad(){
    if (!audioparam::R().isinit()){
#if 0
      if (InitLockPlayAddr()){
#else
      if (true){
#endif
        InitializeCriticalSection(&gcri_);
        audioparam::R().setinit(true);
        return true;
      }else{
        return false;
      }
    }
    return true;
  }

  bool webaudio::StartPlugin(){
    StartHook();
    return true;
  }

  bool webaudio::StopPlugin(){
    StopHook();
    return true;
  }

  bool webaudio::OnLButtonDown(int paneid,int xpos, int ypos){
    if(!audioparam::R().ismuting){
      StartPlugin();
      active_icon_ = icons_.at(1);
      audioparam::R().ismuting = true;
    }else{
      if (!audioparam::R().isrecording){
        //when recording, then leave it there.
        StopPlugin();
      }
      active_icon_ = icons_.at(0);
      audioparam::R().ismuting = false;
    }

    C6BeeMenuStatusBar** pSB=nsplugin::GetPPSB();
    int SBnum=nsplugin::GetSBSize();
    for (int i=0;i<SBnum;++i){
      if ((*pSB)!=NULL && ::IsWindow((*pSB)->m_hWnd)){
        (*pSB)->SetIcon(paneid,active_icon_);
      }
      pSB++;
    }
    return true;
  }

  bool webaudio::OnRButtonDown(int){
    return true;
  }

  // Do not modify enabled_ here. the caller will modify it
  bool webaudio::disenable_plugin(){
    if(enabled_){
      if(audioparam::R().ismuting){
        if(!audioparam::R().isrecording){
          StopPlugin();
        }
        active_icon_ = icons_.at(0);
        audioparam::R().ismuting = false;
      }
    }else{
      if(audioparam::R().ismuting){// modified
        StartPlugin();
        active_icon_ = icons_.at(1);
        audioparam::R().ismuting = true;
      }
    }
    return true;
  }

  bool webaudio::StartHook(){
    if(!ishooked_){
      HookwaveOutWrite();
#if 0
      HookLock();
      HookPlay();
#endif
      ishooked_=true;
    }
    return true;
  }

  bool webaudio::StopHook(){
    if(ishooked_){
      UnHookwaveOutWrite();
#if 0
      UnHookLock();//origianlly no this fun here?!
      UnHookPlay();
      gDSoundVolume=DSBVOLUME_MIN;
#endif
      ishooked_=false;
    }
    return true;
  }

  bool webaudio::ProcessBasedInit(){
    if(pid_==0){
      pid_ = GetCurrentProcessId();
      return true;
    }else{
      return false;
    }
  }

  bool webaudio::ProcessBasedTerm(){
    //No need to stop hook ?? need! gcri_ already be deleted... 
    //can not be used any more- undefined
    if(audioparam::R().ismuting){
      webaudio::StopHook();
      audioparam::R().ismuting = false;
    }
    ::DeleteCriticalSection(&webaudio::gcri_);
    for (vector<HICON>::const_iterator i=icons_.begin();i!=icons_.end();++i){
      DestroyIcon(*i);
    }
    return true;
  }

  bool webaudio::ThreadBasedInit(){
    return true;
  }
  bool webaudio::ThreadBasedTerm(){
    return true;
  }

  bool webaudio::IsHooked(){
    return webaudio::ishooked_;
  }

  bool webaudio::HookwaveOutWrite(){
    bool result=false;
    ::EnterCriticalSection(&gcri_);
    HMODULE hlib = LoadLibraryA("winmm.dll");
    if (hlib==NULL){
      ::LeaveCriticalSection(&gcri_);
      return result;
    }
    auto_libloader _a(hlib);
    //Can use typedef DWORD (CALLBACK *LPFNREGISTER)(DWORD,DWORD);
    MywaveOutWrite pwaveOutWrite = reinterpret_cast<MywaveOutWrite>(GetProcAddress(hlib,"waveOutWrite"));
    DWORD dwsize = 0;
    HANDLE hprocess=::GetCurrentProcess();
    int bytesize = sizeof(ori_waveOutWriteaddrbuffer);
    if(!::ReadProcessMemory(hprocess, (LPCVOID)pwaveOutWrite, ori_waveOutWriteaddrbuffer, bytesize, &dwsize)){
      ::LeaveCriticalSection(&gcri_);
      return result;
    }
    DWORD tmp = reinterpret_cast<DWORD>(NewwaveOutWrite);
    ::memcpy(jumpbytes + 1, &tmp, sizeof(tmp));
    result = (::WriteProcessMemory(hprocess, (LPVOID)pwaveOutWrite,
      jumpbytes, bytesize, &dwsize) ? true: false);
    memset(jumpbytes+1,0x00,4);
    ::LeaveCriticalSection(&gcri_);
    return result;
  }

#if 0

  bool webaudio::InitLockPlayAddr(){
    LPDIRECTSOUND pSnd=NULL;
    if(SUCCEEDED(::DirectSoundCreate(NULL, &pSnd, NULL))){
      static const DWORD SAMPERSEC=22050,BLOCKALIGN=4,BITSPERSAM=16;
      WAVEFORMATEX wf={WAVE_FORMAT_PCM,2,SAMPERSEC,
        SAMPERSEC*BLOCKALIGN,BLOCKALIGN,
        BITSPERSAM,sizeof(WAVEFORMATEX)};
      DSBUFFERDESC dsbdesc={};
      dsbdesc.dwSize = sizeof(DSBUFFERDESC);
      // Buffer size is determined by hardware.
      // Indicates that the buffer is a primary sound buffer.
      // If this value is not specified, a secondary sound buffer will be created.
      dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
      dsbdesc.dwBufferBytes = 0;
      dsbdesc.lpwfxFormat = NULL; //must be NULL
      if (SUCCEEDED(pSnd->SetCooperativeLevel(::GetDesktopWindow(), DSSCL_NORMAL))){//??
        LPDIRECTSOUNDBUFFER tmplpdsb= audioparam::R().getlpDsb();
        if SUCCEEDED(pSnd->CreateSoundBuffer(&dsbdesc, &tmplpdsb, NULL)){
          // lpDsb: Address of a variable that receives the IDirectSoundBuffer
          // interface of the new buffer object.
          // According to memory layout of IDirectSoundBuffer,Lock(...) and
          //Play(...) are 12nd and 13rd function of the it,
          // So their address are class base address + 4*11 and + 4*12.
          DWORD dwTmp = (DWORD)(*((DWORD *)tmplpdsb));
          audioparam::R().setlockaddr(dwTmp + 0x2C); //0x2C = 4*11
          audioparam::R().setplayaddr(dwTmp + 0x30); //0x30 = 4*12
          return true;
        }
      }
    }
    return false;
  }

  bool webaudio::HookLock(){
    ::EnterCriticalSection(&gcri_);
    DWORD dwSize = 0;
    HANDLE hProcess = NULL;
    hProcess = ::GetCurrentProcess();
    if(audioparam::R().getlockaddr() != 0){
      if(!::ReadProcessMemory(hProcess, (LPCVOID)audioparam::R().getlockaddr(), ori_lockaddressbuffer, 2 * sizeof(DWORD), &dwSize)){
        ::LeaveCriticalSection(&gcri_);
        return false;
      }
      DWORD tmp = (DWORD)NewLock;
      memcpy(jumpbytes + 1, &tmp, sizeof(tmp));
      if(!::WriteProcessMemory(hProcess, (LPVOID)audioparam::R().getlockaddr(), jumpbytes, 2 * sizeof(DWORD), &dwSize))
      {
        memset(jumpbytes+1,0x0,4);
        ::LeaveCriticalSection(&gcri_);
        return false;
      }
      memset(jumpbytes+1,0x0,4);
    }
    ::LeaveCriticalSection(&gcri_);
    return true;
  }

  bool webaudio::HookPlay(){
    ::EnterCriticalSection(&gcri_);
    DWORD dwSize = 0;
    HANDLE hProcess = NULL;
    hProcess = ::GetCurrentProcess();
    if(audioparam::R().getplayaddr() != 0){
      if(!::ReadProcessMemory(hProcess, (LPCVOID)audioparam::R().getplayaddr(),
        ori_playaddrbuffer, 2 * sizeof(DWORD), &dwSize)){
        ::LeaveCriticalSection(&gcri_);
        return false;
      }
      DWORD tmp = (DWORD)NewPlay;
      memcpy(jumpbytes + 1, &tmp, sizeof(tmp));
      DWORD dwOldProtect=0;
      LPVOID ppfn = (LPVOID)audioparam::R().getplayaddr();
#if 0
      if (VirtualProtect(ppfn, sizeof(ppfn), PAGE_EXECUTE_READWRITE, &dwOldProtect)){//PAGE_EXECUTE_READWRITE
        if(!::WriteProcessMemory(hProcess, ppfn, jumpbytes, 2 * sizeof(DWORD), &dwSize)){
          memset(jumpbytes+1,0x0,4);
          ::LeaveCriticalSection(&gcri_);
          return false;
        }
        VirtualProtect(ppfn, sizeof(ppfn), dwOldProtect, &dwOldProtect);
      }
#endif
      if(!::WriteProcessMemory(hProcess, (LPVOID)audioparam::R().getplayaddr(), jumpbytes, 2 * sizeof(DWORD), &dwSize)){
        memset(jumpbytes+1,0x0,4);
        ::LeaveCriticalSection(&gcri_);
        return false;
      }
      memset(jumpbytes+1,0x0,4);
    }
    ::LeaveCriticalSection(&gcri_);
    return true;
  }

  bool webaudio::UnHookLock(){
    ::EnterCriticalSection(&gcri_);
    DWORD dwSize = 0;
    HANDLE hProcess = ::GetCurrentProcess();
    if(audioparam::R().getlockaddr() != NULL){
      if(!::WriteProcessMemory(hProcess, (LPVOID)audioparam::R().getlockaddr(),
        ori_lockaddressbuffer, 2 * sizeof(DWORD), &dwSize)){
        ::LeaveCriticalSection(&gcri_);
        return false;
      }
    }
    ::LeaveCriticalSection(&gcri_);
    return true;
  }

  bool webaudio::UnHookPlay(){
    ::EnterCriticalSection(&gcri_);
    DWORD dwSize = 0;
    HANDLE hProcess = ::GetCurrentProcess();
    if(audioparam::R().getplayaddr() != 0){
      if(!::WriteProcessMemory(hProcess, (LPVOID)audioparam::R().getplayaddr(),
        ori_playaddrbuffer, 2 * sizeof(DWORD), &dwSize)){
        ::LeaveCriticalSection(&gcri_);
        return false;
      }
    }
    ::LeaveCriticalSection(&gcri_);
    return true;
  }

  HRESULT WINAPI webaudio::NewLock(LPDIRECTSOUNDBUFFER _lpDsb,
    DWORD dwWriteCursor, DWORD dwWriteBytes, LPVOID *lplpvAudioPtr1,
    LPDWORD lpdwAudioBytes1, LPVOID *lplpvAudioPtr2, LPDWORD lpdwAudioBytes2,
    DWORD dwFlags)
  {
    ::EnterCriticalSection(&gcri_);
    UnHookLock();
    HRESULT hr = _lpDsb->Lock(dwWriteCursor, dwWriteBytes, lplpvAudioPtr1,
      lpdwAudioBytes1, lplpvAudioPtr2, lpdwAudioBytes2, dwFlags);
    if(audioparam::R().ismuting){
      HookLock();
      if(gDSoundVolume == DSBVOLUME_MIN){
        _lpDsb->GetVolume(&gDSoundVolume);
      }
      _lpDsb->SetVolume(DSBVOLUME_MIN);
    }else{
      _lpDsb->SetVolume(gDSoundVolume);
    }
    ::LeaveCriticalSection(&gcri_);
    return hr;
  }

  HRESULT WINAPI webaudio::NewPlay(LPDIRECTSOUNDBUFFER _lpDsb, DWORD d1,
    DWORD d2, DWORD d3)
  {
    ::EnterCriticalSection(&gcri_);
    UnHookPlay();
    HRESULT hr = _lpDsb->Play(d1, d2, d3);
    HookPlay();
    if(audioparam::R().ismuting){
      _lpDsb->SetVolume(DSBVOLUME_MIN);
    }
    ::LeaveCriticalSection(&gcri_);
    return hr;
  }
  #endif

  MMRESULT WINAPI webaudio::NewwaveOutWrite(HWAVEOUT hwo, 
    LPWAVEHDR pwh, UINT cbwh)
  {
    ::EnterCriticalSection(&gcri_);
    if(audioparam::R().isrecording){
      webaudiorec::R().addData((char*)pwh->lpData,pwh->dwBufferLength);
      //addData((char*)pwh->lpData,pwh->dwBufferLength);
    }
    if(audioparam::R().ismuting){
      memset((LPVOID)pwh->lpData, 0x00, pwh->dwBufferLength);
    }
    UnHookwaveOutWrite();
    MMRESULT ret = waveOutWrite(hwo, pwh, cbwh);
    HookwaveOutWrite();
    ::LeaveCriticalSection(&gcri_);
    return ret;
  }

  bool webaudio::UnHookwaveOutWrite(){
    bool result=false;
    ::EnterCriticalSection(&gcri_);
    HANDLE hProcess = ::GetCurrentProcess();
    HMODULE hlib = LoadLibraryA("winmm.dll");
    if (hlib==NULL){
      ::LeaveCriticalSection(&gcri_);
      return result;
    }
    auto_libloader _a(hlib);
    DWORD pwaveOutWrite = (DWORD)GetProcAddress( hlib, "waveOutWrite" );
    DWORD dwSize = 0;
    result = (::WriteProcessMemory(hProcess, (LPVOID)pwaveOutWrite, ori_waveOutWriteaddrbuffer,
      2 * sizeof(DWORD), &dwSize)?true:false);
    ::LeaveCriticalSection(&gcri_);
    return result;
  }



};