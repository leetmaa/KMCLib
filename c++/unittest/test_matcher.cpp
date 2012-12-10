/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_matcher.h"

// Include the file to test.
#include "../src/matcher.h"

#include "../src/matchlistentry.h"


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testConstruction()
{
    // Construct.
    Matcher m;
}

// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchMatchList()
{
    // Construct.
    Matcher m;

    // Setup two match lists to check.
    std::vector<MatchListEntry> process_match_list;
    process_match_list.push_back(MatchListEntry(3,2,1.2,Coordinate(0.1,0.2,0.34)));
    process_match_list.push_back(MatchListEntry(1,4,1.6,Coordinate(1.1,0.2,0.34)));
    process_match_list.push_back(MatchListEntry(8,2,1.9,Coordinate(0.1,5.2,0.34)));

    std::vector<MatchListEntry> index_match_list;
    index_match_list.push_back(MatchListEntry(3,2,1.2,Coordinate(0.1,0.2,0.34)));
    index_match_list.push_back(MatchListEntry(1,4,1.6,Coordinate(1.1,0.2,0.34)));
    index_match_list.push_back(MatchListEntry(8,2,1.9,Coordinate(0.1,5.2,0.34)));

    // These two are equal.
    CPPUNIT_ASSERT( m.isMatch(process_match_list, index_match_list) );

    // It works also if the index match list is longer.
    index_match_list.push_back(MatchListEntry(8,2,4.9,Coordinate(3.1,5.2,0.34)));

    // These two are still equal.
    CPPUNIT_ASSERT( m.isMatch(process_match_list, index_match_list) );

    // If we add another not matching element to the process_match_list vecctor
    // we get a non-match.
    process_match_list.push_back(MatchListEntry(1,2,4.9,Coordinate(3.1,5.2,0.34)));

    // These two are no longer equal.
    CPPUNIT_ASSERT( !m.isMatch(process_match_list, index_match_list) );

    // But if they differ in the update index they are still equal.
    process_match_list[3] = MatchListEntry(8,1,4.9,Coordinate(3.1,5.2,0.34));

    // These two are again equal.
    CPPUNIT_ASSERT( m.isMatch(process_match_list, index_match_list) );

}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchIndexList()
{
    // Construct.
    Matcher m;

    // NEEDS IMPLEMENTATION
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testCalculateMatchingProcess()
{
    // Construct.
    Matcher m;

    // NEEDS IMPLEMENTATION
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testCalculateMatchingInteractions()
{
    // Construct.
    Matcher m;

    // NEEDS IMPLEMENTATION
}

