#include "stdafx.h"
#include "HBarCombo.h"

#include "HorizontalBar.h"

////////////////////////////////////////////////////////////////////////////////
// CBandEditCtrl
CBandEditCtrl::CBandEditCtrl():
m_cRef(0),
gotfocus(false),
m_bAuto(true),
m_pBand(NULL)
{}

CBandEditCtrl::~CBandEditCtrl()
{
  if (IsWindow()){
    DestroyWindow();
  }
}

STDMETHODIMP CBandEditCtrl::TranslateAcceleratorIO(LPMSG lpMsg)
{
  if (!gotfocus) return S_FALSE;

  int nVirtKey = (int)(lpMsg->wParam);
  if(lpMsg->message==WM_KEYUP && nVirtKey == VK_RETURN )
  {
    lpMsg->wParam = 0;
    ::PostMessage(GetParent(), WM_HBAR_SEARCH_NEXT, 0, 0);
    return S_OK;
  }else if ((WM_KEYUP == lpMsg->message || WM_KEYDOWN == lpMsg->message) &&
    VK_BACK==nVirtKey||(VK_END<=nVirtKey && VK_DOWN>=nVirtKey)||VK_DELETE==nVirtKey||
    ((GetKeyState(VK_CONTROL)&0x80)&&(nVirtKey=='C'||nVirtKey=='V'||nVirtKey=='X'))){
      if(nVirtKey=='X'){
        ::PostMessage(GetParent(), kWM_HBarCOMBOSEARCH, HBAR_SHOWWINDOW, HBAR_SHOWWINDOW);
      }
      m_bAuto = false;
      if (!TranslateMessage(lpMsg))
        return S_FALSE;
      DispatchMessage(lpMsg);
      m_bAuto = true;
      if(WM_KEYDOWN == lpMsg->message && VK_BACK == nVirtKey)
        m_bAuto = false;
      return S_OK;
  }else{
    return S_FALSE;
  }
}

LRESULT CBandEditCtrl::OnSetFocus(WORD,WORD,HWND,BOOL&)
{
  gotfocus=true;
  m_pBand->FocusChange(true);
  //::PostMessage(GetParent(), kWM_HBarCOMBOSEARCH, 0, 0);
  return 0;
}

LRESULT CBandEditCtrl::OnLostFocus(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
  gotfocus=false;
  if (m_pBand){
    if(gotfocus){
      m_pBand->FocusChange(false);
    }
  }
  return 0;
}

STDMETHODIMP CBandEditCtrl::QueryInterface(REFIID riid, VOID** ppv)   
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

STDMETHODIMP_(ULONG) CBandEditCtrl::AddRef(void)     
{     
  return ++m_cRef;     
}  

STDMETHODIMP_(ULONG) CBandEditCtrl::Release(void)   
{      
  if (0!=--m_cRef)         
    return m_cRef;        
  delete this;     
  return 0;     
}      


STDMETHODIMP CBandEditCtrl::DragEnter(LPDATAOBJECT pDataObj , DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)    
{     
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

STDMETHODIMP CBandEditCtrl::DragOver(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)   
{     
  if (m_pIDataObject == NULL) 
    *pdwEffect=DROPEFFECT_NONE;         
  else 
    *pdwEffect=DROPEFFECT_COPY;

  return NOERROR;     
}       

STDMETHODIMP CBandEditCtrl::DragLeave(void)     
{     
  m_pIDataObject->Release();
  return NOERROR;     
}      

STDMETHODIMP CBandEditCtrl::Drop(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)     
{
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
    //LPTSTR sClipboard = (LPTSTR)GlobalLock(stmg.hGlobal);
    char* sClipboard = (char*)GlobalLock(stmg.hGlobal);
    SetWindowTextA(m_hWnd,sClipboard);
    GlobalUnlock(stmg.hGlobal);
    if (stmg.hGlobal != NULL) 
      ReleaseStgMedium(&stmg);         
  }
  *pdwEffect=DROPEFFECT_COPY;
  //拖动之后立即开始搜索!-不必，自己会搜索的了。
  //::PostMessage(GetParent(), kWM_HBarCOMBOSEARCH, 0, 0);
  return NOERROR;     
} 

LRESULT CBandEditCtrl::OnEditChange(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){
  //TCHAR str[200];
  //GetWindowText(str, 200);
  //int nLength = GetWindowTextLength();
  ::PostMessage(GetParent(), kWM_HBarCOMBOSEARCH, 0, 0);
  return 0;
}

void CBandEditCtrl::SetBand(Chorizontalbar* p_Band)
{
  m_pBand=p_Band;
}
