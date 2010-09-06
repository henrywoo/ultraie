#ifndef _STUB_SKELETON_HTTPSOCKET_H_
#define _STUB_SKELETON_HTTPSOCKET_H_

#include "6bees_socket.h"
#include "6bees_const.h"

using namespace _6bees_const;

namespace _6bees_socket{

  /// @brief A wrapper of http socket
  /// @todo to be merged with 6bees_socket
  class httpclient{
  public:
    httpclient();

    /// @return succeed - true
    /// @return fail - false
    bool ConnectHttpServer(const char* hostname,int port);

    /// @brief server status code
    /// @return -1 - no response
    /// @return 0 - did not get status code
    int GetServerState();

    /// @brief Get one field of response
    /// @param[in] name field name
    /// @param[in|out] outbuffer field value(a pointer to char buffer)
    /// @param[in] outbuffersize max length of value char buffer
    /// @return good: length of field value ; fail: -1
    int GetField(const char* name, char* outbuffer, int outbuffersize);

    /// @brief Get HTTP response header
    /// @param[out] Length the length of response header
    /// @return succeed - the pointer to response header buffer
    /// @return fail - NULL
    char*  GetResponseHeader(int &Length);

    /// Format request header before sending them to remote server
    /// @see SendReqAndGetRespHeader()
    /// @see GetRequestHeader()
    /// @param pServer host name. eg, www.google.com
    /// @param pObject remote url file name eg, /index.html
    /// @param Length  length of formatted request header
    /// @param pCookie an HTTP cookie previously sent by the server with Set-Cookie
    /// @param pReferer Referer page address. eg, http://www.baidu.com/index.html
    /// @param nFrom Downloading start from nFrom bytes of the url content, \
    /// key point of resume download, 0 means ignore this
    /// @param nTo Downloading end at nTo bytes of the url content, key point of resume download
    ///
    /// @return full request header
    const char*  FormatRequestHeader(
      const char *pServer,
      const char *pObject,
      long &Length,
      const char* pCookie=NULL,
      const char *pReferer=NULL,
      long nFrom=0,
      long nTo=0);

    /// @return succeed - true
    /// @return fail - false
    bool SendReqAndGetRespHeader();

    /// @return succeed - pointer to http data buffer
    /// @return fail - NULL
    char* GetHTTPData(int& clen);

  private:
    std::auto_ptr<Iblksock> sockcon_;

    /// request header, max length MAXHEADERSIZE
    char requestheader_[MAXHEADERSIZE];

    /// response header, max length MAXHEADERSIZE
    char responseheader_[MAXHEADERSIZE];

    /// remote peer's port
    int port_;

    /// is connected to http server
    bool httpconnected_;

    /// have we got response header
    bool httpresponsed_;

    /// size of response header
    int responseheadersize_;
  };

};

#endif