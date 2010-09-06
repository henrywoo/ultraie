#ifndef __PLUGINS_mediadetection__
#define __PLUGINS_mediadetection__

#include "../uuplugin/uuplugins.h"
#include "helperfunc.h"

namespace nsplugin{

  // Process-bound
  class mediadetection: public uuplugin
  {
  private:
    bool FilterMediaURL(const wchar_t* szUrl);
    void ProcessMediaURL(const wstring& lowerurl,METYPE mt);

    void MediaRes(string& dbcontent,wstring& fname,char*& flag);

  public:
    mediadetection();
    ~mediadetection();

    virtual  bool ProcessBasedInit();
    virtual  bool ProcessBasedTerm();

    virtual  bool ThreadBasedInit();
    virtual  bool ThreadBasedTerm();

    //Called when triggering DISPID_NAVIGATECOMPLETE2
    virtual bool OnNavigateComplete2(int paneid,const wchar_t* url);
    virtual void OnDocumentComplete(const wstring& _url,const wstring& _title,
      const wstring& _iconurl);
    virtual void OnBeforeNaviagte2(const wstring& _url);
    virtual void OnQuit(const wstring& _url);
    virtual bool OnClickMenuItem(int id);

    virtual bool init();
    virtual bool StartPlugin();
    virtual bool StopPlugin();

    virtual bool OnLButtonDown(int paneid,int xpos,int ypos);
    virtual bool OnRButtonDown(int);
    virtual bool disenable_plugin();
    virtual void AssociateImage(int wID,int& _img);

    // For ultra://history/...
    virtual void StartUltraProtocol(const wchar_t* _url,
      std::string& dbcontent,
      std::wstring& fname,
      char*& flag,BYTE* pbuffer,ULONG buffersize);
    virtual void APPURLFilter(const wchar_t* _url);
    virtual void OnHookMsg(UINT,WPARAM,LPARAM,BOOL&){};
  };

};




#endif
