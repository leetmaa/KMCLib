/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_MATCHER__
#define __TEST_MATCHER__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_Matcher : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_Matcher );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testIsMatchMatchList );
    CPPUNIT_TEST( testIsMatchWildcard );
    CPPUNIT_TEST( testIsMatchIndexListMinimal );
    CPPUNIT_TEST( testIsMatchIndexListMinimalPeriodic );
    CPPUNIT_TEST( testIsMatchIndexListComplicatedPeriodic );
    CPPUNIT_TEST( testCalculateMatchingProcess );
    CPPUNIT_TEST( testUpdateProcesses );
    CPPUNIT_TEST( testCalculateMatchingInteractions );
    CPPUNIT_TEST( testUpdateRates );
    CPPUNIT_TEST( testUpdateSingleRate );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testIsMatchMatchList();
    void testIsMatchWildcard();
    void testIsMatchIndexListMinimal();
    void testIsMatchIndexListMinimalPeriodic();
    void testIsMatchIndexListComplicatedPeriodic();
    void testCalculateMatchingProcess();
    void testUpdateProcesses();
    void testCalculateMatchingInteractions();
    void testUpdateRates();
    void testUpdateSingleRate();

};

#endif

