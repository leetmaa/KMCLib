/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  configuration.cpp
 *  \brief File for the implementation code of the Configuration class.
 */


#include <cstdio>
#include <algorithm>

#include "configuration.h"
#include "latticemap.h"
#include "process.h"

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

    // Loop through the possible types map and find out what the maximum is.
    std::map<std::string,int>::const_iterator it1 = possible_types.begin();
    int max_type = 0;
    for ( ; it1 != possible_types.end(); ++it1)
    {
        if (it1->second > max_type)
        {
            max_type = it1->second;
        }
    }
    type_names_ = std::vector<std::string>(max_type+1,"");

    // Setup the types from the elements strings.
    for (size_t i = 0; i < elements_.size(); ++i)
    {
        const std::string element = elements_[i];
        const int type = possible_types.find(element)->second;
        types_.push_back(type);
        // Setup the reverse mapping.
        type_names_[type] = element;
    }
}


// -----------------------------------------------------------------------------
//
std::vector<MatchListEntry> Configuration::matchList(const int origin_index,
                                                     const std::vector<int> & indices,
                                                     const LatticeMap & lattice_map) const
{
    // Setup the return data.
    std::vector<MatchListEntry> match_list;

    // Extract the coordinate of the first index.
    const Coordinate center = coordinates_[origin_index];
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

    // Sort and return.
    std::sort(match_list.begin(), match_list.end());
    return match_list;
}


// -----------------------------------------------------------------------------
//
void Configuration::performProcess(Process & process,
                                   const int site_index,
                                   const LatticeMap & lattice_map)
{
    // Get the neighbrourhood out.
    const std::vector<int> neighbourhood = lattice_map.neighbourIndices(site_index);

    // Create the proper match list.
    const std::vector<MatchListEntry> & process_match_list = process.matchList();
    const std::vector<MatchListEntry> site_match_list = matchList(site_index, neighbourhood, lattice_map);

    // Iterators to the match list entries.
    std::vector<MatchListEntry>::const_iterator it1 = process_match_list.begin();
    std::vector<MatchListEntry>::const_iterator it2 = site_match_list.begin();
    std::vector<int>::iterator it3 = process.affectedIndices().begin();

    // Loop over the match lists and get the types and indices out.
    for( ; it1 != process_match_list.end(); ++it1, ++it2)
    {
        // Get the type out of the process match list.
        const int update_type = (*it1).updateType();

        // Get the index out of the configuration match list.
        const int index = (*it2).index();

        if (types_[index] != update_type)
        {
            // Set the type at this index.
            types_[index]    = update_type;
            elements_[index] = type_names_[update_type];
            // Mark this index as affected.
            (*it3) = index;
            ++it3;
        }
    }
}
