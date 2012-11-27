/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  latticemodel.cpp
 *  \brief File for the implementation code of the LatticeModel class.
 */


#include "latticemodel.h"
#include "configuration.h"
#include "matchlistentry.h"

#include <cstdio>

// -----------------------------------------------------------------------------
//
LatticeModel::LatticeModel(Configuration & configuration,
                           const LatticeMap & lattice_map,
                           const Interactions & interactions) :
    configuration_(configuration),
    lattice_map_(lattice_map),
    interactions_(interactions)
{
    // Setup the mapping between coordinates and processes.
    calculateInitialMatching();
}


// -----------------------------------------------------------------------------
//
void LatticeModel::calculateInitialMatching()
{
    // Match all centeres.
    std::vector<int> indices;
    for(size_t i = 0; i < configuration_.elements().size(); ++i)
    {
        indices.push_back(i);
    }
    calculateMatching(indices);
}


// -----------------------------------------------------------------------------
//
void LatticeModel::calculateMatching(const std::vector<int> & indices)
{
    // Loop through all provided indices.
    for(size_t i = 0; i < indices.size(); ++i)
    {
        const int index = indices[i];

        // Match against all processes.
        for (int j = 0; j < interactions_.processes().size(); ++j)
        {
            Process & process = interactions_.processes()[j];
            match(process, index);
        }
    }
}


// -----------------------------------------------------------------------------
//
void LatticeModel::match(Process & process,
                         const int index) const
{
    // Check in the indices list of this process to see if we have
    // a previous match.
    const bool in_list = process.isListed(index);

    // Get the neighbrourhood out.
    std::vector<int> neighbourhood = lattice_map_.neighbourIndices(index);

    // Match the neighbrourhood with the process.
    const bool is_match = isMatch(process, neighbourhood);

    // If match and not previous match - add to the list.
    if (is_match && !in_list)
    {
        process.addSite(index);
    }
    // If no match and previous match - remove from the list.
    else if (!is_match && in_list)
    {
        process.removeSite(index);
    }
}


// -----------------------------------------------------------------------------
//
bool LatticeModel::isMatch(const Process & process,
                           std::vector<int> & neighbourhood) const
{
    // Return false if not enough sites in the neighbourhood.
    if (neighbourhood.size() < process.sites().size())
    {
        return false;
    }

    // Get the types and distances match lists.
    const std::vector<MatchListEntry> & process_match_list = process.matchList();
    std::vector<MatchListEntry> index_match_list     = configuration_.matchList(neighbourhood, lattice_map_);

    std::vector<MatchListEntry>::const_iterator it1 = process_match_list.begin();
    std::vector<MatchListEntry>::const_iterator it2 = index_match_list.begin();

    for( ; it1 != process_match_list.end(); ++it1, ++it2)
    {
        // Return false on mismatch.
        if ((*it1) != (*it2))
        {
            return false;
        }
    }

    // All match, return true.
    return true;
}


// -----------------------------------------------------------------------------
//
void LatticeModel::singleStep()
{
    printf("Taking a step.\n");
    // NOTHING HERE YET
}

