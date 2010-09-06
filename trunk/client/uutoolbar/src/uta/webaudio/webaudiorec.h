#ifndef __PLUGINS_WEBAUDIOREC__
#define __PLUGINS_WEBAUDIOREC__

#include "mp3creator.h"
#include "../uuplugin/uuplugins.h"
#include "guard.h"

namespace nsplugin{

  // Singleton / Processed based
  class webaudiorec: public uuplugin
  {
  private:
    static bool initialized;
    static webaudiorec* pinstance;

    static std::vector<char> thunkdata_;
    static cmutex lock_;

    webaudiorec();
  public:
    static CMP3Creator mp3creator_;

    static bool addData(char* pData,int len);
    static unsigned long moveoutData();

    ~webaudiorec();

    virtual bool init();
    virtual bool StartPlugin();
    virtual bool StopPlugin();

    virtual  bool ProcessBasedInit();
    virtual  bool ProcessBasedTerm();

    virtual  bool ThreadBasedInit();
    virtual  bool ThreadBasedTerm();

    virtual bool OnLButtonDown(int paneid,int xpos,int ypos);
    virtual bool OnRButtonDown(int paneid);
    virtual bool disenable_plugin();
    virtual bool OnNavigateComplete2(int paneid,const wchar_t* url){return true;}
    virtual bool OnClickMenuItem(int id){return true;}
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

    static webaudiorec& R();
  };

};




#endif
