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
                         const std::string track_type) :
    history_buffer_(configuration.elements().size(), std::vector<std::pair<Coordinate, double> >(0)),
    histogram_buffer_(n_bins, Coordinate(0.0, 0.0, 0.0)),
    histogram_buffer_sqr_(n_bins, Coordinate(0.0, 0.0, 0.0)),
    histogram_bin_counts_(n_bins, 0),
    track_type_(track_type),
    t_max_(t_max),
    bin_size_(t_max/n_bins),
    history_steps_(history_steps)
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
                               bin_size_,
                               histogram_buffer_,
                               histogram_buffer_sqr_,
                               histogram_bin_counts_);
        }
    }
}


// -----------------------------------------------------------------------------
//
void calculateAndBinMSD(const std::vector< std::pair<Coordinate, double> > & history,
                        const double binsize,
                        std::vector<Coordinate> & histogram,
                        std::vector<Coordinate> & histogram_sqr,
                        std::vector<int> & bin_counters)
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
            const Coordinate diff = history[i].first - history[0].first;
            const Coordinate sqr_diff = diff.outerProdDiag(diff);
            const Coordinate sqr_diff_sqr = sqr_diff.outerProdDiag(sqr_diff);

            // Store in the histograms.
            histogram[bin]     += sqr_diff;
            histogram_sqr[bin] += sqr_diff_sqr;
            ++bin_counters[bin];
        }
    }
}
