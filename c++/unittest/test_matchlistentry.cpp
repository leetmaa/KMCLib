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
    // NEEDS IMPLEMENTATION
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testNotEqualOperator()
{
    // NEEDS IMPLEMENTATION
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testLessOperator()
{
    // NEEDS IMPLEMENTATION
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testSamePoint()
{
    // Two equal.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(3);
        m1.match_types[2] = 3;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( m1.samePoint(m2) );
        CPPUNIT_ASSERT( m2.samePoint(m1) );
    }

    // Still the same point if we change th match types completely.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(4);
        m1.match_types[2] = 11;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( m1.samePoint(m2) );
        CPPUNIT_ASSERT( m2.samePoint(m1) );
    }

    // Not the same point if we chang the distance.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(4);
        m1.match_types[2] = 11;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2001;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( !m1.samePoint(m2) );
        CPPUNIT_ASSERT( !m2.samePoint(m1) );
    }
    // Not the same point if we chang the coordinate in x.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(4);
        m1.match_types[2] = 11;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.101,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( !m1.samePoint(m2) );
        CPPUNIT_ASSERT( !m2.samePoint(m1) );
    }
    // Not the same point if we chang the coordinate in y.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(4);
        m1.match_types[2] = 11;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2001;
        m1.coordinate = Coordinate(0.1,0.201,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( !m1.samePoint(m2) );
        CPPUNIT_ASSERT( !m2.samePoint(m1) );
    }
    // Not the same point if we chang the coordinate in z.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(4);
        m1.match_types[2] = 11;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2001;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.3401);

        CPPUNIT_ASSERT( !m1.samePoint(m2) );
        CPPUNIT_ASSERT( !m2.samePoint(m1) );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testProcessBucketEntryConstruction()
{
    // NEEDS IMPLEMENTATION

    // Construct.
    ProcessBucketMatchListEntry m0;
    m0.distance = 1.2;
    m0.coordinate = Coordinate(0.1,0.2,0.34);
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testConfigBucketEntryConstruction()
{
    // Construct.
    ConfigBucketMatchListEntry m0;
    m0.match_types = TypeBucket(5);
    m0.match_types[0] = 1;
    m0.match_types[1] = 0;
    m0.match_types[2] = 3;
    m0.match_types[3] = 6;
    m0.match_types[4] = 0;
    m0.match_types[5] = 9;
    m0.distance = 1.2;
    m0.coordinate = Coordinate(0.1,0.2,0.34);
    m0.index = 123;
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testProcessConfigNotEqual()
{
    // Two equal.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(3);
        m1.match_types[2] = 3;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( m1.match(m2) );
    }

    // Two equal - by wildcard.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(3);
        m1.match_types[0] = 1;  // Wildcard flag for the process bucket entry.

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[1] = 2;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( m1.match(m2) );
    }

    // Two not equal in index, should equate to equal.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(3);
        m1.match_types[2] = 3;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 2;

        CPPUNIT_ASSERT( m1.match(m2) );
    }

    // Two not equal in match type should equate to not equal.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(3);
        m1.match_types[2] = 4;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        // m2 match at m1, but not the other way around.
        CPPUNIT_ASSERT( !m1.match(m2) );
    }

    // Two not equal in distance should equate to not equal.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(3);
        m1.match_types[2] = 3;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.22;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( !m1.match(m2) );
    }

    // If the distance difference is below eps_ hardcoded on the class
    // the difference is not visible.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(3);
        m1.match_types[2] = 3;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.200000001;
        m2.coordinate = Coordinate(0.1,0.2,0.34);

        CPPUNIT_ASSERT( m1.match(m2) );
    }

    // Two not equal in coordinate should equate to not equal.
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(3);
        m1.match_types[2] = 3;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.11,0.2,0.34);

        CPPUNIT_ASSERT( !m1.match(m2) );
    }
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(3);
        m1.match_types[2] = 3;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.coordinate = Coordinate(0.1,0.21,0.34);

        CPPUNIT_ASSERT( !m1.match(m2) );
    }
    {
        ProcessBucketMatchListEntry m1;
        m1.match_types = TypeBucket(3);
        m1.match_types[2] = 3;

        m1.update_types = TypeBucket(3);
        m1.update_types[2] = 2;

        m1.distance = 1.2;
        m1.coordinate = Coordinate(0.1,0.2,0.34);

        ConfigBucketMatchListEntry m2;
        m2.match_types = TypeBucket(3);
        m2.match_types[2] = 3;
        m2.distance = 1.2;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.coordinate = Coordinate(0.1,0.2,0.341);

        CPPUNIT_ASSERT( !m1.match(m2) );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testProcessBucketLessOperator()
{
   // Two equal.
    {
        ProcessBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ConfigBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in index -> equal
    {
        ProcessBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ConfigBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 3;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in match type, thus equal.
    {
        ProcessBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ConfigBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in update type
    {
        ProcessBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ConfigBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );

    }

    // e1 smaller than e2 in x
    {
        ProcessBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ConfigBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.11,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );

    }

    // e1 smaller than e2 in y
    {
        ProcessBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ConfigBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.21,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in z
    {
        ProcessBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ConfigBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.341);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in z, also when the difference is tiny.
    {
        ProcessBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ConfigBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.340000000000001);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testConfigBucketLessOperator()
{
   // Two equal.
    {
        ConfigBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ProcessBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in index -> equal
    {
        ConfigBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ProcessBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 3;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in match type, thus equal.
    {
        ConfigBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ProcessBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in update type
    {
        ConfigBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ProcessBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT( !(m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );

    }

    // e1 smaller than e2 in x
    {
        ConfigBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ProcessBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.11,0.2,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );

    }

    // e1 smaller than e2 in y
    {
        ConfigBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ProcessBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.21,0.34);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in z
    {
        ConfigBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ProcessBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.341);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }

    // e1 smaller than e2 in z, also when the difference is tiny.
    {
        ConfigBucketMatchListEntry m1;
        m1.distance = 1.20;
        m1.coordinate = Coordinate(0.1,0.2,0.34);
        m1.index = 1;

        ProcessBucketMatchListEntry m2;
        m2.distance = 1.20;
        m2.coordinate = Coordinate(0.1,0.2,0.340000000000001);
        m2.index = 1;

        CPPUNIT_ASSERT(  (m1 < m2) );
        CPPUNIT_ASSERT( !(m2 < m1) );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testQueryProcessBucket()
{
    // NEEDS IMPLEMENTATION
}


// -------------------------------------------------------------------------- //
//
void Test_MatchListEntry::testQueryConfigBucket()
{
    // NEEDS IMPLEMENTATION
}

