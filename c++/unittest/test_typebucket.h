/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_TYPEBUCKET__
#define __TEST_TYPEBUCKET__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_TypeBucket : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_TypeBucket );
    CPPUNIT_TEST( testDefaultConstruction );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testCopyConstruction );
    CPPUNIT_TEST( testAccessOperator );
    CPPUNIT_TEST( testIdentical );
    CPPUNIT_TEST( testEqualOperator );
    CPPUNIT_TEST( testNotEqualOperator );
    CPPUNIT_TEST( testGreaterOrEqual );
    CPPUNIT_TEST( testLessThan );
    CPPUNIT_TEST( testComparisonOperator );
    CPPUNIT_TEST( testMatch );
    CPPUNIT_TEST( testAdd );
    CPPUNIT_TEST_SUITE_END();

    void testDefaultConstruction();
    void testConstruction();
    void testCopyConstruction();
    void testAccessOperator();
    void testIdentical();
    void testEqualOperator();
    void testNotEqualOperator();
    void testGreaterOrEqual();
    void testLessThan();
    void testAssignmentOperator();
    void testComparisonOperator();
    void testMatch();
    void testAdd();

};

#endif

