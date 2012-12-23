/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_matchlistentry.h"

// Include the files to test.
#include "../src/matchlistentry.h"


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testConstruction()
{
    // Construct.
    MatchListEntry e0;
    MatchListEntry e(1324,2,1.2,Coordinate(0.1,0.2,0.34));
    //
    MinimalMatchListEntry m0;
    m0.match_type = 1324;
    m0.update_type = 2;
    m0.distance = 1.2;
    m0.coordinate = Coordinate(0.1,0.2,0.34);
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testNotEqualOperator()
{
    // Two equal.
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( !(e1 != e2) );

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

    // Two not equal in index, should equate to equal.
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.2,0.34), 1);
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34), 2);
        CPPUNIT_ASSERT( !(e1 != e2) );

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
        MatchListEntry e1(1321,2,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );

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
        MatchListEntry e1(1324,1,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( !(e1 != e2) );

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
        MatchListEntry e1(1324,2,1.2003,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2000,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );

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
        MatchListEntry e1(1324,2,1.200000001,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.200000000,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( !(e1 != e2) );

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
        MatchListEntry e1(1324,2,1.2001,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2000,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1001,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1000,0.2,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.4,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.2,0.24));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.100001,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.100000,0.2,0.34));
        CPPUNIT_ASSERT( !(e1 != e2) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.200002,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.200000,0.34));
        CPPUNIT_ASSERT( !(e1 != e2) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.2,0.3400001));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.3400000));
        CPPUNIT_ASSERT( !(e1 != e2) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( !(e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.2,0.34), 1);
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34), 3);
        CPPUNIT_ASSERT( !(e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );

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

    // e1 smaller than e2 in match type
    {
        MatchListEntry e1(1321,2,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT(  (e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );

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

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in update type
    {
        MatchListEntry e1(1324,1,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( !(e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.01,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.10,0.2,0.34));
        CPPUNIT_ASSERT(  (e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.10,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.10,0.3,0.34));
        CPPUNIT_ASSERT( (e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.10,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.10,0.2,0.44));
        CPPUNIT_ASSERT( (e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );

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
        MatchListEntry e1(1324,2,1.2,Coordinate(0.10,0.2,0.44));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.10,0.2,0.440000000000001));
        CPPUNIT_ASSERT( (e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );

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
    // Construct.
    MatchListEntry e(1324,2,1.2,Coordinate(0.1,0.2,0.34),3);

    // Query and check.
    CPPUNIT_ASSERT_EQUAL(e.matchType(), 1324);
    CPPUNIT_ASSERT_EQUAL(e.updateType(), 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(e.distance(), 1.2, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(e.coordinate().x(), 0.1, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(e.coordinate().y(), 0.2, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(e.coordinate().z(), 0.34, 1.0e-14);

}

