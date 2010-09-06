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