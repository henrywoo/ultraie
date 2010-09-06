#ifndef __CLIENTMNG_MAINDLG__
#define __CLIENTMNG_MAINDLG__

#include "resource.h"
#include <atlctrlx.h>
#include <atlhost.h>

#include "getaddress.h"
#include <comutil.h>

//#define __TEST__

bool SetMyImageList(HBITMAP hbmp,CImageList& m_ImageList,COLORREF maskcolor,int ix, int iy, int imgnum,COLORREF bgcolor){
  if (m_ImageList.IsNull()){
    if (!m_ImageList.Create(ix,iy,ILC_COLOR16|ILC_MASK,imgnum,1)){
      return false;
    }
    m_ImageList.Add(hbmp,maskcolor);
    ::DeleteObject(hbmp);
    if(bgcolor)
      m_ImageList.SetBkColor(bgcolor);
  }
  return true;
}

extern CAppModule _Module;

class CMainDlg :
  public CAxDialogImpl<CMainDlg>,
  public CUpdateUI<CMainDlg>,
  public CMessageFilter,
  public CIdleHandler
{
private:
  CComPtr<IWebBrowser2> p;
public:
  CWindow _inittxt;
  CAxWindow wndIE;
  CBitmapButton m_CLogIn;

  CImageList m_imglst;
#ifdef __TEST__
  enum { IDD = IDD_MAINDLG3 };
#else
  enum { IDD = IDD_MAINDLG2 };
#endif
  BEGIN_UPDATE_UI_MAP(CMainDlg)
  END_UPDATE_UI_MAP()

  BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_LOGIN_,BN_CLICKED,   OnSignin)
    MESSAGE_HANDLER(WM_DESTROY,    OnDestroy)
    COMMAND_ID_HANDLER(IDCANCEL, OnEXIT)
  END_MSG_MAP()

  LRESULT OnSignin(WORD,WORD wID,HWND,BOOL&){
    _inittxt.SetWindowText(L"正在连接,请稍等...");
    wndIE.QueryControl(&p);
    if (p){
      p->put_Silent(VARIANT_TRUE);
      p->put_RegisterAsBrowser(VARIANT_FALSE);
      p->put_RegisterAsDropTarget(VARIANT_FALSE);
      p->put_AddressBar(VARIANT_FALSE);
      p->put_Offline(VARIANT_FALSE);

      CComPtr<IAxWinAmbientDispatch> spHost(0);
      if( SUCCEEDED(wndIE.QueryHost(IID_IAxWinAmbientDispatch, (LPVOID*) &spHost)) ) {
        spHost->put_AllowContextMenu(VARIANT_FALSE);
        spHost->put_DocHostFlags(//DOCHOSTUIFLAG_DIALOG |
          DOCHOSTUIFLAG_NO3DBORDER|
          DOCHOSTUIFLAG_DISABLE_HELP_MENU|
          DOCHOSTUIFLAG_SCROLL_NO);
      }
      CComVariant v;
      string url=GetUUGardenURL();
      if (url.empty()){
        ::MessageBox(NULL,L"网站暂时维护中,请明日再试.",L"网站维护",MB_OK|MB_ICONINFORMATION);
        DestroyWindow();
        ::PostQuitMessage(1);
        return 0;
      }
#ifndef __TEST__
      std::string base64url = base64_encode(reinterpret_cast<const unsigned char*>(url.c_str()), (unsigned int)url.size());
      url += "/win32client.php?u="+base64url;
#else
      url += "/test.html";
#endif
      p->Navigate(CComBSTR(url.c_str()),&v,&v,&v,&v);
      wndIE.ShowWindow(SW_SHOW);
      m_CLogIn.ShowWindow(SW_HIDE);
      _inittxt.ShowWindow(SW_HIDE);
    }
    return 0;
  }

  CMainDlg(){}

  virtual ~CMainDlg(){}

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
    HBITMAP hbmp2= (HBITMAP)LoadImage(_Module.GetResourceInstance(),MAKEINTRESOURCE(BUTTON_ClientMng),IMAGE_BITMAP,45*2,22,LR_DEFAULTSIZE|LR_SHARED);
    SetMyImageList(hbmp2,m_imglst,RGB(255,0,255),45,22,2,GetSysColor(COLOR_3DFACE));

    HICON hi =(HICON)::LoadImage(_Module.GetResourceInstance(),MAKEINTRESOURCE(IDI_ULTRAIE_CLIENTMG),
      IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    SetIcon(hi,FALSE);

    _inittxt.Attach(GetDlgItem(IDC_INITTXT));

    m_CLogIn.SubclassWindow(GetDlgItem(IDC_LOGIN_));
#ifndef __TEST__
    m_CLogIn.SetToolTipText(L"点击按钮连接网站.");
#endif
    m_CLogIn.SetImageList(m_imglst);
    m_CLogIn.SetImages(1,0,0,0);
    m_CLogIn.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);
    

    wndIE = GetDlgItem(IDC_IE);
    wndIE.ShowWindow(SW_HIDE);

    CMessageLoop* pLoop = _Module.GetMessageLoop();
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);
    UIAddChildWindowContainer(m_hWnd);

    return TRUE;
  }
};



#endif
