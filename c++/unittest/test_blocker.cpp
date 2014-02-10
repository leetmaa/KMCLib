/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_blocker.h"

// Include the files to test.
#include "blocker.h"

#include "coordinate.h"

// -------------------------------------------------------------------------- //
//
void Test_Blocker::testConstruction()
{
    // Make sure we can construct the blocker.
    const int nbins = 12;
    const int blocksize = 1223;
    Blocker blocker(nbins, blocksize);
}


// -------------------------------------------------------------------------- //
//
void Test_Blocker::testRegisterStep()
{
    // Setup a blocker.
    const int nbins = 4;
    const int blocksize = 3;
    Blocker blocker(nbins, blocksize);

    // Check that the block data is empty.
    const int bin0 = 3;
    std::vector< std::vector<Coordinate> > blocks = blocker.hstBlocks();

    CPPUNIT_ASSERT_EQUAL(static_cast<int>(blocks.size()), 4);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(blocks[bin0].size()), 0);

    // Register a step.
    const Coordinate value0(0.1, 0.2, 0.3);
    blocker.registerStep(bin0, value0);

    // Still zero length.
    blocks = blocker.hstBlocks();

    CPPUNIT_ASSERT_EQUAL(static_cast<int>(blocks[bin0].size()), 0);

    // Register a step.
    const Coordinate value1(5.2, 5.3, 5.4);
    blocker.registerStep(bin0, value1);

    // Still zero length.
    blocks = blocker.hstBlocks();

    CPPUNIT_ASSERT_EQUAL(static_cast<int>(blocks[bin0].size()), 0);

    // Register a step.
    const Coordinate value2(3.3, 4.4, 5.5);
    blocker.registerStep(bin0, value2);

    // Now the number of steps is the same as the blocksize. We should
    // have one block at this bin.
    blocks = blocker.hstBlocks();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(blocks[bin0].size()), 1);

    // All other blocks are zero.
    for (int i = 0; i < nbins; ++i)
    {
        if (i != bin0)
        {
            CPPUNIT_ASSERT_EQUAL(static_cast<int>(blocks[i].size()), 0);
        }
    }

    // Check the content of the block. It should be the
    // average of all previous steps.
    const Coordinate ref((0.1 + 5.2 + 3.3),
                         (0.2 + 5.3 + 4.4),
                         (0.3 + 5.4 + 5.5));

    CPPUNIT_ASSERT_DOUBLES_EQUAL( blocks[bin0][0].x(), ref.x(), 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( blocks[bin0][0].y(), ref.y(), 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( blocks[bin0][0].z(), ref.z(), 1.0e-12);

    // Adding once more to the bin does not change anything.
    blocker.registerStep(bin0, value2);
    blocks = blocker.hstBlocks();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(blocks[bin0].size()), 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( blocks[bin0][0].x(), ref.x(), 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( blocks[bin0][0].y(), ref.y(), 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( blocks[bin0][0].z(), ref.z(), 1.0e-12);

    // But adding two more does.
    blocker.registerStep(bin0, value2);
    blocker.registerStep(bin0, value2);
    blocks = blocker.hstBlocks();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(blocks[bin0].size()), 2);

}


// -------------------------------------------------------------------------- //
//
void Test_Blocker::testValues()
{
    // Setup a simple blocker.
    const int nbins = 3;
    const int blocksize = 1;
    Blocker blocker(nbins, blocksize);

    // Add data.
    blocker.registerStep(0, Coordinate(0.1, 0.2, 0.3));
    blocker.registerStep(0, Coordinate(0.1, 0.2, 0.3));
    blocker.registerStep(1, Coordinate(3.1, 3.2, 3.3));
    blocker.registerStep(2, Coordinate(5.1, 5.2, 5.3));

    // Generate histogram MSD data.
    std::vector<int> bin_counts(3);
    bin_counts[0] = 3;
    bin_counts[1] = 2;
    bin_counts[2] = 5;

    std::vector<Coordinate> histogram;
    histogram.push_back(Coordinate(3.0, 4.5, 3.3));
    histogram.push_back(Coordinate(6.2, 8.5, 7.3));
    histogram.push_back(Coordinate(7.2, 6.5, 5.3));

    // Get the values.
    const std::vector< std::pair<Coordinate, Coordinate> > values =     \
        blocker.values(bin_counts,
                       histogram);

    // Construct the reference for the first bin.
    const Coordinate rho(histogram[0].x()/bin_counts[0],
                         histogram[0].y()/bin_counts[0],
                         histogram[0].z()/bin_counts[0]);

    // Divide the block data with the block size == 1.
    const Coordinate block_data(0.1, 0.2, 0.3);

    // Subtract the value and square the difference.
    const Coordinate diff = (block_data - rho);
    const Coordinate diff_sqr(diff.x()*diff.x(),
                              diff.y()*diff.y(),
                              diff.z()*diff.z());
    const Coordinate tmp = diff_sqr + diff_sqr;
    const Coordinate sum(tmp.x()/2.0, tmp.y()/2.0, tmp.z()/2.0);

    // to get the estimated standard deviation.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[0].first.x(), std::sqrt(sum.x()), 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[0].first.y(), std::sqrt(sum.y()), 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[0].first.z(), std::sqrt(sum.z()), 1.0e-12);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[1].first.x(), -1.0, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[1].first.y(), -1.0, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[1].first.z(), -1.0, 1.0e-12);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[2].first.x(), -1.0, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[2].first.y(), -1.0, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[2].first.z(), -1.0, 1.0e-12);

    // Check the error estimate of the standard deviation.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[0].second.x(), std::sqrt(sum.x()/2.0), 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[0].second.y(), std::sqrt(sum.y()/2.0), 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[0].second.z(), std::sqrt(sum.z()/2.0), 1.0e-12);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[1].second.x(), -1.0, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[1].second.y(), -1.0, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[1].second.z(), -1.0, 1.0e-12);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[2].second.x(), -1.0, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[2].second.y(), -1.0, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(values[2].second.z(), -1.0, 1.0e-12);

}
