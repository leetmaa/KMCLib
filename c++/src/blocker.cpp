/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  blocker.cpp
 *  \brief File for the implementation code of the Blocker class.
 */


#include <cstdio>


#include "blocker.h"
#include "coordinate.h"

// -----------------------------------------------------------------------------
//
Blocker::Blocker(const int nbins, const int blocksize) :
    blocksize_(blocksize),
    counts_since_last_block_(nbins, 0),
    histogram_block_(nbins, Coordinate(0.0, 0.0, 0.0)),
    hst_blocks_(nbins, std::vector<Coordinate>(0))
{
    // NOTHING HERE
}


// -----------------------------------------------------------------------------
//
void Blocker::registerStep(const int bin, const Coordinate value)
{
    histogram_block_[bin] += value;
    counts_since_last_block_[bin] += 1;

    if (counts_since_last_block_[bin] == blocksize_)
    {
        // This is the block histogram value for this bin.
        hst_blocks_[bin].push_back(histogram_block_[bin]);

        // Reset the count.
        counts_since_last_block_[bin] = 0;
        histogram_block_[bin] = Coordinate(0.0, 0.0, 0.0);
    }
}


// -----------------------------------------------------------------------------
//
std::vector< std::pair<Coordinate, Coordinate> > Blocker::values(const std::vector<int> & histogram_bin_counts,
                                                                 const std::vector<Coordinate> & histogram_buffer) const
{
    // Setup the data per bin.
    std::vector< std::pair<Coordinate, Coordinate> > \
        data_per_bin(histogram_bin_counts.size(),
                     std::pair<Coordinate,Coordinate>(Coordinate(0.0, 0.0, 0.0),
                                                      Coordinate(0.0, 0.0, 0.0)));
    // For each bin.
    for (size_t i = 0; i < hst_blocks_.size(); ++i)
    {
        // Get the MSD value at this bin.
        const int bincount = (histogram_bin_counts[i] != 0) ? histogram_bin_counts[i] : 1;
        const Coordinate rho_run(histogram_buffer[i].x()/bincount,
                                 histogram_buffer[i].y()/bincount,
                                 histogram_buffer[i].z()/bincount);

        // For each block at this bin.
        const int nblocks = static_cast<int>(hst_blocks_[i].size());

        // If there are any blocks that is.
        std::pair<Coordinate,Coordinate>sigma(Coordinate(-1.0, -1.0, -1.0), Coordinate(-1.0, -1.0, -1.0));

        if (nblocks > 1)
        {
            Coordinate bin_sum(0.0, 0.0, 0.0);
            for (int j = 0; j < nblocks; ++j)
            {
                // Divide the block data with the blocksize.
                const Coordinate tmp0(hst_blocks_[i][j].x()/blocksize_,
                                      hst_blocks_[i][j].y()/blocksize_,
                                      hst_blocks_[i][j].z()/blocksize_);

                // Subtract the value for the whole run.
                const Coordinate tmp1 = tmp0 - rho_run;

                // Square this value.
                const Coordinate sqr(tmp1.x()*tmp1.x(),
                                     tmp1.y()*tmp1.y(),
                                     tmp1.z()*tmp1.z());
                // And sum.
                bin_sum += sqr;
            }

            // Get the c0 value according to the blocking algorithm.
            const Coordinate c0_value(bin_sum.x()/nblocks,
                                      bin_sum.y()/nblocks,
                                      bin_sum.z()/nblocks);

            // Calculate the estimate of the standard deviation.
            const double std_x = std::sqrt(c0_value.x()/(nblocks-1.0));
            const double std_y = std::sqrt(c0_value.y()/(nblocks-1.0));
            const double std_z = std::sqrt(c0_value.z()/(nblocks-1.0));
            const Coordinate std(std_x, std_y, std_z);

            // Calculate the standard deviation of the standard deviation estimate.
            const double std_std_x = std_x / std::sqrt(2*nblocks-2.0);
            const double std_std_y = std_y / std::sqrt(2*nblocks-2.0);
            const double std_std_z = std_z / std::sqrt(2*nblocks-2.0);
            const Coordinate std_std(std_std_x, std_std_y, std_std_z);

            sigma = std::pair<Coordinate,Coordinate>(std, std_std);
        }

        // Store the data.
        data_per_bin[i] = sigma;
    }

    // Return the data.
    return data_per_bin;
}

