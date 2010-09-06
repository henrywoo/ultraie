#include "ToolBarCombo.h"
#include "ToolBarCOM.h"
#include "RegIterator.h"
#include "6bees_const.h"
#include "6bees_util.h"
#include <string>
#include "globalvar.h"

extern HFONT EDITTXTFONT;

CToolBarCombo::CToolBarCombo():p_CToolBarCOM(NULL),gotfocus(false),m_cRef(0),m_bAuto(true){}

CToolBarCombo::~CToolBarCombo(){
  if (IsWindow()){
    DestroyWindow();
  }
}

void CToolBarCombo::SetBand(CToolBarCOM* p_Band){
  p_CToolBarCOM=p_Band;
}

LRESULT CToolBarCombo::OnSetFocus(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
  gotfocus=true;
  if (p_CToolBarCOM){
    p_CToolBarCOM->FocusChange(true);
  }
  return 0;
}

LRESULT CToolBarCombo::OnLostFocus(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
  gotfocus=false;
  if (p_CToolBarCOM){
    if(gotfocus)
      p_CToolBarCOM->FocusChange(false);
  }
  return 0;
}

STDMETHODIMP CToolBarCombo::TranslateAcceleratorIO(LPMSG lpMsg){
  if (!gotfocus) return S_FALSE;
  int nVirtKey = (int)(lpMsg->wParam);
  if(lpMsg->message==WM_KEYUP && nVirtKey == VK_RETURN ){
    lpMsg->wParam = 0;
    ::PostMessage(GetParent(),kWM_TBCOMBOSEARCH_WEB,lpMsg->wParam,lpMsg->lParam);
    return S_OK;
  }else if ((WM_KEYUP == lpMsg->message || WM_KEYDOWN == lpMsg->message) &&
    VK_BACK == nVirtKey || (VK_END <= nVirtKey && VK_DOWN >= nVirtKey) ||
    VK_DELETE == nVirtKey ||((GetKeyState(VK_CONTROL)&0x80) && (nVirtKey=='C' || nVirtKey=='V' || nVirtKey=='X'))){
      m_bAuto = false;
      if (!TranslateMessage(lpMsg)) return S_FALSE;
      DispatchMessage(lpMsg);
      m_bAuto = true;
      if(WM_KEYDOWN == lpMsg->message && VK_BACK == nVirtKey)
        m_bAuto = false;
      return S_OK;
  }else{
    return S_FALSE;
  }
}

LRESULT CToolBarCombo::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  ///////////////////////////////////////////////////////////////////
  // draw combox skin
  ///////////////////////////////////////////////////////////////////
  RECT rectcombo;
  this->GetClientRect(&rectcombo);

  CPaintDC dc(m_hWnd);
  CDC dcMem;
  dcMem.CreateCompatibleDC(dc);
  dc.SaveDC();
  dcMem.SaveDC();

  //left
  //dcMem.SelectBitmap(bmp_left);
  //dc.BitBlt(0,1,2,21,dcMem,0,0,SRCCOPY);
  //dcMem.RestoreDC(-1);

  //center
  HBITMAP h = uutoolbar::bmpsgton::R().h_;
  dcMem.SelectBitmap(h);
  //dc.StretchBlt(2,1,rectcombo.right-2-20,21,dcMem,0,0,1,21,SRCCOPY);
  BITMAP bm;
  ::GetObject(h, sizeof(BITMAP), &bm);
  dc.StretchBlt(0,0,
    rectcombo.right-20,
    _6bees_const::ktbbuttonheight,
    dcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
  dcMem.RestoreDC(-1);

  //right
  dcMem.SelectBitmap(uutoolbar::bmpsgton::R().h2_);
  dc.BitBlt(rectcombo.right-20,0,20,_6bees_const::ktbbuttonheight,dcMem,0,0,SRCCOPY);
  //dc.StretchBlt(rectcombo.right-20,0,20,_6bees_const::ktbbuttonheight,dcMem,0,0,20,_6bees_const::ktbbuttonheight,SRCCOPY);
  dcMem.RestoreDC(-1);
  dc.RestoreDC(-1);

  return 0;
}

bool CToolBarCombo::InsertHistory(){
  ResetContent();
  for (RegIterator it; it; it++)
    InsertString(-1, it);
  return true;
}

// This will add the history of data used
LRESULT CToolBarCombo::AddtoHistory(const _bstr_t& bstrURL){
  CRegKey keyAppID;
  if(std::basic_string<TCHAR>(bstrURL).find_first_not_of(_T(" "))!=std::string::npos)  {
    int h = GetCount();
    int pos = FindStringExact(-1,bstrURL);
    if(pos == CB_ERR){
      if(h<50){
        InsertString(0,bstrURL);
      }else{
        DeleteString(h-1);
        InsertString(0,bstrURL);
      }
    }else{
      DeleteString(pos);
      InsertString(0,bstrURL);
    }

    {
      CRegKey keyAppID;
      keyAppID.Attach(Get6BeeAppRegistryKey());
      keyAppID.RecurseDeleteKey(_T("History"));
    }

    {
      CRegKey keyAppID;
      keyAppID.Create(Get6BeeAppRegistryKey(),_T("History"));

      for(int i=0;i < GetCount();i++)
      {
        TCHAR cValue[1024]={};
        GetLBText(i, cValue);
        keyAppID.SetDWORDValue(cValue,1);
      }
    }

    SetCurSel(0);
  }

  return 0;
}

// ComboBox Drag and Drop
//===============================================================================================================
STDMETHODIMP CToolBarCombo::QueryInterface(REFIID riid, VOID** ppv)   
{     
  *ppv=NULL;      
  if (IID_IUnknown==riid || IID_IDropTarget==riid)         
    *ppv=this;      
  if (NULL!=*ppv) 
  {         
    ((LPUNKNOWN)*ppv)->AddRef();       
    return NOERROR;         
  }      

  return E_NOINTERFACE;
}   

STDMETHODIMP_(ULONG) CToolBarCombo::AddRef(void){     
  return ++m_cRef;     
}  

STDMETHODIMP_(ULONG) CToolBarCombo::Release(void){      
  if (0!=--m_cRef)         
    return m_cRef;        
  delete this;     
  return 0;     
}      


STDMETHODIMP CToolBarCombo::DragEnter(LPDATAOBJECT pDataObj , DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect){     
  m_pIDataObject=NULL;
  ATL::CString sText;

  FORMATETC formatDetails;     
  STGMEDIUM stmg;     

  // Let's get the text from the "clipboard"
  formatDetails.cfFormat = CF_TEXT;
  formatDetails.dwAspect = DVASPECT_CONTENT;
  formatDetails.lindex = -1;
  formatDetails.ptd = NULL;
  formatDetails.tymed = TYMED_HGLOBAL;

  if (SUCCEEDED(pDataObj->GetData(&formatDetails, &stmg)))
  {
    sText = (LPSTR)GlobalLock(stmg.hGlobal);
    GlobalUnlock(stmg.hGlobal);

    if (stmg.hGlobal != NULL) 
      ReleaseStgMedium(&stmg);             
  }

  *pdwEffect=DROPEFFECT_COPY;

  m_pIDataObject=pDataObj;     
  m_pIDataObject->AddRef();      

  return NOERROR;     
}       

STDMETHODIMP CToolBarCombo::DragOver(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect){     
  if (m_pIDataObject == NULL) 
    *pdwEffect=DROPEFFECT_NONE;         
  else 
    *pdwEffect=DROPEFFECT_COPY;
  return NOERROR;     
}       

STDMETHODIMP CToolBarCombo::DragLeave(void)     
{     
  m_pIDataObject->Release();
  return NOERROR;     
}      

STDMETHODIMP CToolBarCombo::Drop(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect){     
  *pdwEffect=DROPEFFECT_NONE;      
  if (m_pIDataObject == NULL)         
    return E_FAIL;
  DragLeave();      

  FORMATETC formatDetails;     
  STGMEDIUM stmg;     

  // Let's get the text from the "clipboard"
  formatDetails.cfFormat = CF_TEXT;
  formatDetails.dwAspect = DVASPECT_CONTENT;
  formatDetails.lindex = -1;
  formatDetails.ptd = NULL;
  formatDetails.tymed = TYMED_HGLOBAL;

  if (SUCCEEDED(pDataObj->GetData(&formatDetails, &stmg)))
  {  
    char* sClipboard = (char*)GlobalLock(stmg.hGlobal);
    ::SetWindowTextA(m_hWnd,sClipboard);
    AddtoHistory(sClipboard);
    GlobalUnlock(stmg.hGlobal);
    if (stmg.hGlobal != NULL) 
      ReleaseStgMedium(&stmg);         
  }
  *pdwEffect=DROPEFFECT_COPY;
  //search immediately after dragging
  ::PostMessage(GetParent(),kWM_TBCOMBOSEARCH_WEB, 0, 0);
  return NOERROR;     
} 

void CToolBarCombo::updateBoxLen(const wchar_t* txt){
  const wchar_t* str=NULL;
  if (txt==NULL){
    wchar_t edittxet[1024]={};
    GetWindowText(edittxet,1024);
    int nLength = GetWindowTextLength();
    if (nLength==0){
      return;
    }else{
      str=edittxet;
    }
  }else{
    str=txt;
  }

  CPaintDC dcPaint(m_hWnd);  
  dcPaint.SelectFont(EDITTXTFONT);
  SIZE fontsize;
  dcPaint.GetTextExtent(str,(int)wcslen(str),&fontsize);
  
  RECT comboxRect;
  GetClientRect(&comboxRect);
  int slen=comboxRect.right - comboxRect.left;

  static const int TEMPLEN=45;
  if (fontsize.cx>slen-TEMPLEN){
    int diff=fontsize.cx-slen+TEMPLEN;
    ::SendMessage(GetParent().m_hWnd,WM_SEP_TOOLBAR_MOUSEMOVE,1,diff+100);
  }else{
    if (fontsize.cx+200<slen-TEMPLEN){
      int diff=slen-TEMPLEN-(fontsize.cx+200);
      ::SendMessage(GetParent().m_hWnd,WM_SEP_TOOLBAR_MOUSEMOVE,1,-diff-100);
    }
  }
}

LRESULT CToolBarCombo::OnUpdateBoxLen(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
  updateBoxLen();
  bHandled=false;
  return 0;
}

LRESULT CToolBarCombo::OnEditChange(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
  // Get the text in the edit box
  wchar_t str[1024]={};
  GetWindowText(str, 1024);
  int nLength = GetWindowTextLength();

  updateBoxLen(str);

  if (!m_bAuto){
    return 0;
  }
  // Currently selected range
  DWORD dwCurSel = GetEditSel();
  WORD dStart = LOWORD(dwCurSel), dEnd = HIWORD(dwCurSel);

  // Search for, and select in, and string in the combo box that is prefixed by the text in the edit box
  //if (SelectString(-1, _bstr_t(str)) == CB_ERR){
  if (SelectString(-1, str) == CB_ERR){
    SetWindowText(str);    // No text selected, so restore what was there before
    if (dwCurSel != CB_ERR)
      SetEditSel(dStart, dEnd);  // restore cursor postion
  }
  // Set the text selection as the additional text that we have added
  if (dEnd < nLength && dwCurSel != CB_ERR)
    SetEditSel(dStart, dEnd);
  else
    SetEditSel(nLength, -1);
  return 0;
}

LRESULT CToolBarCombo::OnSelChange(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
  int iItem = GetCurSel();
  if(iItem>=0){
    wchar_t cValue[1024]={};             
    GetLBText(iItem, cValue);

    updateBoxLen(cValue);

    AddtoHistory(_bstr_t(cValue));
    //search after selecting...
    ::PostMessage(GetParent(),kWM_TBCOMBOSEARCH_WEB, 0, 0);
  }
  return 0;
}