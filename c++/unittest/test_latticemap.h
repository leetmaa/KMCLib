/*
  Copyright (c)  2012-2013  Mikael Leetmaa

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
    CPPUNIT_TEST( testConstructionAndQuery );
    CPPUNIT_TEST( testIndicesFromCell );
    CPPUNIT_TEST( testIndexFromMoveInfo );
    CPPUNIT_TEST( testIndexFromMoveInfoPeriodic );
    CPPUNIT_TEST( testNeighbourIndices );
    CPPUNIT_TEST( testNeighbourIndicesMinimal );
    CPPUNIT_TEST( testNeighbourIndicesMinimal2 );
    CPPUNIT_TEST( testNeighbourIndicesLong );
    CPPUNIT_TEST( testSupersetNeighbourIndices );
    CPPUNIT_TEST( testWrap );
    CPPUNIT_TEST( testWrapLong );
    CPPUNIT_TEST( testBasisSiteFromIndex );
    CPPUNIT_TEST_SUITE_END();

    void testConstructionAndQuery();
    void testIndicesFromCell();
    void testIndexFromMoveInfo();
    void testIndexFromMoveInfoPeriodic();
    void testNeighbourIndices();
    void testNeighbourIndicesMinimal();
    void testNeighbourIndicesMinimal2();
    void testNeighbourIndicesLong();
    void testSupersetNeighbourIndices();
    void testWrap();
    void testWrapLong();
    void testBasisSiteFromIndex();

};

#endif

