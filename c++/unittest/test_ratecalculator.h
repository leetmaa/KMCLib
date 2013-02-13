/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_RATECALCULATOR__
#define __TEST_RATECALCULATOR__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_RateCalculator : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_RateCalculator );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testRateCallback );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testRateCallback();

};

#endif

