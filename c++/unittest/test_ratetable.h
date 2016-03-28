/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_RATETABLE__
#define __TEST_RATETABLE__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_RateTable : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_RateTable );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testStored );
    CPPUNIT_TEST( testStoreAndRetrieve );
    CPPUNIT_TEST( testRetrieveFail );
    CPPUNIT_TEST( testStoreFail );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testStored();
    void testStoreAndRetrieve();
    void testRetrieveFail();
    void testStoreFail();
};

#endif

