#ifndef __CHTTPSOCKET_TEST_H_
#define __CHTTPSOCKET_TEST_H_

#include <cppunit\extensions\HelperMacros.h>

using namespace CppUnit;

class chttpsockettest : public TestFixture{

  CPPUNIT_TEST_SUITE(chttpsockettest);
  
  CPPUNIT_TEST(testGetHTML);
  CPPUNIT_TEST(testGetField);
  CPPUNIT_TEST(testSetTimeOut);
  CPPUNIT_TEST(testSetTimeOut_HINTERNET);
  
  
  CPPUNIT_TEST_SUITE_END();

protected:
  int v1,v2;
public:
  chttpsockettest(){}
  void setUp();
  void tearDown();
  
  void testGetField();
  void testGetHTML();
  void testSetTimeOut();
  void testSetTimeOut_HINTERNET();
};

#endif