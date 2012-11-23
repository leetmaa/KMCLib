/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_LATTICEMAP__
#define __TEST_LATTICEMAP__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_LatticeMap : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_LatticeMap );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();

};

#endif

