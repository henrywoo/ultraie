// UltraIE - Yet Another IE Add-on
// Copyright (C) 2006-2010
// Simon Wu Fuheng (simonwoo2000 AT gmail.com), Singapore
// Homepage: http://www.linkedin.com/in/simonwoo
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#define NULL_COLUMN      -1
#define NULL_ITEM      -1
#define NULL_SUBITEM      -1
#define ITEM_HEIGHT_MARGIN    6
#define ITEM_WIDTH_MARGIN    20
#define ITEM_SCROLL_OFFSET    5
#define DRAG_HEADER_OFFSET    4
#define DRAG_ITEM_OFFSET    3
#define ITEM_EDIT_MARGIN    5
#define TOOLTIP_TOOL_ID      1000
#define RESIZE_COLUMN_TIMER    1001
#define RESIZE_COLUMN_PERIOD    20
#define ITEM_VISIBLE_TIMER    1002
#define ITEM_VISIBLE_PERIOD    250
#define SEARCH_PERIOD      800
#define ITEM_AUTOSCROLL_TIMER    1003
#define ITEM_SCROLL_TIMER    1004
#define ITEM_SCROLL_PERIOD    20
#define ITEM_SCROLL_UNIT_MAX    8
#define ITEM_SCROLL_UNIT_MIN    4

#define ITEM_FORMAT_NONE    0
#define ITEM_FORMAT_HYPERLINK    1
#define ITEM_FORMAT_PROGRESS    2
#define ITEM_FORMAT_FILESTATUS    3

#define ITEM_FLAGS_NONE      0x0000
#define ITEM_FLAGS_LEFT      0x0001
#define ITEM_FLAGS_RIGHT    0x0002
#define ITEM_FLAGS_CENTRE    0x0004
#define ITEM_FLAGS_READ_ONLY    0x0008
#define ITEM_FLAGS_EDIT_UPPER    0x0010
#define ITEM_FLAGS_EDIT_NUMBER    0x0020
#define ITEM_FLAGS_EDIT_FLOAT    0x0040
#define ITEM_FLAGS_EDIT_NEGATIVE  0x0080
#define ITEM_FLAGS_EDIT_OPERATOR  0x0100
#define ITEM_FLAGS_COMBO_EDIT    0x0200
#define ITEM_FLAGS_DATE_ONLY    0x0400
#define ITEM_FLAGS_TIME_ONLY    0x0800
#define ITEM_FLAGS_DATETIME_NONE  0x1000
#define ITEM_FLAGS_PROGRESS_SOLID  0x2000

#define ITEM_IMAGE_NONE      -1
#define ITEM_IMAGE_DOWN      0
#define ITEM_IMAGE_UP      1


#define HITTEST_FLAG_NONE    0x0000
#define HITTEST_FLAG_HEADER_DIVIDER  0x0001
#define HITTEST_FLAG_HEADER_LEFT  0x0002
#define HITTEST_FLAG_HEADER_RIGHT  0x0004

#define LCN_FIRST     (0U - 1500U )
#define LCN_LAST     (0U - 1600U )

#define LCN_SELECTED     (LCN_FIRST - 1 )
#define LCN_LEFTCLICK     (LCN_FIRST - 2 )
#define LCN_RIGHTCLICK     (LCN_FIRST - 3 )
#define LCN_DBLCLICK     (LCN_FIRST - 4 )
#define LCN_ENDEDIT     (LCN_FIRST - 5 )
#define LCN_MODIFIED     (LCN_FIRST - 6 )
#define LCN_HYPERLINK     (LCN_FIRST - 7 )

#define _SIMPLECOLUMN_

enum filestatus {readyforupload,uploading,uploadstopped,finishedupload};
 
#ifndef _SIMPLECOLUMN_
enum FileColumns{colStatus, colFileName, colSize, colProgess, colSpeed,colTimeremaing, colTimepassed, colFileType,colLink};
#else
enum FileColumns{colStatus, colFileName, colSize, colProgess, colSpeed,colLink};
#endif

struct CListNotify{
  NMHDR m_hdrNotify;
  int m_nItem;
  int m_nSubItem;
  TCHAR m_nExitChar;
  LPCTSTR m_lpszItemText;
  LPSYSTEMTIME m_lpItemDate;
  POINT m_cp;// simonwoo added - record the coordinate of mouse when click down
};

template <class T, class TEqual=CSimpleArrayEqualHelper<T> >
class CListArray: public CSimpleArray<T, TEqual>{
public:
  void Reverse(){
    reverse( m_aT, m_aT + m_nSize);
  }

  void Sort(){
    sort( m_aT, m_aT + m_nSize);
  }

  template< class Pred > inline void Sort( Pred Compare){
    sort( m_aT, m_aT + m_nSize, Compare);
  }

  BOOL IsEmpty(){
    return(GetSize() == 0);
  }

  BOOL InsertAt( int nIndex, const T& t){
    if(nIndex < 0 )
      return FALSE;
    if(nIndex >= m_nSize )
      return(Add( t)!= -1);
    if(m_nSize == m_nAllocSize){
      int nNewAllocSize =(m_nAllocSize == 0)? 1 :(m_nSize * 2);
      T* aT = (T*)realloc( m_aT, nNewAllocSize * sizeof( T));
      if(aT == NULL )
        return FALSE;
      m_nAllocSize = nNewAllocSize;
      m_aT = aT;
    }
    MoveMemory((LPVOID)( m_aT + nIndex + 1 ), (LPVOID)( m_aT + nIndex ),(m_nSize - nIndex)* sizeof( T));
    InternalSetAtIndex( nIndex, t);
    m_nSize++;
    return TRUE;
  }
};
