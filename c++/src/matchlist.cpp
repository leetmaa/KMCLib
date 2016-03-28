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

    // The update types to set up.
    std::vector<TypeBucket> update_types;

    // If update info is present.
    if (second.updateInfo().size() != 0)
    {
        // Translate the update information to integer representation.
        std::vector<std::map<std::string, int> > update_info = second.updateInfo();

        for (size_t i = 0; i < update_info.size(); ++i)
        {
            // Get the update info for this site.
            const std::map<std::string, int> & info = update_info[i];
            TypeBucket update(second.possibleTypes().size());

            // Loop through the info and determine the corresponding integer representation.
            std::map<std::string, int>::const_iterator it1 = info.begin();
            for ( ; it1 != info.end(); ++it1 )
            {
                const int type = second.possibleTypes().find(it1->first)->second;
                const int diff = it1->second;
                update[type] = diff;
            }

            update_types.push_back(update);
        }
    }

    // If no update info was given we create it here.
    else
    {
        for (size_t i = 0; i < first.elements().size(); ++i)
        {
            const TypeBucket & t1 = first.types()[i];
            const TypeBucket & t2 = second.types()[i];
            TypeBucket update = t2;

            for (int j = 0; j < update.size(); ++j)
            {
                update[j] -= t1[j];
            }
            update_types.push_back(update);
        }
    }


    // Get the first coordinate out to calculate the distance against.
    const Coordinate origin = coords[0];

    // Transform the configurations into match lists.
    for (size_t i = 0; i < first.elements().size(); ++i)
    {
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

        // Set the values.
        pm.match_types  = first.types()[i];
        pm.update_types = update_types[i];

        pm.distance    = distance;
        pm.coordinate  = coordinate;
        pm.index       = -1;

        // Handle move coordinates separately later.
        pm.has_move_coordinate = false;
        pm.move_coordinate = Coordinate(0.0, 0.0, 0.0);

        // Add the entry.
        match_list.push_back(pm);

        // ML: FIXME:
        // If not all elements are zero in the update type,
        // increase the length of the affected indices list.

        for (int j = 0; j < pm.update_types.size(); ++j)
        {
            if (pm.update_types[j] != 0)
            {
                affected_indices.push_back(0);
                break;
            }
        }
    }
}


// -----------------------------------------------------------------------------
//
double multiplicity(const ProcessBucketMatchList & process_match_list,
                    const ConfigBucketMatchList & config_match_list)
{
    // Loop through the match lists and count the multiplicity for each site.
    ProcessBucketMatchList::const_iterator it1 = process_match_list.begin();
    ProcessBucketMatchList::const_iterator end = process_match_list.end();
    ConfigBucketMatchList::const_iterator it2  = config_match_list.begin();

    int m = 1;

    for ( ; it1 != end; ++it1, ++it2 )
    {
        const TypeBucket & t1 = it1->match_types;
        const TypeBucket & t2 = it2->match_types;

        // Wild cards are skipped.
        if (t1[0] == 0)
        {
            // For each element type.
            for (int i = 1; i < t1.size(); ++i)
            {
                // We only consider cases where the occupations is > 0.
                if (t1[i] > 0)
                {
                    // Local variables we need.
                    const int n = t2[i];
                    const int r = t1[i];
                    const int n_minus_r = n - r;
                    const int max = std::max(r, n_minus_r);
                    const int min = std::min(r, n_minus_r);

                    if (min != 0)
                    {
                        // Calculate n! / (r!(n-r)!)

                        // Get the numerator.
                        int local_n = t2[i];
                        for (int j = t2[i] - 1; j > max; --j)
                        {
                            local_n *= j;
                        }

                        // Calculate the denominator.
                        int local_r = min;
                        for (int j = min-1; j > 0; --j)
                        {
                            local_r *= j;
                        }

                        // Multiply m by (n! / (r! (n-r)!))
                        m *= (local_n / local_r);
                    }
                }
            }
        }
    }

    // Done.
    return static_cast<double>(m);
}
