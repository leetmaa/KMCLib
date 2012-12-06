/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  configuration.cpp
 *  \brief File for the implementation code of the Configuration class.
 */


#include <cstdio>

#include "configuration.h"
#include "latticemap.h"

// -----------------------------------------------------------------------------
//
Configuration::Configuration(std::vector<std::vector<double> > const & coordinates,
                             std::vector<std::string> const & elements,
                             const std::map<std::string,int> & possible_types) :
    elements_(elements)
{
    // Setup the coordinates.
    for (size_t i = 0; i < coordinates.size(); ++i)
    {
        coordinates_.push_back( Coordinate(coordinates[i][0],
                                           coordinates[i][1],
                                           coordinates[i][2]));
    }

    // Setup the types from the elements strings.
    for (size_t i = 0; i < elements_.size(); ++i)
    {
        const std::string element = elements_[i];
        const int type = possible_types.find(element)->second;
        types_.push_back(type);
    }
}


// -----------------------------------------------------------------------------
//
std::vector<MatchListEntry> Configuration::matchList(const std::vector<int> & indices,
                                                     const LatticeMap & lattice_map) const
{
    // Setup the return data.
    std::vector<MatchListEntry> match_list;

    // Extract the coordinate of the first index.
    const int idx0 = indices[0];
    const Coordinate center = coordinates_[idx0];
    const Coordinate origin(0.0, 0.0, 0.0);

    // For each index: center, wrap and calculate the distance.
    for (size_t i = 0; i < indices.size(); ++i)
    {
        // Get the index.
        const int index = indices[i];

        // Center.
        Coordinate c = coordinates_[index] - center;

        // Wrap with coorect periodicity.
        lattice_map.wrap(c);

        // Calculate the distance.
        const double distance = c.distance(origin);

        // Get the type.
        const int match_type = types_[index];

        // Push back on the match list.
        match_list.push_back(MatchListEntry(match_type,
                                            match_type,
                                            distance,
                                            c,
                                            index));
    }

    // DONE
    return match_list;
}
