#ifndef __INC_BLOCKINGSOCKET_H__
#define __INC_BLOCKINGSOCKET_H__

#include <string>
#include "6bees_util.h"
#include "uulogging.h"
#include "6bees_str.h"

#ifndef _WINSOCK2API_
#define _WINSOCK2API_
#include <winsock2.h>
#endif
#pragma comment(lib, "ws2_32.lib")

namespace _6bees_socket{

  typedef const struct sockaddr* LPCSOCKADDR;

  class sckexception {
  public:
    sckexception(wchar_t* pchMessage);
    ~sckexception() {}
    virtual BOOL GetErrorMessage(wchar_t* lpstrError, UINT nMaxError, PUINT pnHelpContext = NULL);
    virtual std::wstring GetErrorMessage(PUINT pnHelpContext = NULL);

  private:
    int m_nError;
    std::wstring m_strMessage;
  };

  class CSockAddr: public sockaddr_in{
  public:
    /// contor 1 - default constructor
    CSockAddr(){
      sin_family = AF_INET;
      sin_port = 0;
      sin_addr.s_addr = 0;// ip address
    }

    /// contor 2 - copy constructor
    CSockAddr(const SOCKADDR& sa){
      memcpy(this, &sa, sizeof(SOCKADDR));
    }

    /// contor 3 - copy constructor
    CSockAddr(const SOCKADDR_IN& sin){
      memcpy(this, &sin, sizeof(SOCKADDR_IN));
    }

    /// contor 4
    CSockAddr(ULONG ulAddr, unsigned short ushPort = 0){ // parms are host byte ordered
      sin_family      = AF_INET;          // specifies address family
      sin_port        = htons(ushPort);   // ip port
      sin_addr.s_addr = htonl(ulAddr);    // ip address
    }

    /// contor 5
    CSockAddr(const char* pchIP, unsigned short ushPort = 0){ // dotted IP addr string
      sin_family = AF_INET;               // specifies address family
      sin_port = htons(ushPort);          // ip port
      sin_addr.s_addr = inet_addr(pchIP); // ip address
    } // already network byte ordered

    // @return Return the address in dotted-decimal format
    std::wstring DottedDecimal(){
      return _6bees_str::wstringmaker()<<inet_ntoa(sin_addr);
    }

    // Get port and address (even though they're public)
    unsigned short Port() const{
      return ntohs(sin_port);
    }

    ULONG IPAddr() const{
      return ntohl(sin_addr.s_addr);
    }

    // operators added for efficiency
    CSockAddr& operator=(const SOCKADDR& sa){
      memcpy(this, &sa, sizeof(SOCKADDR));
      return *this;
    }

    CSockAddr& operator=(const SOCKADDR_IN& sin){
      memcpy(this, &sin, sizeof(SOCKADDR_IN));
      return *this;
    }

    operator SOCKADDR() { return *((LPSOCKADDR) this); }

    operator LPSOCKADDR() { return (LPSOCKADDR) this; }

    operator LPSOCKADDR_IN() { return (LPSOCKADDR_IN) this; }
  };

  /// @brief interface of blocking socket
  class Iblksock {
  public:
    virtual ~Iblksock() {};
    virtual Iblksock* CreateInstance() const = 0;
    virtual void Create(int nType = SOCK_STREAM) = 0;
    virtual void Connect(LPCSOCKADDR psa) const = 0;
    virtual void Bind(LPCSOCKADDR psa) const = 0;
    virtual void Listen() const = 0;
    virtual bool Accept(Iblksock& s, LPSOCKADDR psa) const = 0;
    virtual void Cleanup() = 0;
    virtual void Close() = 0;
    virtual int  Send(const char* pch, int nSize, int nSecs) const = 0;
    virtual int  Write(const char* pch, int nSize, int nSecs) const = 0;
    virtual int  Receive(char* pch, int nSize, int nSecs) const = 0;
    virtual void GetSockAddr(LPSOCKADDR psa) const = 0;
    virtual operator SOCKET() const = 0; //+#
    virtual bool CheckReadability() const = 0;
    virtual CSockAddr GetHostByName(const char* pchName, unsigned short ushPort = 0) = 0;
    virtual const char* GetHostByAddr(LPCSOCKADDR psa) = 0;
  };

  /// @brief implementation of blocking socket
  /// @note member functions truly block and must not be used in UI threads \
  /// use this class as an alternative to the MFC CSocket class
  class Cblksock : public Iblksock
  {
  public:
    Cblksock():socket_(NULL){
      WSADATA wsaData;
      if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR){
        UUDEBUG((LOG_ERROR,"Window Socket cannot be started."));
      }
    }
    ~Cblksock(){
      Cleanup();
      WSACleanup();
    }
    void Cleanup();
    void Create(int nType = SOCK_STREAM);
    void Close();
    void Bind(LPCSOCKADDR psa) const;
    void Listen() const;
    void Connect(LPCSOCKADDR psa) const;
    bool Accept(Iblksock& s, LPSOCKADDR psa) const;

    /// @param[in] pch request header
    /// @param[in] nSize length of request header
    /// @param[in] nSecs time out seconds
    /// @return bytes sent
    int  Send(const char* pch, int nSize, int nSecs) const;

    int  Write(const char* pch, int nSize, int nSecs) const;

    /// Receive data from remote server
    /// @param[in] pch request header
    /// @param[in] nSize length of request header
    /// @param[in] nSecs time out seconds
    /// @return succeed: bytes received
    /// @return fail: -1
    int  Receive(char* pch, int nSize, int nSecs) const;

    int  SendDatagram(const char* pch, int nSize, LPCSOCKADDR psa, int nSecs) const;
    int  ReceiveDatagram(char* pch, int nSize, LPSOCKADDR psa, int nSecs) const;
    void GetPeerAddr(LPSOCKADDR psa) const;
    void GetSockAddr(LPSOCKADDR psa) const;
    bool CheckReadability() const;
    CSockAddr GetHostByName(const char* pchName, unsigned short ushPort = 0);
    const char* GetHostByAddr(LPCSOCKADDR psa);
    operator SOCKET() const { return socket_; }
    virtual Iblksock* CreateInstance() const;
  private:
    SOCKET socket_;
  };

  /// new a blocking socket
  Iblksock* CreateNewBlockSocket();

};

#endif // INC_BLOCKINGSOCKET_H
