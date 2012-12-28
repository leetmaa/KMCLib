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
static std::vector<MatchListEntry> tmp_match_list__(0);
// ML:
static std::vector<MinimalMatchListEntry> tmp_minimal_match_list__(0);

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
const std::vector<MatchListEntry> & Configuration::matchList(const int origin_index,
                                                             const std::vector<int> & indices,
                                                             const LatticeMap & lattice_map) const
{
    // Setup the return data.
    const size_t size = indices.size();
    tmp_match_list__.resize(size);
    //std::vector<MatchListEntry> match_list(size);

    // Extract the coordinate of the first index.
    const Coordinate center = coordinates_[origin_index];
    const Coordinate origin(0.0, 0.0, 0.0);

    // For each index: center, wrap and calculate the distance.
    for (size_t i = 0; i < size; ++i)
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

        // Save in the match list.
        tmp_match_list__[i] = MatchListEntry(match_type,
                                             match_type,
                                             distance,
                                             c,
                                             index);
        /*
        match_list.push_back(MatchListEntry(match_type,
                                            match_type,
                                            distance,
                                            c,
                                            index));
        */
    }

    // Sort and return.
    std::sort(tmp_match_list__.begin(), tmp_match_list__.end());
    //std::sort(match_list.begin(), match_list.end());
    return tmp_match_list__;
    //return match_list;
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

    const int repetitions_a = lattice_map.repetitionsA();
    const int repetitions_b = lattice_map.repetitionsB();
    const int repetitions_c = lattice_map.repetitionsC();

    const double half_cell_a = 1.0 * repetitions_a / 2.0;
    const double half_cell_b = 1.0 * repetitions_b / 2.0;
    const double half_cell_c = 1.0 * repetitions_c / 2.0;

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
            //lattice_map.wrap(c);

            // Wrap explicitly.
            if (c[0] >= half_cell_a)
            {
                c[0] -= repetitions_a;
            }
            else if (c[0] < -half_cell_a)
            {
                c[0] += repetitions_a;
            }
            if (c[1] >= half_cell_b)
            {
                c[1] -= repetitions_b;
            }
            else if (c[1] < -half_cell_b)
            {
                c[1] += repetitions_b;
            }
            if (c[2] >= half_cell_c)
            {
                c[2] -= repetitions_c;
            }
            else if (c[2] < -half_cell_c)
            {
                c[2] += repetitions_c;
            }

            // Get the distance.
            const double distance = std::sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);

            // Get the type.
            const int match_type = types_[(*it_index)];

            // Save in the match list.
            (*it_match_list).match_type  = match_type;
            (*it_match_list).update_type = match_type;
            (*it_match_list).distance    = distance;
            (*it_match_list).coordinate  = c;
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
            //lattice_map.wrap(c);

            // Wrap explicitly.
            if (c[0] >= half_cell_a)
            {
                c[0] -= repetitions_a;
            }
            else if (c[0] < -half_cell_a)
            {
                c[0] += repetitions_a;
            }

            if (c[1] >= half_cell_b)
            {
                c[1] -= repetitions_b;
            }
            else if (c[1] < -half_cell_b)
            {
                c[1] += repetitions_b;
            }

            // Get the distance.
            const double distance = std::sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);

            // Get the type.
            const int match_type = types_[(*it_index)];

            // Save in the match list.
            (*it_match_list).match_type  = match_type;
            (*it_match_list).update_type = match_type;
            (*it_match_list).distance    = distance;
            (*it_match_list).coordinate  = c;
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
        //lattice_map.wrap(c);

        // Wrap explicitly.
        if (periodic_a)
        {
            if (c[0] >= half_cell_a)
            {
                c[0] -= repetitions_a;
            }
            else if (c[0] < -half_cell_a)
            {
                c[0] += repetitions_a;
            }
        }
        if (periodic_b)
        {
            if (c[1] >= half_cell_b)
            {
                c[1] -= repetitions_b;
            }
            else if (c[1] < -half_cell_b)
            {
                c[1] += repetitions_b;
            }
        }
        if (periodic_c)
        {
            if (c[2] >= half_cell_c)
            {
                c[2] -= repetitions_c;
            }
            else if (c[2] < -half_cell_c)
            {
                c[2] += repetitions_c;
            }
        }

        // Explicit distance calculation.

        //const double distance = (std::pow(c[0], 2) +
        //                         std::pow(c[1], 2) +
        //                         std::pow(c[2], 2));

        // Calculate the distance.
        // const double distance = c.distanceToOrigin();

        // Get the type.
        const int match_type = types_[(*it_index)];

        // Save in the match list.
        (*it_match_list).match_type  = match_type;
        (*it_match_list).update_type = match_type;
        (*it_match_list).distance    = std::sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
        (*it_match_list).coordinate  = c;
    }
    } // FIXME
    // Sort and return.
    std::sort(tmp_minimal_match_list__.begin(), tmp_minimal_match_list__.end());
    return tmp_minimal_match_list__;
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
