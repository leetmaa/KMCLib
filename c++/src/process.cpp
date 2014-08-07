/*
  Copyright (c)  2012-2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  process.cpp
 *  \brief File for the implementation code of the Process class.
 */

#include <algorithm>
#include <cstdio>

#include "process.h"
#include "random.h"
#include "configuration.h"
#include "matchlistentry.h"

// -----------------------------------------------------------------------------
//
Process::Process(const Configuration & first,
                 const Configuration & second,
                 const double rate,
                 const std::vector<int> & basis_sites,
                 const std::vector<int> & move_origins,
                 const std::vector<Coordinate> & move_vectors,
                 const int process_number) :
    cache_rate_(false),
    process_number_(process_number),
    range_(1),
    rate_(rate),
    cutoff_(0.0),
    sites_(0),
    affected_indices_(0),
    basis_sites_(basis_sites),
    id_moves_(0)
{
    // Generate the matchlist from the configurations.
    configurationsToMatchList(first,
                              second,
                              range_,
                              cutoff_,
                              match_list_,
                              affected_indices_);

    // Loop over the move vector origins and place the move vectors
    // on the match list entries before sorting.
    for (size_t i = 0; i < move_origins.size(); ++i)
    {
        const int move_origin = move_origins[i];
        match_list_[move_origin].move_coordinate = move_vectors[i];
        match_list_[move_origin].has_move_coordinate = true;
    }

    // Sort the match list.
    std::sort(match_list_.begin(), match_list_.end());

    // Find out which index in the match list each move vector
    // points to.
    for (size_t i = 0; i < match_list_.size(); ++i)
    {
        if (match_list_[i].has_move_coordinate)
        {
            // If this move vector is different from zero we go on and try to find
            // which index in the sorted match list it points to.

            // Get the move vector out.
            const Coordinate & move_vector = match_list_[i].move_coordinate;

            // Setup the destination coordinate.
            const Coordinate destination = match_list_[i].coordinate + move_vector;

            for (size_t j = 0; j < match_list_.size(); ++j)
            {
                // We can only move to a coordinate which also has a
                // move coordinate.
                if (match_list_[j].has_move_coordinate && (j != i) )
                {
                    // If the difference is small enough we have a match.
                    const Coordinate diff = match_list_[j].coordinate - destination;

                    if (diff.norm() < 1.0e-6)
                    {
                        id_moves_.push_back(std::pair<int,int>(i,j));
                        break;
                    }
                }
            }
        }
    }
}


// -----------------------------------------------------------------------------
//
void Process::addSite(const int index, const double rate)
{
    sites_.push_back(index);
}

// -----------------------------------------------------------------------------
//
void Process::removeSite(const int index)
{
    // Find the index to remove.
    std::vector<int>::iterator it1 = std::find(sites_.begin(),
                                               sites_.end(),
                                               index);
    // Swap the index to remove with the last index.
    std::vector<int>::iterator last = sites_.end()-1;
    std::swap((*it1), (*last));
    // Remove the last index from the list.
    sites_.pop_back();
}

// -----------------------------------------------------------------------------
//
int Process::pickSite() const
{
    // Draw an integer between 0 and sites_.size() - 1
    const int rnd = static_cast<int>(randomDouble01() * sites_.size());
    return sites_[rnd];
}

// -----------------------------------------------------------------------------
//
bool Process::isListed(const int index) const
{
    // Search in the list to find out if it is added.
    return std::find(sites_.begin(), sites_.end(), index) != sites_.end();
}

