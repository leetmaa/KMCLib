/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_latticemap.h"

// Include the files to test.
#include "../src/latticemap.h"

#include <algorithm>

// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testConstruction()
{
    // Construct a map.
    CPPUNIT_ASSERT_NO_THROW( LatticeMap map(3,3,4,5,false,true,false) );

    // DONE.

}

// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testIndicesFromCell()
{
    // LOCAL SCOPE FOR CLARITY
    {
        // A map with 1 in the basis.
        const int basis = 1;
        const int nA = 3;
        const int nB = 2;
        const int nC = 4;
        const LatticeMap map(basis,nA,nB,nC,false,false,false);

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < nA; ++i) {
            for (int j = 0; j < nB; ++j) {
                for (int k = 0; k < nC; ++k) {
                    CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[0], index );
                    ++index;
                }
            }
        }
    }
    {
        // A map with 1 in the basis and other periodicity.
        const int basis = 1;
        const int nA = 3;
        const int nB = 2;
        const int nC = 4;
        const LatticeMap map(basis,nA,nB,nC,true,true,true);

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < nA; ++i) {
            for (int j = 0; j < nB; ++j) {
                for (int k = 0; k < nC; ++k) {
                    CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[0], index );
                    ++index;
                }
            }
        }
    }
    {
        // A map with basis 2
        const int basis = 2;
        const int nA = 3;
        const int nB = 2;
        const int nC = 4;
        const LatticeMap map(basis,nA,nB,nC,false,false,false);

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < nA; ++i) {
            for (int j = 0; j < nB; ++j) {
                for (int k = 0; k < nC; ++k) {
                    for (int l = 0; l < basis; ++l) {
                        CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[l], index );
                        ++index;
                    }
                }
            }
        }
    }

    {
        // A huge map with basis 12 (1 200 000 points)
        const int basis = 12;
        const int nA = 100;
        const int nB = 100;
        const int nC = 10;

        const LatticeMap map(basis,nA,nB,nC,false,false,false);

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < nA; ++i) {
            for (int j = 0; j < nB; ++j) {
                for (int k = 0; k < nC; ++k) {
                    for (int l = 0; l < basis; ++l) {

                        CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[l], index );
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
void Test_LatticeMap::testNeighbourIndices()
{
    // Test that the neighbour indices are calculated and returned correctly.

    // Simplest case, base one, no periodicity and the queried cell in the
    // middle. This should return all indices.

    const int basis = 1;
    const int nA = 3;
    const int nB = 3;
    const int nC = 3;
    const LatticeMap map(basis,nA,nB,nC,false,false,false);

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
    const LatticeMap map2(basis,nA,nB,nC,true,true,true);

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
    const LatticeMap map3(basis,nA,nB,nC,false,false,true);

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
    const LatticeMap map4(basis,nA,nB,nC,false,true,false);
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
    const LatticeMap map6(basis*2,nA,nB,nC,false,true,false);
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
