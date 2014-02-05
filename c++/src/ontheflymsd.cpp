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
    bin_size_(t_max_/n_bins),
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
        // Add to the step count.
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

            // Register the step at the blocker.
            blocker.registerStep(bin, sqr_diff);
        }
    }
}
