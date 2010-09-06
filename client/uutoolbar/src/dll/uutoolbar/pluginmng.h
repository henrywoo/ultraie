#ifndef __UUTOOLBAR_PLUGINMNG__
#define __UUTOOLBAR_PLUGINMNG__

#include <vector>
#include "../../uta/uuplugin/uuplugins.h"
#include "basictypes.h"

namespace nsplugin{
  using namespace std;

  class PluginMng{
  private:
    static bool initialized;
    static PluginMng* pinstance;
    static cmutex lock_;

    DISALLOW_COPY_AND_ASSIGN(PluginMng);
    PluginMng();
    vector<uuplugin*> plugins_;
    vector<uuplugin*> disabled_plugins_;
    vector<int> partswidth_;// = {2,112,142,172} for two plugins

    bool LoadPlugins();

    // only used in GetPluginByLocation()
    uuplugin* GetEnabled(int ordernum);
  public:
    static PluginMng& R();
    ~PluginMng();
    void AddPlugin(uuplugin *parser);
    
    int GetEnabledPluginNum();
    int GetCurrentPaneID(uuplugin*);
    int GetAllPluginNum();
    int GetTotalWidth();

    bool InitPlugins();
    //used when initialization and refreshing pane layout
    void fillpartswidth();
    uuplugin* GetPlugin(int);

    // when click status bar, return the plugin we click
    // param@ the x coordinate of click point
    uuplugin* GetPluginByLocation(int,int&);
    vector<uuplugin*>& PluginMng::GetPluginVec();

    vector<int>& GetPartsWidthVec();
    bool IsPointOverIcon(int x);
    int* GetPartsWidthPtr();

    void OnNavComp2(const wchar_t* url);
    void OnDocComp(const wstring& url,const wstring& title,
      const wstring& iconurl);
    void OnBeforeNav2(const wstring& url);
    void OnQuit(const wstring& url);
    void StartUltraProtocol(const wchar_t* _url,
      std::string& dbcontent,
      std::wstring& fname,
      char*& flag,BYTE* pbuffer,ULONG buffersize);
    void APPURLFilter(const wchar_t* _url);
  };
};



#endif