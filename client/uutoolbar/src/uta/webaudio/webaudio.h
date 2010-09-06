#ifndef __UUTOOLBAR_WEBAUDIO__
#define __UUTOOLBAR_WEBAUDIO__

#include "../uuplugin/uuplugins.h"
#include "audioparam.h"

namespace nsplugin{

  // Singleton / Processed based
  class webaudio: public uuplugin
  {
  private:
    webaudio();

    static bool initgurad();
    static DWORD ori_waveOutWriteaddrbuffer[2];

    static BYTE jumpbytes[8];
    static CRITICAL_SECTION gcri_;
    static bool ishooked_;

    static bool HookwaveOutWrite();
    static bool UnHookwaveOutWrite();
    static MMRESULT WINAPI NewwaveOutWrite(HWAVEOUT hwo, 
      LPWAVEHDR pwh, UINT cbwh);

#if 0
    static LONG gDSoundVolume;
    static DWORD ori_lockaddressbuffer[2];
    static DWORD ori_playaddrbuffer[2];
    static bool HookLock();
    static bool HookPlay();
    static bool UnHookLock();
    static bool UnHookPlay();
    static HRESULT WINAPI NewLock(LPDIRECTSOUNDBUFFER lpDsb,
      DWORD dwWriteCursor,
      DWORD dwWriteBytes,LPVOID *lplpvAudioPtr1,LPDWORD lpdwAudioBytes1,
      LPVOID *lplpvAudioPtr2,LPDWORD lpdwAudioBytes2,DWORD dwFlags);
    static HRESULT WINAPI NewPlay(LPDIRECTSOUNDBUFFER lpDsb,
      DWORD d1, DWORD d2, DWORD d3);
    static bool InitLockPlayAddr();
#endif
   

    static bool IsHooked();
    static bool StartHook();
    static bool StopHook();

  public:
    typedef MMRESULT (*MywaveOutWrite)(HWAVEOUT hwo,
      LPWAVEHDR pwh, UINT cbwh);

    ~webaudio();
    virtual  bool ProcessBasedInit();
    virtual  bool ProcessBasedTerm();

    virtual  bool ThreadBasedInit();
    virtual  bool ThreadBasedTerm();

  public:
    virtual bool init();
    virtual bool StartPlugin();
    virtual bool StopPlugin();

    virtual bool OnLButtonDown(int paneid,int xpos,int ypos);
    virtual bool OnRButtonDown(int paneid);
    virtual bool disenable_plugin();
    virtual bool OnNavigateComplete2(int paneid,const wchar_t* url)
    {return true;}
    virtual bool OnClickMenuItem(int id)
    {return true;}
    virtual void AssociateImage(int wID,int& _img){}
    virtual void OnDocumentComplete(const wstring& _url,const wstring& _title,
      const wstring& _iconurl){};
    virtual void OnBeforeNaviagte2(const wstring& _url){};
    virtual void OnQuit(const wstring& _url){};
    virtual void StartUltraProtocol(const wchar_t* _url,
      std::string& dbcontent,
      std::wstring& fname,
      char*& flag,BYTE* pbuffer,ULONG buffersize){};
    virtual void APPURLFilter(const wchar_t* _url){};
    virtual void OnHookMsg(UINT,WPARAM,LPARAM,BOOL&){};

    static webaudio& R();
  };

};




#endif
