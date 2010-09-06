#include "reflectionwnd.h"
#include <6bees_util.h>

#include <copydatamsg.h>
#include "ToolBarCOM.h"
#include "globalvar.h"
#include "pluginmng.h"
#include "../../dll/6beeres/resource.h"
#include "../../dll/6beecommond/6beecommond.h"

#pragma comment(lib,"6beecommond.lib")

using namespace uutoolbar;

const int ktbbuttonxppadding_x = 6;
const int ktbbuttonxppadding_y = 0;

WNDPROC gs_procPrevWnd = NULL;
WNDPROC gs_procPrevWnd2 = NULL;

int status_height=0;
int offset_var=0;

BOOL check_zoom_animate_number = FALSE;

extern BOOL haszoom;
extern BOOL hasanimate;

const int STATWNDCONFIG_WIDTH = 60;

hash_map<DWORD,LONG_PTR> hwnd_to_wndproc;
hash_map<DWORD,LONG_PTR> hwnd_to_wndproc2;
hash_map<DWORD,BOOL>     tid_to_Progress;
hash_map<DWORD,IEFrameHook*> tid_to_pINoteBookFrame3;
hash_map<DWORD,CToolBarWnd*> PT_to_6BeeTBWnd;

//////////////////////////////////////////////////////////////////////////
//  Hook IE Window Message

LRESULT CALLBACK IEWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  DWORD tid = GetCurrentThreadId();
  if(hwnd_to_wndproc.find(tid) == hwnd_to_wndproc.end()){
    return 0;
  }
  LONG_PTR proc = hwnd_to_wndproc[tid];
  if(!proc){
    return 0;
  }
  if(tid_to_pINoteBookFrame3.find(tid) == tid_to_pINoteBookFrame3.end()){
    return 0;
  }
  IEFrameHook* gs_pNoteBookFrame = tid_to_pINoteBookFrame3[tid];
  if(gs_pNoteBookFrame){
    switch(uMsg){
      case WM_SIZE:
        // very important. Sometimes gs_pNoteBookFrame will be invalid. the program will crash. At this time lParam is 0.
        if (lParam!=0){
          gs_pNoteBookFrame->IEFrameSizeChanging(HIWORD(lParam));
        }
        break;
      case WM_THEMECHANGED:
        gs_pNoteBookFrame->Setoffset_var();
        break;
      default:
        break;
    } 
  }
  return CallWindowProc((WNDPROC)proc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK IEStatusWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  //ATL::CString tid;
  //tid.Format(_T("%d-%d"),GetCurrentProcessId(),GetCurrentThreadId());
  DWORD tid = GetCurrentThreadId();
  if(hwnd_to_wndproc2.find(tid) == hwnd_to_wndproc2.end()){
    return 0;
  }
  LONG_PTR proc = hwnd_to_wndproc2[tid];
  if(!proc){
    return 0;
  }
  IEFrameHook* gs_pNoteBookFrame = tid_to_pINoteBookFrame3[tid];
  if(gs_pNoteBookFrame){
    switch(uMsg){
      case WM_SHOWWINDOW:
        gs_pNoteBookFrame->ShowHideStatusBar(wParam==1);
        break;
      default:
        break;
    }
  }
  return CallWindowProc((WNDPROC)proc, hWnd, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////
ReflectionWnd::ReflectionWnd(void):
RawIEtabWnd(NULL),
themechange_count_v6(0)
{
  tid_ = GetCurrentThreadId();
  IEFrameHook* tmp = this;
  tid_to_pINoteBookFrame3.insert(std::make_pair(tid_,tmp));

  RawIEtabWnd=NULL;
  RawStatusWnd=NULL;
  RECT RawStatusWndRect={0};
  RECT tabwndrect={0};
}

ReflectionWnd::~ReflectionWnd(void){
  if (IsWindow()){
    if(RawIEtabWnd && (hwnd_to_wndproc.find(tid_)!=hwnd_to_wndproc.end())){
      ::SetWindowLongPtr(RawIEtabWnd, GWL_WNDPROC, hwnd_to_wndproc[tid_]);
      hwnd_to_wndproc.erase(tid_);
    }
    if(RawStatusWnd && (hwnd_to_wndproc2.find(tid_)!=hwnd_to_wndproc2.end())){
      ::SetWindowLongPtr( RawStatusWnd, GWL_WNDPROC, hwnd_to_wndproc2[tid_]);
      hwnd_to_wndproc2.erase(tid_);
    }
    if(tid_to_Progress.find(tid_)!=tid_to_Progress.end()){
      tid_to_Progress.erase(tid_);
    }
    if(tid_to_pINoteBookFrame3.find(tid_)!=tid_to_pINoteBookFrame3.end()){
      tid_to_pINoteBookFrame3.erase(tid_);
    }

    GlobalVar::R().GetTidStatusWndMap().erase(GetCurrentThreadId());
    DestroyWindow();
  }
}

LRESULT ReflectionWnd::OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  //use WS_CLIPCHILDREN to avoid flicking!
  static const DWORD DEFAULT_TOOLBAR_STYLE = WS_CLIPCHILDREN | WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_TABSTOP |
    TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST | TBSTYLE_AUTOSIZE | // For TB_SETPADDING
    TBSTYLE_CUSTOMERASE | CCS_TOP | CCS_NODIVIDER | CCS_NOPARENTALIGN | CCS_NORESIZE;
  bool b = _6beed_util::IsXPTheme();
  status_height=b?22:20;
  offset_var=b?3:10;
  offset=GlobalVar::R().kstatusbar_offset+nsplugin::PluginMng::R().GetTotalWidth()-offset_var;//+100
//#ifdef HASTOOLBARWND
  RECT rect;
  ::GetClientRect(m_hWnd,&rect);
  m_CToolBarWnd.Create(m_hWnd,rect,NULL,DEFAULT_TOOLBAR_STYLE);
  PT_to_6BeeTBWnd.insert(std::make_pair(tid_,&m_CToolBarWnd));
//#endif
  return 0;
}

LRESULT ReflectionWnd::OnCopyData(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  PCOPYDATASTRUCT pMyCDS = (PCOPYDATASTRUCT) lParam;
  ULONG_PTR a= pMyCDS->dwData;
  for(hash_map<DWORD,CToolBarWnd*>::iterator i = PT_to_6BeeTBWnd.begin();i!=PT_to_6BeeTBWnd.end();i++){
    CToolBarWnd* j = i->second;
    switch(pMyCDS->dwData){
      case CPYDATA_LOGIN:
        j->RefreshTButtons(true);
        _6beed_share::u_setLoggedin(true);
        break;
      case CPYDATA_LOGOUT:
        j->RefreshTButtons(false);
        _6beed_share::u_setLoggedin(false);
        break;
      default:
        break;
    }
  }
  bHandled = true;
  return 0;
}

void    ReflectionWnd::ProgressChange(long progress,long progressMax){
  ::GetClientRect(RawIEtabWnd,&tabwndrect);

  if(tid_to_Progress.find(tid_) == tid_to_Progress.end()){
    tid_to_Progress.insert(std::make_pair(tid_,(progress==-1)?TRUE:FALSE));
  }else{
    tid_to_Progress[tid_] = (progress==-1)?TRUE:FALSE;
  }
  if(!haszoom){
    if(::FindWindowEx(RawStatusWnd,NULL,_T("ToolbarWindow32"),_T("Zoom Level"))==NULL){
      haszoom = FALSE;//If there is no Zoom Level window.  //::MessageBox(NULL, _T("cannot get Zoom Level!"), _T("OK"), MB_OK | MB_ICONINFORMATION);
    }else{haszoom=TRUE;}
    offset=GlobalVar::R().kstatusbar_offset+nsplugin::PluginMng::R().GetTotalWidth()-offset_var-(haszoom==FALSE?100:0)-(hasanimate==FALSE?42:0);
  }
  if(!hasanimate){
    if(::FindWindowEx(RawStatusWnd,NULL,_T("SysAnimate32"),NULL)==NULL){
      hasanimate = FALSE;//If there is no Animate window.
    }else{hasanimate=TRUE;}
    offset=GlobalVar::R().kstatusbar_offset+nsplugin::PluginMng::R().GetTotalWidth()-offset_var-(haszoom==FALSE?100:0)-(hasanimate==FALSE?42:0);
  }

  C6BeeStatusWnd* _m_6BeeStatusWnd = GlobalVar::R().GetCurrentStatusWnd();
  if(_m_6BeeStatusWnd){
    if(progress >= 0 && progress!=progressMax){    
      if(!::IsWindowVisible(_m_6BeeStatusWnd->m_Progress.m_hWnd)){
        _m_6BeeStatusWnd->m_Progress.ShowWindow(SW_SHOW);
      }
      if(progressMax>0)
        _m_6BeeStatusWnd->m_Progress.SetPos(progress*100/progressMax);
    }else{
      _m_6BeeStatusWnd->m_Progress.SetPos(100);
      _m_6BeeStatusWnd->m_Progress.ShowWindow(SW_HIDE);
      tid_to_Progress[tid_] = TRUE;
    }
  }
}

void ReflectionWnd::SetProgress(BOOL a){
  if(tid_to_Progress.find(tid_) == tid_to_Progress.end()){
    tid_to_Progress.insert(std::make_pair(tid_,a));
  }else{
    tid_to_Progress[tid_] = a;
  }
}

void ReflectionWnd::IEFrameSizeChanging(int cy){
  C6BeeStatusWnd* _m_6BeeStatusWnd = GlobalVar::R().GetCurrentStatusWnd();
  if(_m_6BeeStatusWnd){
    RECT TBRect={};
    ::GetWindowRect(::GetParent(_m_6BeeStatusWnd->m_hWnd),&TBRect);
    int cx = TBRect.right-TBRect.left;
    int sb_width = nsplugin::PluginMng::R().GetTotalWidth();
    switch(_6beed_share::u_getieversion()){
      case 7:
      case 8:
      /// @todo check haszoom and hasanimate
        offset=GlobalVar::R().kstatusbar_offset+sb_width-offset_var-(haszoom?0:100)-(hasanimate?0:42);// Update offset
        ::SetWindowPos(_m_6BeeStatusWnd->m_hWnd,HWND_TOP,cx-offset,cy-status_height,sb_width,status_height,SWP_FRAMECHANGED);
        break;
      case 6:
        offset=GlobalVar::R().kstatusbar_offset+sb_width-offset_var;// Update offset
        ::SetWindowPos(_m_6BeeStatusWnd->m_hWnd,HWND_TOP,cx-offset,cy-status_height,sb_width,status_height,SWP_FRAMECHANGED);
        ::SetWindowPos(_m_6BeeStatusWnd->m_StatusBar.m_hWnd,HWND_TOP,-1,0,sb_width+1,status_height,SWP_FRAMECHANGED);
        break;
    }
  }
}

void ReflectionWnd::ShowHideStatusBar(bool a){
  C6BeeStatusWnd* _m_6BeeStatusWnd = GlobalVar::R().GetCurrentStatusWnd();
  if(_m_6BeeStatusWnd){
    _m_6BeeStatusWnd->ShowWindow(a ? SW_SHOW : SW_HIDE);
  }
}

// Under WindowXP, IE6's WM_THEMECHANGED will appear two times continuously, but IE7 just one time
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ReflectionWnd::Setoffset_var(){
  bool b = _6beed_util::IsXPTheme();
  CToolBarWnd& tbwnd = GetToolBar();
  switch (_6beed_share::u_getieversion()){
  case 7:
  case 8:
    {
      status_height = b ? 22 : 20;// a temperory workaround
      offset_var    = b ? 3  : 10;
      offset=GlobalVar::R().kstatusbar_offset+nsplugin::PluginMng::R().GetTotalWidth()-offset_var-(haszoom?0:100);
      if(b)
        ::SendMessage(GetToolBar().m_hWnd, TB_SETPADDING, 0, MAKELONG(ktbbuttonxppadding_x,ktbbuttonxppadding_y));
      else
        ::SendMessage(GetToolBar().m_hWnd, TB_SETPADDING, 0, MAKELONG(TBBUTTON_CLASSIC_PADDING_X,TBBUTTON_CLASSIC_PADDING_Y));
    }break;
  case 6:
    {
      status_height = b ? 22 : 20;// a temperory workaround!
      offset_var    = b ? 8  : 10;
      offset=GlobalVar::R().kstatusbar_offset+nsplugin::PluginMng::R().GetTotalWidth()-offset_var;
      if(b)
        ::SendMessage(GetToolBar().m_hWnd, TB_SETPADDING, 0, MAKELONG(ktbbuttonxppadding_x,ktbbuttonxppadding_y));
      else
        ::SendMessage(GetToolBar().m_hWnd, TB_SETPADDING, 0, MAKELONG(TBBUTTON_CLASSIC_PADDING_X,TBBUTTON_CLASSIC_PADDING_Y));
    }break;
  }
}

//////////////////////////////////////////////////////////////////////////
VOID ReflectionWnd::CreateMinuStatusWindow(){
  switch(_6beed_share::u_getieversion()){
  case 6:
    CreateMinuStatusWindow_V6();
    break;
  case 7:
  case 8:
    CreateMinuStatusWindow_V7();
    break;
  default:
    break;
  }
}


HRESULT ReflectionWnd::CreateMinuStatusWindow_V6(){
  if(CToolBarCOM::iswebie){
    HWND p1 = ::GetParent(this->m_hWnd);
    if(p1==NULL) return S_OK;
    HWND p2 = ::GetParent(p1);
    if(p2==NULL) return S_OK;
    HWND tt = ::GetParent(p2);

    if(tt!=NULL){
      RawIEtabWnd  = tt;
      if(::FindWindowEx(RawIEtabWnd,NULL,NULL,_T("STATUSBAR_1"))!=NULL){
      return S_OK;
    }
      ::GetClientRect(RawIEtabWnd,&tabwndrect);

      RawStatusWnd  = ::FindWindowEx(RawIEtabWnd,NULL,_T("msctls_statusbar32"),NULL);
      if(RawStatusWnd==NULL){
      return S_OK;
    }
      ::GetClientRect(RawStatusWnd,&RawStatusWndRect);
      status_height = RawStatusWndRect.bottom - RawStatusWndRect.top;
      offset_var = (status_height==20) ? 10 : 8;


      offset=GlobalVar::R().kstatusbar_offset+nsplugin::PluginMng::R().GetTotalWidth()-offset_var;
      m_6BeeStatusWnd.SBRect.bottom = status_height;
      //m_6BeeStatusConfigWnd.SBRect.bottom = status_height;

    }else{
      /*::MessageBox(NULL, _T("cannot get the current active window!"), _T("OK"), MB_OK | MB_ICONINFORMATION);*/
      return S_OK;
    }

    int tabheight = tabwndrect.bottom - tabwndrect.top;
    int tabwidth  = tabwndrect.right  - tabwndrect.left;
    RECT SBRect={tabwidth-offset,
    tabheight-status_height,
    tabwidth-offset+nsplugin::PluginMng::R().GetTotalWidth(),
    tabheight};
    m_6BeeStatusWnd.Create(RawIEtabWnd,SBRect,_T("STATUSBAR_1"),WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_EX_TOPMOST);

    //SBRect.left  = tabwidth-RIGHT_OFFSET;
    //SBRect.right = SBRect.left + STATWNDCONFIG_WIDTH;//167
    //m_6BeeStatusConfigWnd.Create(RawIEtabWnd,SBRect,_T("STATUSBAR_2"),WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_EX_TOPMOST);

    SBRect.left  = 100;
    SBRect.right = SBRect.left + STATWNDCONFIG_WIDTH;//167
    TidStatusWndMap& tsmap = GlobalVar::R().GetTidStatusWndMap();
    tsmap[GetCurrentThreadId()] = &m_6BeeStatusWnd;
    //PT_to_StatusConfigWnd.insert(std::make_pair(tid,&m_6BeeStatusConfigWnd));

    if(RawIEtabWnd!=NULL){
      gs_procPrevWnd = (WNDPROC)::SetWindowLongPtr( RawIEtabWnd, GWL_WNDPROC, (LONG_PTR)IEWindowProc);
      hwnd_to_wndproc.insert(std::make_pair(tid_,(LONG_PTR)gs_procPrevWnd));
    }
    if(RawStatusWnd!=NULL){
      gs_procPrevWnd2 = (WNDPROC)::SetWindowLongPtr( RawStatusWnd, GWL_WNDPROC, (LONG_PTR)IEStatusWindowProc);
      hwnd_to_wndproc2.insert(std::make_pair(tid_,(LONG_PTR)gs_procPrevWnd2));
    }
    ::SetWindowPos(m_6BeeStatusWnd.m_hWnd,HWND_TOP,tabwndrect.right-offset,
      tabwndrect.bottom-status_height,nsplugin::PluginMng::R().GetTotalWidth(),status_height,SWP_FRAMECHANGED);
  }
  return S_OK;
}

HRESULT ReflectionWnd::CreateMinuStatusWindow_V7(){
  if(CToolBarCOM::iswebie){
    int tmpwidth = nsplugin::PluginMng::R().GetTotalWidth();
    HWND p1 = ::GetParent(this->m_hWnd);
    HWND p2 = ::GetParent(p1);
    HWND p3 = ::GetParent(p2);
    HWND tt = ::GetParent(p3);
    if (!::IsWindow(tt)){
      return S_OK;
    }
    switch(_6beed_share::u_getieversion()){
      case 8:
        RawIEtabWnd = ::GetNextWindow(p3,GW_HWNDNEXT);
        break;
      case 7:
        RawIEtabWnd = ::GetNextWindow(p3,GW_HWNDPREV);
        break;
    }
    if (!::IsWindow(RawIEtabWnd)){
      return S_OK;
    }
    HWND _h = ::FindWindowExA(RawIEtabWnd,NULL,NULL,"STATUSBAR_1");
    if(::IsWindow(_h)){
      return S_OK;
    }
    ::GetClientRect(RawIEtabWnd,&tabwndrect);
    HWND _astatus  = ::FindWindowEx(RawIEtabWnd,NULL,_T("msctls_statusbar32"),NULL);
    if(!::IsWindow(_astatus)){
      return S_OK;
    }
    RawStatusWnd = _astatus;
    ::GetClientRect(RawStatusWnd,&RawStatusWndRect);
    status_height = RawStatusWndRect.bottom - RawStatusWndRect.top;
    offset_var = (status_height==20)?10:3;
    offset=GlobalVar::R().kstatusbar_offset+tmpwidth-offset_var-(haszoom?0:100)-(hasanimate?0:42);

    m_6BeeStatusWnd.SBRect.bottom = status_height;
    //m_6BeeStatusConfigWnd.SBRect.bottom = status_height;

    if(tid_to_Progress.find(tid_) != tid_to_Progress.end() && tid_to_Progress[tid_]){
      offset=GlobalVar::R().kstatusbar_offset+tmpwidth-offset_var-(haszoom?0:100)-(hasanimate?0:42);
    }

    int tabheight = tabwndrect.bottom - tabwndrect.top;
    int tabwidth  = tabwndrect.right  - tabwndrect.left;
    //RECT SBRect={tabwidth-offset,tabheight-status_height,tabwidth-offset+nsplugin::PluginMng::R().GetTotalWidth(),tabheight};
    RECT SBRect={tabwidth-offset,
      tabheight-status_height,
      tabwidth-offset+tmpwidth,
      tabheight};
    m_6BeeStatusWnd.Create(RawIEtabWnd, SBRect, _T("STATUSBAR_1"),
      WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |WS_EX_TOPMOST);

    SBRect.left  = 100;
    SBRect.right = SBRect.left + STATWNDCONFIG_WIDTH;

    TidStatusWndMap& tsmap = GlobalVar::R().GetTidStatusWndMap();
    tsmap[GetCurrentThreadId()] = &m_6BeeStatusWnd;

    if(RawIEtabWnd){
      gs_procPrevWnd = (WNDPROC)::SetWindowLongPtr( RawIEtabWnd, GWL_WNDPROC,
        (LONG_PTR)IEWindowProc);// Hook IE Frame window message
      hwnd_to_wndproc.insert(std::make_pair(tid_,(LONG_PTR)gs_procPrevWnd));
    }
    if(RawStatusWnd){
      gs_procPrevWnd2 = (WNDPROC)::SetWindowLongPtr( RawStatusWnd, GWL_WNDPROC,
        (LONG_PTR)IEStatusWindowProc);// Hook Status Bar Message
      hwnd_to_wndproc2.insert(std::make_pair(tid_,(LONG_PTR)gs_procPrevWnd2));
    }
    ::SetWindowPos(m_6BeeStatusWnd.m_hWnd,HWND_TOP,tabwndrect.right-offset,
      //tabwndrect.bottom-status_height,nsplugin::PluginMng::R().GetTotalWidth(),status_height,SWP_FRAMECHANGED);
      tabwndrect.bottom-status_height,tmpwidth,status_height,SWP_FRAMECHANGED);
  }
  return S_OK;
}

/*
void ReflectionWnd::check_zoom_animate(){
  if(CToolBarCOM::iswebie && u_getieversion()==7 && !check_zoom_animate_number){
    check_zoom_animate_number = TRUE;
    HWND p1 = ::GetParent(this->m_hWnd);
    if(p1==NULL) return;
    HWND p2 = ::GetParent(p1);
    if(p2==NULL) return;
    HWND p3 = ::GetParent(p2);
    RawIEtabWnd = ::GetNextWindow(p3,GW_HWNDPREV);
    if(RawIEtabWnd!=NULL){
      ::GetClientRect(RawIEtabWnd,&tabwndrect);
      HWND _astatus  = ::FindWindowEx(RawIEtabWnd,NULL,_T("msctls_statusbar32"),NULL);
      if(_astatus==NULL){return;}
      RawStatusWnd = _astatus;
      if(::FindWindowEx(RawStatusWnd,NULL,_T("ToolbarWindow32"),_T("Zoom Level"))==NULL){
        haszoom = FALSE;//If there is no Zoom Level window.
      }else{
        haszoom=TRUE;
      }
      if(::FindWindowEx(RawStatusWnd,NULL,_T("SysAnimate32"),NULL)==NULL){
        hasanimate = FALSE;//If there is no Animate window.
      }else{
        hasanimate=TRUE;
      }
      ATL::CStringA cmd;
      cmd.Format("update UUConfig set haszoom=%d,hasanimate=%d",haszoom?1:0,hasanimate?1:0);
      _6beed_util::CConfig::R().updatedb_direct(cmd.GetString());
    }
  }
}
*/
