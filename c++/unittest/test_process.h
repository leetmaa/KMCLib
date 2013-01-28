/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_PROCESS__
#define __TEST_PROCESS__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_Process : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_Process );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testMatchList );
    CPPUNIT_TEST( testMatchListLong );
    CPPUNIT_TEST( testAddAndRemoveSite );
    CPPUNIT_TEST( testPickSite );
    CPPUNIT_TEST( testAffectedIndices );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testMatchList();
    void testMatchListLong();
    void testAddAndRemoveSite();
    void testPickSite();
    void testAffectedIndices();

};

#endif

