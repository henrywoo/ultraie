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

#include "icetest.h"
#include <iostream>

#include <icesingleton.h>
#include "../../src/dll/6beecommond/6beecommond.h"

using namespace std;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ice_test,"ultratest2");

void ice_test::setUp(){}

void ice_test::tearDown(){}

int a=1;

void string_replace(string& str,const string &needle,const string &s){
  string::size_type lastpos=0, thispos=0;
  while ((thispos = str.find(needle, lastpos)) != string::npos){
    str.replace(thispos, needle.length(), s);
    lastpos = thispos + s.size();
  }
}


UINT WINAPI LoginProc(LPVOID pParam){
  try{
    UUTOOLBAR::UserPrx _user = MyICE::R();
    if(_user._ptr!=NULL){
      cout << "A = " << a++ << "Thread ID: " << GetCurrentThreadId() << ". Connect ICE Server successfully" << endl;
      string sid;
      if(_user->Login(_6beed_share::u_getEmail(),_6beed_share::u_getPassword(),sid)){
        cout << "Thread ID: " << GetCurrentThreadId() << ". Log in successfully" << endl;
        if(!_user->Logout(sid)){
          cout << "Thread ID: " << GetCurrentThreadId() << ". Log out failed" << endl;
        }
      }
    }else{
      cout << "Thread ID: " << GetCurrentThreadId() << ". Connect ICE Server failed" << endl;
    }
  }catch(const Ice::Exception& e){
    cout << __FUNCTION__  << ":(" << __LINE__ << ") - Unexpected run-time error: " << e << endl;
  }catch(...){
    cout << "ERROR" << endl;
  }
  return 0;
}

void ice_test::logintest(){

  string fname = "ACE Programmer's Guide, The Practical Design Patterns for Network and Systems Programming.chm";
  string_replace(fname,"'"," \\'");

  for (int i=0;i<10;i++){
    ::_beginthreadex(NULL,0,LoginProc,NULL,0,NULL);
  }
  Sleep(1000*1000);

}