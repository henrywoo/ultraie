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

#include <cppunit\extensions\TestFactoryRegistry.h>
#include <cppunit\ui\text\TestRunner.h>
#include "../../dll/6beecommond/6beecommond.h"

using namespace CppUnit;

#include <hash_map>
#include <algorithm>
#include <string>
#include <6bees_net.h>

using namespace std;


int main(int argc, char* argv[]){

  _6beed_util::GetVersionBlk(L"S:\\uutoolbar\\bin\\debug\\6beemousehook.dll");

  //int clen=0;
  //_6bees_net::GetDataByHTTP("http://www.ultraie.com/setcookie.php",clen);
  
  TextUi::TestRunner runner;
  TestFactoryRegistry &registry = TestFactoryRegistry::getRegistry("ultratest");
  runner.addTest(registry.makeTest());
  runner.run();
  return 1;
}