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

// Temporary data for the match list return.
static std::vector<MinimalMatchListEntry> tmp_minimal_match_list__(0);

// -----------------------------------------------------------------------------
//
Configuration::Configuration(std::vector<std::vector<double> > const & coordinates,
                             std::vector<std::string> const & elements,
                             const std::map<std::string,int> & possible_types) :
    elements_(elements),
    match_lists_(elements_.size())
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
void Configuration::initMatchLists(const LatticeMap & lattice_map)
{
    // Loop over all indices.
    for (size_t i = 0; i < types_.size(); ++i)
    {
        // Calculate and store the match list.
        const int origin_index = i;
        const std::vector<int> neighbourhood = lattice_map.neighbourIndices(origin_index);
        match_lists_[i] = minimalMatchList(origin_index,
                                           neighbourhood,
                                           lattice_map);
    }
}


// -----------------------------------------------------------------------------
//
const std::vector<MinimalMatchListEntry> & Configuration::minimalMatchList(const int index)
{
    // Update the match list's types information.
    std::vector<MinimalMatchListEntry>::iterator it1   = match_lists_[index].begin();
    const std::vector<MinimalMatchListEntry>::const_iterator end = match_lists_[index].end();

    for ( ; it1 != end; ++it1 )
    {
        (*it1).match_type = types_[(*it1).index];
    }

    // Return.
    return match_lists_[index];
}


// -----------------------------------------------------------------------------
//
const std::vector<MinimalMatchListEntry> & Configuration::minimalMatchList(const int origin_index,
                                                                           const std::vector<int> & indices,
                                                                           const LatticeMap & lattice_map) const
{
    // Setup the return data.
    tmp_minimal_match_list__.resize(indices.size());

    // Extract the coordinate of the first index.
    const Coordinate center = coordinates_[origin_index];

    // Setup the needed iterators.
    std::vector<int>::const_iterator it_index  = indices.begin();
    const std::vector<int>::const_iterator end = indices.end();
    std::vector<MinimalMatchListEntry>::iterator it_match_list = tmp_minimal_match_list__.begin();

    const bool periodic_a = lattice_map.periodicA();
    const bool periodic_b = lattice_map.periodicB();
    const bool periodic_c = lattice_map.periodicC();

    // Since we know the periodicity outside the loop we can make the
    // logics outside also.

    // Periodic a-b-c
    if (periodic_a && periodic_b && periodic_c)
    {
        // Loop, calculate and add to the return list.
        for ( ; it_index != end; ++it_index, ++it_match_list)
        {
            // Center.
            Coordinate c = coordinates_[(*it_index)] - center;

            // Wrap with coorect periodicity.
            lattice_map.wrap(c, 0);
            lattice_map.wrap(c, 1);
            lattice_map.wrap(c, 2);

            // Get the distance.
            const double distance = c.distanceToOrigin();

            // Get the type.
            const int match_type = types_[(*it_index)];

            // Save in the match list.
            (*it_match_list).match_type  = match_type;
            (*it_match_list).update_type = -1;
            (*it_match_list).distance    = distance;
            (*it_match_list).coordinate  = c;
            (*it_match_list).index       = (*it_index);
        }
    }
    // Periodic a-b
    else if (periodic_a && periodic_b)
    {
        // Loop, calculate and add to the return list.
        for ( ; it_index != end; ++it_index, ++it_match_list)
        {
            // Center.
            Coordinate c = coordinates_[(*it_index)] - center;

            // Wrap with coorect periodicity.
            lattice_map.wrap(c, 0);
            lattice_map.wrap(c, 1);

            // Get the distance.
            const double distance = c.distanceToOrigin();

            // Get the type.
            const int match_type = types_[(*it_index)];

            // Save in the match list.
            (*it_match_list).match_type  = match_type;
            (*it_match_list).update_type = -1;
            (*it_match_list).distance    = distance;
            (*it_match_list).coordinate  = c;
            (*it_match_list).index       = (*it_index);
        }
    }
    else {
    // Periodic b-c

    // Periodic a-c

    // Periodic a

    // Periodic b

    // Periodic c

    // Loop, calculate and add to the return list.
    for ( ; it_index != end; ++it_index, ++it_match_list)
    {
        // Center.
        Coordinate c = coordinates_[(*it_index)] - center;

        // Wrap with coorect periodicity.
        lattice_map.wrap(c);

        const double distance = c.distanceToOrigin();

        // Get the type.
        const int match_type = types_[(*it_index)];

        // Save in the match list.
        (*it_match_list).match_type  = match_type;
        (*it_match_list).update_type = -1;
        (*it_match_list).distance    = distance;
        (*it_match_list).coordinate  = c;
        (*it_match_list).index       = (*it_index);
    }
    } // FIXME
    // Sort and return.
    std::sort(tmp_minimal_match_list__.begin(), tmp_minimal_match_list__.end());
    return tmp_minimal_match_list__;
}


// -----------------------------------------------------------------------------
//
void Configuration::performProcess(Process & process,
                                   const int site_index)
{
    // Get the proper match lists.
    const std::vector<MinimalMatchListEntry> & process_match_list = process.minimalMatchList();
    const std::vector<MinimalMatchListEntry> & site_match_list    = minimalMatchList(site_index);

    // Iterators to the match list entries.
    std::vector<MinimalMatchListEntry>::const_iterator it1 = process_match_list.begin();
    std::vector<MinimalMatchListEntry>::const_iterator it2 = site_match_list.begin();
    std::vector<int>::iterator it3 = process.affectedIndices().begin();

    // Loop over the match lists and get the types and indices out.
    for( ; it1 != process_match_list.end(); ++it1, ++it2)
    {
        // Get the type out of the process match list.
        const int update_type = (*it1).update_type;

        // Get the index out of the configuration match list.
        const int index = (*it2).index;

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
