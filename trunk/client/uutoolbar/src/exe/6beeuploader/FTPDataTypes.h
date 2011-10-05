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

#ifndef INC_FTPDATATYPES_H
#define INC_FTPDATATYPES_H

#include <vector>
#include "Define6BeeType.h"
#include "time.h" // for clock()

namespace nsFTP{

	typedef std::vector<char> TByteVector;

	// constants
	const TCHAR ANONYMOUS_USER[] = _T("anonymous");
	enum ftpconsts {
		FTP_PORT_DEFAULT = 21, ///< The default port that an FTP service listens to on a remote host
		FTP_ERROR  = -1,
		FTP_OK =  0,
		FTP_NOTOK  =  1
  };

		/// Transmission Modes
		class CTransferMode
		{
		public:
			enum T_enTransferMode { tmStream, tmBlock, tmCompressed };

			CTransferMode(const CTransferMode& transferMode) :
			m_enTransferMode(transferMode.AsEnum()) {}

			bool operator==(const T_enTransferMode& rhs) const { return m_enTransferMode==rhs; }
			bool operator!=(const T_enTransferMode& rhs) const { return m_enTransferMode!=rhs; }
			bool operator==(const CTransferMode& rhs) const { return m_enTransferMode==rhs.AsEnum(); }
			bool operator!=(const CTransferMode& rhs) const { return m_enTransferMode!=rhs.AsEnum(); }

			CTransferMode& operator=(const CTransferMode& rhs) {
        m_enTransferMode = rhs.AsEnum();
        return *this;
      }

			T_enTransferMode AsEnum() const { return m_enTransferMode; }

			static const CTransferMode Stream() { return tmStream; }
			static const CTransferMode Block() { return tmBlock; }
			static const CTransferMode Compressed() { return tmCompressed; }

		private:
			CTransferMode(T_enTransferMode enTransferMode) : m_enTransferMode(enTransferMode) {}
			T_enTransferMode m_enTransferMode;
		};

		class CFirewallType;
		typedef std::vector<CFirewallType> TFirewallTypeVector;

		/// Firewall Type
		class CFirewallType
		{
		public:
			// don't change order of enumeration
			enum T_enFirewallType {
				ftNone, ftSiteHostName, ftUserAfterLogon, ftProxyOpen, ftTransparent,
				ftUserWithNoLogon, ftUserFireIDatRemotehost, ftUserRemoteIDatRemoteHostFireID, 
				ftUserRemoteIDatFireIDatRemoteHost };

				CFirewallType() : m_enFirewallType(ftNone) {}
				CFirewallType(const CFirewallType& firewallType) :
				m_enFirewallType(firewallType.AsEnum()) {}

				bool operator==(const T_enFirewallType& rhs) const { return m_enFirewallType==rhs; }
				bool operator!=(const T_enFirewallType& rhs) const { return m_enFirewallType!=rhs; }
				bool operator==(const CFirewallType& rhs) const { return m_enFirewallType==rhs.AsEnum(); }
				bool operator!=(const CFirewallType& rhs) const { return m_enFirewallType!=rhs.AsEnum(); }

				CFirewallType& operator=(const CFirewallType& rhs) { m_enFirewallType = rhs.AsEnum(); return *this; }

				T_enFirewallType AsEnum() const { return m_enFirewallType; }

				std::wstring AsDisplayString() const;
				std::wstring AsStorageString() const;
				static void GetAllTypes(TFirewallTypeVector& vTypes);

				static const CFirewallType None()    { return ftNone;    }
				static const CFirewallType SiteHostName() { return ftSiteHostName; }
				static const CFirewallType UserAfterLogon()    { return ftUserAfterLogon;    }
				static const CFirewallType ProxyOpen()    { return ftProxyOpen;    }
				static const CFirewallType Transparent()  { return ftTransparent;  }
				static const CFirewallType UserWithNoLogon()   { return ftUserWithNoLogon;   }
				static const CFirewallType UserFireIDatRemotehost()  { return ftUserFireIDatRemotehost;  }
				static const CFirewallType UserRemoteIDatRemoteHostFireID()   { return ftUserRemoteIDatRemoteHostFireID;   }
				static const CFirewallType UserRemoteIDatFireIDatRemoteHost() { return ftUserRemoteIDatFireIDatRemoteHost; }

		private:
			CFirewallType(T_enFirewallType enFirewallType) : m_enFirewallType(enFirewallType) {}
			T_enFirewallType m_enFirewallType;
		};

		/// @brief Representation Type - 1st param (see CRepresentation)
    class CType
    {
    public:
 enum T_enType { tyASCII, tyEBCDIC, tyImage, tyLocalByte };

 CType(const CType& type) :
 m_enType(type.AsEnum()) {}

 bool operator==(const T_enType& rhs) const { return m_enType==rhs; }
 bool operator!=(const T_enType& rhs) const { return m_enType!=rhs; }
 bool operator==(const CType& rhs) const { return m_enType==rhs.AsEnum(); }
 bool operator!=(const CType& rhs) const { return m_enType!=rhs.AsEnum(); }

 CType& operator=(const CType& rhs) { m_enType = rhs.AsEnum(); return *this; }

 T_enType AsEnum() const { return m_enType; }

 static const CType ASCII() { return tyASCII; }
 static const CType EBCDIC()    { return tyEBCDIC;    }
 static const CType Image() { return tyImage; }
 static const CType LocalByte() { return tyLocalByte; }

    private:
 CType(T_enType enType) : m_enType(enType) {}
 T_enType m_enType;
    };

		/// @brief Representation Type - 2nd param (see CRepresentation)
		class CTypeFormat
		{
		public:
			enum T_enTypeFormat { tfNonPrint, tfTelnetFormat, tfCarriageControl };

			CTypeFormat(const CTypeFormat& typeFormat) :
			m_enTypeFormat(typeFormat.AsEnum()) {}

			bool operator==(const T_enTypeFormat& rhs) const { return m_enTypeFormat==rhs; }
			bool operator!=(const T_enTypeFormat& rhs) const { return m_enTypeFormat!=rhs; }
			bool operator==(const CTypeFormat& rhs) const { return m_enTypeFormat==rhs.AsEnum(); }
			bool operator!=(const CTypeFormat& rhs) const { return m_enTypeFormat!=rhs.AsEnum(); }

			CTypeFormat& operator=(const CTypeFormat& rhs) { m_enTypeFormat = rhs.AsEnum(); return *this; }

			T_enTypeFormat AsEnum() const { return m_enTypeFormat; }

			static const CTypeFormat NonPrint()   { return tfNonPrint;   }
			static const CTypeFormat TelnetFormat()    { return tfTelnetFormat;    }
			static const CTypeFormat CarriageControl() { return tfCarriageControl; }

		private:
			CTypeFormat(T_enTypeFormat enTypeFormat) : m_enTypeFormat(enTypeFormat) {}
			T_enTypeFormat m_enTypeFormat;
		};

		/// Representation Type (see also CType and CTypeFormat)
		class CRepresentation
		{
		public:
			/// there is only NonPrint as FormatType supported yet
			CRepresentation(CType Type) : m_Type(Type), m_Format(CTypeFormat::NonPrint()) {}

			bool operator!=(const CRepresentation& rep1) const { return rep1.m_Type != m_Type || rep1.m_Format != m_Format; }
			bool operator==(const CRepresentation& rep1) const { return rep1.m_Type == m_Type && rep1.m_Format == m_Format; }
			CRepresentation& operator=(const CRepresentation& rhs)
			{ 
				m_Type = rhs.m_Type;
				m_Format = rhs.m_Format;
				return *this;
			}

			const CType&  Type()   const { return m_Type; }
			const CTypeFormat& Format() const { return m_Format; }

		private:
			CType  m_Type;
			CTypeFormat m_Format;
		};

		/// @brief Represents ftp-commands that use the data port (usually port 20).
		class CDatachannelCmd
		{
		public:
			enum T_enDatachannelCmd { cmdLIST, cmdNLST, cmdRETR, cmdSTOR, cmdSTOU, cmdAPPE };

			CDatachannelCmd(const CDatachannelCmd& datachannelCmd) :
			m_enDatachannelCmd(datachannelCmd.AsEnum()) {}

			bool operator==(T_enDatachannelCmd rhs) const { return m_enDatachannelCmd==rhs; }
			bool operator!=(T_enDatachannelCmd rhs) const { return m_enDatachannelCmd!=rhs; }

			bool operator==(const CDatachannelCmd& rhs) const { return m_enDatachannelCmd==rhs.AsEnum(); }
			bool operator!=(const CDatachannelCmd& rhs) const { return m_enDatachannelCmd!=rhs.AsEnum(); }

			CDatachannelCmd& operator=(const CDatachannelCmd& rhs)
			{
				m_enDatachannelCmd = rhs.AsEnum();
				return *this;
			}

			T_enDatachannelCmd AsEnum() const { return m_enDatachannelCmd; }

			static const CDatachannelCmd LIST() { return cmdLIST; }
			static const CDatachannelCmd NLST() { return cmdNLST; }
			static const CDatachannelCmd RETR() { return cmdRETR; }
			static const CDatachannelCmd STOR() { return cmdSTOR; }
			static const CDatachannelCmd STOU() { return cmdSTOU; }
			static const CDatachannelCmd APPE() { return cmdAPPE; }

		private:
			CDatachannelCmd(T_enDatachannelCmd enDatachannelCmd) : m_enDatachannelCmd(enDatachannelCmd) {}
			T_enDatachannelCmd m_enDatachannelCmd;
		};

		/// @brief Structure for logon information.
		///
		/// Holds all necessary parameters for logging on a ftp-server.
		/// Includes also the parameters which are needed for firewall logon.
		class CLogonInfo{
		public:
			CLogonInfo();
			explicit CLogonInfo(const std::wstring& strHostname,
   unsigned short ushHostport=FTP_PORT_DEFAULT,
   const std::wstring& strUsername=ANONYMOUS_USER,
   const std::wstring& strPassword=_T(""),
   const std::wstring& strAccount=_T(""));

			void SetHost(const std::wstring& strHostname,
   unsigned short ushHostport=FTP_PORT_DEFAULT,
   const std::wstring& strUsername=ANONYMOUS_USER, 
				const std::wstring& strPassword=_T("anonymous@user.com"),
   const std::wstring& strAccount=_T(""));

			void SetFirewall(const std::wstring& strFwHostname,
   const std::wstring& strFwUsername,
   const std::wstring& strFwPassword,
				unsigned short ushFwPort,
   const CFirewallType& crFwType);

			void DisableFirewall() { m_FwType = CFirewallType::None(); }

			const std::wstring&  Hostname()   const  { return m_strHostname;    }
			unsigned short Hostport()   const  { return m_ushHostport;    }
			const std::wstring&  Username()   const  { return m_strUsername;    }
			const std::wstring&  Password()   const  { return m_strPassword;    }
			const std::wstring&  Account()    const  { return m_strAccount; }
			const std::wstring&  FwHost() const  { return m_strFwHostname;  }
			const std::wstring&  FwUsername() const  { return m_strFwUsername;  }
			const std::wstring&  FwPassword() const  { return m_strFwPassword;  }
			unsigned short FwPort() const  { return m_ushFwPort; }
			const CFirewallType& FwType() const  { return m_FwType;    }

		private:
			std::wstring   m_strHostname;   ///< name or ip-address of the ftp-server
			unsigned short    m_ushHostport;   ///< port of the ftp-server
			std::wstring   m_strUsername;   ///< username for ftp-server
			std::wstring   m_strPassword;   ///< password for ftp-server
			std::wstring   m_strAccount;    ///< account mostly needed on ftp-servers running on unix/linux
			std::wstring   m_strFwHostname; ///< name or ip-address of the firewall
			std::wstring   m_strFwUsername; ///< username for firewall
			std::wstring   m_strFwPassword; ///< password for firewall
			unsigned short    m_ushFwPort; ///< port of the firewall
			CFirewallType  m_FwType;   ///< type of firewall
		};

		/// Holds a response of a ftp-server.
		class CReply{
			std::wstring m_strResponse;

			/// Holds the reply code.
			class CCode{
				enum{ csize = 4 };
				TCHAR m_szCode[csize];
			public:
				CCode(){
					//std::fill_n(m_szCode, sizeof(m_szCode)/sizeof(TCHAR), 0);
					for (int i=0;i<csize;i++){
						m_szCode[i] = 0x00;
					}
				}
				LPCTSTR Value() const { return m_szCode; }
				bool Set(const std::wstring& strCode)
				{
					if(strCode.length()!=3||strCode[0]<_T('1')||strCode[0]>_T('5')||strCode[1]<_T('0')||strCode[1]>_T('5'))
					{
						for (int i=0;i<csize;i++){
							m_szCode[i] = 0x00;
						}
						return false;
					}
					//std::copy(strCode.begin(), strCode.end(), m_szCode);
					for(int i=0;i<(int)strCode.size() && i<csize;i++){
						m_szCode[i] = strCode[i];
					}
					return true;
				}

				bool IsPositiveReply() const { return IsPositivePreliminaryReply() || IsPositiveCompletionReply() || IsPositiveIntermediateReply(); }
				bool IsNegativeReply() const { return IsTransientNegativeCompletionReply() || IsPermanentNegativeCompletionReply(); }

				bool IsPositivePreliminaryReply() const    { return m_szCode[0] == _T('1'); }
				bool IsPositiveCompletionReply() const { return m_szCode[0] == _T('2'); }
				bool IsPositiveIntermediateReply() const   { return m_szCode[0] == _T('3'); }
				bool IsTransientNegativeCompletionReply() const { return m_szCode[0] == _T('4'); }
				bool IsPermanentNegativeCompletionReply() const { return m_szCode[0] == _T('5'); }

				bool IsRefferingToSyntax() const  { return m_szCode[1] == _T('0'); }
				bool IsRefferingToInformation() const   { return m_szCode[1] == _T('1'); }
				bool IsRefferingToConnections() const   { return m_szCode[1] == _T('2'); }
				bool IsRefferingToAuthenticationAndAccounting() const { return m_szCode[1] == _T('3'); }
				bool IsRefferingToUnspecified() const   { return m_szCode[1] == _T('4'); }
				bool IsRefferingToFileSystem() const   { return m_szCode[1] == _T('5'); }
			} m_Code;
		public:
			bool Set(const std::wstring& strResponse){
				m_strResponse = strResponse;
				if( m_strResponse.length()>2 )
					return m_Code.Set(m_strResponse.substr(0, 3));
				return false;
			}
			const std::wstring& Value() const { return m_strResponse; }
			const CCode& Code() const { return m_Code; }
		};
}

#endif // INC_FTPDATATYPES_H
