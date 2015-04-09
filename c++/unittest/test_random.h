/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#ifndef __TEST_RANDOM__
#define __TEST_RANDOM__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_Random : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_Random );
    CPPUNIT_TEST( testSeedAndCall );
    CPPUNIT_TEST( testCallMT );
    CPPUNIT_TEST( testCallRANLUX24 );
    CPPUNIT_TEST( testCallRANLUX48 );
    CPPUNIT_TEST( testCallMINSTD );
    CPPUNIT_TEST_SUITE_END();

    void testSeedAndCall();
    void testCallMT();
    void testCallRANLUX24();
    void testCallRANLUX48();
    void testCallMINSTD();

};

#endif

