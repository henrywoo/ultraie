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

#include "ListCtrl.h"
#include "stdafx.h"
/***********************************************************************************************************************/

class CUserProfile
{
public:
	CUserProfile()
	{
		m_bAdministrator = FALSE;
		m_strUserName = _T( "" );
		m_strDepartment = _T( "" );
		m_strPhoneNumber = _T( "" );
	}
	
	CUserProfile( BOOL bAdministrator, LPCTSTR lpszUserName, LPCTSTR lpszDepartment, LPCTSTR lpszPhoneNumber )
	{
		m_bAdministrator = bAdministrator;
		m_strUserName = lpszUserName;
		m_strDepartment = lpszDepartment;
		m_strPhoneNumber = lpszPhoneNumber;
	}

	BOOL m_bAdministrator;
	CString m_strUserName;
	CString m_strDepartment;
	CString m_strPhoneNumber;
};



/***********************************************************************************************************************/
class CListUserData:
	public CListImpl< CListUserData >
{
public:
	DECLARE_WND_CLASS( _T( "UserList" ) )

protected:
	CImageList m_ilItemImages;
	CListArray < CUserProfile > m_aUsers;
	enum UserColumns { colName, colDept, colPhone };

public:
	BOOL Initialise()
	{
		if ( !m_ilItemImages.CreateFromImage( IDB_EXAMPLE, 16, 0, RGB( 255, 0, 255 ), IMAGE_BITMAP, LR_CREATEDIBSECTION ) )
			return FALSE;
			
		SetImageList( m_ilItemImages );
		
		AddColumn( _T( "User Name" ), 150 );
		AddColumn( _T( "Department" ), 100 );
		AddColumn( _T( "Phone Number" ), 100 );
		
		return CListImpl< CListUserData >::Initialise();
	}
	
	int AddUser( CUserProfile userProfile )
	{
		m_aUsers.Add( userProfile );
		return CListImpl< CListUserData >::AddItem() ? GetItemCount() - 1 : -1;
	}
	
	int AddUser( BOOL bAdministrator, LPCTSTR lpszUserName, LPCTSTR lpszDepartment, LPCTSTR lpszPhoneNumber )
	{
		return AddUser( CUserProfile( bAdministrator, lpszUserName, lpszDepartment, lpszPhoneNumber ) );
	}
	
	int GetItemCount() // required by CListImpl
	{
		return m_aUsers.GetSize();
	}
	
	BOOL GetUser( int nItem, CUserProfile& userProfile )
	{
		if ( nItem < 0 || nItem >= GetItemCount() ) 
			return FALSE;
		userProfile = m_aUsers[ nItem ];
		return TRUE;
	}
	
	CString GetItemText( int nItem, int nSubItem ) // required by CListImpl
	{
		CUserProfile userProfile;
		if ( !GetUser( nItem, userProfile ) )
			return _T( "" );
		switch ( (UserColumns)nSubItem )
		{
			case colName:	return userProfile.m_strUserName;
			case colDept:	return userProfile.m_strDepartment;
			case colPhone:	return userProfile.m_strPhoneNumber;
		}
		return _T( "" );
	}
	
	int GetItemImage( int nItem, int nSubItem ) // overrides CListImpl::GetItemImage
	{
		if ( (UserColumns)nSubItem != colName )
			return -1;
		CUserProfile userProfile;
		if ( !GetUser( nItem, userProfile ) )
			return -1;
		return userProfile.m_bAdministrator ? 6 : 0;
	}
	
	void ReverseItems() // overrides CListImpl::ReverseItems
	{
		m_aUsers.Reverse();
	}
	
	class CompareItem
	{
	public:
		CompareItem( UserColumns colColumn ) : m_colColumn( colColumn ) {}
		inline bool operator() ( const CUserProfile& userProfile1, const CUserProfile& userProfile2 )
		{
			switch ( m_colColumn )
			{
				case colName:	return ( userProfile1.m_strUserName.Compare( userProfile2.m_strUserName ) < 0 );
				case colDept:	return ( userProfile1.m_strDepartment.Compare( userProfile2.m_strDepartment ) < 0 );
				case colPhone:	return ( userProfile1.m_strPhoneNumber.Compare( userProfile2.m_strPhoneNumber ) < 0 );
				
			}
			return false;
		}
		
	protected:
		UserColumns m_colColumn;
	};
	
	void SortItems( int nColumn, BOOL bAscending ) // overrides CListImpl::SortItems
	{
		m_aUsers.Sort( CompareItem( (UserColumns)nColumn ) );
	}

	int GetItemProgress( int nItem, int nSubItem )
	{
		return 0;
	}
};

/***********************************************************************************************************************/