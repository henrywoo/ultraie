#include "utils_6bees_net_test.h"
#include "6bees_net.h"
#include "../../dll/6beecommond/6beecommond.h"
#include <iostream>
using namespace std;
using namespace _6bees_net;

#pragma comment(lib,"6beecommond.lib")

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(utils_6bees_net_test,"ultratest");

void utils_6bees_net_test::setUp(){
}

void utils_6bees_net_test::tearDown(){
}

void utils_6bees_net_test::testapr_uri_parse(){
  const char* info = "http://simonwoo:wufuheng@localhost:8080/index.php?q=apache#";
  apr_uri_t t;
  bool result = _6beeparseuri(info,t);
  CPPUNIT_ASSERT((t.port==8080));

  result = _6beeparseuri("ftp://www.ultraie.com/index.html?q=123&h=12#good",t);
  CPPUNIT_ASSERT((strcmp(t.scheme,"ftp")==0));

  result = _6beeparseuri(NULL,t);
  CPPUNIT_ASSERT((!result));

  result = _6beeparseuri("http://123.12.1.1:",t);
  CPPUNIT_ASSERT((result));
}

void utils_6bees_net_test::testGetContentLen(){
  CPPUNIT_ASSERT((GetContentLen("http://www.baidu.com/favicon.ico")==2550));
  CPPUNIT_ASSERT((GetContentLen("http://www.baidu.com")>0));
  CPPUNIT_ASSERT((GetContentLen("http://www.baidu.com3")==-1));
  CPPUNIT_ASSERT((GetContentLen("123")==-1));
  CPPUNIT_ASSERT((GetContentLen("")==-1));
  CPPUNIT_ASSERT((GetContentLen(NULL)==-1));
}

void utils_6bees_net_test::testIsSupportResume(){
  
  CPPUNIT_ASSERT((!IsSupportResume("http://www.163.com/index.html")));
  CPPUNIT_ASSERT((!IsSupportResume("http://www.163.com")));
  CPPUNIT_ASSERT((!IsSupportResume("http://www.ultraue.com")));
  CPPUNIT_ASSERT((!IsSupportResume("")));
  CPPUNIT_ASSERT((!IsSupportResume(NULL)));
  CPPUNIT_ASSERT((!IsSupportResume("http://www.msn.com")));

  //CPPUNIT_ASSERT((IsSupportResume("http://www.ultraie.com")));
  CPPUNIT_ASSERT((IsSupportResume("http://www.baidu.com/favicon.ico")));
}

void utils_6bees_net_test::testDownloadURLToFile(){
  const char* f1="C:\\baidu.html";
  int clen2 = DownloadURLToFile("http://media16.tube8.com/flv/87fae5abc897dfab2bfeb4adc4cb0367/4a09a971/0904/27/49f5cd8fe5d26/49f5cd8fe5d26.flv","c:\\tube8.flv");
  int clen = DownloadURLToFile("http://www.baidu.com",f1);
  BOOL result = PathFileExistsA(f1);
  CPPUNIT_ASSERT(result);
  int fsize;
  bool b = _6beed_util::GetFileSizeA(f1,fsize);
  if (result){
    DeleteFileA(f1);
  }
  if(b){
    CPPUNIT_ASSERT((clen==fsize));
  }else{
    CPPUNIT_ASSERT(0);
  }

  clen = DownloadURLToFile("http://www.baidu.com2",f1);
  CPPUNIT_ASSERT((clen==-1));
}