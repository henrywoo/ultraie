#ifndef __PLUGINS_TIPDLG__
#define __PLUGINS_TIPDLG__

#pragma once
#include "resource.h"
//#include "util.h"

#include <BalloonTip.h>
#include <HTMLListBox.h>

#include <vector>
#include <string>
#include <6BeeCommons.h>
#include <atlctrlx.h>
#include <imgcap_setting.h>
#include <6beeres.h>
#include "../Stub_Skeleton/6bees_util.h"

#include "helperfunc.h"

class CTipDialog :
	public CAxDialogImpl<CTipDialog>,
	public CBalloonDialog<CTipDialog>
{
private:
	CImageList m_imglst;
  WTL::CBitmapButton m_upload;

	CHTMLListCtrl m_list;
  std::vector<nsplugin::CMediaRes> mres;
public:
	enum { IDD = IDD_TIP };

	BEGIN_MSG_MAP(CTipDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)

		MESSAGE_HANDLER(WM_ADDMEDIA, OnAddMedia)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)

		NOTIFY_HANDLER(IDC_MEDIALIST, HLN_INITIALIZE, OnList_Initialize)
		NOTIFY_HANDLER(IDC_MEDIALIST, HLN_INSERTITEM, OnList_InsertItem)
		NOTIFY_HANDLER(IDC_MEDIALIST, HLN_SELCHANGED, OnList_SelChanged)

		CHAIN_MSG_MAP( CBalloonDialog<CTipDialog> )
		REFLECT_NOTIFICATIONS()
		ALT_MSG_MAP(1)
		CHAIN_MSG_MAP_ALT( CBalloonDialog<CTipDialog>, 1 )
	END_MSG_MAP()

  CTipDialog(){}

	virtual ~CTipDialog(){
		m_imglst.Destroy();
	}
	LRESULT OnInitDialog(UINT,WPARAM,LPARAM,BOOL&bHandled){
		HBITMAP hbmp=(HBITMAP)_6beed_util::CMyRes::R().GetImg(IDB_CLOSE,IMAGE_BITMAP,18*2,18);
		SetMyImageList(hbmp,m_imglst,MASKPURPLE,18,18,2,RGB(188,222,235));
		m_upload.SubclassWindow(GetDlgItem(IDOK));
		m_upload.SetImageList(m_imglst);
		m_upload.SetImages(0,-1,1,-1);
		m_upload.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);


		m_list.SubclassWindow(GetDlgItem(IDC_MEDIALIST), IDR_MEDIATIP);
		ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW );
		::SetForegroundWindow(m_hWnd);
		bHandled = FALSE; // CBalloonDialog needs this...
		return 0;
	}
	LRESULT OnWhiteLabelCtlColorStatic(UINT , WPARAM wParam, LPARAM , BOOL& ){
		CDCHandle dc( (HDC) wParam );
		dc.SetTextColor(RGB(0,0,200));
		dc.SetBkColor(RGB(255,255,255)),dc.SetBkMode(TRANSPARENT);
		return (LRESULT) (HBRUSH) ::GetStockObject(WHITE_BRUSH);
	}
	LRESULT OnOK(WORD,WORD wID,HWND,BOOL&){
		mres.clear();
		m_list.ResetContent();
		EndDialog(wID);
		return 0;
	}

	LRESULT OnLButtonDown(UINT , WPARAM wParam, LPARAM lParam, BOOL& bHandled){
		HCURSOR hcur = LoadCursor(NULL,IDC_SIZEALL);
		SetCursor(hcur);
		SendMessage(WM_SYSCOMMAND,SC_MOVE | HTCAPTION,0);
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnLButtonUp(UINT , WPARAM wParam, LPARAM lParam, BOOL& bHandled){
		HCURSOR hcur = LoadCursor(NULL,IDC_ARROW);
		SetCursor(hcur);
		bHandled = FALSE;
		return 0;
	}

  void AddMedia(const nsplugin::CMediaRes& m){
		for (unsigned int i=0;i<mres.size();i++){
			if (mres[i].mediaurl == m.mediaurl){
				return;
			}
		}
		mres.push_back(m);
	}

	LRESULT OnAddMedia(UINT , WPARAM wParam, LPARAM lParam, BOOL& bHandled){
		nsplugin::CMediaRes * m = reinterpret_cast<nsplugin::CMediaRes*>(lParam);
		bool if_add=true;
		for (unsigned int i=0;i<mres.size();i++){
			if (mres[i].mediaurl == m->mediaurl){
				if_add=false;
			}
		}
		if(if_add){
			mres.push_back(*m);
			m_list.ResetContent();
			Show();
		}
		bHandled = TRUE;
		return 0;
	}

	// TODO 
	void Show(){
		static std::wstring resdllpath=_6beed_util::Get6BEEPath(_6beed_const::ResDLLName);
		static wchar_t szHTML[10240]={};
		int msize = (int)mres.size();
		for(int i=0;i<msize;i++){
			const wchar_t* directstr = mres[i].direct ? _T("Direct Download") : _T("Save Target As...") ;
			const wchar_t* mresurl=mres[i].mediaurl.c_str();
			const wchar_t* mrestitle=mres[i].title.c_str();
			std::wstring tmp= mres[i].mediaurl.substr(0,30);
			const wchar_t* simpURL = tmp.c_str();
      wchar_t* res_size=NULL;
      if (mres[i].size>0){
        TCHAR szFileSize[30]={};
        StrFormatByteSize(mres[i].size,szFileSize,30);
        res_size = szFileSize;
      }else{
        res_size = L"N/A";
      }
      int res_id = nsplugin::GetMimeResID(mres[i].type);

      swprintf_s(szHTML,10240,
        L"<IMG SRC='res://%s/%d' style='padding:2px;'/>"
        L"<SPAN><b>%d . %s</b>&nbsp;(size:%s)(%s)</SPAN><DIV CLASS=\"INFODIV\" STYLE=\"display:none\">"
        L"<TABLE cellspacing=0 cellpadding=0 style='border:0px solid #eeeeee'><TR><TD width=\"100%%\">"
        L"<a href='#' onclick=\"var meizz=window.open('%s');meizz.document.execCommand('SaveAs');meizz.close();\">%s...</a>"
        L"</TD>"
        L"<TD><a href='#' onclick=\"var eizz=window.open('%s');eizz.document.execCommand('SaveAs');eizz.close();\">"
        L"<IMG src=\"res://%s/114\" style='padding:2px;border:0px;'/></a></TD></TR></TABLE></DIV>\n",
        resdllpath.c_str(),
        res_id,
        i+1,
        mrestitle,
        res_size,
        directstr,
        mresurl,simpURL,mresurl,resdllpath.c_str());
			m_list.AddString(szHTML);
		}
	}

	LRESULT OnList_Initialize(int, LPNMHDR, BOOL& ){
		Show();
		return 0;
	}

	LRESULT OnList_InsertItem(int, LPNMHDR pnmh, BOOL& ){
		LPNMHTMLLIST lpht = (LPNMHTMLLIST)pnmh;
		lpht->pNew->put_className(CComBSTR(L"ULTRAIE_ITEMNORMAL"));
		return 0;
	}

	LRESULT OnList_SelChanged(int, LPNMHDR pnmh, BOOL& ){
		LPNMHTMLLIST lpht = (LPNMHTMLLIST)pnmh;
		if( lpht->pNew )
			lpht->pNew->put_className(CComBSTR(L"ULTRAIE_ITEMSEL"));
		if( lpht->pOld )
			lpht->pOld->put_className(CComBSTR(L"ULTRAIE_ITEMNORMAL"));

		// Call back into HTML to expand/collapse item
		CComPtr<IHTMLDocument2> spDoc(0);
		if(!m_list._GetDhtmlDocument(spDoc))
			return S_FALSE;

		CComDispatchDriver dd(spDoc);
		if( lpht->pOld ) {
			CComQIPtr<IDispatch> spDisp(lpht->pOld);
			CComVariant v(spDisp);
			dd.Invoke1(L"OnItemCollapsing", &v);
		}
		if( lpht->pNew ) {
			CComQIPtr<IDispatch> spDisp(lpht->pNew);
			CComVariant v(spDisp);
			dd.Invoke1(L"OnItemExpanding", &v);
		}
		return 0;
	}
};

#endif //_TIPDLG_