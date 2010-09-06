#ifndef __6BEEUPLOADER_CLISTCTRLDATA_H__
#define __6BEEUPLOADER_CLISTCTRLDATA_H__

#include "ListCtrl.h"

struct CSubItem{
  CString m_strText;
  int m_nImage;
  int m_Progress;
  UINT m_nFormat;
  UINT m_nFlags;
  CListArray< CString > m_aComboList;
  HFONT m_hFont;
  COLORREF m_rgbBackground;
  COLORREF m_rgbText;
};

template < class TData = DWORD >
struct CListItem{
  CListArray < CSubItem > m_aSubItems;
  CString m_strToolTip;
  TData m_tData;  
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TData >
class CListCtrlData:  public CListWindowImpl< CListCtrlData< TData > >{
public:
  DECLARE_WND_CLASS( _T( "ListCtrl" ) )
protected:
  CListArray < CListItem< TData > > m_aItems;
public:
  int AddItem( CListItem< TData >& listItem ){
    if ( !m_aItems.Add( listItem ) )
      return -1;
    return CListImpl< CListCtrlData >::AddItem() ? GetItemCount() - 1 : -1;
  }
  
  int AddItem( LPCTSTR lpszText, int nImage=ITEM_IMAGE_NONE, UINT nFormat=ITEM_FORMAT_NONE, UINT nFlags=ITEM_FLAGS_NONE ){
    CSubItem listSubItem;
    listSubItem.m_nImage = ITEM_IMAGE_NONE;
    listSubItem.m_nFormat = nFormat;
    listSubItem.m_nFlags = ValidateFlags( nFlags );
    listSubItem.m_hFont = NULL;
    listSubItem.m_rgbBackground = m_rgbBackground;
    listSubItem.m_rgbText = m_rgbItemText;
    
    CListItem< TData > listItem;
    for ( int nSubItem = 0; nSubItem < GetColumnCount(); nSubItem++ )
      listItem.m_aSubItems.Add( listSubItem );
    listItem.m_aSubItems[ 0 ].m_strText = lpszText;// set item details for first subitem
    listItem.m_aSubItems[ 0 ].m_nImage = nImage;
    
    return AddItem( listItem );
  }
  
  BOOL DeleteItem( int nItem ){
    if ( nItem < 0 || nItem >= GetItemCount() ) 
      return FALSE;
    return m_aItems.RemoveAt( nItem ) ? CListImpl< CListCtrlData >::DeleteItem( nItem ) : FALSE;
  }
  
  BOOL DeleteAllItems(){
    m_aItems.RemoveAll();
    return CListImpl< CListCtrlData >::DeleteAllItems();
  }
  
  int GetItemCount(){
    return m_aItems.GetSize();
  }
  
  BOOL GetItem( int nItem, CListItem< TData >& listItem ){
    if ( nItem < 0 || nItem >= GetItemCount() ) 
      return FALSE;
    listItem = m_aItems[ nItem ];
    return TRUE;
  }
  
  BOOL GetSubItem( int nItem, int nSubItem, CSubItem& listSubItem ){
    CListItem< TData > listItem;
    if ( !GetItem( nItem, listItem ) )
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= (int)listItem.m_aSubItems.GetSize() )
      return FALSE;
    listSubItem = listItem.m_aSubItems[ nSubItem ];
    return TRUE;
  }
  
  CString GetItemText( int nItem, int nSubItem ){
    CSubItem listSubItem;
    return GetSubItem( nItem, nSubItem, listSubItem ) ? listSubItem.m_strText : _T( "" );
  }
  
  int GetItemImage( int nItem, int nSubItem ){
    CSubItem listSubItem;
    return GetSubItem( nItem, nSubItem, listSubItem ) ? listSubItem.m_nImage : ITEM_IMAGE_NONE;
  }

  int GetItemProgress( int nItem, int nSubItem ){ // simonwoo modified here!
    CSubItem listSubItem;
    return GetSubItem( nItem, nSubItem, listSubItem ) ? listSubItem.m_Progress : 0;
  }

  virtual ATL::CString GetItemFilePath(int nItem){
    return _T("");
  };
  
  UINT GetItemFormat( int nItem, int nSubItem ){
    CSubItem listSubItem;
    if ( !GetSubItem( nItem, nSubItem, listSubItem ) )
      return FALSE;
    return listSubItem.m_nFormat == ITEM_FORMAT_NONE ? GetColumnFormat( IndexToOrder( nSubItem ) ) : listSubItem.m_nFormat;
  }
  
  UINT GetItemFlags( int nItem, int nSubItem ){
    CSubItem listSubItem;
    if ( !GetSubItem( nItem, nSubItem, listSubItem ) )
      return FALSE;
    return listSubItem.m_nFlags == ITEM_FLAGS_NONE ? GetColumnFlags( IndexToOrder( nSubItem ) ) : listSubItem.m_nFlags;
  }
  
  HFONT GetItemFont( int nItem, int nSubItem ){
    CSubItem listSubItem;
    if ( !GetSubItem( nItem, nSubItem, listSubItem ) )
      return FALSE;
    return listSubItem.m_hFont == NULL ? CListImpl< CListCtrlData >::GetItemFont( nItem, nSubItem ) : listSubItem.m_hFont;
  }
  
  BOOL GetItemColours( int nItem, int nSubItem, COLORREF& rgbBackground, COLORREF& rgbText ){
    CSubItem listSubItem;
    if ( !GetSubItem( nItem, nSubItem, listSubItem ) )
      return FALSE;
    rgbBackground = listSubItem.m_rgbBackground;
    rgbText = listSubItem.m_rgbText;
    return TRUE;
  }
  
  CString GetItemToolTip( int nItem, int nSubItem ){
    CListItem< TData > listItem;
    return GetItem( nItem, listItem ) ? listItem.m_strToolTip : _T( "" );
  }
  
  BOOL GetItemData( int nItem, TData& tData ){
    CListItem< TData > listItem;
    if ( !GetItem( nItem, listItem ) )
      return FALSE;
    tData = listItem.m_tData;
    return TRUE;
  }
  
  BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText ){
    if ( nItem < 0 || nItem >= GetItemCount() ) 
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= (int)m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_strText = lpszText;
    return TRUE;
  }

  BOOL SetItemProgress( int nItem, int nSubItem, int _m_Progress ){// simonwoo added this!
    if ( nItem < 0 || nItem >= GetItemCount() ) 
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= (int)m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    if (_m_Progress > 100)
      _m_Progress = 100;
    if (_m_Progress < 0)
      _m_Progress = 0;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_Progress = _m_Progress;
    return TRUE;
  }
  
  BOOL SetItemImage( int nItem, int nSubItem, int nImage ){
    if ( nItem < 0 || nItem >= GetItemCount() ) 
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= (int)m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_nImage = nImage;
    return TRUE;
  }
  
  BOOL SetItemFormat( int nItem, int nSubItem, UINT nFormat, UINT nFlags = ITEM_FLAGS_NONE ){
    if ( nItem < 0 || nItem >= GetItemCount() ) 
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= (int)m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_nFormat = nFormat;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_nFlags = nFlags;
    return TRUE;
  }
  
  BOOL SetItemFormat( int nItem, int nSubItem, UINT nFormat, UINT nFlags, CListArray < CString >& aComboList ){
    if ( nItem < 0 || nItem >= GetItemCount() ) 
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= (int)m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_nFormat = nFormat;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_nFlags = nFlags;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_aComboList = aComboList;
    return TRUE;
  }
  
  BOOL SetItemFont( int nItem, int nSubItem, HFONT hFont ){
    if ( nItem < 0 || nItem >= GetItemCount() ) 
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= (int)m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_hFont = hFont;
    return TRUE;
  }
  
  BOOL SetItemColours( int nItem, int nSubItem, COLORREF rgbBackground, COLORREF rgbText ){
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= (int)m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_rgbBackground = rgbBackground;
    m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_rgbText = rgbText;
    return TRUE;
  }
  
  void ReverseItems(){
    m_aItems.Reverse();
  }
  
  class CompareItem{
  public:
    CompareItem( int nColumn ) : m_nColumn( nColumn ) {}
    inline bool operator() ( const CListItem< TData >& listItem1, const CListItem< TData >& listItem2 ){
      return ( listItem1.m_aSubItems[ m_nColumn ].m_strText.Compare( listItem2.m_aSubItems[ m_nColumn ].m_strText ) < 0 );
    }
  protected:
    int m_nColumn;
  };
  
  void SortItems( int nColumn, BOOL bAscending ){
    m_aItems.Sort( CompareItem( nColumn ) );
  }
  
  BOOL SetItemToolTip( int nItem, LPCTSTR lpszToolTip ){
    if ( nItem < 0 || nItem >= GetItemCount() ) 
      return FALSE;
    m_aItems[ nItem ].m_strToolTip = lpszToolTip;
    return TRUE;
  }
  
  BOOL SetItemData( int nItem, TData& tData ){
    if ( nItem < 0 || nItem >= GetItemCount() ) 
      return FALSE;
    m_aItems[ nItem ].m_tData = tData;
    return TRUE;
  }
};
typedef CListCtrlData< DWORD > CListCtrl;

#endif