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
    MatchListEntry e(1324,2,1.2,Coordinate(0.1,0.2,0.34));
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
    }

    // Two not equal in index, should equate to equal.
    {
        MatchListEntry e1(1322,2,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( !(e1 != e2) );
    }

    // Two not equal in type should equate to not equal.
    {
        MatchListEntry e1(1324,1,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );
    }

    // Two not equal in distance should equate to not equal.
    {
        MatchListEntry e1(1324,2,1.2003,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2000,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );
    }

    // If the distance difference is below eps_ hardcoded on the class
    // the difference is not visible.
    {
        MatchListEntry e1(1324,2,1.200000001,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.200000000,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( !(e1 != e2) );
    }

    // But if the difference is larger it becomes visible.
    {
        MatchListEntry e1(1324,2,1.2001,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2000,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );
    }

    // Two not equal in coordinate should equate to not equal.
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1001,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1000,0.2,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );
    }
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.4,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );
    }
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.2,0.24));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( (e1 != e2) );
    }

    // But if the difference is smaller than eps_ the value is
    // regarded as equal.
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.100001,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.100000,0.2,0.34));
        CPPUNIT_ASSERT( !(e1 != e2) );
    }
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.200002,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.200000,0.34));
        CPPUNIT_ASSERT( !(e1 != e2) );
    }
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.1,0.2,0.3400001));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.3400000));
        CPPUNIT_ASSERT( !(e1 != e2) );
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
    }

    // e1 smaller than e2 in index -> equal
    {
        MatchListEntry e1(1323,2,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( !(e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );
    }

    // e1 smaller than e2 in type
    {
        MatchListEntry e1(1324,1,1.2,Coordinate(0.1,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.1,0.2,0.34));
        CPPUNIT_ASSERT( (e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );
    }

    // e1 smaller than e2 in x
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.01,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.10,0.2,0.34));
        CPPUNIT_ASSERT( (e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );
    }

    // e1 smaller than e2 in y
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.10,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.10,0.3,0.34));
        CPPUNIT_ASSERT( (e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );
    }

    // e1 smaller than e2 in z
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.10,0.2,0.34));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.10,0.2,0.44));
        CPPUNIT_ASSERT( (e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );
    }

    // e1 smaller than e2 in z, also when the difference is tiny.
    {
        MatchListEntry e1(1324,2,1.2,Coordinate(0.10,0.2,0.44));
        MatchListEntry e2(1324,2,1.2,Coordinate(0.10,0.2,0.440000000000001));
        CPPUNIT_ASSERT( (e1 < e2) );
        CPPUNIT_ASSERT( !(e2 < e1) );
    }
}
