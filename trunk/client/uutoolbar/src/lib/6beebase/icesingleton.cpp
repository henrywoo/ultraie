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

#include "IceSingleton.h"
#include <Ice/Ice.h>
#ifdef NDEBUG
#pragma comment(lib,"ice.lib")
#pragma comment(lib,"iceutil.lib")
#else
#pragma comment(lib,"iced.lib")
#pragma comment(lib,"iceutild.lib")
#endif

#include "uulogging.h"
#include "6bees_net.h"
#include <string>
#include <6bees_lang.h>

bool MyICE::initialized = false;
MyICE* MyICE::pinstance = NULL;
cmutex MyICE::lock_;

/// double check
MyICE& MyICE::Instance(){
  if(!initialized){
    cguard g(lock_);
    if(pinstance == NULL){ 
      static MyICE p_;
      pinstance = &p_;
      if(pinstance){ //make sure compiler cannot reorder 
        initialized = true; // only set to true after creation 
      }
    } 
  } 
  return *pinstance;
}

UUTOOLBAR::UserPrx MyICE::R(){
  MyICE& m = Instance();
  if (m._user==NULL){
    m.InitialzieICE();
  }
  return m._user;
}

MyICE::MyICE():_user(NULL){}

/// @brief initialize ICE run time
/// @todo multi-thread scenario
/// @todo to call ic->destroy()
void MyICE::InitialzieICE(){
  static char* endpoint=NULL;
  try{
    int argc = 1; 
    char **argv = new char*[1];
    argv[0]=new char[1];
    strcpy_s(argv[0],1,"");
    Ice::CommunicatorPtr ic = Ice::initialize(argc, argv);

    if (endpoint==NULL){
      int clen=0;
      endpoint = _6bees_net::GetDataByHTTP(epurl,clen);
      //endpoint = "UUICE:tcp -h 118.123.7.85 -p 9090 -t 9000 -z";
      if (endpoint==NULL){
        UUDEBUG((LOG_ERROR,"Cannot connect to endpoint URL."));
        return;
      }else{
        Ice::ObjectPrx base = ic->stringToProxy(endpoint);
        _user = UUTOOLBAR::UserPrx::checkedCast(base);
      }
    }else{
      Ice::ObjectPrx base = ic->stringToProxy(endpoint);
      _user = UUTOOLBAR::UserPrx::checkedCast(base);
    }
  }catch(const Ice::Exception& e){
    const char* s = e.what();
     UUDEBUG((LOG_ERROR,s));
  }catch (...) {
    UUDEBUG((LOG_ERROR,"ICE unexpected error!"));
  } 
}