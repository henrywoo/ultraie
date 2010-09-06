#ifndef __ICE_TEST__
#define __ICE_TEST__

#include <cppunit\extensions\HelperMacros.h>

using namespace CppUnit;

class ice_test : public TestFixture{

  CPPUNIT_TEST_SUITE(ice_test);

  CPPUNIT_TEST(logintest);

  CPPUNIT_TEST_SUITE_END();

protected:
public:
  ice_test(){}
  void setUp();
  void tearDown();

  void logintest();

};

#endif