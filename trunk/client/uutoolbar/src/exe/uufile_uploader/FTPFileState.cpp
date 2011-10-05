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

////////////////////////////////////////////////////////////////////////////////
// 
// Copyright (c) 2004 Thomas Oswald
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FTPFileState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace nsFTP;

CFTPFileStatus::CFTPFileStatus() :
   m_fTryCwd(false),
   m_fTryRetr(false),
   m_enSizeType(stUnknown),
   m_lSize(-1),
   m_enModificationTimeType(mttUnknown),
   m_mtime(0),
   m_enIDType(idUnknown)
{
}

CFTPFileStatus::CFTPFileStatus(const CFTPFileStatus& src) :
   m_strName(src.m_strName),
   m_strPath(src.m_strPath),
   m_fTryCwd(src.m_fTryCwd),
   m_fTryRetr(src.m_fTryRetr),
   m_enSizeType(src.m_enSizeType),
   m_lSize(src.m_lSize),
   m_enModificationTimeType(src.m_enModificationTimeType),
   m_mtime(src.m_mtime),
   m_strAttributes(src.m_strAttributes),
   m_strUID(src.m_strUID),
   m_strGID(src.m_strGID),
   m_strLink(src.m_strLink),
   m_enIDType(src.m_enIDType),
   m_strID(src.m_strID)
#ifdef _DEBUG
   ,m_strMTime(src.m_strMTime)
#endif
{
}

CFTPFileStatus::~CFTPFileStatus()
{
}

CFTPFileStatus& CFTPFileStatus::operator=(const CFTPFileStatus& rhs)
{
   if( &rhs == this )
 return *this;

   m_strName  = rhs.m_strName;
   m_strPath  = rhs.m_strPath;
   m_fTryCwd  = rhs.m_fTryCwd;
   m_fTryRetr = rhs.m_fTryRetr;
   m_enSizeType   = rhs.m_enSizeType;
   m_lSize   = rhs.m_lSize;
   m_enModificationTimeType = rhs.m_enModificationTimeType;
   m_mtime   = rhs.m_mtime;
   m_strAttributes = rhs.m_strAttributes;
   m_strUID   = rhs.m_strUID;
   m_strGID   = rhs.m_strGID;
   m_strLink  = rhs.m_strLink;
   m_enIDType = rhs.m_enIDType;
   m_strID   = rhs.m_strID;
#ifdef _DEBUG
   m_strMTime = rhs.m_strMTime;
#endif

   return *this;
}

bool CFTPFileStatus::operator==(const CFTPFileStatus& rhs) const
{
   return m_strName  == rhs.m_strName  &&
 m_strPath  == rhs.m_strPath  &&
 m_fTryCwd  == rhs.m_fTryCwd  &&
 m_fTryRetr == rhs.m_fTryRetr &&
 m_enSizeType   == rhs.m_enSizeType   &&
 m_lSize   == rhs.m_lSize   &&
 m_enModificationTimeType == rhs.m_enModificationTimeType &&
 m_mtime   == rhs.m_mtime   &&
 m_strAttributes == rhs.m_strAttributes &&
 m_strUID   == rhs.m_strUID   &&
 m_strGID   == rhs.m_strGID   &&
 m_strLink  == rhs.m_strLink  &&
 m_enIDType == rhs.m_enIDType &&
#ifdef _DEBUG
 m_strMTime == rhs.m_strMTime &&
#endif
 m_strID   == rhs.m_strID;
}

bool CFTPFileStatus::operator!=(const CFTPFileStatus& rhs) const
{
   return !operator==(rhs);
}

void CFTPFileStatus::Reset()
{
   m_strName.erase();
   m_strPath.erase();
   m_fTryCwd  = false;
   m_fTryRetr = false;
   m_enSizeType   = stUnknown;
   m_lSize   = -1;
   m_enModificationTimeType = mttUnknown;
   m_mtime   = 0;
   m_strAttributes.erase();
   m_strUID.erase();
   m_strGID.erase();
   m_strLink.erase();
   m_enIDType = idUnknown;
   m_strID.erase();
#ifdef _DEBUG
   m_strMTime.erase();
#endif
}
