
#include "pluginmng.h"
#include <string>
#include "../../dll/6beecommond/6beecommond.h"
#include "6bees_util.h"

#pragma comment(lib,"6beecommond.lib")

namespace nsplugin{

  static const int kPaneOffset=2;
  static const int kSettingPaneLen=110;

  bool PluginMng::initialized = false;
  PluginMng* PluginMng::pinstance = NULL;
  cmutex PluginMng::lock_;
  
  void AddParser( uuplugin *parser ){
    PluginMng::R().AddPlugin(parser);
  }

  C6BeeMenuStatusBar* GetC6BeeMenuStatusBarPtr(){
    return C6BeeMenuStatusBar::tid2menustatusbarptr_[GetCurrentThreadId()];
  }

  C6BeeMenuStatusBar** GetPPSB(){
    return &C6BeeMenuStatusBar::statusbarvec_.at(0);
  }

  int GetSBSize(){
    return (int)C6BeeMenuStatusBar::statusbarvec_.size();
  }

  int GetCurrentPaneID(uuplugin* p){
    return PluginMng::R().GetCurrentPaneID(p);
  }

  typedef vector<uuplugin*>::const_iterator uuiterator;

  PluginMng::PluginMng(){
    //InitPlugins();
  }

  PluginMng::~PluginMng(){
    for (int i=0;i<(int)plugins_.size();i++){
      uuplugin* uup = plugins_.at(i);
      CRegKey crk;
      long rk=crk.Open(HKEY_LOCAL_MACHINE, _6bees_const::kplugindisabled, KEY_READ|KEY_WRITE);
      if (rk==ERROR_SUCCESS){
        ATL::CString menuid;
        menuid.Format(_T("%d"),uup->menuwID_);
        if(uup->enabled_){
          crk.DeleteValue(menuid);
        }else{
          crk.SetDWORDValue(menuid,1);
        }
      }
    }
  }

  /// @note if put PluginMng into heap(using new PluginMng()), the destructor won't be called!!
  /// So here put it into static area.
  PluginMng& PluginMng::R(){
    if(!initialized){
      cguard g(lock_);
      if(pinstance == NULL){
        static PluginMng p_;
        pinstance = &p_;
        if(pinstance){ //make sure compiler cannot reorder 
          initialized = true; // only set to true after creation 
          p_.InitPlugins();
        }
      } 
    } 
    return *pinstance;
  }

  ///process based init
  /// todo - destructor order(I am not sure if it is correct!!)
  bool PluginMng::InitPlugins(){
    if (plugins_.empty()){
      LoadPlugins();
      uuiterator vb=plugins_.begin(),ve=plugins_.end();
      for (uuiterator i=vb;i!=ve;++i){
        uuplugin* p= *i;
        CRegKey crk;
        long rk=crk.Open(HKEY_LOCAL_MACHINE, _6bees_const::kplugindisabled, KEY_READ|KEY_WRITE);
        if (rk==ERROR_SUCCESS){
          DWORD i=0;
          ATL::CString menuid;
          menuid.Format(_T("%d"),p->menuwID_);
          if (crk.QueryDWORDValue(menuid,i)==ERROR_SUCCESS){
            p->enabled_ = false;
          }
        }
        if (p->enabled_){
          p->init();
        }else{
          disabled_plugins_.push_back(p);
        }
      }
      fillpartswidth();
    }
    return true;
  }

  /// implementation
  /// find all DLLs and load them, the static object in DLL will be init!
  /// plugins_ will be push_back too!!!
  bool PluginMng::LoadPlugins(){
    using _6bees_util::ultrapath;
    const wstring& path = ultrapath::R().get(ultrapath::plugindir);
    if(_6bees_util::MakeSureDir(path.c_str())){
      vector<wstring> ultras;
      _6bees_util::GetAllPluginAbsPath(path.c_str(),ultras);
      for (vector<wstring>::iterator i=ultras.begin();i!=ultras.end();++i){
        LoadLibrary(i->c_str());
      }
    }
    return true;
  }

  int PluginMng::GetCurrentPaneID(uuplugin* p){
    int j=1;
    for (int i=0;i<(int)plugins_.size();i++){
      if (plugins_.at(i)->enabled_){
        j++;
        if(p==plugins_.at(i))
          break;
      }
    }
    return j;
  }
  

  // A callback function!!!
  void PluginMng::AddPlugin(uuplugin* _p){
    plugins_.push_back(_p);
  }

  // will be called when creating status panes
  int* PluginMng::GetPartsWidthPtr(){
    return &partswidth_[0];
  }

  vector<int>& PluginMng::GetPartsWidthVec(){
    return partswidth_;
  }

  vector<uuplugin*>& PluginMng::GetPluginVec(){
    return plugins_;
  }

  bool PluginMng::IsPointOverIcon(int x){
    static const int SB_IMG_WIDTH=16;
    for (vector<int>::const_iterator i=partswidth_.begin();i!=partswidth_.end();++i){
      if (*i<x && x<*i+SB_IMG_WIDTH){
        return true;
      }
    }
    return false;
  }


  // will be called when creating status panes
  int PluginMng::GetEnabledPluginNum(){
    int j=0;
    for (int i=0;i<(int)plugins_.size();i++){
      if (plugins_.at(i)->enabled_){
        j++;
      }
    }
    return j;
  }

  int PluginMng::GetAllPluginNum(){
    return (int)plugins_.size();
  }

  int PluginMng::GetTotalWidth(){
    if (!partswidth_.empty()){
      return partswidth_.at(partswidth_.size()-1);
    }else{
      return 0;
    }
  }

  uuplugin* PluginMng::GetPluginByLocation(int xpos,int& paneid){
    if (xpos>2 && xpos<18){
      paneid = 1;
      return (uuplugin*)0x0001;
    }
    int pnum = (int)partswidth_.size();
    for(int i=0;i<pnum;++i){
      if(xpos>partswidth_.at(i)){
        if(i==pnum-1){
          if(i>=1){
            paneid = i+1;
            return GetEnabled(i-1);
          }else{
            return NULL;
          } 
        }else{
          if (xpos<partswidth_.at(i+1)){
            paneid = i+1;
            if(i>=1)
              return GetEnabled(i-1);
            else
              return NULL;
          }else{
            continue;
          }
        }
      }
    }
    return NULL;
  }

  void PluginMng::OnNavComp2(const wchar_t* url){
    int paneid_=1;
    int pnum = PluginMng::R().GetAllPluginNum();
    for (int i=0;i<pnum;i++){
      uuplugin* uup=PluginMng::R().GetPlugin(i);
      if (uup->enabled_){
        paneid_++;
        uup->OnNavigateComplete2(paneid_,url);
      }
    }
  }

  void PluginMng::OnDocComp(const wstring& url,const wstring& title,
    const wstring& iconurl)
  {
    int pnum = PluginMng::R().GetAllPluginNum();
    for (int i=0;i<pnum;i++){
      uuplugin* uup=PluginMng::R().GetPlugin(i);
      if (uup->enabled_){
        uup->OnDocumentComplete(url,title,iconurl);
      }
    }
  }

  void PluginMng::OnBeforeNav2(const wstring& url){
    int pnum = PluginMng::R().GetAllPluginNum();
    for (int i=0;i<pnum;i++){
      uuplugin* uup=PluginMng::R().GetPlugin(i);
      if (uup->enabled_){
        uup->OnBeforeNaviagte2(url);
      }
    }
  }

  void PluginMng::OnQuit(const wstring& url){
    int pnum = PluginMng::R().GetAllPluginNum();
    for (int i=0;i<pnum;i++){
      uuplugin* uup=PluginMng::R().GetPlugin(i);
      if (uup->enabled_){
        uup->OnQuit(url);
      }
    }
  }

  void PluginMng::StartUltraProtocol(const wchar_t* _url,
    std::string& dbcontent,
    std::wstring& fname,
    char*& flag,BYTE* pbuffer,ULONG buffersize)
  {
    int pnum = PluginMng::R().GetAllPluginNum();
    for (int i=0;i<pnum;i++){
      uuplugin* uup=PluginMng::R().GetPlugin(i);
      if (uup->enabled_){
        uup->StartUltraProtocol(_url,dbcontent,fname,flag,pbuffer,buffersize);
      }
    }
  }

  void PluginMng::APPURLFilter(const wchar_t* _url){
    int pnum = PluginMng::R().GetAllPluginNum();
    for (int i=0;i<pnum;i++){
      uuplugin* uup=PluginMng::R().GetPlugin(i);
      if (uup->enabled_){
        uup->APPURLFilter(_url);
      }
    }
  }

  //vector<uuplugin*>& PluginMng::GetPluginVec(){
  //  return plugins_;
  //}

  

  void PluginMng::fillpartswidth(){
    uuiterator vb=plugins_.begin(),ve=plugins_.end();
    partswidth_.clear();
    partswidth_.push_back(kPaneOffset);
    partswidth_.push_back(kPaneOffset+kSettingPaneLen);
    for (uuiterator i=vb;i!=ve;++i){
      uuplugin* p= *i;
      if (p->enabled_){
        if (i==vb){
          partswidth_.push_back(p->len_+kPaneOffset+kSettingPaneLen);
        }else{
          int previous_len = partswidth_.at(partswidth_.size()-1);
          partswidth_.push_back(p->len_+previous_len);
        }
      }
    }
  }

  uuplugin* PluginMng::GetPlugin(int i){
    return plugins_.at(i);
  }


  uuplugin* PluginMng::GetEnabled(int ordernum){
    int j=0;
    for (int i=0;i<(int)plugins_.size();i++){
      if (plugins_.at(i)->enabled_){
        if(j==ordernum)
          return plugins_.at(i);
        j++;
      }
    }
    return NULL;
  }

};
