#ifndef __UTILS_6BEES_NET_TEST__
#define __UTILS_6BEES_NET_TEST__

#include <cppunit\extensions\HelperMacros.h>

using namespace CppUnit;

class utils_6bees_net_test : public TestFixture{

  CPPUNIT_TEST_SUITE(utils_6bees_net_test);

  CPPUNIT_TEST(testapr_uri_parse);
  CPPUNIT_TEST(testGetContentLen);
  CPPUNIT_TEST(testIsSupportResume);
  CPPUNIT_TEST(testDownloadURLToFile);

  CPPUNIT_TEST_SUITE_END();

protected:
public:
  utils_6bees_net_test(){}
  void setUp();
  void tearDown();

  void testapr_uri_parse();
  void testGetContentLen();
  void testIsSupportResume();
  void testDownloadURLToFile();
};

#endif