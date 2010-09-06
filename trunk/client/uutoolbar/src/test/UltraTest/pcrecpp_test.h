#ifndef __PCRECPP_TEST__
#define __PCRECPP_TEST__

#include <cppunit\extensions\HelperMacros.h>

using namespace CppUnit;

class pcrecpp_test : public TestFixture{

  CPPUNIT_TEST_SUITE(pcrecpp_test);

  CPPUNIT_TEST(testmatch);

  CPPUNIT_TEST_SUITE_END();

protected:
public:
  pcrecpp_test(){}
  void setUp();
  void tearDown();

  void testmatch();

};

#endif