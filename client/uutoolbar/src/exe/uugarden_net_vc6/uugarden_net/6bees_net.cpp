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

#include "stdafx.h"
//#include "6bees_ftpclient.h"
//#include "icesingleton.h"
#include "6bees_net.h"
#include "6bees_httpclient.h"


#include "6bees_str.h"
using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;

namespace _6bees_net{
#define T_COLON           0x01        /// ':' ///
#define T_SLASH           0x02        /// '/' ///
#define T_QUESTION        0x04        /// '?' ///
#define T_HASH            0x08        /// '#' ///
#define T_NUL             0x80        /// '\0' ///

/// Note that we optimize the scheme scanning here, we cheat and let the
/// compiler know that it doesn't have to do the & masking.
#define NOTEND_SCHEME     (0xff)
#define NOTEND_HOSTINFO   (T_SLASH | T_QUESTION | T_HASH | T_NUL)
#define NOTEND_PATH       (T_QUESTION | T_HASH | T_NUL)

  using namespace std;
  static const int HTTPNAMELEN= sizeof("http://")-1;// it's 8-1=7

  /// Delimiter table for the ASCII character set
  static const unsigned char uri_delims[256] = {
    T_NUL,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,T_HASH,0,0,0,0,0,0,0,0,0,0,0,T_SLASH,
    0,0,0,0,0,0,0,0,0,0,T_COLON,0,0,0,0,T_QUESTION,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

  int apr_uri_port_of_scheme(const char *scheme_str){
    schemes_t *scheme;
    if (scheme_str){
      for (scheme = schemes; scheme->name != NULL; ++scheme){
        if (_stricmp(scheme_str, scheme->name) == 0){
          return scheme->default_port;
        }
      }
    }
    return -1;
  }

  /// @remark Parse a given URI, fill in all supplied fields of a uri_components
  /// structure. This eliminates the necessity of extracting host, port,
  /// path, query info repeatedly in the modules.
  /// @return 0 means successful; -1 means failed
  int apr_uri_parse(const char* uri, apr_uri_t& uptr){
    if (uri==NULL || strlen(uri)<3){
      return -1;
    }
    const char *s;
    const char *s1;
    const char *hostinfo;
    char *endstr;
    int port;
    int v6_offset1 = 0, v6_offset2 = 0;

    /// Initialize the structure. parse_uri() and parse_uri_components()
    /// can be called more than once per request.
    uptr.cleanup();
    uptr.is_initialized = 1;

    /// We assume the processor has a branch predictor like most --
    /// it assumes forward branches are untaken and backwards are taken.  That's
    /// the reason for the gotos.  -djg
    ///
    if (uri[0] == '/'){
      /// RFC2396 #4.3 says that two leading slashes mean we have an
      /// authority component, not a path!  Fixing this looks scary
      /// with the gotos here.  But if the existing logic is valid,
      /// then presumably a goto pointing to deal_with_authority works.
      ///
      /// RFC2396 describes this as resolving an ambiguity.  In the
      /// case of three or more slashes there would seem to be no
      /// ambiguity, so it is a path after all.
      ///
      if (uri[1] == '/' && uri[2] != '/'){
        s = uri + 2 ;
        goto deal_with_authority ;
      }

deal_with_path:
      /// we expect uri to point to first character of path ... remember
      /// that the path could be empty -- http://foobar?query for example
      ///
      s = uri;
      while ((uri_delims[*(unsigned char *)s] & NOTEND_PATH) == 0){
        ++s;
      }
      if (s != uri){
        apr_pstrmemdup(uptr.path,uri,s-uri);
      }
      if (*s == 0){
        return 0;
      }
      if (*s == '?'){
        ++s;
        s1 = strchr(s, '#');
        if (s1){
          apr_pstrdup(uptr.fragment,s1+1);
          apr_pstrmemdup(uptr.query,s,s1-s);
        }else{
          apr_pstrdup(uptr.query,s);
        }
        return 0;
      }
      /// otherwise it's a fragment
      apr_pstrdup(uptr.fragment,s+1);
      return 0;
    }

    /// find the scheme:
    s = uri;
    while ((uri_delims[*(unsigned char *)s] & NOTEND_SCHEME) == 0){
      ++s;
    }
    /// scheme must be non-empty and followed by :
    if (s == uri || s[0] != ':'){
      goto deal_with_path;        /// backwards predicted taken!
    }
    apr_pstrmemdup(uptr.scheme,uri,s-uri);
    if (s[1] != '/' || s[2] != '/'){
      uri = s + 1;
      goto deal_with_path;
    }
    s += 3;
deal_with_authority:
    hostinfo = s;
    while ((uri_delims[*(unsigned char *)s] & NOTEND_HOSTINFO) == 0){
      ++s;
    }
    uri = s;        /// whatever follows hostinfo is start of uri
    apr_pstrmemdup(uptr.hostinfo,hostinfo,uri - hostinfo);

    /// If there's a username:password@host:port, the @ we want is the last @...
    /// too bad there's no memrchr()... For the C purists, note that hostinfo
    /// is definately not the first character of the original uri so therefore
    /// &hostinfo[-1] < &hostinfo[0] ... and this loop is valid C.
    do {
      --s;
    } while (s >= hostinfo && *s != '@');
    if (s < hostinfo){
      /// again we want the common case to be fall through
deal_with_host:
      /// We expect hostinfo to point to the first character of
      /// the hostname.  If there's a port it is the first colon,
      /// except with IPv6.
      if (*hostinfo == '['){
        v6_offset1 = 1;
        v6_offset2 = 2;
        s = (const char*)memchr(hostinfo, ']', uri-hostinfo);
        if (s == NULL){
          return -1;
        }
        if (*++s != ':'){
          s = NULL; /// no port
        }
      }else{
        s = (const char*)memchr(hostinfo, ':', uri - hostinfo);
      }
      if (s == NULL){
        /// we expect the common case to have no port
        apr_pstrmemdup(uptr.hostname,hostinfo + v6_offset1,uri - hostinfo - v6_offset2);
        goto deal_with_path;
      }
      apr_pstrmemdup(uptr.hostname,hostinfo + v6_offset1, s - hostinfo - v6_offset2);
      ++s;
      apr_pstrmemdup(uptr.port_str, s, uri - s);
      if (uri != s){
        port = strtol(uptr.port_str, &endstr, 10);
        uptr.port = port;
        if (*endstr == '\0'){
          goto deal_with_path;
        }
        return -1;
      }
      uptr.port = apr_uri_port_of_scheme(uptr.scheme);
      goto deal_with_path;
    }

    /// first colon delimits username:password
    s1 = (const char*)memchr(hostinfo, ':', s - hostinfo);
    if (s1){
      apr_pstrmemdup(uptr.user,hostinfo,s1 - hostinfo);
      ++s1;
      apr_pstrmemdup(uptr.password,s1,s - s1);
    } else {
      apr_pstrmemdup(uptr.user,hostinfo,s - hostinfo);
    }
    hostinfo = s + 1;
    goto deal_with_host;
  }

  bool apr_pstrmemdup(char*& buffer,const char *s, size_t n){
    if (s == NULL){
      return false;
    }
    if (buffer){
      delete [] buffer;
    }
    buffer = new char[n+1];
    memcpy(buffer, s, n);
    buffer[n] = '\0';
    return true;
  }

  bool apr_pstrdup(char*& buffer, const char *s){
    if (s == NULL){
      return false;
    }
    if (buffer){
      delete [] buffer;
    }
    size_t len = strlen(s)+1;
    buffer = new char[len];
    memcpy(buffer,s,len);
    return true;
  }

  bool _6beeparseuri(const char* uri, apr_uri_t& uptr){
    if (apr_uri_parse(uri,uptr) == -1)
      return false;
    if (uptr.port == -1 && uptr.port_str==NULL && uptr.scheme!=NULL){
      uptr.port = apr_uri_port_of_scheme(uptr.scheme);
      if (uptr.port!=-1){
        string tmp = stringmaker() << uptr.port;
        uptr.port_str = new char[tmp.size()+1];
        strcpy(uptr.port_str,&tmp[0]);
      }else{
        uptr.port_str = new char[1];
        memset(uptr.port_str,'\0',1);
        string info=stringmaker() << "unknown schema. url: " << uri;
      }
    }
    return true;
  }

  bool IsSupportResume(const char* url){
    apr_uri_t t;
    if(_6beeparseuri(url,t)){
      _6bees_socket::httpclient _httpclient;
      if(_httpclient.ConnectHttpServer(t.hostname,t.port)){
        long a=0;
        string pObj = t.path;
        if (t.query){
          pObj.append("?").append(t.query);
        }
        _httpclient.FormatRequestHeader(t.hostname,pObj.c_str(),a,NULL,url,0,1);
        if(_httpclient.SendReqAndGetRespHeader()){
          return (_httpclient.GetServerState() == 206);
        }
      }
    }
    return false;
  }

  /// caller responsible for releasing memory
  char* GetDataByHTTP(const char* url,int& clen,int from,int to){
    apr_uri_t t;
    if(_6beeparseuri(url,t)){
      _6bees_socket::httpclient _httpclient;
      if(_httpclient.ConnectHttpServer(t.hostname,t.port)){
        long a=0;
        string pObj = t.path;
        if (t.query){
          pObj.append("?").append(t.query);
        }
        _httpclient.FormatRequestHeader(t.hostname,pObj.c_str(),a,NULL,url,from,to);
        if(_httpclient.SendReqAndGetRespHeader()){
          int status=_httpclient.GetServerState();
          if (status>0 && status<400){
            return _httpclient.GetHTTPData(clen);
          }
        }
      }
    }
    return NULL;
  }

  /// caller responsible for releasing memory
  char* GetDataByHTTP(const wchar_t* url,int& clen,int from,int to){
    string urla_=stringmaker(CP_ACP) << url;
    return GetDataByHTTP(urla_.c_str(),clen,from,to);
  }

  int DownloadURLToFile(const char* url,const char* filepath){
    int clen=0;
    char* buffer=NULL;
    return DownloadURLToFile(url,filepath,buffer);
  }

  int DownloadURLToFile(const char* url,const char* filepath,char*& buffer){
    int clen=0;
    if ((buffer=GetDataByHTTP(url,clen))!=NULL){
      HANDLE hFile = ::CreateFileA(filepath,
        GENERIC_WRITE, NULL, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      if (hFile!=INVALID_HANDLE_VALUE){
        DWORD dwWrite=0;
        if(::WriteFile(hFile, buffer, clen, &dwWrite, NULL)){
          CloseHandle(hFile);
          return dwWrite;
        }else{
          string info="Cannot write data into file ";
          info += filepath;
        }
      }else{
        string info="Cannot create file ";
        info += filepath;
      }
    }
    return -1;
  }

  long GetContentLen(const char* url){
    apr_uri_t uri;
    if(_6beeparseuri(url,uri)){
      _6bees_socket::httpclient _httpclient;
      if(_httpclient.ConnectHttpServer(uri.hostname,uri.port)){
        long a=0;
        string pObj = uri.path;
        if (uri.query){
          pObj.append("?").append(uri.query);
        }
        _httpclient.FormatRequestHeader(uri.hostname,pObj.c_str(),a,NULL,url,0,0);
        if(_httpclient.SendReqAndGetRespHeader()){
          int status=_httpclient.GetServerState();
          if (status>0 && status<400){
            char clen[20];
            if(_httpclient.GetField("Content-Length",clen,20) != -1){
              return atoi(clen);
            }
          }
        }
      }
    }
    return -1;
  }

};