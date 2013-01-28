/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_matchlistentry.h"

// Include the files to test.
#include "matchlistentry.h"


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testConstruction()
{
    // Construct.
    MinimalMatchListEntry m0;
    m0.match_type = 1324;
    m0.update_type = 2;
    m0.distance = 1.2;
    m0.coordinate = Coordinate(0.1,0.2,0.34);
    m0.index = 123;
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testNotEqualOperator()
{
    // Two equal.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( !(m1 != m2) );
    }

    // Two equal - by wildcard.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 0;
        m1.update_type = 2;
        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( !(m1 != m2) );
        CPPUNIT_ASSERT(  (m2 != m1) );
    }

    // Two not equal in index, should equate to equal.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 2;

        CPPUNIT_ASSERT( !(m1 != m2) );
    }

    // Two not equal in match type should equate to not equal.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1322;
        m1.update_type = 2;
        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( (m1 != m2) );

    }

    // Two not equal in update type should still equate to equal.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1322;
        m1.update_type = 2;
        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1322;
        m2.update_type = 33;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 != m2) );
    }

    // Two not equal in distance should equate to not equal.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1322;
        m1.update_type = 2;
        m1.distance = 1.23;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1322;
        m2.update_type = 2;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( (m1 != m2) );
    }

    // If the distance difference is below eps_ hardcoded on the class
    // the difference is not visible.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.200000001;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.200000000;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 != m2) );

    }

    // But if the difference is larger it becomes visible.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.2001;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.200000000;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( (m1 != m2) );

    }

    // Two not equal in coordinate should equate to not equal.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1001,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( (m1 != m2) );

    }
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.4,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( (m1 != m2) );
    }
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.24);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( (m1 != m2) );
    }

    // But if the difference is smaller than eps_ the value is
    // regarded as equal.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.100001,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 != m2) );

    }
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.200002,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 != m2) );
    }
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.3400001);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 != m2) );
    }

}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testLessOperator()
{
   // Two equal.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in index -> equal
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 3;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in match type, thus equal.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1322;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in update type
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );

    }

    // e1 smaller than e2 in x
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.11,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );

    }

    // e1 smaller than e2 in y
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.21,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in z
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.341);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in z, also when the difference is tiny.
    {
        MinimalMatchListEntry m1;
        m1.match_type = 1324;
        m1.update_type = 2;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        MinimalMatchListEntry m2;
        m2.match_type = 1324;
        m2.update_type = 2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.340000000000001);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testQuery()
{
    MinimalMatchListEntry m1;
    m1.match_type = 1324;
    m1.update_type = 2;
    m1.distance = 1.20;
    m1.coordinate = Coordinate(0.1,0.2,0.34);
    m1.index = 1;

    CPPUNIT_ASSERT_EQUAL( m1.match_type, 1324 );
    CPPUNIT_ASSERT_EQUAL( m1.update_type, 2 );
    CPPUNIT_ASSERT_EQUAL( m1.index, 1 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( m1.distance, 1.20, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( m1.coordinate.x(), 0.1, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( m1.coordinate.y(), 0.2, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( m1.coordinate.z(), 0.34, 1.0e-14);

}

