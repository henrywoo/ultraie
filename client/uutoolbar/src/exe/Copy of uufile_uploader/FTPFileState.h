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

#ifndef INC_FTPFILESTATE_H
#define INC_FTPFILESTATE_H

#include <string>
#include <vector>
#include "rc_smart_ptr.h"
#include "Define6BeeType.h"

namespace nsFTP
{

	class CFTPFileStatus;
	typedef nsSP::RCIPtr<CFTPFileStatus>   TSpFTPFileStatus;
	typedef std::vector<TSpFTPFileStatus>  TSpFTPFileStatusVector;

	/// Holds status information about a file on the ftp server.
	class CFTPFileStatus
	{
	public:
		enum T_enSizeType {
			stUnknown,
			stBinary,   ///< size is the number of octets in TYPE I
			stASCII,    ///< size is the number of octets in TYPE A
		};

		/// When a time zone is unknown, it is assumed to be GMT. You may want
		/// to use localtime() for LOCAL times, along with an indication that the
		/// time is correct in the local time zone, and gmtime() for REMOTE* times.
		enum T_enMTimeType {
			mttUnknown,
			mttLocal,   ///< time is correct
			mttRemoteMinute, ///< time zone and secs are unknown
			mttRemoteDay,    ///< time zone and time of day are unknown
		};

		enum T_enIDType {
			idUnknown,
			idFull, ///< unique identifier for files on this FTP server
		};

		friend class CFTPListParse;

	public:
		CFTPFileStatus();
		CFTPFileStatus(const CFTPFileStatus& src);
		~CFTPFileStatus();

		CFTPFileStatus& operator=(const CFTPFileStatus& rhs);
		bool operator==(const CFTPFileStatus& rhs) const;
		bool operator!=(const CFTPFileStatus& rhs) const;

	public:
		void Reset();

		const std::wstring& Name()  const { return m_strName;  };
		const std::wstring& Path()  const { return m_strPath;  };
		bool  IsCwdPossible()  const { return m_fTryCwd;  };
		bool  IsRetrPossible() const { return m_fTryRetr; };
		T_enSizeType   SizeType()  const { return m_enSizeType;   };
		long  Size()  const { return m_lSize;   };
		T_enMTimeType  MTimeType() const { return m_enModificationTimeType; };
		time_t    MTime() const { return m_mtime;   };
		const std::wstring& Attributes() const { return m_strAttributes; };
		const std::wstring& UID()  const { return m_strUID;   };
		const std::wstring& GID()  const { return m_strGID;   };
		const std::wstring& Link()  const { return m_strLink;  };
		T_enIDType IDType()    const { return m_enIDType; };
		const std::wstring& ID()   const { return m_strID;   };
		bool  IsDot() const { return m_strName==_T(".") || m_strName==_T(".."); };
#ifdef _DEBUG
		const std::wstring& MTimeStr()  const { return m_strMTime; };
#endif

		std::wstring&  Name()    { return m_strName;  };
		std::wstring&  Path()    { return m_strPath;  };
		bool& CwdPossible()  { return m_fTryCwd;  };
		bool& RetrPossible() { return m_fTryRetr; };
		T_enSizeType&  SizeType() { return m_enSizeType;   };
		long& Size()    { return m_lSize;   };
		T_enMTimeType& MTimeType()    { return m_enModificationTimeType; };
		time_t&   MTime()   { return m_mtime;   };
		std::wstring&  Attributes()   { return m_strAttributes; };
		std::wstring&  UID() { return m_strUID;   };
		std::wstring&  GID() { return m_strGID;   };
		std::wstring&  Link()    { return m_strLink;  };
		T_enIDType&    IDType()  { return m_enIDType; };
		std::wstring&  ID()  { return m_strID;   };
#ifdef _DEBUG
		std::wstring&  MTimeStr() { return m_strMTime; };
#endif

	private:
		std::wstring   m_strName;
		std::wstring   m_strPath;
		bool  m_fTryCwd;   ///< false if cwd is definitely pointless, true otherwise
		bool  m_fTryRetr;  ///< false if RETR is definitely pointless, true otherwise 
		T_enSizeType   m_enSizeType;
		long  m_lSize;    ///< number of octets
		T_enMTimeType  m_enModificationTimeType;
		time_t    m_mtime;    ///< modification time
		std::wstring   m_strAttributes;  ///< file attributes
		std::wstring   m_strUID;   ///< user identifier (owner of file)
		std::wstring   m_strGID;   ///< group identifier
		std::wstring   m_strLink;   ///< number of links to file
		T_enIDType m_enIDType;  ///< identifier type
		std::wstring   m_strID;    ///< identifier
#ifdef _DEBUG
		std::wstring   m_strMTime;  ///< for debug only
#endif
	};

	/// Function object for sorting the filelists
	class CFTPFileStatusContainerSort
	{
		/// Baseclass for functionobjects
		class CSortCriteria
		{
		public:
			virtual bool Compare(const TSpFTPFileStatus& x, const TSpFTPFileStatus& y) const = 0;
		};

		const CSortCriteria& m_rSortBase;  ///< sortkey
		const bool  m_fAscending; ///< sort order
		const bool  m_fDirFirst;  ///< if true, then directories are before the files
		///< if false, directories and files are mixed up

	public:
		CFTPFileStatusContainerSort(const CSortCriteria& rSortBase, bool fAscending, bool fDirFirst) :
		  m_rSortBase(rSortBase),
			  m_fAscending(fAscending),
			  m_fDirFirst(fDirFirst)
		  {};

		  /// entry function for STL sort algorithm
		  bool operator()(const TSpFTPFileStatus& x, const TSpFTPFileStatus& y) const
		  {
			  if( m_fDirFirst )
			  {
				  if( x->Name().size()<3 || y->Name().size()<3 )
				  {
					  if( x->Name().at(0)==_T('.') && y->Name().at(0)==_T('.') )
						  return m_fAscending && x->Name()==_T(".");
					  else if( x->Name().at(0)==_T('.') )
						  return m_fAscending && true;
					  else if( y->Name().at(0)==_T('.') )
						  return m_fAscending && false;
				  }

				  if( x->IsCwdPossible() != y->IsCwdPossible() )
				  {
					  if( x->IsCwdPossible() && !y->IsCwdPossible() )
						  return true;
					  return false;
				  }
			  }

			  if( m_fAscending )
				  return !m_rSortBase.Compare(x, y);
			  else
				  return m_rSortBase.Compare(x, y);
		  }

		  /// Order by name.
		  class CName : public CSortCriteria
		  {
		  public:
			  virtual bool Compare(const TSpFTPFileStatus& x, const TSpFTPFileStatus& y) const
			  {
				  return x->Name() > y->Name();
			  }
		  };

		  /// Order by size.
		  class CSize : public CSortCriteria
		  {
		  public:
			  virtual bool Compare(const TSpFTPFileStatus& x, const TSpFTPFileStatus& y) const
			  {
				  return x->Size() > y->Size();
			  }
		  };

		  /// Order by modification date.
		  class CMTime : public CSortCriteria
		  {
		  public:
			  virtual bool Compare(const TSpFTPFileStatus& x, const TSpFTPFileStatus& y) const
			  {
				  return x->MTime() > y->MTime();
			  }
		  };

		  /// Order by attributes.
		  class CAttributes : public CSortCriteria
		  {
		  public:
			  virtual bool Compare(const TSpFTPFileStatus& x, const TSpFTPFileStatus& y) const
			  {
				  return x->Attributes() > y->Attributes();
			  }
		  };

		  /// Order by user id.
		  class CUID : public CSortCriteria
		  {
		  public:
			  virtual bool Compare(const TSpFTPFileStatus& x, const TSpFTPFileStatus& y) const
			  {
				  return x->UID() > y->UID();
			  }
		  };

		  /// Order by group id.
		  class CGID : public CSortCriteria
		  {
		  public:
			  virtual bool Compare(const TSpFTPFileStatus& x, const TSpFTPFileStatus& y) const
			  {
				  return x->GID() > y->GID();
			  }
		  };

		  /// Order by link.
		  class CLink : public CSortCriteria
		  {
		  public:
			  virtual bool Compare(const TSpFTPFileStatus& x, const TSpFTPFileStatus& y) const
			  {
				  return x->Link() > y->Link();
			  }
		  };

		  /// Order by id.
		  class CID : public CSortCriteria
		  {
		  public:
			  virtual bool Compare(const TSpFTPFileStatus& x, const TSpFTPFileStatus& y) const
			  {
				  return x->ID() > y->ID();
			  }
		  };
	};

}
#endif // INC_FTPFILESTATE_H
