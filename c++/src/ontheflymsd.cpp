/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  ontheflymsd.cpp
 *  \brief File for the implementation code of the OnTheFlyMSD class.
 */

#include "ontheflymsd.h"
#include "configuration.h"
#include <cstdio>

// -----------------------------------------------------------------------------
//
OnTheFlyMSD::OnTheFlyMSD(const Configuration & configuration,
                         const int history_steps,
                         const int n_bins,
                         const double t_max,
                         const double t0,
                         const std::string track_type,
                         const std::vector<Coordinate> & abc_to_xyz,
                         const int blocksize) :
    history_buffer_(configuration.elements().size(), std::vector<std::pair<Coordinate, double> >(0)),
    histogram_buffer_(n_bins, Coordinate(0.0, 0.0, 0.0)),
    histogram_buffer_sqr_(n_bins, Coordinate(0.0, 0.0, 0.0)),
    histogram_bin_counts_(n_bins, 0),
    track_type_(track_type),
    t_max_(t_max),
    bin_size_(t_max/n_bins),
    history_steps_(history_steps),
    history_steps_bin_counts_(history_steps-1, std::vector<int>(n_bins, 0)),
    abc_to_xyz_(abc_to_xyz),
    hstep_counts_(history_steps, 0),
    blocker_(n_bins, blocksize)
{
    // Populate the history buffer with initial coordinates for tracked atoms.
    const std::vector<Coordinate> & atom_id_coords = configuration.atomIDCoordinates();
    const std::vector<std::string> & types  = configuration.atomIDElements();

    for (size_t i = 0; i < atom_id_coords.size(); ++i)
    {
        if (types[i] == track_type_)
        {
            history_buffer_[i].push_back(std::pair<Coordinate, double>(atom_id_coords[i], t0));
        }
    }
}


// -----------------------------------------------------------------------------
//
void OnTheFlyMSD::registerStep(const double time,
                               const Configuration & configuration)
{
    // Get the moved atom IDs.
    const std::vector<int> & moved_atom_ids = configuration.movedAtomIDs();
    const std::vector<std::string> & types  = configuration.atomIDElements();

    for (size_t i = 0; i < moved_atom_ids.size(); ++i)
    {
        // Check if this id is one of our moved types.
        const int id = moved_atom_ids[i];

        if (types[id] == track_type_)
        {
            // Make place in the history buffer.
            if (history_buffer_[id].size() < history_steps_)
            {
                history_buffer_[id].push_back(std::pair<Coordinate, double>(Coordinate(0.0, 0.0, 0.0), 0.0));
            }

            // Move the history.
            for (int j = history_buffer_[id].size() - 1; j > 0; --j)
            {
                history_buffer_[id][j] = history_buffer_[id][j-1];
            }

            // Store the new coordinate in the history buffer.
            history_buffer_[id][0] = std::pair<Coordinate, double>(configuration.atomIDCoordinates()[id], time);

            // Calculate and bin the values.
            calculateAndBinMSD(history_buffer_[id],
                               abc_to_xyz_,
                               bin_size_,
                               histogram_buffer_,
                               histogram_buffer_sqr_,
                               histogram_bin_counts_,
                               history_steps_bin_counts_,
                               hstep_counts_,
                               blocker_);
        }
    }
}


// -----------------------------------------------------------------------------
//
std::vector< std::pair<Coordinate, Coordinate> > OnTheFlyMSD::printBlockerValues() const
{
    printf("This is where we place the first blocker output\n");

    // Setup the data per bin.
    std::vector< std::pair<Coordinate, Coordinate> > \
        data_per_bin(histogram_bin_counts_.size(),
                     std::pair<Coordinate,Coordinate>(Coordinate(0.0, 0.0, 0.0),
                                                      Coordinate(0.0, 0.0, 0.0)));

    const Blocker & blocker = blocker_;

    // Get the block data.
    const std::vector< std::vector<Coordinate> > & block_data = blocker.hstBlocks();
    const double blocksize = blocker.blocksize();
    const size_t nbins     = block_data.size();

    // For each bin.
    for (size_t i = 0; i < nbins; ++i)
    {
        // Get the MSD value at this bin.
        const int bincount = (histogram_bin_counts_[i] != 0) ? histogram_bin_counts_[i] : 1;

        // FIXME
        const Coordinate rho_run((histogram_buffer_[i].x()+histogram_buffer_[i].y())/bincount,
                                 histogram_buffer_[i].y()/bincount,
                                 (histogram_buffer_[i].x()+histogram_buffer_[i].y()+histogram_buffer_[i].z())/bincount);

        // For each block at this bin.
        const int nblocks = static_cast<int>(block_data[i].size());

        // If there are any blocks that is.
        std::pair<Coordinate,Coordinate>sigma(Coordinate(-1.0, -1.0, -1.0), Coordinate(-1.0, -1.0, -1.0));

        if (nblocks > 0)
        {
            Coordinate bin_sum(0.0, 0.0, 0.0);
            for (int j = 0; j < nblocks; ++j)
            {
                // Divide the block data with the blocksize.
                // FIXME
                const Coordinate tmp0((block_data[i][j].x()+block_data[i][j].y())/blocksize,
                                      block_data[i][j].y()/blocksize,
                                      (block_data[i][j].x()+block_data[i][j].y() + block_data[i][j].z())/blocksize);

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


// -----------------------------------------------------------------------------
//
void calculateAndBinMSD(const std::vector< std::pair<Coordinate, double> > & history,
                        const std::vector<Coordinate> & abc_to_xyz,
                        const double binsize,
                        std::vector<Coordinate> & histogram,
                        std::vector<Coordinate> & histogram_sqr,
                        std::vector<int> & bin_counters,
                        std::vector< std::vector<int> > & hsteps_bin_counts)
{
    // Loop over the history buffer.
    for (size_t i = 1; i < history.size(); ++i)
    {
        // Calculat the bin.
        const double dt  = history[0].second - history[i].second;
        const size_t bin = static_cast<int>(dt/binsize);

        if (bin < histogram.size())
        {
            // If within range, calculate the squared diff and the squared diff squared.
            const Coordinate diff_abc = (history[i].first - history[0].first);

            // Transform the abc difference to xyz.
            const Coordinate diff(diff_abc.dot(abc_to_xyz[0]),
                                  diff_abc.dot(abc_to_xyz[1]),
                                  diff_abc.dot(abc_to_xyz[2]));

            const Coordinate sqr_diff = diff.outerProdDiag(diff);
            const Coordinate sqr_diff_sqr = sqr_diff.outerProdDiag(sqr_diff);

            // Store in the histograms.
            histogram[bin]     += sqr_diff;
            histogram_sqr[bin] += sqr_diff_sqr;
            ++bin_counters[bin];
            ++hsteps_bin_counts[i-1][bin];
        }
    }
}


// -----------------------------------------------------------------------------
//
void calculateAndBinMSD(const std::vector< std::pair<Coordinate, double> > & history,
                        const std::vector<Coordinate> & abc_to_xyz,
                        const double binsize,
                        std::vector<Coordinate> & histogram,
                        std::vector<Coordinate> & histogram_sqr,
                        std::vector<int> & bin_counters,
                        std::vector< std::vector<int> > & hsteps_bin_counts,
                        std::vector<int> & hstep_counts,
                        Blocker & blocker)
{
    // Loop over the history buffer.
    for (size_t i = 1; i < history.size(); ++i)
    {
        // ML: FIXME
        ++hstep_counts[i-1];

        // Calculat the bin.
        const double dt  = history[0].second - history[i].second;
        const size_t bin = static_cast<int>(dt/binsize);

        if (bin < histogram.size())
        {
            // If within range, calculate the squared diff and the squared diff squared.
            const Coordinate diff_abc = (history[i].first - history[0].first);

            // Transform the abc difference to xyz.
            const Coordinate diff(diff_abc.dot(abc_to_xyz[0]),
                                  diff_abc.dot(abc_to_xyz[1]),
                                  diff_abc.dot(abc_to_xyz[2]));

            const Coordinate sqr_diff = diff.outerProdDiag(diff);
            const Coordinate sqr_diff_sqr = sqr_diff.outerProdDiag(sqr_diff);

            // Store in the histograms.
            histogram[bin]     += sqr_diff;
            histogram_sqr[bin] += sqr_diff_sqr;
            ++bin_counters[bin];
            ++hsteps_bin_counts[i-1][bin];

            // ML:
            blocker.registerStep(bin, sqr_diff);
        }
    }
}


// -----------------------------------------------------------------------------
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
