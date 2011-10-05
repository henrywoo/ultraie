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

#include "6bees_ftpdatatypes.h"

using namespace _6bees_socket;

/// returns the string which is used for display
std::wstring CFirewallType::AsDisplayString() const
{
  switch( m_enFirewallType )
  {
  case ftNone: return _T("no firewall");
  case ftSiteHostName:  return _T("SITE hostname");
  case ftUserAfterLogon: return _T("USER after logon");
  case ftProxyOpen: return _T("proxy OPEN");
  case ftTransparent:   return _T("Transparent");
  case ftUserWithNoLogon:    return _T("USER with no logon");
  case ftUserFireIDatRemotehost:  return _T("USER fireID@remotehost");
  case ftUserRemoteIDatRemoteHostFireID:    return _T("USER remoteID@remotehost fireID");
  case ftUserRemoteIDatFireIDatRemoteHost:  return _T("USER remoteID@fireID@remotehost");
  }
  return _T("");
}

/// return the string which is used for storage (e.g. in an XML- or INI-file)
std::wstring CFirewallType::AsStorageString() const{
  switch( m_enFirewallType ){
  case ftNone: return _T("NO_FIREWALL");
  case ftSiteHostName:  return _T("SITE_HOSTNAME");
  case ftUserAfterLogon: return _T("USER_AFTER_LOGON");
  case ftProxyOpen: return _T("PROXY_OPEN");
  case ftTransparent:   return _T("TRANSPARENT");
  case ftUserWithNoLogon:    return _T("USER_WITH_NO_LOGON");
  case ftUserFireIDatRemotehost:  return _T("USER_FIREID@REMOTEHOST");
  case ftUserRemoteIDatRemoteHostFireID:    return _T("USER_REMOTEID@REMOTEHOST_FIREID");
  case ftUserRemoteIDatFireIDatRemoteHost:  return _T("USER_REMOTEID@FIREID@REMOTEHOST");
  }
  return _T("");
}

/// returns all available firewall types
void CFirewallType::GetAllTypes(TFirewallTypeVector& vTypes)
{
  vTypes.resize(9);
  vTypes[0] = ftNone;
  vTypes[1] = ftSiteHostName;
  vTypes[2] = ftUserAfterLogon;
  vTypes[3] = ftProxyOpen;
  vTypes[4] = ftTransparent;
  vTypes[5] = ftUserWithNoLogon;
  vTypes[6] = ftUserFireIDatRemotehost;
  vTypes[7] = ftUserRemoteIDatRemoteHostFireID;
  vTypes[8] = ftUserRemoteIDatFireIDatRemoteHost;
}


CLogonInfo::CLogonInfo() :
m_ushHostport(FTP_PORT_DEFAULT),
m_strUsername(ANONYMOUS_USER),
m_ushFwPort(FTP_PORT_DEFAULT),
m_FwType(CFirewallType::None())
{
}

CLogonInfo::CLogonInfo(const std::wstring& strHostname,
                       unsigned short ushHostport,
                       const std::wstring& strUsername,
                       const std::wstring& strPassword,
                       const std::wstring& strAccount) :
m_strHostname(strHostname),
m_ushHostport(ushHostport),
m_strUsername(strUsername),
m_strPassword(strPassword),
m_strAccount(strAccount),
m_ushFwPort(FTP_PORT_DEFAULT),
m_FwType(CFirewallType::None())
{}

void CLogonInfo::SetHost(const std::wstring& strHostname,
                         unsigned short ushHostport,
                         const std::wstring& strUsername,
                         const std::wstring& strPassword,
                         const std::wstring& strAccount)
{
  m_strHostname  = strHostname;
  m_ushHostport  = ushHostport;
  m_strUsername  = strUsername;
  m_strPassword  = strPassword;
  m_strAccount   = strAccount;
}

void CLogonInfo::SetFirewall(const std::wstring& strFwHostname,
                             const std::wstring& strFwUsername,
                             const std::wstring& strFwPassword,
                             unsigned short ushFwPort,
                             const CFirewallType& crFwType)
{
  m_strFwHostname   = strFwHostname;
  m_strFwUsername   = strFwUsername;
  m_strFwPassword   = strFwPassword;
  m_ushFwPort  = ushFwPort;
  m_FwType = crFwType;
}
