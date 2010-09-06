#include "stdafx.h"
#include "6bees_socket.h"
#include <algorithm>

namespace _6bees_socket{

  //************************* Class sckexception **************************//
  sckexception::sckexception(wchar_t* pchMessage){
    m_strMessage = pchMessage;
    m_nError     = WSAGetLastError();
  }

  /*BOOL sckexception::GetErrorMessage(wchar_t* lpstrError,UINT nMaxError,PUINT pnHelpContext){
    if( m_nError == 0 ){
      _snwprintf_s(lpstrError,nMaxError,nMaxError,L"%s error",m_strMessage.c_str());
    }else{
      _snwprintf_s(lpstrError,nMaxError,nMaxError,L"%s error 0x%08x",m_strMessage.c_str(),m_nError);
    }
    return TRUE;
  }

  std::wstring sckexception::GetErrorMessage(PUINT){
    wchar_t szBuffer[512] = {};
    GetErrorMessage(szBuffer,512,NULL);
    return szBuffer;
  }*/


  //******************************** Class Cblksock ****************************//

  Iblksock* Cblksock::CreateInstance() const {
    return new Cblksock();
  }

  void Cblksock::Cleanup(){
    // doesn't throw an exception because it's called in a catch block
    if(socket_==NULL)
      return;
    _ASSERT(closesocket(socket_)!=SOCKET_ERROR);
    socket_ = NULL;
  }

  void Cblksock::Create(int nType /* = SOCK_STREAM */){
    _ASSERT( socket_==0 );
    if( (socket_=socket(AF_INET, nType, 0))==INVALID_SOCKET ){
      int iResult = WSAGetLastError();
      throw sckexception(_T("Create"));
    }
  }

  void Cblksock::Bind(LPCSOCKADDR psa) const{
    _ASSERT( socket_!=0 );
    if( bind(socket_, psa, sizeof(SOCKADDR))==SOCKET_ERROR ){
      throw sckexception(_T("Bind"));
    }
  }

  void Cblksock::Listen() const {
    _ASSERT( socket_!=0 );
    if( listen(socket_, 5)==SOCKET_ERROR ){
      throw sckexception(_T("Listen"));
    }
  }

  bool Cblksock::Accept(Iblksock& sConnect, LPSOCKADDR psa) const {
    Cblksock* pConnect = static_cast<Cblksock*>(&sConnect); //+#
    _ASSERT( socket_!=0 );
    _ASSERT( pConnect->socket_==0 );

    int nLengthAddr = sizeof(SOCKADDR);
    pConnect->socket_ = accept(socket_, psa, &nLengthAddr);

    if( pConnect->operator SOCKET()==INVALID_SOCKET ){
      // no exception if the listen was canceled
      if( WSAGetLastError() !=WSAEINTR ){
        throw sckexception(_T("Accept"));
      }
      return false;
    }
    return true;
  }

  void Cblksock::Close(){
    if( socket_ && closesocket(socket_)==SOCKET_ERROR ){
      // should be OK to close if closed already
      throw sckexception(_T("Close"));
    }
    socket_ = 0;
  }

  void Cblksock::Connect(LPCSOCKADDR psa) const {
    _ASSERT( socket_!=0 );
    // should timeout by itself
    if( connect(socket_, psa, sizeof(SOCKADDR))==SOCKET_ERROR ){
      throw sckexception(_T("Connect"));
    }
  }

  int Cblksock::Write(const char* pch, int nSize, int nSecs) const {
    int nBytesSent = 0;
    int nBytesThisTime;
    const char* pch1 = pch;
    do{
      nBytesThisTime = Send(pch1, nSize - nBytesSent, nSecs);
      nBytesSent += nBytesThisTime;
      pch1 += nBytesThisTime;
    } while( nBytesSent<nSize );
    return nBytesSent;
  }

  int Cblksock::Send(const char* pch, int nSize, int nSecs) const {
    _ASSERT( socket_!=0 );
    // returned value will be less than nSize if client cancels the reading
    FD_SET  fd = { 1, socket_ };
    TIMEVAL tv = { nSecs, 0 };
    if( select(0, NULL, &fd, NULL, &tv)==0 ){
      throw sckexception(_T("Send timeout"));
    }
    int nBytesSent;
    if( (nBytesSent=send(socket_, pch, nSize, 0))==SOCKET_ERROR ){
      throw sckexception(_T("Send"));
    }
    return nBytesSent;
  }

  bool Cblksock::CheckReadability() const {
    _ASSERT( socket_!=0 );

    FD_SET  fd = { 1, socket_ };
    TIMEVAL tv = { 0, 0 };

    const int iRet = select(0, &fd, NULL, NULL, &tv);

    if( iRet==SOCKET_ERROR ){
      throw sckexception(_T("Socket Error"));
    }

    return iRet == 1;
  }

  int Cblksock::Receive(char* pch, int nSize, int nSecs) const{
    _ASSERT( socket_!=0 );
    FD_SET  fd = { 1, socket_ };
    TIMEVAL tv = { nSecs, 0 };
    if( select(0, &fd, NULL, NULL, &tv)==0 ){
      //throw sckexception(_T("Receive timeout"));
      return -1;
    }
    int nBytesReceived;
    if( (nBytesReceived=recv(socket_, pch, nSize, 0))==SOCKET_ERROR ){
      //throw sckexception(_T("Receive"));
      return -1;
    }
    return nBytesReceived;
  }

  int Cblksock::ReceiveDatagram(char* pch, int nSize, LPSOCKADDR psa, int nSecs) const {
    _ASSERT( socket_!=0 );

    FD_SET  fd = { 1, socket_ };
    TIMEVAL tv = { nSecs, 0 };

    if( select(0, &fd, NULL, NULL, &tv)==0 ){
      throw sckexception(_T("Receive timeout"));
    }

    // input buffer should be big enough for the entire datagram
    int nFromSize = sizeof(SOCKADDR);
    int nBytesReceived = recvfrom(socket_, pch, nSize, 0, psa, &nFromSize);

    if( nBytesReceived==SOCKET_ERROR ){
      throw sckexception(_T("ReceiveDatagram"));
    }

    return nBytesReceived;
  }

  int Cblksock::SendDatagram(const char* pch, int nSize, LPCSOCKADDR psa, int nSecs) const {
    _ASSERT( socket_!=0 );

    FD_SET  fd = { 1, socket_ };
    TIMEVAL tv = { nSecs, 0 };

    if( select(0, NULL, &fd, NULL, &tv)==0 ){
      throw sckexception(_T("Send timeout"));
    }

    int nBytesSent = sendto(socket_, pch, nSize, 0, psa, sizeof(SOCKADDR));
    if( nBytesSent==SOCKET_ERROR ){
      throw sckexception(_T("SendDatagram"));
    }

    return nBytesSent;
  }

  void Cblksock::GetPeerAddr(LPSOCKADDR psa) const {
    _ASSERT( socket_!=0 );

    // gets the address of the socket at the other end
    int nLengthAddr = sizeof(SOCKADDR);
    if( getpeername(socket_, psa, &nLengthAddr)==SOCKET_ERROR ){
      throw sckexception(_T("GetPeerName"));
    }
  }

  void Cblksock::GetSockAddr(LPSOCKADDR psa) const {
    _ASSERT( socket_!=0 );

    // gets the address of the socket at this end
    int nLengthAddr = sizeof(SOCKADDR);
    if( getsockname(socket_, psa, &nLengthAddr)==SOCKET_ERROR ) 
    {
      throw sckexception(_T("GetSockName"));
    }
  }

  CSockAddr Cblksock::GetHostByName(const char* pchName, unsigned short ushPort /* = 0 */){
    hostent* pHostEnt = gethostbyname(pchName);

    if( pHostEnt==NULL){
      throw sckexception(_T("GetHostByName"));
    }

    ULONG* pulAddr = (ULONG*) pHostEnt->h_addr_list[0];
    SOCKADDR_IN sockTemp;
    sockTemp.sin_family = AF_INET;
    sockTemp.sin_port = htons(ushPort);
    sockTemp.sin_addr.s_addr = *pulAddr; // address is already in network byte order
    return sockTemp;
  }

  const char* Cblksock::GetHostByAddr(LPCSOCKADDR psa){
    hostent* pHostEnt = gethostbyaddr((char*) &((LPSOCKADDR_IN) psa)->sin_addr.s_addr, 4, PF_INET);

    if( pHostEnt==NULL ){
      throw sckexception(_T("GetHostByAddr"));
    }

    return pHostEnt->h_name; // caller shouldn't delete this memory
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  //**************************** Class CHttpBlockingSocket ****************************//
  ///////////////////////////////////////////////////////////////////////////////////////

  /*
  CHttpBlockingSocket::CHttpBlockingSocket(){
  m_pReadBuf = new char[nSizeRecv];
  m_nReadBuf = 0;
  }

  CHttpBlockingSocket::~CHttpBlockingSocket(){
  delete[] m_pReadBuf;
  }

  int CHttpBlockingSocket::ReadHttpHeaderLine(char* pch, int nSize, int nSecs)
  // reads an entire header line through CRLF (or socket close)
  // inserts zero string terminator, object maintains a buffer
  {
  int  nBytesThisTime = m_nReadBuf;
  ptrdiff_t nLineLength    = 0;
  char*     pch1   = m_pReadBuf;
  char*     pch2   = NULL;

  do 
  {
  // look for lf (assume preceded by cr)
  if( (pch2=(LPSTR)memchr(pch1 , '\n', nBytesThisTime))!=NULL ){
  _ASSERT( (pch2)>m_pReadBuf );
  _ASSERT( *(pch2 - 1)=='\r' );
  nLineLength = (pch2 - m_pReadBuf) + 1;
  if( nLineLength>=nSize )
  nLineLength = nSize - 1;
  memcpy(pch, m_pReadBuf, nLineLength); // copy the line to caller
  m_nReadBuf -= static_cast<unsigned int>(nLineLength);
  memmove(m_pReadBuf, pch2 + 1, m_nReadBuf); // shift remaining characters left
  break;
  }
  pch1 += nBytesThisTime;
  nBytesThisTime = Receive(m_pReadBuf + m_nReadBuf, nSizeRecv - m_nReadBuf, nSecs);
  if( nBytesThisTime<=0 ){
  // sender closed socket or line longer than buffer
  throw sckexception(_T("ReadHeaderLine"));
  }
  m_nReadBuf += nBytesThisTime;
  #pragma warning(disable:4127)
  } while( true );
  #pragma warning(default:4127)
  *(pch + nLineLength) = _T('\0');

  return static_cast<unsigned int>(nLineLength);
  }

  // reads remainder of a transmission through buffer full or socket close
  // (assume headers have been read already)
  int CHttpBlockingSocket::ReadHttpResponse(char* pch, int nSize, int nSecs){
  int nBytesToRead, nBytesThisTime, nBytesRead = 0;

  if( m_nReadBuf>0){ // copy anything already in the recv buffer
  memcpy(pch, m_pReadBuf, m_nReadBuf);
  pch += m_nReadBuf;
  nBytesRead = m_nReadBuf;
  m_nReadBuf = 0;
  }
  do
  { // now pass the rest of the data directly to the caller
  nBytesToRead = min(static_cast<int>(nSizeRecv), nSize - nBytesRead);
  nBytesThisTime = Receive(pch, nBytesToRead, nSecs);
  if( nBytesThisTime<=0 )
  break; // sender closed the socket
  pch += nBytesThisTime;
  nBytesRead += nBytesThisTime;
  } while( nBytesRead<=nSize );

  return nBytesRead;
  }
  */

  Iblksock* CreateNewBlockSocket(){
    return new Cblksock();
  }

};