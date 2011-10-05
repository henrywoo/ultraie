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

#include "chttpsocket_test.h"
#include "..\Stub_Skeleton\chttpsocket.h"
#include "..\Stub_Skeleton\6bees_net.h"
#
#include <time.h>
#include <iostream>
using namespace std;
#

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(chttpsockettest,"ultratest");

void chttpsockettest::setUp(){
  v1 = 0;
  v2 = 1;
}

void chttpsockettest::tearDown(){
}

void chttpsockettest::testGetHTML(){
  const char* hostname = "www.baidu.com";
  const char* urlfile = "/index.html";

  chttpsocket chttsocket;
  chttsocket.CreateTCPSocket();
  chttsocket.SetTimeout(5000);// 5s
  if(chttsocket.Connect(hostname)){
    long a=0;
    chttsocket.FormatRequestHeader(hostname,urlfile,a,NULL,hostname,0,0);
    chttsocket.SendReqAndGetRespHeader();
    int status=chttsocket.GetServerState();
    if (status<400){
      char* httpcontent = chttsocket.GetHTTPContent();
      CPPUNIT_ASSERT(httpcontent!=NULL);
      if (httpcontent!=NULL){
        size_t contentsize = strlen(httpcontent);
        cout << httpcontent << endl;
        delete [] httpcontent;
        CPPUNIT_ASSERT((contentsize>4000&&contentsize<6000));
      }
    }
  }
}


void chttpsockettest::testGetField(){
  static const int baiduiconsize = 2550;
  int tmp=0;

  const char* hostname = "www.baidu.com";
  const char* urlfile = "/favicon.ico";

  chttpsocket chttsocket;
  chttsocket.CreateTCPSocket();
  chttsocket.SetTimeout(5000);// 5s
  if(chttsocket.Connect(hostname)){
    long a=0;
    chttsocket.FormatRequestHeader(hostname,urlfile,a,NULL,hostname,0,0);
    chttsocket.SendReqAndGetRespHeader();
    int status=chttsocket.GetServerState();
    if (status<400){
      char iconsize[20]={};
      int cl=chttsocket.GetField("Content-Length",iconsize,20);
      tmp = (((cl==-1) ? -1 : atoi(iconsize)));
    }
  }
  CPPUNIT_ASSERT(tmp==baiduiconsize);
}

//A bug about chttsocket's SetTimeout
void chttpsockettest::testSetTimeOut(){
  const char* hostname = "www.uufile.com";
  const char* urlfile = "/favicon.ico";
  chttpsocket chttsocket;
  chttsocket.CreateTCPSocket();
  chttsocket.SetTimeout(25000);// 25s
  time_t t1=time(NULL);
  if(chttsocket.Connect(hostname)){
    cout<< "connected to " << hostname << endl;
    long a=0;
    chttsocket.FormatRequestHeader(hostname,urlfile,a,NULL,hostname,0,0);
    chttsocket.SendReqAndGetRespHeader();
    time_t t2=time(NULL);
    cout << t2-t1 << endl;
    int status=chttsocket.GetServerState();
    CPPUNIT_ASSERT(status==0);
  }
}

void chttpsockettest::testSetTimeOut_HINTERNET(){
  string html;
  _6bees_net::GetnKHTML(_T("http://www.baidu.com/"),html,0,true);
  int contentsize = (int)html.size();
  CPPUNIT_ASSERT((contentsize>4000&&contentsize<6000));

  html="";
  time_t t1=time(NULL);
  _6bees_net::GetnKHTML(_T("http://www.uufile.com/"),html,0,true);
  time_t t2=time(NULL);
  cout << t2-t1 << endl;
  contentsize = (int)html.size();
  CPPUNIT_ASSERT((contentsize==0));
}