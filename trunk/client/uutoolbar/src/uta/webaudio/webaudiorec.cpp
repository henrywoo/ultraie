#include "webaudiorec.h"
#include "audioparam.h"
#include <atlstr.h>
#include "../../dll/6beecommond/6beecommond.h"
#include "6bees_util.h"
#include "6bees_str.h"
#include <6bees_lang.h>
using namespace _6bees_str;

#include "webaudio.h"

const size_t MAXFNAME = 50;

namespace nsplugin{

#ifdef __CHINESE__
#define ktip_rec L"ÍøÒ³Â¼Òô»ú-Â¼ÖÆÍøÒ³ÉùÒô"
#define kcap_rec L"ÍøÒ³Â¼Òô»ú"
#else
#define ktip_rec L"Web Audio Recorder"
#define kcap_rec L"Web Audio Recorder"
#endif

  std::vector<char> webaudiorec::thunkdata_;
  cmutex webaudiorec::lock_;
  bool webaudiorec::initialized = false;
  webaudiorec* webaudiorec::pinstance = NULL;

  CMP3Creator webaudiorec::mp3creator_;

  UINT WINAPI thread_WriteSoundThunk(LPVOID p){
    ULONG step=0;
    while(audioparam::R().isrecording){
      ULONG tmp=webaudiorec::R().moveoutData();
      if (true){/// @todo -make changeable
        ULONG tmp2=tmp/(1024*50);
        if(tmp2!= step){
          step=tmp2;
          /*C6BeeMenuStatusBar* _p = (C6BeeMenuStatusBar*)p;
          _p->UpdateRecordingInfo(tmp);*/
          wchar_t szFileSize[30]={};
          StrFormatByteSize(tmp,szFileSize,30);

          C6BeeMenuStatusBar** pSB=nsplugin::GetPPSB();
          int SBnum=nsplugin::GetSBSize();
          int paneid = GetCurrentPaneID(static_cast<uuplugin*>(p));
          for (int i=0;i<SBnum;++i){
            if(*pSB!=NULL && ::IsWindow((*pSB)->m_hWnd)){
              (*pSB)->SetText(paneid,szFileSize);
            }
            pSB++;
          }
        }
      }
    }
    return 0;
  }

  bool webaudiorec::addData(char* pData,int len){
    cguard guard(lock_);
    std::copy(pData,pData+len,back_inserter(thunkdata_));
    return true;
  }

  unsigned long webaudiorec::moveoutData(){
    cguard guard(lock_);
    if(mp3creator_.m_breadytorecordmp3 && !thunkdata_.empty()){
      mp3creator_.WriteDatatoFile(&(thunkdata_.at(0)),(int)thunkdata_.size());
    }
    thunkdata_.clear();
    return mp3creator_.dwDone_mp3;
  }

  webaudiorec& webaudiorec::R(){
    if(!initialized){
      cguard g(lock_);
      if(pinstance == NULL){ 
        static webaudiorec p_;
        pinstance = &p_;
        if(pinstance){ //make sure compiler cannot reorder 
          initialized = true; // only set to true after creation 
        }
      } 
    } 
    return *pinstance;
  }

  webaudiorec::webaudiorec(){
    //init image list
    enabled_ = true;
    len_ = 80;
    caption_= kcap_rec;
    tip_ = ktip_rec;
    menuwID_ = 1319;
  }

  webaudiorec::~webaudiorec(){
    for (vector<HICON>::const_iterator i=icons_.begin();i!=icons_.end();++i){
      DestroyIcon(*i);
    }
  }

  bool webaudiorec::ProcessBasedInit(){
    return true;
  }

  bool webaudiorec::ProcessBasedTerm(){
    if (mp3creator_.m_breadytorecordmp3){
      mp3creator_.CloseMP3File();
    }
    thunkdata_.clear();
    return true;
  }

  bool webaudiorec::ThreadBasedInit(){
    return true;
  }

  bool webaudiorec::ThreadBasedTerm(){
    return true;
  }

  bool webaudiorec::init(){
    //::MessageBoxA(NULL,"GGPR init","GGPR",NULL);
    return true;
  }
  bool webaudiorec::StartPlugin(){
    return true;
  }
  bool webaudiorec::StopPlugin(){
    return true;
  }

  bool webaudiorec::OnLButtonDown(int paneid,int xpos,int ypos){
    if (audioparam::R().isrecording){
      active_icon_ = icons_.at(0);
      audioparam::R().isrecording = false;
      if(!audioparam::R().ismuting){
        webaudio::R().StopPlugin();
      }
      if (mp3creator_.m_breadytorecordmp3){
        mp3creator_.CloseMP3File();
      }
    }else{
      active_icon_ = icons_.at(1);
      audioparam::R().isrecording = true;

      // 1. creaye mp3 file
      if(!mp3creator_.m_breadytorecordmp3){
        C6BeeMenuStatusBar* pSB = GetC6BeeMenuStatusBarPtr();
#if 0
        wchar_t* _wurl = pSB->GetWebURL();
        ATL::CString filename(_wurl);
        delete [] _wurl;

        if(filename.Find(_T("http://"))!=0){return 0;}
        int cut=filename.Find(_T("/"),8);
        if (cut<0){return 0;}
        filename.Truncate(cut);
        filename=filename.Right(filename.GetLength()-7);
#endif

        wchar_t* _wtitle = pSB->GetWebTitle();
        if (_wtitle==NULL){
          _wtitle = pSB->GetWebURL();
        }
        wstring filename = GenFileName(_wtitle);
        delete [] _wtitle;
        if (filename.size()>MAXFNAME){
          filename = filename.substr(0,MAXFNAME);
        }
        using _6bees_util::ultrapath;
        const wstring& upath = ultrapath::R().get(ultrapath::audiodir);
        if(!_6bees_util::MakeSureDir(upath.c_str())){
          return false;
        }
        string mp3filename=stringmaker(CP_ACP)<< upath << filename << ".mp3";
        if(!mp3creator_.CreateMP3File(mp3filename.c_str())){
          return 0;
        }
      }
      // 2. hook
      webaudio::R().StartPlugin();

      // 3. record
      ::_beginthreadex(NULL,0,thread_WriteSoundThunk,(LPVOID)this,0,NULL);
    }

    C6BeeMenuStatusBar** pSB=nsplugin::GetPPSB();
    int SBnum=nsplugin::GetSBSize();
    for (int i=0;i<SBnum;++i){
      if ((*pSB)!=NULL && ::IsWindow((*pSB)->m_hWnd)){
        (*pSB)->SetIcon(paneid,active_icon_);
        (*pSB)->SetText(paneid,audioparam::R().isrecording ? kisrecording:L"");
      }
      pSB++;
    }
    return true;
  }
  bool webaudiorec::OnRButtonDown(int){
    return true;
  }


  bool webaudiorec::disenable_plugin(){
    if(enabled_){
      if(audioparam::R().isrecording){
        if(!audioparam::R().ismuting){
          StopPlugin();
        }
        active_icon_ = icons_.at(0);
        audioparam::R().isrecording = false;
      }
    }else{
      if(audioparam::R().isrecording){// modified
        StartPlugin();
        active_icon_ = icons_.at(1);
        audioparam::R().isrecording = true;
      }
    }
    return true;
  }


};

