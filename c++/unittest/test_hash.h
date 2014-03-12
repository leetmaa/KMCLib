/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_HASH__
#define __TEST_HASH__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_Hash : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_Hash );
    CPPUNIT_TEST( testMD5String );
    CPPUNIT_TEST( test64MD5String );
    CPPUNIT_TEST( testHashCustomRateInput );
    CPPUNIT_TEST_SUITE_END();

    void testMD5String();
    void test64MD5String();
    void testHashCustomRateInput();

};

#endif

