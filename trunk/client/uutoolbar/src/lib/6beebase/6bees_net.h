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

#ifndef __6BEES_NET__
#define __6BEES_NET__

#include <atlbase.h>
#include <atlstr.h>
#include <string>
#include <wininet.h>
#pragma comment(lib,"wininet.lib")
#include "6bees_const.h"

/// @brief for network, based on _6bees_socket
namespace _6bees_net{
  using namespace std;
  using namespace _6bees_const;

  typedef struct schemes_t schemes_t;

  /// @brief Structure to store various schemes and their default ports
  struct schemes_t {
    /// The name of the scheme
    const char *name;
    /// The default port for the scheme
    int default_port;
  };

  /// Some WWW schemes and their default ports; this is basically /etc/services
  /// This will become global when the protocol abstraction comes
  /// As the schemes are searched by a linear search,
  /// they are sorted by their expected frequency
  static schemes_t schemes[] ={
    {"http",     APR_URI_HTTP_DEFAULT_PORT},
    {"ultra",    APR_URI_HTTP_DEFAULT_PORT},
    {"ftp",      APR_URI_FTP_DEFAULT_PORT},
    {"https",    APR_URI_HTTPS_DEFAULT_PORT},
    {"gopher",   APR_URI_GOPHER_DEFAULT_PORT},
    {"ldap",     APR_URI_LDAP_DEFAULT_PORT},
    {"nntp",     APR_URI_NNTP_DEFAULT_PORT},
    {"snews",    APR_URI_SNEWS_DEFAULT_PORT},
    {"imap",     APR_URI_IMAP_DEFAULT_PORT},
    {"pop",      APR_URI_POP_DEFAULT_PORT},
    {"sip",      APR_URI_SIP_DEFAULT_PORT},
    {"rtsp",     APR_URI_RTSP_DEFAULT_PORT},
    {"wais",     APR_URI_WAIS_DEFAULT_PORT},
    {"z39.50r",  APR_URI_WAIS_DEFAULT_PORT},
    {"z39.50s",  APR_URI_WAIS_DEFAULT_PORT},
    {"prospero", APR_URI_PROSPERO_DEFAULT_PORT},
    {"nfs",      APR_URI_NFS_DEFAULT_PORT},
    {"tip",      APR_URI_TIP_DEFAULT_PORT},
    {"acap",     APR_URI_ACAP_DEFAULT_PORT},
    {"telnet",   APR_URI_TELNET_DEFAULT_PORT},
    {"ssh",      APR_URI_SSH_DEFAULT_PORT},
    { NULL, 0xFFFF }     /// unknown port
  };

  #define CLEANVAR(x) if(x) delete [] x; x = NULL

  /// @brief A structure to encompass all of the fields in a uri
  struct apr_uri_t {
    apr_uri_t():scheme(NULL),
      hostinfo(NULL),
      user(NULL),
      password(NULL),
      hostname(NULL),
      port_str(NULL),
      path(NULL),
      query(NULL),
      fragment(NULL),
      port(-1)
    {}
    ~apr_uri_t(){
      cleanup();
    }

    void cleanup(){
      CLEANVAR(scheme);
      CLEANVAR(hostinfo);
      CLEANVAR(user);
      CLEANVAR(password);
      CLEANVAR(hostname);
      CLEANVAR(port_str);
      CLEANVAR(path);
      CLEANVAR(query);
      CLEANVAR(fragment);
      port = -1;
      is_initialized = 0;
    }
    /// scheme ("http"/"ftp"/...)
    char *scheme;
    /// combined [user[:password]\@]host[:port]
    char *hostinfo;
    /// user name, as in http://user:passwd\@host:port/
    char *user;
    /// password, as in http://user:passwd\@host:port/
    char *password;
    /// hostname from URI (or from Host: header)
    char *hostname;
    /// port string (integer representation is in "port")
    char *port_str;
    /// the request path (or "/" if only scheme://host was given)
    char *path;
    /// Everything after a '?' in the path, if present
    char *query;
    /// Trailing "#fragment" string, if present
    char *fragment;
    /// The port number, numeric, valid only if port_str != NULL
    int port;
    /// has the structure been initialized
    unsigned int is_initialized:1;
  };

  /// get scheme's default port
  /// internal called by apr_uri_parse()
  int apr_uri_port_of_scheme(const char *scheme_str);

  /// memory copy, caller in charge of releasing memory of buffer
  /// @note internal called by apr_uri_parse()
  bool apr_pstrmemdup(char*& buffer,const char *s, size_t n);

  /// string copy, caller in charge of releasing memory of buffer
  /// internal called by apr_uri_parse()
  bool apr_pstrdup(char*& buffer, const char *s);

  /// @note do not call this directly, you should use _6beeparseuri()
  /// @see _6beeparseuri()
  int apr_uri_parse(const char* uri, apr_uri_t& uptr);

  /// @brief parse url information and put into an apr_uri_t struct
  bool _6beeparseuri(const char* uri, apr_uri_t& uptr);

  /// @return the length of content, -1 means failed
  long GetContentLen(const char* url);

  /// @remark if the url supports resume downloading
  /// @todo put the result to cache - a map<uri,bool>
  bool IsSupportResume(const char* url);

  char* GetDataByHTTP(const char* url,int& clen,int from=0,int to=0);
  char* GetDataByHTTP(const wchar_t* url,int& clen,int from=0,int to=0);


  /// @param url the url to be downloaded
  /// @param filepath download url data to filepath
  /// @return If the function succeeds,the return value is the length of the url data buffer
  /// @return If the function fails, then return -1;
  int DownloadURLToFile(const char* url,const char* filepath);

  /// @param url the url to be downloaded
  /// @param filepath download url data to filepath
  /// @param buffer reference to the pointer of url data buffer
  /// @return fail: -1; succeed: length of the buffer of url data
  /// @remark the caller should release the buffer after using it
  int DownloadURLToFile(const char* url,const char* filepath,char*& buffer);

};

#endif