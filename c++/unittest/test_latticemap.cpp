/*
  Copyright (c)  2012-2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_latticemap.h"

// Include the files to test.
#include "latticemap.h"

#include "coordinate.h"
#include "configuration.h"
#include "matcher.h"

#include <algorithm>

// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testConstructionAndQuery()
{
    // Construct a map.

    std::vector<int> repetitions(3);
    repetitions[0] = 3;
    repetitions[1] = 5;
    repetitions[2] = 6;

    std::vector<bool> periodicity(3, true);

    CPPUNIT_ASSERT_NO_THROW( LatticeMap map(3, repetitions, periodicity) );

    LatticeMap map(3, repetitions, periodicity);

    // Test the query functions.
    CPPUNIT_ASSERT( map.periodicA() );
    CPPUNIT_ASSERT( map.periodicB() );
    CPPUNIT_ASSERT( map.periodicC() );

    periodicity[0] = false;
    LatticeMap map2(3, repetitions, periodicity);
    CPPUNIT_ASSERT( !map2.periodicA() );
    CPPUNIT_ASSERT(  map2.periodicB() );
    CPPUNIT_ASSERT(  map2.periodicC() );

    periodicity[1] = false;
    LatticeMap map3(3, repetitions, periodicity);
    CPPUNIT_ASSERT( !map3.periodicA() );
    CPPUNIT_ASSERT( !map3.periodicB() );
    CPPUNIT_ASSERT(  map3.periodicC() );

    periodicity[2] = false;
    LatticeMap map4(3, repetitions, periodicity);
    CPPUNIT_ASSERT( !map4.periodicA() );
    CPPUNIT_ASSERT( !map4.periodicB() );
    CPPUNIT_ASSERT( !map4.periodicC() );
    CPPUNIT_ASSERT_EQUAL( map4.nBasis(), 3 );


}

// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testIndicesFromCell()
{
    // This also tests the reverse mapping LatticeMap::indexToCell(...)

    {
        // A map with 1 in the basis.
        const int basis = 1;
        std::vector<int> repetitions(3);
        repetitions[0] = 3;
        repetitions[1] = 2;
        repetitions[2] = 4;
        const std::vector<bool> periodicity(3, false);
        const LatticeMap map(basis, repetitions, periodicity);

        // Check the basis.
        CPPUNIT_ASSERT_EQUAL( map.nBasis(), basis );

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < repetitions[0]; ++i) {
            for (int j = 0; j < repetitions[1]; ++j) {
                for (int k = 0; k < repetitions[2]; ++k) {
                    CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[0], index );
                    // Check the reverse mapping.
                    int tmp_i, tmp_j, tmp_k;

                    map.indexToCell(index, tmp_i, tmp_j, tmp_k);
                    CPPUNIT_ASSERT_EQUAL(tmp_i, i);
                    CPPUNIT_ASSERT_EQUAL(tmp_j, j);
                    CPPUNIT_ASSERT_EQUAL(tmp_k, k);

                    ++index;
                }
            }
        }
    }
    {
        // A map with 1 in the basis and other periodicity.
        const int basis = 1;
        std::vector<int> repetitions(3);
        repetitions[0] = 3;
        repetitions[1] = 2;
        repetitions[2] = 4;
        const std::vector<bool> periodicity(3, true);
        const LatticeMap map(basis, repetitions, periodicity);

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < repetitions[0]; ++i) {
            for (int j = 0; j < repetitions[1]; ++j) {
                for (int k = 0; k < repetitions[2]; ++k) {
                    CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[0], index );

                    // Check the reverse mapping.
                    int tmp_i, tmp_j, tmp_k;
                    map.indexToCell(index, tmp_i, tmp_j, tmp_k);
                    CPPUNIT_ASSERT_EQUAL(tmp_i, i);
                    CPPUNIT_ASSERT_EQUAL(tmp_j, j);
                    CPPUNIT_ASSERT_EQUAL(tmp_k, k);

                    ++index;
                }
            }
        }
    }
    {
        // A map with basis 2
        const int basis = 2;
        std::vector<int> repetitions(3);
        repetitions[0] = 3;
        repetitions[1] = 2;
        repetitions[2] = 4;
        const std::vector<bool> periodicity(3, false);
        const LatticeMap map(basis, repetitions, periodicity);

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < repetitions[0]; ++i) {
            for (int j = 0; j < repetitions[1]; ++j) {
                for (int k = 0; k < repetitions[2]; ++k) {
                    for (int l = 0; l < basis; ++l) {
                        CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[l], index );

                        // Check the reverse mapping.
                        int tmp_i, tmp_j, tmp_k;
                        map.indexToCell(index, tmp_i, tmp_j, tmp_k);
                        CPPUNIT_ASSERT_EQUAL(tmp_i, i);
                        CPPUNIT_ASSERT_EQUAL(tmp_j, j);
                        CPPUNIT_ASSERT_EQUAL(tmp_k, k);

                        ++index;
                    }
                }
            }
        }
    }

    {
        // A huge map with basis 12 (1 200 000 points)
        const int basis = 12;
        std::vector<int> repetitions(3);
        repetitions[0] = 100;
        repetitions[1] = 100;
        repetitions[2] = 10;
        const std::vector<bool> periodicity(3, false);
        const LatticeMap map(basis, repetitions, periodicity);

        // Check the basis.
        CPPUNIT_ASSERT_EQUAL( map.nBasis(), basis );

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < repetitions[0]; ++i) {
            for (int j = 0; j < repetitions[1]; ++j) {
                for (int k = 0; k < repetitions[2]; ++k) {
                    for (int l = 0; l < basis; ++l) {

                        CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[l], index );

                        // Check the reverse mapping.
                        int tmp_i, tmp_j, tmp_k;
                        map.indexToCell(index, tmp_i, tmp_j, tmp_k);
                        CPPUNIT_ASSERT_EQUAL(tmp_i, i);
                        CPPUNIT_ASSERT_EQUAL(tmp_j, j);
                        CPPUNIT_ASSERT_EQUAL(tmp_k, k);

                        ++index;
                    }
                }
            }
        }
    }

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testIndexFromMoveInfo()
{
    // Get us a lattice map to test. Non-periodic.
    const int map_basis = 3;
    std::vector<int> repetitions(3);
    repetitions[0] = 4;
    repetitions[1] = 2;
    repetitions[2] = 3;
    const std::vector<bool> periodicity(3, false);
    const LatticeMap map(map_basis, repetitions, periodicity);

    // Setup a dummy move info.
    const int move_i =  1;
    const int move_j = -1;
    const int move_k = -2;
    const int basis  = -1;

    // Index 16 is in cell 0,1,2 at basis possition 1.
    const int index  = 16;

    // The above move info should take us to the index
    // at basis position 0 in cell 1, 0, 0.
    const int ref_index = map.indicesFromCell(1,0,0)[0];

    const int test_index = map.indexFromMoveInfo(index,
                                                 move_i,
                                                 move_j,
                                                 move_k,
                                                 basis);

    // Check that this is true.
    CPPUNIT_ASSERT_EQUAL( ref_index, test_index );
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testIndexFromMoveInfoPeriodic()
{
    // Get us a lattice map to test. Periodic.
    const int map_basis = 3;
    std::vector<int> repetitions(3);
    repetitions[0] = 4;
    repetitions[1] = 2;
    repetitions[2] = 3;
    const std::vector<bool> periodicity(3, true);
    const LatticeMap map(map_basis, repetitions, periodicity);

    // Setup a dummy move info.
    const int move_i = -3;
    const int move_j = -1;
    const int move_k = -2;
    const int basis  =  0;

    // Index 2 is in cell 0,0,0 at basis possition 2.
    const int index  = 2;

    // The above move info should take us to the index
    // at basis position 2 in cell 1, 1, 1.
    const int ref_index = map.indicesFromCell(1,1,1)[2];

    const int test_index = map.indexFromMoveInfo(index,
                                                 move_i,
                                                 move_j,
                                                 move_k,
                                                 basis);

    // Check that this is true.
    CPPUNIT_ASSERT_EQUAL( ref_index, test_index );
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testNeighbourIndices()
{
    // Test that the neighbour indices are calculated and returned correctly.

    // Simplest case, base one, no periodicity and the queried cell in the
    // middle. This should return all indices.

    const int basis = 1;
    std::vector<int> repetitions(3);
    repetitions[0] = 3;
    repetitions[1] = 3;
    repetitions[2] = 3;
    const std::vector<bool> periodicity(3, false);
    const LatticeMap map(basis, repetitions, periodicity);

    // Get the central cell index.
    const int central_index = map.indicesFromCell(1,1,1)[0];

    // Get the neighbours.
    std::vector<int> neighbours = map.neighbourIndices(central_index);

    // The length of neighbours should be as many as there are cells.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(neighbours.size()), 3*3*3);

    // If we sort the indices they should be just the incremental numbers
    // from 0 to 26
    std::sort(neighbours.begin(), neighbours.end());
    for (int i = 0; i < 3*3*3; ++i)
    {
        CPPUNIT_ASSERT_EQUAL(neighbours[i], i);
    }

    // Now, check any other cell with periodicity.
    // The neighbours should be the same
    // after sorting.
    const std::vector<bool> periodicity2(3, true);
    const LatticeMap map2(basis, repetitions, periodicity2);

    std::vector<int> neighbours2 = map2.neighbourIndices(0);
    std::sort(neighbours2.begin(), neighbours2.end());

    std::vector<int> neighbours3 = map2.neighbourIndices(26);
    std::sort(neighbours3.begin(), neighbours3.end());

    // Check.
    for (size_t i = 0; i < neighbours.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(neighbours3[i], neighbours[i]);
    }

    // Remove periodicity from the first two directions.
    std::vector<bool> periodicity3(3, false);
    periodicity3[2] = true;
    const LatticeMap map3(basis, repetitions, periodicity3);

    // This should cut the 15 wrapped indices in the a and b directions.
    const std::vector<int> & neighbours4 = map3.neighbourIndices(0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(neighbours4.size()), static_cast<int>(neighbours.size())-15);

    // We should have indices: 2,0,1
    //                         5,3,4
    //                         11,9,10
    //                         14,12,13

    CPPUNIT_ASSERT_EQUAL(neighbours4[0], 2);
    CPPUNIT_ASSERT_EQUAL(neighbours4[1], 0);
    CPPUNIT_ASSERT_EQUAL(neighbours4[2], 1);

    CPPUNIT_ASSERT_EQUAL(neighbours4[3], 5);
    CPPUNIT_ASSERT_EQUAL(neighbours4[4], 3);
    CPPUNIT_ASSERT_EQUAL(neighbours4[5], 4);

    CPPUNIT_ASSERT_EQUAL(neighbours4[6], 11);
    CPPUNIT_ASSERT_EQUAL(neighbours4[7], 9);
    CPPUNIT_ASSERT_EQUAL(neighbours4[8], 10);

    CPPUNIT_ASSERT_EQUAL(neighbours4[9], 14);
    CPPUNIT_ASSERT_EQUAL(neighbours4[10], 12);
    CPPUNIT_ASSERT_EQUAL(neighbours4[11], 13);

    // Removing periodicity in the x,z directions instead we should have:
    // 6,7,0,1,3,4,15,16,9,10,12,13
    std::vector<bool> periodicity4(3, false);
    periodicity4[1] = true;
    const LatticeMap map4(basis, repetitions, periodicity4);

    const std::vector<int> & neighbours5 = map4.neighbourIndices(0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(neighbours5.size()), static_cast<int>(neighbours.size())-15);

    CPPUNIT_ASSERT_EQUAL(neighbours5[0], 6);
    CPPUNIT_ASSERT_EQUAL(neighbours5[1], 7);
    CPPUNIT_ASSERT_EQUAL(neighbours5[2], 0);

    CPPUNIT_ASSERT_EQUAL(neighbours5[3], 1);
    CPPUNIT_ASSERT_EQUAL(neighbours5[4], 3);
    CPPUNIT_ASSERT_EQUAL(neighbours5[5], 4);

    CPPUNIT_ASSERT_EQUAL(neighbours5[6], 15);
    CPPUNIT_ASSERT_EQUAL(neighbours5[7], 16);
    CPPUNIT_ASSERT_EQUAL(neighbours5[8],  9);

    CPPUNIT_ASSERT_EQUAL(neighbours5[9],  10);
    CPPUNIT_ASSERT_EQUAL(neighbours5[10], 12);
    CPPUNIT_ASSERT_EQUAL(neighbours5[11], 13);

    // Test with basis 2.
    const LatticeMap map6(basis*2, repetitions, periodicity4);
    const std::vector<int> & neighbours6 = map6.neighbourIndices(0);

    // Expecting twice the size.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(neighbours6.size())/2, static_cast<int>(neighbours.size())-15);

    // Checking the first half of the indices.
    CPPUNIT_ASSERT_EQUAL(neighbours6[0], 12);
    CPPUNIT_ASSERT_EQUAL(neighbours6[1], 13);
    CPPUNIT_ASSERT_EQUAL(neighbours6[2], 14);

    CPPUNIT_ASSERT_EQUAL(neighbours6[3], 15);
    CPPUNIT_ASSERT_EQUAL(neighbours6[4],  0);
    CPPUNIT_ASSERT_EQUAL(neighbours6[5],  1);

    CPPUNIT_ASSERT_EQUAL(neighbours6[6],  2);
    CPPUNIT_ASSERT_EQUAL(neighbours6[7],  3);
    CPPUNIT_ASSERT_EQUAL(neighbours6[8],  6);

    CPPUNIT_ASSERT_EQUAL(neighbours6[9],  7);
    CPPUNIT_ASSERT_EQUAL(neighbours6[10], 8);
    CPPUNIT_ASSERT_EQUAL(neighbours6[11], 9);

    // DONE.
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testNeighbourIndicesMinimal()
{
    // Minimal case, base two, no periodicity. One cell repetition.
    // This should return all indices in the cell.

    const int basis = 2;
    std::vector<int> repetitions(3);
    repetitions[0] = 1;
    repetitions[1] = 1;
    repetitions[2] = 1;
    const std::vector<bool> periodicity(3, false);
    const LatticeMap map(basis, repetitions, periodicity);

    // Get the central index.
    const int central_index = 0;

    // Get the neighbours.
    std::vector<int> neighbours = map.neighbourIndices(central_index);

    // The length of neighbours should be as many as there are cells times the basis.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(neighbours.size()), 1*1*1*basis);

    // If we sort the indices they should be just the incremental numbers
    // from 0 to 1
    std::sort(neighbours.begin(), neighbours.end());
    CPPUNIT_ASSERT_EQUAL(neighbours[0], 0);
    CPPUNIT_ASSERT_EQUAL(neighbours[1], 1);

    // DONE.
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testNeighbourIndicesMinimal2()
{
    // Minimal case, base two, no periodicity. Two repetitions in in z.
    // This should return all indices in the cell.

    const int basis = 2;
    std::vector<int> repetitions(3);
    repetitions[0] = 1;
    repetitions[1] = 1;
    repetitions[2] = 2;
    const std::vector<bool> periodicity(3, false);
    const LatticeMap map(basis, repetitions, periodicity);

    // Get the central index.
    const int central_index = 1;

    // Get the neighbours.
    std::vector<int> neighbours = map.neighbourIndices(central_index);

    // The length of neighbours should be as many as there are cells times the basis.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(neighbours.size()), 1*1*2*basis);

    // If we sort the indices they should be just the incremental numbers
    // from 0 to 3
    std::sort(neighbours.begin(), neighbours.end());
    CPPUNIT_ASSERT_EQUAL(neighbours[0], 0);
    CPPUNIT_ASSERT_EQUAL(neighbours[1], 1);
    CPPUNIT_ASSERT_EQUAL(neighbours[2], 2);
    CPPUNIT_ASSERT_EQUAL(neighbours[3], 3);

    // DONE.
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testNeighbourIndicesLong()
{
    // Realistic case, base two, periodicity in y and z. 16, 14 and 15 x, y and z repetitions.
    const int basis = 2;
    std::vector<int> repetitions(3);
    repetitions[0] = 16;
    repetitions[1] = 14;
    repetitions[2] = 15;
    const std::vector<bool> periodicity(3, true);
    const LatticeMap map(basis, repetitions, periodicity);

    // Setup a configuration.
    std::vector<std::vector<double> > coordinates(repetitions[0]*repetitions[1]*repetitions[2]*basis, std::vector<double>(3,0.0));
    std::vector<std::vector<std::string> > types;
    int cnt = 0;
    for (int i = 0; i < repetitions[0]; ++i)
    {
        for (int j = 0; j < repetitions[1]; ++j)
        {
            for (int k = 0; k < repetitions[2]; ++k)
            {
                coordinates[2*cnt][0] = i + 0.0;
                coordinates[2*cnt][1] = j + 0.0;
                coordinates[2*cnt][2] = k + 0.0;
                coordinates[2*cnt+1][0] = i + 0.5;
                coordinates[2*cnt+1][1] = j + 0.25;
                coordinates[2*cnt+1][2] = k + 0.25;
                ++cnt;
                types.push_back(std::vector<std::string>(1, "A"));
                types.push_back(std::vector<std::string>(1, "B"));
            }
        }
    }

    std::map<std::string,int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    Configuration configuration(coordinates, types, possible_types);

    // Get the central index.
    const int central_index = 10;

    // Get the neighbours.
    const std::vector<int> default_neighbours     = map.neighbourIndices(central_index);
    const std::vector<int> one_shell_neighbours   = map.neighbourIndices(central_index, 1);
    const std::vector<int> two_shell_neighbours   = map.neighbourIndices(central_index, 2);
    const std::vector<int> three_shell_neighbours = map.neighbourIndices(central_index, 3);

    // Check the sizes of these lists.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(default_neighbours.size()),     basis * 3 * 3 * 3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(one_shell_neighbours.size()),   basis * 3 * 3 * 3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(two_shell_neighbours.size()),   basis * 5 * 5 * 5 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(three_shell_neighbours.size()), basis * 7 * 7 * 7 );

    // Calculate corresponding match lists.
    ConfigBucketMatchList default_matchlist =                           \
        configuration.configMatchList(central_index, default_neighbours, map);

    ConfigBucketMatchList one_shell_matchlist =            \
        configuration.configMatchList(central_index, one_shell_neighbours, map);

    ConfigBucketMatchList two_shell_matchlist =                                               \
        configuration.configMatchList(central_index, two_shell_neighbours, map);

    ConfigBucketMatchList three_shell_matchlist =                                             \
        configuration.configMatchList(central_index, three_shell_neighbours, map);

    // Now, compare the match lists.
    ConfigBucketMatchList::const_iterator it0 = default_matchlist.begin();
    ConfigBucketMatchList::const_iterator it1 = one_shell_matchlist.begin();

    // These should be identical.
    for ( ; it0 != default_matchlist.end(); ++it0, ++it1 )
    {
        CPPUNIT_ASSERT( (*it0).match_types.identical((*it1).match_types) );
        CPPUNIT_ASSERT( samePoint(*it0, *it1) );
    }

    // Check that they are correctly sorted.
    ConfigBucketMatchList::const_iterator it2 = two_shell_matchlist.begin();
    double prev_dist = 0.0;
    for ( ; it2 != two_shell_matchlist.end(); ++it2 )
    {
        CPPUNIT_ASSERT( prev_dist <= (*it2).distance );
        prev_dist = (*it2).distance;
    }

    ConfigBucketMatchList::const_iterator it3 = three_shell_matchlist.begin();
    prev_dist = 0.0;
    for ( ; it3 != three_shell_matchlist.end(); ++it3 )
    {
        CPPUNIT_ASSERT( prev_dist <= (*it3).distance );
        prev_dist = (*it3).distance;
    }

    // Check a few hardcoded values.
    ConfigBucketMatchListEntry m2 = two_shell_matchlist[5*4*3*2+1];
    ConfigBucketMatchListEntry m3 = three_shell_matchlist[7*6*5*2+5];

    CPPUNIT_ASSERT_DOUBLES_EQUAL( m2.x, -2.0000000000e+00, 1.0e-10 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( m2.y, -1.0000000000e+00, 1.0e-10 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( m2.z, -1.0000000000e+00, 1.0e-10 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( m3.x, -2.0000000000e+00, 1.0e-10 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( m3.y, -3.0000000000e+00, 1.0e-10 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( m3.z, -1.0000000000e+00, 1.0e-10 );

    // DONE.
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testSupersetNeighbourIndices()
{
    // Construct a 1D map with two basis sites.
    std::vector<int> repetitions(3);
    repetitions[0] = 10;
    repetitions[1] = 1;
    repetitions[2] = 1;
    std::vector<bool> periodicity(3, false);
    periodicity[0] = true;
    LatticeMap map(2, repetitions, periodicity);

    // Get the neighbours out of these indices, with three shells cutoff.
    const int index0 = 0;

    const int shells = 2;
    std::vector<int> neighbours_idx0 = map.neighbourIndices(index0, shells);
    const int index1 = 1;
    const std::vector<int> neighbours_idx1 = map.neighbourIndices(index1, shells);

    // Check that these are equal.
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[0], neighbours_idx1[0]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[1], neighbours_idx1[1]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[2], neighbours_idx1[2]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[3], neighbours_idx1[3]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[4], neighbours_idx1[4]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[5], neighbours_idx1[5]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[6], neighbours_idx1[6]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[7], neighbours_idx1[7]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[8], neighbours_idx1[8]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[9], neighbours_idx1[9]);

    // Check that their overlap is the same after sorting.

    std::vector<int> indices(2, 0);
    indices[1] = 1;
    const std::vector<int> neighbours_idx01 =           \
        map.supersetNeighbourIndices(indices, shells);
    std::sort(neighbours_idx0.begin(), neighbours_idx0.end());

    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[0], neighbours_idx01[0]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[1], neighbours_idx01[1]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[2], neighbours_idx01[2]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[3], neighbours_idx01[3]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[4], neighbours_idx01[4]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[5], neighbours_idx01[5]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[6], neighbours_idx01[6]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[7], neighbours_idx01[7]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[8], neighbours_idx01[8]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx0[9], neighbours_idx01[9]);

    // Adding index 4 should include indices 6,7,8,9 in the overlap.
    indices.push_back(4);
    const std::vector<int> neighbours_idx014 =          \
        map.supersetNeighbourIndices(indices, shells);

    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[0], neighbours_idx01[0]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[1], neighbours_idx01[1]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[2], neighbours_idx01[2]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[3], neighbours_idx01[3]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[4], neighbours_idx01[4]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[5], neighbours_idx01[5]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[6], 6);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[7], 7);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[8], 8);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[9], 9);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[10], neighbours_idx01[6]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[11], neighbours_idx01[7]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[12], neighbours_idx01[8]);
    CPPUNIT_ASSERT_EQUAL( neighbours_idx014[13], neighbours_idx01[9]);

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testWrap()
{
    // Construct a periodic map.
    std::vector<int> repetitions(3);
    repetitions[0] = 3;
    repetitions[1] = 5;
    repetitions[2] = 6;
    const std::vector<bool> periodicity(3, true);
    LatticeMap map(3, repetitions, periodicity);

    // Check a few coordinates.

    {
        // Should be wrapped in all directions.
        Coordinate c(1.5, 2.5, 3.0);
        const Coordinate ref(-1.5, -2.5, -3.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should not be wrapped.
        Coordinate c(-1.5, -2.5, -3.0);
        const Coordinate ref(-1.5, -2.5, -3.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should be wrapped in a.
        Coordinate c(1.50, -2.5, 2.0);
        const Coordinate ref(-1.50, -2.5, 2.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should be wrapped in a.
        Coordinate c(-1.51, -2.5, 2.0);
        const Coordinate ref(1.49, -2.5, 2.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should be wrapped in b.
        Coordinate c(-1.47, 2.50, 2.0);
        const Coordinate ref(-1.47, -2.5, 2.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);

    }
    {
        // Should be wrapped in b.
        Coordinate c(-1.47, -4.30, 2.0);
        const Coordinate ref(-1.47, 0.70, 2.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should be wrapped in c.
        Coordinate c(-1.47, 0.70, 3.001);
        const Coordinate ref(-1.47, 0.70, -2.999);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should be wrapped in c.
        Coordinate c(-1.47, 0.70, -3.001);
        const Coordinate ref(-1.47, 0.70, 2.999);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }


    {
        // Check the componentwise wrapping.
        Coordinate c(1.5, 2.5, 3.0);
        const Coordinate refA(-1.5, 2.5, 3.0);
        const Coordinate refAB(-1.5, -2.5, 3.0);
        const Coordinate refABC(-1.5, -2.5, -3.0);
        // Wrap and check.
        map.wrap(c,0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(refA.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(refA.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(refA.z(), c.z(), 1.0e-14);
        map.wrap(c,1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(refAB.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(refAB.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(refAB.z(), c.z(), 1.0e-14);
        map.wrap(c,2);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(refABC.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(refABC.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(refABC.z(), c.z(), 1.0e-14);
    }
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testWrapLong()
{
    // Wrapping longer than one cell is not needed and will not be
    // supported now. This test checks the expected behaviour.

    // Construct a periodic map.
    std::vector<int> repetitions(3);
    repetitions[0] = 3;
    repetitions[1] = 5;
    repetitions[2] = 6;
    const std::vector<bool> periodicity(3, true);
    LatticeMap map(3, repetitions, periodicity);

    {
        // Should be wrapped in all directions.
        Coordinate c(1.5+3.0, 2.5+5.0, 3.0+12.0);

        // If wrapping more than one box length was supported this
        // would be the correct reference.
        const Coordinate ref(-1.5, -2.5, -3.0);

        // Now this is what we expect to get instead.
        const Coordinate ref2(1.5, 2.5, 9.0);

        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref2.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref2.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref2.z(), c.z(), 1.0e-14);
    }
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testBasisSiteFromIndex()
{
    // Construct a periodic map with 3 in the basis.
    std::vector<int> repetitions(3);
    repetitions[0] = 3;
    repetitions[1] = 5;
    repetitions[2] = 6;
    const std::vector<bool> periodicity(3, true);
    LatticeMap map(3, repetitions, periodicity);

    // Get the first cell basis.
    CPPUNIT_ASSERT_EQUAL( map.basisSiteFromIndex(0), 0 );
    CPPUNIT_ASSERT_EQUAL( map.basisSiteFromIndex(1), 1 );
    CPPUNIT_ASSERT_EQUAL( map.basisSiteFromIndex(2), 2 );

    // The result must be invariant if we add a multiple of 3 (the basis size)
    CPPUNIT_ASSERT_EQUAL( map.basisSiteFromIndex(18), 0 );
    CPPUNIT_ASSERT_EQUAL( map.basisSiteFromIndex(19), 1 );
    CPPUNIT_ASSERT_EQUAL( map.basisSiteFromIndex(20), 2 );
    CPPUNIT_ASSERT_EQUAL( map.basisSiteFromIndex(21), 0 );
    CPPUNIT_ASSERT_EQUAL( map.basisSiteFromIndex(22), 1 );
    CPPUNIT_ASSERT_EQUAL( map.basisSiteFromIndex(23), 2 );

    // Setup a new map with basis size 2
    LatticeMap map2(2, repetitions, periodicity);

    // Get the first cell basis.
    CPPUNIT_ASSERT_EQUAL( map2.basisSiteFromIndex(0), 0 );
    CPPUNIT_ASSERT_EQUAL( map2.basisSiteFromIndex(1), 1 );
    CPPUNIT_ASSERT_EQUAL( map2.basisSiteFromIndex(2), 0 );
    CPPUNIT_ASSERT_EQUAL( map2.basisSiteFromIndex(3), 1 );
    CPPUNIT_ASSERT_EQUAL( map2.basisSiteFromIndex(200), 0 );
    CPPUNIT_ASSERT_EQUAL( map2.basisSiteFromIndex(201), 1 );

}
