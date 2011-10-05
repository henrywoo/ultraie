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

#include "getaddress.h"
#include "6bees_httpclient.h"
#include "6bees_net.h"
#include <iostream>

using namespace std;

using namespace _6bees_net;

#define URLNUM 4
const char* kurls[URLNUM] = {
  "http://hi.baidu.com/denmarkpink/blog/item/1b2c2a0bdf379f1795ca6bf8.html",// denmarkpink wfh168178
  "http://groups.google.com.sg/group/uugardennet",// wufuheng wfh168178@
  "http://blog.163.com/wfh_178/blog/static/117482150200941611010784/",// wfh_178 wfh168178
  "http://uugardennet.blog.sohu.com/116550584.html"//, // 5yaohechuntao wfh168178
  //"http://uugardennet.blogspot.com/2009/05/yard.html", // 5yaohechuntao xiaweiying
  //"http://blogs.myspace.com/index.cfm?fuseaction=blog.ListAll&friendID=133829262" // wufuheng@gmail.com wfh168178
};

bool TestConnection(const string& url_){
  string url=url_ + "/favicon.ico";
  apr_uri_t t;
  if(_6beeparseuri(url.c_str(),t)){
    _6bees_socket::httpclient _httpclient;
    _httpclient.ConnectHttpServer(t.hostname,80);
    long a=0;
    string pObj = t.path;
    if (t.query){
      pObj.append("?").append(t.query);
    }
    _httpclient.FormatRequestHeader(t.hostname,pObj.c_str(),a,NULL,t.hostname,0,0);
    _httpclient.SendReqAndGetRespHeader();
    int status=_httpclient.GetServerState();
    if (status>199 && status<400){
      char contentsize[10]={};
      int result=_httpclient.GetField("Content-Length",contentsize,10);
      return strcmp(contentsize,"1150")==0;
    }
  }
  return false;
}

string GetUUGardenURL(){
  for (int i=0;i<URLNUM;++i){
    string uugarden_urls=GetFromOtherWebsite(kurls[i]);
    if (!uugarden_urls.empty()){
      vector<string> vurl = _6bees_util::Split(uugarden_urls,"|");
      for (vector<string>::iterator i=vurl.begin();i!=vurl.end();++i){
        if (TestConnection(*i)){
          return *i;
        }
      }
    }
  }
  return string();
}


string GetFromOtherWebsite(const string& url){
  string target;
  apr_uri_t t;
  if(_6beeparseuri(url.c_str(),t)){
    _6bees_socket::httpclient _httpclient;
    _httpclient.ConnectHttpServer(t.hostname,80);
    long a=0;
    string pObj = t.path;
    if (t.query){
      pObj.append("?").append(t.query);
    }
    _httpclient.FormatRequestHeader(t.hostname,pObj.c_str(),a,NULL,t.hostname,0,0);
    _httpclient.SendReqAndGetRespHeader();
    int status=_httpclient.GetServerState();
    if (status>0 && status<400){
      int clen;
      char* httpcontent = _httpclient.GetHTTPData(clen);
      if (httpcontent!=NULL){
        string html(httpcontent);
        string tmp = findaddress(html);
        target = base64_decode(tmp);
        delete [] httpcontent;
      }
    }
  }
  return target;
}

//return base64 code of urls
string findaddress(const string& html){
  string flag = "newwebaddress";
  size_t s1 = html.find(flag.c_str());
  size_t s2 = html.find(flag.c_str(),s1+flag.size());
  if (s1!=string::npos && s2!=string::npos && s2>s1){
    return html.substr(s1+flag.size(),s2-s1);
  }
  return string();
}

static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) {
  int in_len = (int)encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = (unsigned char)base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = (unsigned char)base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}
