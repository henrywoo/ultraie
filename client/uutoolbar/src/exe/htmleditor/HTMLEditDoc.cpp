#include "stdafx.h"
#include "HTMLEdit.h"
#include "HTMLEditDoc.h"
#include "HTMLEditView.h"

IMPLEMENT_DYNCREATE(CHTMLEditDoc, CHtmlEditDoc)
BEGIN_MESSAGE_MAP(CHTMLEditDoc, CHtmlEditDoc)
END_MESSAGE_MAP()

CHTMLEditDoc::CHTMLEditDoc(){}
BOOL CHTMLEditDoc::DoFileSave(){
  POSITION pos=GetFirstViewPosition();   
  while(pos){   
    CHTMLEditView* pView=dynamic_cast<CHTMLEditView *>(GetNextView(pos));
    pView->CreateSnapshotThumb_NotifyWnd();
  }
  return CDocument::DoFileSave();
}

CHTMLEditDoc::~CHTMLEditDoc(){}

BOOL CHTMLEditDoc::OnNewDocument(){
  if (!CHtmlEditDoc::OnNewDocument())
    return FALSE;
  return TRUE;
}