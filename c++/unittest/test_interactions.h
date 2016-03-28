/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_INTERACTIONS__
#define __TEST_INTERACTIONS__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_Interactions : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_Interactions );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testQuery );
    CPPUNIT_TEST( testUpdateAndPick );
    CPPUNIT_TEST( testUpdateAndPickCustom );
    CPPUNIT_TEST( testMaxRange );
    CPPUNIT_TEST( testUpdateProcessMatchLists );
    CPPUNIT_TEST( testUpdateProcessIDMoves );
    CPPUNIT_TEST( testClearMatching );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testQuery();
    void testUpdateAndPick();
    void testUpdateAndPickCustom();
    void testMaxRange();
    void testUpdateProcessMatchLists();
    void testUpdateProcessIDMoves();
    void testClearMatching();

};

#endif

