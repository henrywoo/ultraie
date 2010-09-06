#ifndef __UUTOOLBAR_C6BEEANIMATIONPANE__
#define __UUTOOLBAR_C6BEEANIMATIONPANE__

#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlctrlx.h>

template <class T>
class CAnimationInPaneImpl
{
protected:
  CAnimateCtrl m_Anim;
  int m_iAnimPane;// Pane ordinal where the progress bar resides, or -1 when off.
public:
  CAnimationInPaneImpl<T>(): m_iAnimPane(-1) {}
  virtual ~CAnimationInPaneImpl<T>(){
    AnimDestroyWindow();
  }
  void AnimDestroyWindow(void){
    if (::IsWindow(m_Anim.m_hWnd)){
      m_Anim.Close();
      m_Anim.ShowWindow(SW_HIDE);
      m_Anim.DestroyWindow();
    }
    m_Anim.m_hWnd = NULL;
    m_iAnimPane = -1;
  }
  BEGIN_MSG_MAP(CAnimationInPaneImpl<T>)
    MESSAGE_HANDLER(SB_SIMPLE, OnSimple)
  END_MSG_MAP()

  LRESULT OnSimple(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){
    if (::IsWindow(m_Anim.m_hWnd))
      m_Anim.ShowWindow(wParam ? SW_HIDE: SW_SHOW);
    bHandled = FALSE;
    return 0;
  }

  BOOL UpdatePanesLayout(void){
    if (m_iAnimPane != -1){
      T* pt = static_cast<T*>(this);
      RECT rc;
      pt->GetRect(m_iAnimPane, &rc);
      ::InflateRect(&rc, -1, -1); 
      m_Anim.MoveWindow(&rc);
    }
    return TRUE;
  }

  BOOL AnimCreate(int iPane,ATL::_U_STRINGorID FileName,int pos=0,
    DWORD dwStyle=ACS_TRANSPARENT|ACS_CENTER|ACS_AUTOPLAY,DWORD dwExStyle=0)
  {
    T* pt = static_cast<T*>(this);// Check there is such a pane
    ATLASSERT(::IsWindow(pt->m_hWnd));
    if (0 > iPane || iPane >= pt->m_nPanes)
      return FALSE;
    if (::IsWindow(m_Anim.m_hWnd))// Check there is not an animation already open.
      return FALSE;
    RECT rc={};// Get the pane's rectangle
    pt->GetRect(iPane,&rc);
    ::InflateRect(&rc, -1, -1);
    switch (pos){
case 1:
  rc.left=rc.right-16;// right side
  break;
case 2:
  rc.left=rc.left+(rc.right-rc.left)/2-16/2;//center
  rc.right=rc.left+16;
  break;
default:
  rc.right=rc.left+16;//animation does display at the left side of pane
  break;
    }

    m_Anim.Create(pt->m_hWnd,rc,NULL,WS_VISIBLE|WS_CHILD|dwStyle,dwExStyle);
    // Create the window, using the status bar (this) as a parent.
    if (!m_Anim.Open(FileName))
      return FALSE;
    if (!m_Anim.Play(0, -1, -1))
      return FALSE;
    m_iAnimPane = iPane;// Hold this, we'll need it to move around.
    return TRUE;
  }

  //  Simple containment
  BOOL AnimSeek(UINT nTo){ return m_Anim.Seek(nTo); }
  BOOL AnimStop(){ m_Anim.Stop(); }
  BOOL AnimPlay(UINT nFrom, UINT nTo, UINT nRep){
    return m_Anim.Play(nFrom, nTo, nRep);
  }
};


#endif