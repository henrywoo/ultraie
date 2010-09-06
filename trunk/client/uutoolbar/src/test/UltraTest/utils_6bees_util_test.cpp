#include "utils_6bees_util_test.h"
#include "6bees_util.h"
#include "6bees_const.h"
#include "6bees_str.h"
using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;
#include "../../dll/6beecommond/6beecommond.h"
#include <iostream>
using namespace std;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(utils_6bees_util_test,"ultratest");

void utils_6bees_util_test::setUp(){
}

void utils_6bees_util_test::tearDown(){
}

void utils_6bees_util_test::testgetFavicon(){

  string s = "http://www.cppblog.com/../../../../../Modules/CaptchaImage/JpegImage.aspx";
  _6bees_util::ProcessURLA(s);

  //getFavicon(IHTMLDocument2Ptr& pHtmlDoc2,const wchar_t* scheme,ATL::CString& faviconURL);
  CPPUNIT_ASSERT((true));
}

void utils_6bees_util_test::testCStringMaker(){
  std::wstring a = wstringmaker() << L"nice" << 123 << "hello";
  CPPUNIT_ASSERT((a==L"nice123hello"));

  std::string b = stringmaker() << L"nice" << 123 << "hello";
  CPPUNIT_ASSERT((b=="nice123hello"));
}

void utils_6bees_util_test::testMakeValidFielname(){
  std::wstring b=_6bees_str::GenFileName(L"[Ja{}\\()#~|s|a:asdfa]s");
  CPPUNIT_ASSERT((b==L" Ja        s a asdfa s"));

  //std::wstring c=_6bees_str::makeValidFilename(L"hello,to,kitty,an,by,if",ENGLISH_STOP_WORDS,_T(","));
  //CPPUNIT_ASSERT((c==L"hello,kitty"));
}

void utils_6bees_util_test::testultrapath(){
  const wstring& p = _6bees_util::ultrapath::R().get(_6bees_util::ultrapath::webcontentsdir);
  wcout << p.size() << "\t" << p.c_str() << endl;

  const wstring p2 = _6bees_util::ultrapath::R().get(_6bees_util::ultrapath::tempicondir);
  wcout << p2.size() << "\t" << p2.c_str() << endl;
  
  BOOL b = PathIsDirectoryW(L"c:\\Documents and Settings\\simonwoo\\application Data\\6beenetworks\\ultraie\\cache\\web\\_ultraIE.com_1237000267");
  CPPUNIT_ASSERT(b);
}