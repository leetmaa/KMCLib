/*
  Copyright (c)  2012-2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  configuration.cpp
 *  \brief File for the implementation code of the Configuration class.
 */


#include <cstdio>
#include <algorithm>
#include <cstdlib>

#include "configuration.h"
#include "latticemap.h"
#include "process.h"
#include "matchlist.h"

// Temporary data for the match list return.
static ConfigBucketMatchList tmp_match_list__(0);


// -----------------------------------------------------------------------------
//
Configuration::Configuration(const std::vector<std::vector<double> >  & coordinates,
                             const std::vector<std::vector<std::string> > & elements,
                             const std::map<std::string,int> & possible_types) :
    n_moved_(0),
    elements_(elements),
    atom_id_elements_(elements_.size()),
    match_lists_(elements_.size()),
    possible_types_(possible_types),
    latest_event_process_(0),
    latest_event_site_(0)
{
    // ML: FIXME: We assume here that if atom id's are to be used, only one
    //            atom per site is present. If more than one atom per site are
    //            present, only the first atom will be detected and labeled with correct ID.
    //            This must be handeled in a generic way in the release version.


    // Setup the coordinates and initial atom ids.
    for (size_t i = 0; i < coordinates.size(); ++i)
    {
        coordinates_.push_back( Coordinate(coordinates[i][0],
                                           coordinates[i][1],
                                           coordinates[i][2]));
        // FIXME
        atom_id_.push_back(i);

        // FIXME
        atom_id_elements_[i] = elements_[i][0];
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
        TypeBucket tb(type_names_.size());
        for (size_t j = 0; j < elements_[i].size(); ++j)
        {
            // Get the element out at this point.
            const std::string element = elements_[i][j];

            // Get the type a numeric value.
            const int type = possible_types.find(element)->second;

            // Increase this type counter.
            tb[type] += 1;
        }

        // Add to the types vector.
        types_.push_back(tb);
    }
}


// -----------------------------------------------------------------------------
//
void Configuration::initMatchLists( const LatticeMap & lattice_map,
                                    const int range )
{
    // Store the max size of minimal_match_list_
    size_t max_size = 0;
    size_t tmp_size;

    // Loop over all lattice sites.
    for (size_t i = 0; i < types_.size(); ++i)
    {
        // Calculate and store the match list.
        const int origin_index = i;
        const std::vector<int> neighbourhood = lattice_map.neighbourIndices(origin_index, range);
        match_lists_[i] = configMatchList(origin_index,
                                          neighbourhood,
                                          lattice_map);

        // Store the maximum size
        tmp_size = match_lists_[i].size();
        if ( tmp_size > max_size )
        {
            max_size = tmp_size;
        }
    }

    // Now that we know the size of the match lists we can allocate
    // memory for the moved_atom_ids_ vector.
    moved_atom_ids_.resize(max_size);
    recent_move_vectors_.resize(max_size);
}


// -----------------------------------------------------------------------------
//
void Configuration::updateMatchList(const int index)
{
    // Update the config match lists.
    ConfigBucketMatchList::iterator it1 = match_lists_[index].begin();
    const ConfigBucketMatchList::const_iterator end = match_lists_[index].end();
    for ( ; it1 != end; ++it1 )
    {
        (*it1).match_types = types_[(*it1).index];
    }
}


// -----------------------------------------------------------------------------
//
const ConfigBucketMatchList & Configuration::configMatchList(const int origin_index,
                                                             const std::vector<int> & indices,
                                                             const LatticeMap & lattice_map) const
{
    // Setup the return data.
    tmp_match_list__.resize(indices.size());

    // Extract the coordinate of the first index.
    const Coordinate center = coordinates_[origin_index];

    // Setup the needed iterators.
    std::vector<int>::const_iterator it_index  = indices.begin();
    const std::vector<int>::const_iterator end = indices.end();
    ConfigBucketMatchList::iterator it_match_list = tmp_match_list__.begin();

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
            (*it_match_list).match_types = types_[(*it_index)];

            // Save in the match list.
            (*it_match_list).distance = distance;
            (*it_match_list).index = (*it_index);
            (*it_match_list).x = c.x();
            (*it_match_list).y = c.y();
            (*it_match_list).z = c.z();
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
            (*it_match_list).match_types = types_[(*it_index)];

            // Save in the match list.
            (*it_match_list).distance = distance;
            (*it_match_list).index = (*it_index);
            (*it_match_list).x = c.x();
            (*it_match_list).y = c.y();
            (*it_match_list).z = c.z();
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
            (*it_match_list).match_types = types_[(*it_index)];

            // Save in the match list.
            (*it_match_list).distance = distance;
            (*it_match_list).index = (*it_index);
            (*it_match_list).x = c.x();
            (*it_match_list).y = c.y();
            (*it_match_list).z = c.z();
        }
    }

    // Sort and return.
    std::sort(tmp_match_list__.begin(), tmp_match_list__.end());
    return tmp_match_list__;
}


// -----------------------------------------------------------------------------
//
void Configuration::performBucketProcess(Process & process,
                                         const int site_index,
                                         const LatticeMap & lattice_map)
{
    // Store the process number for access from python.
    latest_event_process_ = process.processNumber();
    latest_event_site_    = site_index;

    // Get the proper match lists.
    const ProcessBucketMatchList & process_match_list = process.processMatchList();
    const ConfigBucketMatchList & site_match_list     = configMatchList(site_index);

    // Iterators to the match list entries.
    ProcessBucketMatchList::const_iterator it1 = process_match_list.begin();
    ConfigBucketMatchList::const_iterator it2 = site_match_list.begin();

    // Iterators to the info storages.
    std::vector<int>::iterator it3 = process.affectedIndices().begin();
    std::vector<int>::iterator it4 = moved_atom_ids_.begin();
    std::vector<Coordinate>::iterator it5 = recent_move_vectors_.begin();

    // Reset the moved counter.
    n_moved_ = 0;

    // Loop over the match lists and get the types and indices out.
    for( ; it1 != process_match_list.end(); ++it1, ++it2)
    {
        // Find out the update type.
        const TypeBucket & update_types = (*it1).update_types;

        // Get the index out of the configuration match list.
        const int index = (*it2).index;

        // ML: Prototyping.
        int sum = 0;
        for (int i = 0; i  < update_types.size(); ++i)
        {
            // NOTE: Cast needed for clang.
            sum += std::abs(update_types[i]);
        }

        // NOTE: The !(update_types[0] > 0) is needed for handling the wildcard match.
        if (sum > 0 && !(update_types[0] > 0))
        {
            // Get the atom id to apply the move vector to.
            const int atom_id = atom_id_[index];

            // Apply the move vector to the atom coordinate.
            atom_id_coordinates_[atom_id] += (*it1).move_coordinate;

            // Set the type at this index.
            for (int i = 0; i < types_[index].size(); ++i)
            {
                types_[index][i] += update_types[i];
            }

            // Set the elements at this index.

            // ML: FIXME: This is not a good solution.
            //            We should store the update types as
            //            a vector of strings in the match list entry,
            //            instead of regenerating that list every time here.
            std::vector<std::string> elements_at_index;
            for (int i = 0; i < types_[index].size(); ++i)
            {
                for (int j = 0; j < types_[index][i]; ++j)
                {
                    elements_at_index.push_back(type_names_[i]);
                }
            }
            elements_[index] = elements_at_index;

            // Update the atom id element.
            if (!(*it1).has_move_coordinate)
            {
                // ML: FIXME: This behavior should be deprecated.
                //            Now we only take the first occuring type at the site.
                //            This is expected behavior but incorrect in general and
                //            works only for one atom per site simulations.
                atom_id_elements_[atom_id] = elements_[index][0];
            }

            // Mark this index as affected.
            (*it3) = index;
            ++it3;

            // Mark this atom_id as moved.
            (*it4) = atom_id;
            ++it4;
            ++n_moved_;

            // Save this move vector.
            (*it5) = (*it1).move_coordinate;
            ++it5;
        }
    }

    // Perform the moves on all involved atom-IDs.
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


        // ML: FIXME: This behavior should be deprecated.
        // See above comment.
        // Update the element type of this atom ID.
        atom_id_elements_[id] = elements_[index][0];

    }
}


// -----------------------------------------------------------------------------
//
std::vector<int> Configuration::particlesPerType() const
{
    // Allocate space for the return values.
    std::vector<int> particles_per_type(possible_types_.size(), 0);

    // Loop over each site and sum the number of particle.
    for (size_t i = 0; i < types_.size(); ++i)
    {
        for (size_t j = 0; j < particles_per_type.size(); ++j)
        {
            particles_per_type[j] += types_[i][j];
        }
    }

    // DONE.
    return particles_per_type;
}
