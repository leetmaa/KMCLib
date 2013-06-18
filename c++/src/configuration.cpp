/*
  Copyright (c)  2012-2013  Mikael Leetmaa

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
    n_moved_(0),
    elements_(elements),
    atom_id_elements_(elements),
    match_lists_(elements_.size())
{
    // Setup the coordinates and initial atom ids.
    for (size_t i = 0; i < coordinates.size(); ++i)
    {
        coordinates_.push_back( Coordinate(coordinates[i][0],
                                           coordinates[i][1],
                                           coordinates[i][2]));
        atom_id_.push_back(i);
    }

    // Set the atom id coordinates to the same as the coordinates to start with.
    atom_id_coordinates_ = coordinates_;

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

    // Set the size of the type names list.
    type_names_ = std::vector<std::string>(max_type+1);

    // Fill the list.
    it1 = possible_types.begin();
    for ( ; it1 != possible_types.end(); ++it1 )
     {
         type_names_[it1->second] = it1->first;
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
void Configuration::initMatchLists( const LatticeMap & lattice_map,
                                    const int range )
{
    // Loop over all indices.
    for (size_t i = 0; i < types_.size(); ++i)
    {
        // Calculate and store the match list.
        const int origin_index = i;
        const std::vector<int> neighbourhood = lattice_map.neighbourIndices(origin_index, range);
        match_lists_[i] = minimalMatchList(origin_index,
                                           neighbourhood,
                                           lattice_map);
    }

    // Now that we know the size of the match lists we can allocate
    // memory for the moved_atom_ids_ vector.
    const size_t size = match_lists_[0].size();
    moved_atom_ids_.resize(size);
}


// -----------------------------------------------------------------------------
//
void Configuration::updateMatchList(const int index)
{
    // Update the match list's types information.
    std::vector<MinimalMatchListEntry>::iterator it1   = match_lists_[index].begin();
    const std::vector<MinimalMatchListEntry>::const_iterator end = match_lists_[index].end();

    for ( ; it1 != end; ++it1 )
    {
        (*it1).match_type = types_[(*it1).index];
    }
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
        // The general case fore wrapping all directions.
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
    }

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
    // PERFORMME
    // Need to time and optimize the new parts of the routine.

    // Get the proper match lists.
    const std::vector<MinimalMatchListEntry> & process_match_list = process.minimalMatchList();
    const std::vector<MinimalMatchListEntry> & site_match_list    = minimalMatchList(site_index);

    // Iterators to the match list entries.
    std::vector<MinimalMatchListEntry>::const_iterator it1 = process_match_list.begin();
    std::vector<MinimalMatchListEntry>::const_iterator it2 = site_match_list.begin();

    // Iterators to the info storages.
    std::vector<int>::iterator it3 = process.affectedIndices().begin();
    std::vector<int>::iterator it4 = moved_atom_ids_.begin();

    // Reset the moved counter.
    n_moved_ = 0;

    // Loop over the match lists and get the types and indices out.
    for( ; it1 != process_match_list.end(); ++it1, ++it2)
    {
        // Get the type out of the process match list.
        const int update_type = (*it1).update_type;

        // Get the index out of the configuration match list.
        const int index = (*it2).index;

        // NOTE: The > 0 is needed for handling the wildcard match.
        if (types_[index] != update_type && update_type > 0)
        {
            // Get the atom id to apply the move vector to.
            const int atom_id = atom_id_[index];

            // Apply the move vector to the atom coordinate.
            atom_id_coordinates_[atom_id] += (*it1).move_coordinate;

            // Set the type at this index.
            types_[index]    = update_type;
            elements_[index] = type_names_[update_type];

            // Update the atom id element.
            if (!(*it1).has_move_coordinate)
            {
                atom_id_elements_[atom_id] = elements_[index];
            }

            // Mark this index as affected.
            (*it3) = index;
            ++it3;

            // Mark this atom_id as moved.
            (*it4) = atom_id;
            ++it4;
            ++n_moved_;
        }
    }

    // Perform the moves on all involved atom-ids.
    const std::vector< std::pair<int,int> > & process_id_moves = process.idMoves();

    // Local vector to store the atom id updates in.
    std::vector<std::pair<int,int> > id_updates(process_id_moves.size());

    // Setup the id updates list.
    for (size_t i = 0; i < process_id_moves.size(); ++i)
    {
        const int match_list_index_from = process_id_moves[i].first;
        const int match_list_index_to   = process_id_moves[i].second;

        const int lattice_index_from = site_match_list[match_list_index_from].index;
        const int lattice_index_to   = site_match_list[match_list_index_to].index;

        id_updates[i].first  = atom_id_[lattice_index_from];
        id_updates[i].second = lattice_index_to;
    }

    // Apply the id updates on the configuration.
    for (size_t i = 0; i < id_updates.size(); ++i)
    {
        const int id    = id_updates[i].first;
        const int index = id_updates[i].second;

        // Set the atom id at this lattice site index.
        atom_id_[index] = id;
        atom_id_elements_[id] = elements_[index];

    }
}

