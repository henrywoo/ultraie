#ifndef __CHTTPSOCKET_TEST_H2_
#define __CHTTPSOCKET_TEST_H2_

#include <cppunit\extensions\HelperMacros.h>

using namespace CppUnit;

class utils_httpclient_test : public TestFixture{

  CPPUNIT_TEST_SUITE(utils_httpclient_test);

  CPPUNIT_TEST(testGetHTML);
  CPPUNIT_TEST(testSetTimeOut);
  CPPUNIT_TEST(testResumeDownload);

  CPPUNIT_TEST_SUITE_END();

protected:
public:
  utils_httpclient_test(){}
  void setUp();
  void tearDown();

  void testGetHTML();
  void testSetTimeOut();
  void testResumeDownload();
};

#endif