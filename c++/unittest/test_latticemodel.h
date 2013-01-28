/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_LATTICEMODEL__
#define __TEST_LATTICEMODEL__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_LatticeModel : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_LatticeModel );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testSetupAndQuery );
    CPPUNIT_TEST( testSingleStepFunction );
    //CPPUNIT_TEST( testTiming );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testSetupAndQuery();
    void testSingleStepFunction();
    void testTiming();

};

#endif

