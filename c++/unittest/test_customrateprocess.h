/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_CUSTOMRATEPROCESS__
#define __TEST_CUSTOMRATEPROCESS__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_CustomRateProcess : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_CustomRateProcess );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testMatchListLong );
    CPPUNIT_TEST( testTotalRate );
    CPPUNIT_TEST( testTotalRateMultiplicity );
    CPPUNIT_TEST( testAddAndRemoveSite );
    CPPUNIT_TEST( testPickSite );
    CPPUNIT_TEST( testPickSiteMultiplicity );
    CPPUNIT_TEST( testAffectedIndices );
    CPPUNIT_TEST( testCutoffAndRange );
    CPPUNIT_TEST( testProcessNumber );
    CPPUNIT_TEST( testCacheRate );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testMatchListLong();
    void testTotalRate();
    void testTotalRateMultiplicity();
    void testAddAndRemoveSite();
    void testPickSite();
    void testPickSiteMultiplicity();
    void testAffectedIndices();
    void testCutoffAndRange();
    void testProcessNumber();
    void testCacheRate();

};

#endif

