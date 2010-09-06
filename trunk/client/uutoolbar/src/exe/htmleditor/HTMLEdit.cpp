#include "stdafx.h"
#include "HTMLEdit.h"
#include "MainFrm.h"
#include "HTMLEditDoc.h"
#include "HTMLEditView.h"

BEGIN_MESSAGE_MAP(CHTMLEditApp, CWinApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, OpenHTMLFile)
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

CHTMLEditApp::CHTMLEditApp(){}

CHTMLEditApp theApp;

extern MYMODE mode;
BOOL CHTMLEditApp::InitInstance(){
  CWinApp::InitInstance();

  if (!AfxOleInit()){
    AfxMessageBox(IDP_OLE_INIT_FAILED);
    return FALSE;
  }
  AfxEnableControlContainer();

  SetRegistryKey(_T("6BeeNetWroks Pte Ltd, Singapore 2008"));
  LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views
  CSingleDocTemplate* pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(CHTMLEditDoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(CHTMLEditView));
  AddDocTemplate(pDocTemplate);
  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);
  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;
  // The one and only window has been initialized, so show and update it
  if(mode==HIDE_WINDOW_TO_THUMB){
    m_pMainWnd->ShowWindow(SW_HIDE);
  }else{
    m_pMainWnd->ShowWindow(SW_SHOW);
  }
  m_pMainWnd->UpdateWindow();
  // call DragAcceptFiles only if there's a suffix
  //  In an SDI app, this should occur after ProcessShellCommand
  return TRUE;
}

void CHTMLEditApp::ParseCommandLine(CCommandLineInfo& rCmdInfo){
  for (int i = 1; i < __argc; i++)  {
    LPCTSTR pszParam = __targv[i];
    BOOL bFlag = FALSE;
    BOOL bLast = ((i + 1) == __argc);
    if (pszParam[0] == '-' || pszParam[0] == '/'){
      // remove flag specifier
      bFlag = TRUE;
      ++pszParam;
    }
    rCmdInfo.ParseParam(pszParam, bFlag, bLast);
    //if (i==2){
    //  this->NotifyWinName=pszParam;
    //}
  }
}

void CHTMLEditApp::OpenHTMLFile(){
  CWinApp::OnFileOpen();
}

class CAboutDlg : public CDialog{
public:
  CAboutDlg();
  enum { IDD = IDD_ABOUTBOX };
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD){}

void CAboutDlg::DoDataExchange(CDataExchange* pDX){
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

void CHTMLEditApp::OnAppAbout(){
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}