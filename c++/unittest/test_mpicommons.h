/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_MPICOMMONS__
#define __TEST_MPICOMMONS__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_MPICommons : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_MPICommons );
    CPPUNIT_TEST( testSize );
    CPPUNIT_TEST( testRank );
    CPPUNIT_TEST( testIsMaster );
    CPPUNIT_TEST( testBarrier );
    CPPUNIT_TEST_SUITE_END();

    void testSize();
    void testRank();
    void testIsMaster();
    void testBarrier();
};

#endif

