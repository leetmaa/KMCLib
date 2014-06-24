/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  matchlist.cpp
 *  \brief File for the implementation code of the match lists.
 */

#include <cstdlib>

#include "matchlist.h"
#include "configuration.h"

// -----------------------------------------------------------------------------
//
void configurationsToMatchList(const Configuration & first,
                               const Configuration & second,
                               int & range,
                               double & cutoff,
                               ProcessBucketMatchList & match_list,
                               std::vector<int> & affected_indices)
{
    // Get a handle to the coordinates and elements.
    const std::vector<Coordinate> & coords = first.coordinates();

    // Get the first coordinate out to calculate the distance against.
    const Coordinate origin = coords[0];

    // To set the size of the match types list.
    const size_t n_types = first.typeNames().size();

    // Transform the configurations into match lists.
    for (size_t i = 0; i < first.elements().size(); ++i)
    {
        // Get the types as integers.
        // ML: This is where we update for buckets.
        const int first_type  = first.types()[i];
        const int second_type = second.types()[i];

        // Calculate the distance.
        const Coordinate coordinate = coords[i];
        const double distance = coordinate.distance(origin);

        // Save the cutoff.
        if (distance > cutoff)
        {
            cutoff = distance;
        }

        // Calculate the range based on the coordinates, such that all
        // needed coordinates are guarranteed to be included.
        const double x = coordinate.x();
        const int cmp_x = static_cast<int>( ( x < 0.0 ) ? (-1.0*x)+0.99999 : x );
        range = std::max(cmp_x, range);

        const double y = coordinate.y();
        const int cmp_y = static_cast<int>( ( y < 0.0 ) ? (-1.0*y)+0.99999 : y );
        range = std::max(cmp_y, range);

        const double z = coordinate.z();
        const int cmp_z = static_cast<int>( ( z < 0.0 ) ? (-1.0*z)+0.99999 : z );
        range = std::max(cmp_z, range);

        // Set up the match list entry.
        ProcessBucketMatchListEntry pm;

        // Initialize with zeros.
        pm.match_types  = std::vector<int>(n_types, 0);
        pm.update_types = std::vector<int>(n_types, 0);

        // ML: update here later for buckets.
        pm.match_types[first_type]   = 1;
        pm.update_types[second_type] = 1;
        pm.distance    = distance;
        pm.coordinate  = coordinate;
        pm.index       = -1;

        // ML: Handle move coordinates separately later.
        pm.has_move_coordinate = false;
        pm.move_coordinate = Coordinate(0.0, 0.0, 0.0);

        // Add the entry.
        match_list.push_back(pm);

        // If the first and second type differ increase the length of the
        // affected_indices list accordingly.
        for (size_t i = 0; i < n_types; ++i)
        {
            if (pm.match_types[i] != pm.update_types[i])
            {
                affected_indices.push_back(0);
                break;
            }
        }
    }
}

