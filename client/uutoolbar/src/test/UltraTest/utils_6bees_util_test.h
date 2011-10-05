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