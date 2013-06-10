/*
  Copyright (c)  2012-2013  Mikael Leetmaa

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

// -----------------------------------------------------------------------------
//
Process::Process(const Configuration & first,
                 const Configuration & second,
                 const double rate,
                 const std::vector<int> & basis_sites,
                 const std::vector<int> & move_origins,
                 const std::vector<Coordinate> & move_vectors) :
    range_(1),
    rate_(rate),
    cutoff_(0.0),
    sites_(0),
    affected_indices_(0),
    basis_sites_(basis_sites),
    id_moves_(0)
{
    // Get a handle to the coordinates and elements.
    const std::vector<Coordinate> & coords  = first.coordinates();

    // Get the first coordinate out to calculate the distance against.
    const Coordinate origin = coords[0];

    // Transform the configurations into match lists.
    for (size_t i = 0; i < first.elements().size(); ++i)
    {
        // Get the types as integers.
        const int first_type  = first.types()[i];
        const int second_type = second.types()[i];

        // Calculate the distance.
        const Coordinate coordinate = coords[i];
        const double distance = coordinate.distance(origin);

        // Save the cutoff.
        if (distance > cutoff_)
        {
            cutoff_ = distance;
        }

        // Calculate the range based on the coordinates, such that all
        // needed coordinates are guarranteed to be included.
        const double x = coordinate.x();
        const int cmp_x = static_cast<int>( ( x < 0.0 ) ? (-1.0*x)+0.99999 : x );
        range_ = std::max(cmp_x, range_);

        const double y = coordinate.y();
        const int cmp_y = static_cast<int>( ( y < 0.0 ) ? (-1.0*y)+0.99999 : y );
        range_ = std::max(cmp_y, range_);

        const double z = coordinate.z();
        const int cmp_z = static_cast<int>( ( z < 0.0 ) ? (-1.0*z)+0.99999 : z );
        range_ = std::max(cmp_z, range_);

        // Set up the match list.
        MinimalMatchListEntry m;
        m.match_type  = first_type;
        m.update_type = second_type;
        m.distance    = distance;
        m.coordinate  = coordinate;
        m.index       = -1;
        m.has_move_coordinate = false;
        minimal_match_list_.push_back(m);

        // If the first and second type differ increase the length of the
        // affected_sites list accordingly.
        if (first_type != second_type)
        {
            affected_indices_.push_back(0);
        }
    }

    // Loop over the move vector origins and place the move vectors
    // on the match list entries before sorting.
    for (size_t i = 0; i < move_origins.size(); ++i)
    {
        const int move_origin = move_origins[i];
        minimal_match_list_[move_origin].move_coordinate = move_vectors[i];
        minimal_match_list_[move_origin].has_move_coordinate = true;
    }

    // Sort the match list.
    std::sort(minimal_match_list_.begin(), minimal_match_list_.end());

    // Find out which index in the match list each move vector
    // points to.
    for (size_t i = 0; i < minimal_match_list_.size(); ++i)
    {
        if (minimal_match_list_[i].has_move_coordinate)
        {
            // If this move vector is different from zero we go on and try to find
            // which index in the sorted match list it points to.

            // Get the move vector out.
            const Coordinate & move_vector = minimal_match_list_[i].move_coordinate;

            // Setup the destination coordinate.
            const Coordinate destination = minimal_match_list_[i].coordinate + move_vector;

            for (size_t j = 0; j < minimal_match_list_.size(); ++j)
            {
                // We can only move to a coordinate which also has a
                // move coordinate.
                if (minimal_match_list_[j].has_move_coordinate && (j != i) )
                {
                    // If the difference is small enough we have a match.
                    const Coordinate diff = minimal_match_list_[j].coordinate - destination;

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

