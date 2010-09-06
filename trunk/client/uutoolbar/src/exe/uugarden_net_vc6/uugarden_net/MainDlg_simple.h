#ifndef __CLIENTMNG_MAINDLG__
#define __CLIENTMNG_MAINDLG__

#include "resource.h"
#include <atlctrlx.h>
#include <atlhost.h>
#include <copydatamsg.h>
#include "../../dll/6beecommond/6beecommond.h"
#include "6bees_window.h"
#include "6bees_util.h"
#include "uulogging.h"
#include "6bees_const.h"
#include "6bees_net.h"
#include "6bees_str.h"
#include "6bees_gdi.h"
#include "resource.h"
#include "getaddress.h"

using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;
using namespace _6bees_gdi;
#include "../../dll/mousehook/mousehook.h"
#include <imgcap_setting.h>
#include <vector>
#include "../../dll/6beeclientmng_hook/6BeeClientMng_H.h"

#include "helper.h"
#include "atldispa.h"
using std::vector;
using namespace _6beed_share;
using namespace _6bees_const;
using namespace _6bees_util;

extern CAppModule _Module;
class CMainDlg;

class CMainDlg;

struct loginfo{
  CMainDlg* mdlg;
  std::string name;
  std::string passwd;
};
static loginfo info={};

UINT WINAPI ThreadLogin(LPVOID);

static const COLORREF BGCOLOR = RGB(237,255,200);
static const COLORREF TXTCOLOR = RGB(7,5,20);

enum etimer{ TIMER_LOGIN=1,TIMER_LOGOUT,TIMER_BUSYUPLOADING,TIMER_BUSYDOWNLOADING };

//todo
const wchar_t* const khome_url = L"http://www.uugarden.net";

const wchar_t* const tip_ntwkfail = L"错误: 账号密码错误或者网络连接失败.";
const wchar_t* const tip_logining = L"登陆中,请稍等...";
const wchar_t* const kregister=L"请注册一个新账号";
const wchar_t* const kwelcome = L"欢迎";
const wchar_t* const kyouarelogin = L"您已经成功连接上悠悠花园";
const wchar_t* const klogintip = L"请用您的注册账号和密码登陆悠悠花园";
const wchar_t* const klogin = L"登陆悠悠花园";



class CMainDlg :
  public CAxDialogImpl<CMainDlg>,
  public CUpdateUI<CMainDlg>,
  public CMessageFilter,
  public CIdleHandler
{
private:
  CBitmapButton m_CLogIn;
  CBitmapButton m_CRegister;
  CBitmapButton m_NoteImg;

  CImageList m_imglst;
  CImageList m_ImageList;

public:
  CWindow m_Note;


  enum { IDD = IDD_MAINDLG2 };
  BEGIN_UPDATE_UI_MAP(CMainDlg)
  END_UPDATE_UI_MAP()

  BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_DESTROY,    OnDestroy)
    MESSAGE_HANDLER(WM_TIMER,      OnTimer)

    COMMAND_HANDLER(IDC_LOGIN_,BN_CLICKED,   OnSignin)
    COMMAND_HANDLER(IDC_REGISTER_,BN_CLICKED,OnRegister)
    COMMAND_ID_HANDLER(IDCANCEL, OnEXIT)
  END_MSG_MAP()

  CMainDlg(){}

  virtual ~CMainDlg(){
    m_imglst.Destroy();
    m_ImageList.Destroy();
  }

  LRESULT OnEXIT(WORD,WORD wID,HWND,BOOL&){
    CloseDialog(wID);
    return 0;
  }

  LRESULT OnDestroy(UINT,WPARAM,LPARAM,BOOL&){
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);
    return 0;
  }

  void CloseDialog(int nVal){
    DestroyWindow();
    ::PostQuitMessage(nVal);
  }

  virtual BOOL PreTranslateMessage(MSG* pMsg){
    return CWindow::IsDialogMessage(pMsg);
  }

  virtual BOOL OnIdle(){
    return FALSE;
  }

  LRESULT OnInitDialog(UINT,WPARAM,LPARAM,BOOL&){
    HBITMAP hbmp=(HBITMAP)LoadImage(_Module.GetResourceInstance(),MAKEINTRESOURCE(ULTRA_IMGS),IMAGE_BITMAP,16*54,16,LR_DEFAULTSIZE|LR_SHARED);
    SetMyImageList(hbmp,m_ImageList,MASKGREY,16,16,54,GetSysColor(COLOR_3DFACE));

    HBITMAP hbmp2= (HBITMAP)LoadImage(_Module.GetResourceInstance(),MAKEINTRESOURCE(BUTTON_ClientMng),IMAGE_BITMAP,45*4,22,LR_DEFAULTSIZE|LR_SHARED);
    SetMyImageList(hbmp2,m_imglst,MASKPURPLE,45,22,4,GetSysColor(COLOR_3DFACE));

    //string uugarden_url = GetUUGardenURL();

    HICON hi =(HICON)::LoadImage(_Module.GetResourceInstance(),MAKEINTRESOURCE(IDI_ULTRAIE_CLIENTMG),
      IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    SetIcon(hi,FALSE);
    //::DestroyIcon(hi);

    m_Note.Attach(GetDlgItem(IDC_NOTE));


    m_CLogIn.SubclassWindow(GetDlgItem(IDC_LOGIN_));
    m_CLogIn.SetToolTipText(klogin);
    m_CLogIn.SetImageList(m_imglst);
    m_CLogIn.SetImages(1,0,0,0);
    m_CLogIn.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    m_CRegister.SubclassWindow(GetDlgItem(IDC_REGISTER_));
    m_CRegister.SetToolTipText(L"去网站");
    m_CRegister.SetImageList(m_imglst);
    m_CRegister.SetImages(3,2,2,2);
    m_CRegister.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

    //m_CRegister.ShowWindow(SW_HIDE);

    m_NoteImg.SubclassWindow(GetDlgItem(IDC_NOTEIMG));
    m_NoteImg.SetImageList(m_ImageList);
    m_NoteImg.SetImages(pinfo);

    CMessageLoop* pLoop = _Module.GetMessageLoop();
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);
    UIAddChildWindowContainer(m_hWnd);

    m_Note.SetWindowText(klogintip);
    return TRUE;
  }

  void UpdateNoteImg(bool loggedin=true){
    m_NoteImg.SetImages(loggedin?pinfo:pbad);
    m_NoteImg.Invalidate();
    m_NoteImg.UpdateWindow();
  }

  /// kill or set a timer
  /// @param etimer timer channel
  /// @param kill true - kill a timer; false - set a timer
  void UpdateTimer(etimer t,bool kill=true){
    static const int kduration = 500;
    if (kill){
      KillTimer(t);
      Sleep(kduration+100);
      if (t==TIMER_LOGIN){
        /// log in can be failed
        UpdateNoteImg(true);
      }else if(t==TIMER_LOGOUT){
        UpdateNoteImg(false);
      }else{
      }
    }else{
      SetTimer(t,kduration);
    }
  }

  LRESULT OnTimer(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    const static int maxiconsindex= 1;
    static bool b = false;
    if (wParam==TIMER_LOGIN){
      b = !b;
      /// image button flash
      m_NoteImg.SetImages(b?pglobe:pglobelink);
      m_NoteImg.Invalidate();
    }
    return 0;
  }

  LRESULT OnMy6BeeSpace(WORD,WORD,HWND,BOOL&){
    _6beed_util::runIE(khome_url);
    return 0;
  }

  LRESULT OnSignin(WORD,WORD wID,HWND,BOOL&){
    m_Note.SetWindowText(tip_logining);
    SetTimer(TIMER_LOGIN,500);

    info.mdlg = this;
    ::_beginthreadex(NULL,0,ThreadLogin,(LPVOID)&info,0,NULL);
    return 0;
  }

  LRESULT OnRegister(WORD,WORD,HWND,BOOL&){
    _6beed_util::runIE(_6bees_const::kUltraIEURL_REGISTER);
    return 0;
  }

  void PostLogin(std::string& username,std::string& passwordtostore){
    //ShowWindow(SW_HIDE);
    wstring info = wstringmaker() << kwelcome << L" " << username << "," << kyouarelogin;
    m_Note.SetWindowText(info.c_str());

    m_CRegister.ShowWindow(SW_SHOW);
  }
};

void ErrorFunc(CMainDlg* pMainDlg,const wchar_t* errormsg){
  pMainDlg->UpdateNoteImg(false);
  pMainDlg->m_Note.SetWindowText(errormsg);
  UUDEBUG((LOG_ERROR,errormsg));
}

UINT WINAPI ThreadLogin(LPVOID lp){
  try{
    loginfo* li = (loginfo*) lp;
    CMainDlg* pMainDlg = li->mdlg;
    if (pMainDlg==NULL){
      UUDEBUG((LOG_ERROR,"pMainDlg is NULL in ThreadLogin"));
      return 0;
    }
    std::string uuname = li->name;
    std::string ppword = li->passwd;

    wstring loginurl = wstringmaker() << "http://192.168.0.152/bbs/uuassistant.php?u=" << uuname << "&p="<< ppword;
    int clen=0;
    char * pdata = _6bees_net::GetDataByHTTP(loginurl.c_str(),clen);
    if (clen==1 && pdata[0]=='1'){
      //member
      pMainDlg->PostLogin(uuname,ppword);
    }else if (clen==1 && pdata[0]=='0'){
      //not member
      ErrorFunc(pMainDlg,tip_ntwkfail);
    }else{
      //error
      ErrorFunc(pMainDlg,tip_ntwkfail);
    }
    delete [] pdata;
    pMainDlg->UpdateTimer(TIMER_LOGIN,true);
  }catch (...){}
  return 0;
}

#endif
