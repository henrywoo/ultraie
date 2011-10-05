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
// From David J. Kruglinski (Inside Visual C++).
//
////////////////////////////////////////////////////////////////////////////////

// removed baseclass CObject
// removed baseclass CException
// removed all LPCTSTR wchar_t*

// needs winsock.h or afxsock.h in the precompiled headers and the following
// statement in InitInstance
// if(!AfxSocketInit())
// {
//    AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
//    return FALSE;
// }

#ifndef INC_BLOCKINGSOCKET_H
#define INC_BLOCKINGSOCKET_H

#include <string>
#include "Define6BeeType.h"
#include "../Stub_Skeleton/6bees_util.h"

#ifndef _WINSOCK2API_ // 没有包含winsock2.h
#define _WINSOCK2API_ // 避免再包含winsock2.h
#include <winsock2.h>
#endif
#pragma comment(lib, "ws2_32.lib")

namespace _6bees_socket{

	typedef const struct sockaddr* LPCSOCKADDR;

	///////////////////////////////////////////////////////////////////////////////////////
	//***************************** CBlockingSocketException  ***************************//
	///////////////////////////////////////////////////////////////////////////////////////
	class CBlockingSocketException
	{
	public:
		// Constructor
		CBlockingSocketException(wchar_t* pchMessage);

	public:
		~CBlockingSocketException() {}
		virtual BOOL GetErrorMessage(wchar_t* lpstrError, UINT nMaxError, PUINT pnHelpContext = NULL);
		virtual std::wstring GetErrorMessage(PUINT pnHelpContext = NULL);

	private:
		int     m_nError;
		std::wstring m_strMessage;
	};

	///////////////////////////////////////////////////////////////////////////////////////
	//*********************************** CSockAddr  ************************************//
	///////////////////////////////////////////////////////////////////////////////////////
	class CSockAddr : public sockaddr_in
	{
	public:
		// constructors
		CSockAddr(){
			sin_family        = AF_INET; // specifies address family
			sin_port          = 0;       // ip port
			sin_addr.s_addr   = 0;       // ip address
		}

		CSockAddr(const SOCKADDR& sa)
		{ memcpy(this, &sa, sizeof(SOCKADDR)); }

		CSockAddr(const SOCKADDR_IN& sin) 
		{ memcpy(this, &sin, sizeof(SOCKADDR_IN)); }

		CSockAddr(ULONG ulAddr, unsigned short ushPort = 0) // parms are host byte ordered
		{ 
			sin_family      = AF_INET;          // specifies address family
			sin_port        = htons(ushPort);   // ip port
			sin_addr.s_addr = htonl(ulAddr);    // ip address
		}

		CSockAddr(const char* pchIP, unsigned short ushPort = 0) // dotted IP addr string
		{ 
			sin_family = AF_INET;               // specifies address family
			sin_port = htons(ushPort);          // ip port
			sin_addr.s_addr = inet_addr(pchIP); // ip address
		} // already network byte ordered

		// Return the address in dotted-decimal format
    std::wstring DottedDecimal(){
      wstring tmp;
      toWideString(inet_ntoa(sin_addr),tmp);
      return tmp;
    }

		// Get port and address (even though they're public)
		unsigned short Port() const
		{ return ntohs(sin_port); }

		ULONG IPAddr() const
		{ return ntohl(sin_addr.s_addr); }

		// operators added for efficiency
		CSockAddr& operator=(const SOCKADDR& sa)
		{
			memcpy(this, &sa, sizeof(SOCKADDR));
			return *this;
		}

		CSockAddr& operator=(const SOCKADDR_IN& sin)
		{ 
			memcpy(this, &sin, sizeof(SOCKADDR_IN));
			return *this; 
		}

		operator SOCKADDR()
		{ return *((LPSOCKADDR) this); }

		operator LPSOCKADDR()
		{ return (LPSOCKADDR) this; }

		operator LPSOCKADDR_IN()
		{ return (LPSOCKADDR_IN) this; }
	};

	///////////////////////////////////////////////////////////////////////////////////////
	//********************************* CBlockingSocket  ********************************//
	///////////////////////////////////////////////////////////////////////////////////////

	class IBlockingSocket
	{
	public:
		virtual ~IBlockingSocket() {};
		virtual IBlockingSocket* CreateInstance() const = 0;
		virtual void Create(int nType = SOCK_STREAM) = 0;
		virtual void Connect(LPCSOCKADDR psa) const = 0;
		virtual void Bind(LPCSOCKADDR psa) const = 0;
		virtual void Listen() const = 0;
		virtual void Cleanup() = 0;
		virtual bool Accept(IBlockingSocket& s, LPSOCKADDR psa) const = 0;
		virtual void Close() = 0;
		virtual int  Write(const char* pch, int nSize, int nSecs,bool terminatethisthread) const = 0;
		virtual int  Receive(char* pch, int nSize, int nSecs) const = 0;
		virtual void GetSockAddr(LPSOCKADDR psa) const = 0;
		virtual operator SOCKET() const = 0; //+#
		virtual bool CheckReadability() const = 0;
		virtual CSockAddr GetHostByName(const char* pchName, unsigned short ushPort = 0) = 0;
		virtual const char* GetHostByAddr(LPCSOCKADDR psa) = 0;
	};

	// member functions truly block and must not be used in UI threads
	// use this class as an alternative to the MFC CSocket class
	class CBlockingSocket : public IBlockingSocket
	{
	public:
		CBlockingSocket() :
		  m_hSocket(0){}

		  virtual IBlockingSocket* CreateInstance() const;
		  ~CBlockingSocket(){
			  Cleanup();
		  }
		  void Cleanup();
		  void Create(int nType = SOCK_STREAM);
		  void Close();

		  void Bind(LPCSOCKADDR psa) const;
		  void Listen() const;
		  void Connect(LPCSOCKADDR psa) const;
		  bool Accept(IBlockingSocket& s, LPSOCKADDR psa) const;
		  int  Send(const char* pch, int nSize, int nSecs) const;
		  int  Write(const char* pch, int nSize, int nSecs ,bool terminatethisthread) const;
		  int  Receive(char* pch, int nSize, int nSecs) const;
		  int  SendDatagram(const char* pch, int nSize, LPCSOCKADDR psa, int nSecs) const;
		  int  ReceiveDatagram(char* pch, int nSize, LPSOCKADDR psa, int nSecs) const;
		  void GetPeerAddr(LPSOCKADDR psa) const;
		  void GetSockAddr(LPSOCKADDR psa) const;
		  bool CheckReadability() const;

		  CSockAddr   GetHostByName(const char* pchName, unsigned short ushPort = 0);
		  const char* GetHostByAddr(LPCSOCKADDR psa);

		  operator SOCKET() const { return m_hSocket; }

	private:
		SOCKET m_hSocket;
	};

	IBlockingSocket* CreateDefaultBlockingSocketInstance();

	///////////////////////////////////////////////////////////////////////////////////////
	//******************************** CHttpBlockingSocket ******************************//
	///////////////////////////////////////////////////////////////////////////////////////
  /*
	class CHttpBlockingSocket:public CBlockingSocket
	{
	public:
		enum {nSizeRecv = 1000}; // max receive buffer size (> hdr line length)

		CHttpBlockingSocket();
		~CHttpBlockingSocket();

		int ReadHttpHeaderLine(char* pch, int nSize, int nSecs);
		int ReadHttpResponse(char* pch, int nSize, int nSecs);

	private:
		char*  m_pReadBuf; // read buffer
		int    m_nReadBuf; // number of bytes in the read buffer
	};
  */

};

#endif // INC_BLOCKINGSOCKET_H
