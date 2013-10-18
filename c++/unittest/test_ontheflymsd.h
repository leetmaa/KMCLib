/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_ONTHEFLYMSD__
#define __TEST_ONTHEFLYMSD__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_OnTheFlyMSD : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_OnTheFlyMSD );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testStepX );
    CPPUNIT_TEST( testStepY );
    CPPUNIT_TEST( testStepZ );
    CPPUNIT_TEST( testCalculateAndBinMSD );
    CPPUNIT_TEST( testCalculateAndBinMSDTransformation );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testStepX();
    void testStepY();
    void testStepZ();
    void testCalculateAndBinMSD();
    void testCalculateAndBinMSDTransformation();

};

#endif

