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

#include "utils_httpclient_test.h"
#include "6bees_httpclient.h"
#include "6bees_socket.h"

#include <time.h>
#include <iostream>
using namespace std;
using namespace _6bees_socket;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(utils_httpclient_test,"ultratest");

void utils_httpclient_test::setUp(){
}

void utils_httpclient_test::tearDown(){
}

void utils_httpclient_test::testGetHTML(){
  const char* hostname = "www.baidu.com";
  const char* urlfile = "/index.html";

  _6bees_socket::httpclient _httpclient;
  _httpclient.ConnectHttpServer(hostname,80);
  long a=0;
  _httpclient.FormatRequestHeader(hostname,urlfile,a,NULL,hostname,0,0);
  _httpclient.SendReqAndGetRespHeader();
  int status=_httpclient.GetServerState();
  if (status>0 && status<400){
    int clen;
    char* httpcontent = _httpclient.GetHTTPData(clen);
    CPPUNIT_ASSERT(httpcontent!=NULL);
    if (httpcontent!=NULL){
      size_t contentsize = strlen(httpcontent);
      cout << httpcontent << endl;
      delete [] httpcontent;
      CPPUNIT_ASSERT((contentsize>4000&&contentsize<6000));
    }
  }
}

void utils_httpclient_test::testSetTimeOut(){
  const char* hostname = "www.uufile2.com";
  const char* urlfile = "/index.html";

  time_t t1,t2;
  try{
    t1=time(NULL);
    _6bees_socket::httpclient _httpclient;
    _httpclient.ConnectHttpServer(hostname,80);
    long a=0;
    _httpclient.FormatRequestHeader(hostname,urlfile,a,NULL,hostname,0,0);
    _httpclient.SendReqAndGetRespHeader();
    int status=_httpclient.GetServerState();
    if (status>0 && status<400){
      int clen;
      char* httpcontent = _httpclient.GetHTTPData(clen);
      CPPUNIT_ASSERT(0);
    }
  }catch (sckexception& e){
    t2=time(NULL);
    const wchar_t *s = e.GetErrorMessage().c_str();
    UUDEBUG((LOG_ERROR,s));
    CPPUNIT_ASSERT((t2-t1<4));
  }
}

/// Server must support 'resume downloading'
void utils_httpclient_test::testResumeDownload(){
  const char* hostname = "www.baidu.com";
  const char* urlfile = "/favicon.ico";

  try{
    _6bees_socket::httpclient _httpclient;
    _httpclient.ConnectHttpServer(hostname,80);
    long a=0;
    _httpclient.FormatRequestHeader(hostname,urlfile,a,NULL,hostname,10,20);
    _httpclient.SendReqAndGetRespHeader();
    int status=_httpclient.GetServerState();
    // partial content status code - 206
    if (status==206){
      int clen;
      char* httpcontent = _httpclient.GetHTTPData(clen);
      CPPUNIT_ASSERT(httpcontent!=NULL);
      if (httpcontent!=NULL){
        size_t contentsize = strlen(httpcontent);
        cout << httpcontent << endl;
        delete [] httpcontent;
        CPPUNIT_ASSERT((contentsize>9&&contentsize<12));
      }
    }else{
      CPPUNIT_ASSERT(0);
    }
  }catch (...){
    CPPUNIT_ASSERT(0);
  }
}