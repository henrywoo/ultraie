#pragma once

//#include "CListCtrlData.h"
#include "ListCtrl.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "constdata.h"
#include <6bees_util.h>
#include <6bees_str.h>
#include "ListTypes.h"
using namespace _6bees_util;

__int64 simonwoo=0;

#ifdef __CHINESE__
const wchar_t* const kcolfname = L"文件名";
const wchar_t* const kcolfsize = L"大小";
const wchar_t* const kcolprog = L"进度";
const wchar_t* const kcolspeed = L"速度";
const wchar_t* const kcoltremaining = L"剩余";
const wchar_t* const kcoltpassed = L"计时";
const wchar_t* const kcoltype = L"类型";
const wchar_t* const kcollink = L"链接";
#else
const wchar_t* const kcolfname = L"File name";
const wchar_t* const kcolfsize = L"File size";
const wchar_t* const kcolprog = L"Progress";
const wchar_t* const kcolspeed = L"Speed";
const wchar_t* const kcoltremaining = L"Time remaining";
const wchar_t* const kcoltpassed = L"Time passed";
const wchar_t* const kcoltype = L"Type";
const wchar_t* const kcollink = L"Link";
#endif

struct CFileProfile{
  int m_status;
  long m_size_long;
  CString m_size_str; //file size by bytes
  unsigned int m_progress; //(0-100)
  unsigned int m_speed;
  CString m_filename;
  CString m_extension;
  CString m_absPath;
  CString m_timepassed;
  CString m_timeremaining;
  CString m_link;
  CString m_strToolTip;
  short m_tID;// upload thread ID, default is -1
  bool m_isDirectUpload;

  CFileProfile():m_status(0),m_progress(0),m_speed(0),m_tID(-1),m_size_str(),m_filename(),m_extension(),
  m_absPath(),m_timepassed(),m_timeremaining(),m_link(L""),m_isDirectUpload(false)
  {}

  explicit CFileProfile(int _m_status,const wchar_t* _filelocation,int fsize=0,bool _isDirectUpload=false):
  m_progress(0),m_speed(0),m_tID(-1),m_timepassed(),m_timeremaining()
  {
    TCHAR szFileSize[30]={};
    StrFormatByteSize(fsize,szFileSize,30);
    m_size_str = szFileSize;
    m_size_long = fsize;
    m_absPath = _filelocation;
    m_filename = PathFindFileName(_filelocation);
    m_extension = PathFindExtension(_filelocation);
    m_status=_m_status;
    m_isDirectUpload = _isDirectUpload;
  }

  CFileProfile& operator=(const CFileProfile& fp){
    assign(fp);
    return *this;
  }

  CFileProfile(const CFileProfile& fp){
    assign(fp);
  }

  void assign(const CFileProfile& fp){
    m_status=fp.m_status;
    m_size_long=fp.m_size_long;
    m_size_str=fp.m_size_str;
    m_progress=fp.m_progress;
    m_speed=fp.m_speed;
    m_filename=fp.m_filename;
    m_extension=fp.m_extension;
    m_absPath=fp.m_absPath;
    m_timepassed=fp.m_timepassed;
    m_timeremaining=fp.m_timeremaining;
    m_link=fp.m_link;
    m_tID=fp.m_tID;
    m_isDirectUpload=fp.m_isDirectUpload;
  }

  BOOL operator ==(const CFileProfile &a) const{
    return m_absPath == a.m_absPath; 
  }
};

class CListWindowFileData: 
  public CListWindowImpl<CListWindowFileData>
{
public:
  DECLARE_WND_CLASS(_T("CListWindowFileData"))
  CListArray<CFileProfile> m_aFiles;

protected:
  CImageList m_ilItemImages;

public:
  class CompareItem{
  public:
    CompareItem( FileColumns colColumn ):m_colColumn( colColumn){}
    inline bool operator()(const CFileProfile& fileProfile1, const CFileProfile& fileProfile2){
      switch(m_colColumn){
        case colStatus: return(fileProfile1.m_status < fileProfile2.m_status);
        case colFileName:   return(fileProfile1.m_filename.Compare( fileProfile2.m_filename)< 0);
        case colSize:  return(fileProfile1.m_size_long < fileProfile2.m_size_long);
        case colProgess:    return(fileProfile1.m_progress < fileProfile2.m_progress);
        case colSpeed: return(fileProfile1.m_speed < fileProfile2.m_speed);
#ifndef _SIMPLECOLUMN_
        case colTimeremaing:return(fileProfile1.m_timeremaining.Compare( fileProfile2.m_timeremaining) < 0);
        case colTimepassed: return(fileProfile1.m_timepassed.Compare( fileProfile2.m_timepassed)< 0);
        case colFileType:   return(fileProfile1.m_extension.Compare( fileProfile2.m_extension)< 0);
#endif
      }
      return false;
    }    
  protected:
    FileColumns m_colColumn;
  };

  BOOL Initialise(){
    if (!m_ilItemImages.CreateFromImage( IDB_EXAMPLE,16,0,RGB(255,0,255),IMAGE_BITMAP,LR_CREATEDIBSECTION))
      return FALSE;
    SetImageList( m_ilItemImages);
    AddColumn( _T(""), 28,TRUE,ITEM_FORMAT_FILESTATUS,ITEM_IMAGE_NONE);
    AddColumn( kcolfname, 180);
    AddColumn( kcolfsize, 60);
    AddColumn( kcolprog, 80, FALSE, ITEM_FORMAT_PROGRESS,ITEM_IMAGE_NONE);
    AddColumn( kcolspeed, 60);
#ifndef _SIMPLECOLUMN_
    AddColumn( kcoltremaining, 45);
    AddColumn( kcoltpassed,45);
    AddColumn( kcoltype, 45);
#endif
    AddColumn( kcollink, 140,FALSE,ITEM_FORMAT_HYPERLINK);
    return CListWindowImpl<CListWindowFileData>::Initialise();
  }

  int AddFile(const CFileProfile& FileProfile){
    m_aFiles.Add( FileProfile);
    int num =  GetItemCount();
    SetItemStatus(num-1,S_INITAL);
    return CListWindowImpl< CListWindowFileData >::AddItem() ? num - 1 : -1;
  }

  int GetItemCount(){ // required by CListWindowImpl
    return m_aFiles.GetSize();
  }
  
  CString GetItemText( int nItem, int nSubItem){ // required by CListWindowImpl
    CFileProfile fileProfile;
    if (!GetFile(nItem, fileProfile))
      return _T("");
    switch (nSubItem){
    case colStatus:
      return _T("");
    case colFileName:
      return fileProfile.m_filename;
    case colSize:
      return fileProfile.m_size_str;
    case colProgess:
      return _T("");//return fileProfile.m_progress;
    case colSpeed:{
      CString tmp(L"");
      if(fileProfile.m_speed)
        tmp.Format(_T("%dKB/s"),fileProfile.m_speed);
      return tmp; 
      }
#ifndef _SIMPLECOLUMN_
    case colTimeremaing:
      return fileProfile.m_timeremaining;
    case colTimepassed:
      return fileProfile.m_timepassed;
    case colFileType:
      return fileProfile.m_extension;
#endif
    case colLink:
      return fileProfile.m_link;
    }    
    return CString(L"");
  }

  int GetItemProgress( int nItem, int nSubItem){
    CFileProfile fileProfile;
    if(!GetFile( nItem, fileProfile))
      return 0;
    return fileProfile.m_progress;
  }

  ATL::CString GetItemFilePath(int nItem){
    CFileProfile fileProfile;
    if (!GetFile(nItem,fileProfile))
      return _T("");
    return fileProfile.m_absPath;
  }
  
  int SetFtpClientID( const CString& flocation, int ftpclientid){
    int m_aFiles_size=(int)m_aFiles.GetSize();
    for(int i=0;i<m_aFiles_size;i++){
      if (m_aFiles[i].m_absPath==flocation){
        m_aFiles[i].m_tID = ftpclientid;
        return i;
      }
    }
    return 0;
  }

  BOOL UpdateItemProgress(int nItem, int nSubItem, int _m_Progress){
    if (_m_Progress > 100)
      _m_Progress = 100;
    if (_m_Progress < 0)
      _m_Progress = 0;
    m_aFiles[nItem].m_progress = _m_Progress;
    return TRUE;
  }

  BOOL UpdateItemProgress(const CString& fl,__int64 bytes/*uploaded bytes*/,int nItem,
    long curspeed,long timepassed,long timeremaining)
  {
    for(int i=0;i<m_aFiles.GetSize();i++){
      CString& a =m_aFiles[i].m_absPath;
      if (a == fl){
        nItem = i;
        break;
      }
    }
    if(m_aFiles[nItem].m_size_long!=0){
      int p = (int)(bytes*100/m_aFiles[nItem].m_size_long);
      p = (p==0 && bytes>0)? 1 : p ;
      if (curspeed != -1){
        wstring s_timepassed = FormatTime(timepassed);
        wstring s_timeremaining = FormatTime(timeremaining);
        if (m_aFiles[nItem].m_timepassed != s_timepassed.c_str()){
          m_aFiles[nItem].m_progress = p;
          m_aFiles[nItem].m_speed    = curspeed;
          m_aFiles[nItem].m_timepassed    = s_timepassed.c_str();
          m_aFiles[nItem].m_timeremaining = s_timeremaining.c_str();
          wstring ttinfo = _6bees_str::wstringmaker() << L"文件上传中"
            << L"\n文件名:" << m_aFiles[nItem].m_filename.GetString()
            << L"\n文件路径:" << m_aFiles[nItem].m_absPath.GetString()
            << L"\n上传时间:" << s_timepassed
            << L"\n剩余时间:" << s_timeremaining;
          SetItemToolTip(nItem,ttinfo.c_str());
          InvalidateItem(nItem);
        }
      }
      return true;
    }else{
      m_aFiles[nItem].m_progress = 100;
      SetItemStatus(nItem,S_FINISHED);
      InvalidateItem(nItem);
      return false;
    }
  }

  // overrides CListWindowImpl::GetItemImage - must!
  int GetItemImage( int nItem, int nSubItem){
    switch (nSubItem){
    case colFileName:
    case colLink:
      return 1;
    case colStatus:{
      CFileProfile fileProfile;
      if(!GetFile( nItem, fileProfile))
        return -1;
      return fileProfile.m_status;
    }
    default:
      return ITEM_IMAGE_NONE;
    }
  }

  BOOL GetFile( int nItem, CFileProfile & fileProfile){
    if(nItem < 0 || nItem >= GetItemCount())
      return FALSE;
    fileProfile = m_aFiles[nItem];
    return TRUE;
  }

  // overrides CListWindowImpl::SortItems
  void SortItems( int nColumn, BOOL bAscending){
    m_aFiles.Sort( CompareItem((FileColumns)nColumn));
  }

  void ReverseItems(){
    m_aFiles.Reverse();
  }

  BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText){
    return ! (nItem < 0 || nItem >= GetItemCount());
  }

  BOOL SetItemStatus( int nItem , int filestatus){
    if(nItem < 0 || nItem >= GetItemCount())
      return FALSE;
    m_aFiles[nItem].m_status = filestatus;
    wstring statustxt;
    switch(filestatus){
      case S_INITAL:
        statustxt = L"准备上传";break;
      case S_ACTIVE:
        {
          if(m_aFiles[nItem].m_strToolTip.GetLength()==4){
            statustxt = L"正在上传";
          }else{
            return TRUE;
          }
        }break;
      case S_WAITING:
        statustxt = L"等待上传";break;
      case S_SUSPENDED:
        statustxt = L"暂停上传";break;
      case S_FINISHED:
        statustxt = L"上传完毕";break;
    }
    SetItemToolTip(nItem,statustxt.c_str());
    InvalidateItem(nItem);
    return TRUE;
  }

  int GetnItem( const CString& filepath){
    int m_dFiles_size = (int)m_aFiles.GetSize();
    for(int i=0;i<m_dFiles_size;i++){
      if(m_aFiles[i].m_absPath == filepath){
        return i;
      }
    }
    return -1;
  }

  BOOL SetItemToolTip( int nItem, LPCTSTR lpszToolTip )
  {
    if(nItem < 0 || nItem >= GetItemCount())
      return FALSE;
    m_aFiles[nItem].m_strToolTip = lpszToolTip;
    return TRUE;
  }

  CString GetItemToolTip( int nItem, int nSubItem )
  {
    if(nItem < 0 || nItem >= GetItemCount())
      return L"";
    return m_aFiles[nItem].m_strToolTip;
  }
};
