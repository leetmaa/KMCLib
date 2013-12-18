/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_MPIRoutines__
#define __TEST_MPIRoutines__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_MPIRoutines : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_MPIRoutines );
    CPPUNIT_TEST( testDetermineChunks );
    CPPUNIT_TEST( testDistributeToAll );
    CPPUNIT_TEST( testSumOverProcessesInt );
    CPPUNIT_TEST( testSumOverProcessesVectorInt );
    CPPUNIT_TEST( testSumOverProcessesVectorDouble );
    CPPUNIT_TEST( testSplitOverProcesses );
    CPPUNIT_TEST( testJoinOverProcesses );
    CPPUNIT_TEST_SUITE_END();

    void testDetermineChunks();
    void testDistributeToAll();
    void testSumOverProcessesInt();
    void testSumOverProcessesVectorInt();
    void testSumOverProcessesVectorDouble();
    void testSplitOverProcesses();
    void testJoinOverProcesses();

};

#endif

