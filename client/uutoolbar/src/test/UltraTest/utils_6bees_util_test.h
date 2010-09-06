#ifndef __UTILS_6BEES_UTIL_TEST__
#define __UTILS_6BEES_UTIL_TEST__

#include <cppunit\extensions\HelperMacros.h>

using namespace CppUnit;

class utils_6bees_util_test : public TestFixture{

  CPPUNIT_TEST_SUITE(utils_6bees_util_test);

  CPPUNIT_TEST(testgetFavicon);
  CPPUNIT_TEST(testCStringMaker);
  CPPUNIT_TEST(testMakeValidFielname);
  CPPUNIT_TEST(testultrapath);

  CPPUNIT_TEST_SUITE_END();

protected:
public:
  utils_6bees_util_test(){}
  void setUp();
  void tearDown();

  void testgetFavicon();
  void testCStringMaker();
  void testMakeValidFielname();
  void testultrapath();
};

#endif