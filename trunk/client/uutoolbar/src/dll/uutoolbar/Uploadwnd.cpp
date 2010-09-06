#include "stdafx.h"
#include "Uploadwnd.h"
#include "util.h"

CUploadwnd::CUploadwnd():
m_hIcon(NULL)
{
	// Load m_Files from local ....
}

LRESULT CUploadwnd ::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
	this->DlgResize_Init();

	m_hIcon = LoadIcon(ATL::_AtlBaseModule.GetResourceInstance(),MAKEINTRESOURCE(IDI_LOGINWND));//????
	SetIcon(m_hIcon,TRUE);
	DoDataExchange(false);

	m_C6BeeCheckListCtrl.InsertColumn ( 0, _T("Title"), LVCFMT_LEFT, 320, 0 );
	m_C6BeeCheckListCtrl.InsertColumn ( 1, _T("Size"), LVCFMT_LEFT, 60, 0 );
	m_C6BeeCheckListCtrl.InsertColumn ( 2, _T("Progress"), LVCFMT_LEFT, 120, 0 );
	m_C6BeeCheckListCtrl.InsertColumn ( 3, _T("Speed"), LVCFMT_LEFT, 60, 0 );
	m_C6BeeCheckListCtrl.InsertColumn ( 4, _T("Time Left"), LVCFMT_LEFT, 80, 0 );
	m_C6BeeCheckListCtrl.InsertColumn ( 5, _T("Time"), LVCFMT_LEFT, 80, 0 );
	m_C6BeeCheckListCtrl.InsertColumn ( 6, _T("FileType"), LVCFMT_LEFT, 60, 0 );

	return 0;
}


void CUploadwnd ::AddFile(WTL::CString filename){
	WIN32_FILE_ATTRIBUTE_DATA  fileattr;
	if (::GetFileAttributesEx( (LPCTSTR)filename, ::GetFileExInfoStandard, &fileattr )){		
		DWORD filesize = (fileattr.nFileSizeHigh * (MAXWORD+1)) + fileattr.nFileSizeLow; 
		this->m_Files.push_back(filename);
		int row_count = this->m_C6BeeCheckListCtrl.GetItemCount();
		if(row_count>=0){
			int a = m_C6BeeCheckListCtrl.InsertItem ( row_count, filename );
			m_C6BeeCheckListCtrl.SetItem(a,1,LVIF_TEXT,(LPCTSTR)GetFileSizebyUnit(filesize),0,0,0,0);
			m_C6BeeCheckListCtrl.SetItem(a,2,LVIF_TEXT,_T("2"),0,0,0,0);
			m_C6BeeCheckListCtrl.SetItem(a,3,LVIF_TEXT,_T("3"),0,0,0,0);
			m_C6BeeCheckListCtrl.SetItem(a,4,LVIF_TEXT,_T("4"),0,0,0,0);
			m_C6BeeCheckListCtrl.SetItem(a,5,LVIF_TEXT,_T("5"),0,0,0,0);
			m_C6BeeCheckListCtrl.SetItem(a,6,LVIF_TEXT,_T("6"),0,0,0,0);
		}
	}else{
		this->m_Files.push_back(filename);
		int row_count = this->m_C6BeeCheckListCtrl.GetItemCount();
		if(row_count>=0){
			int a = m_C6BeeCheckListCtrl.InsertItem ( row_count, filename );
			m_C6BeeCheckListCtrl.SetItem(a,1,LVIF_TEXT,_T("Unknown"),0,0,0,0);
			m_C6BeeCheckListCtrl.SetItem(a,2,LVIF_TEXT,_T("2"),0,0,0,0);
			m_C6BeeCheckListCtrl.SetItem(a,3,LVIF_TEXT,_T("3"),0,0,0,0);
			m_C6BeeCheckListCtrl.SetItem(a,4,LVIF_TEXT,_T("4"),0,0,0,0);
			m_C6BeeCheckListCtrl.SetItem(a,5,LVIF_TEXT,_T("5"),0,0,0,0);
			m_C6BeeCheckListCtrl.SetItem(a,6,LVIF_TEXT,_T("6"),0,0,0,0);
		}
	}
}

LRESULT CUploadwnd ::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled){
	//EndDialog(wID);
	this->DestroyWindow();
	return 0;
}