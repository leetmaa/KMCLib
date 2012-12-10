/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  matcher.cpp
 *  \brief File for the implementation code of the Matcher class.
 */

#include <cstdio>

#include "matcher.h"
#include "process.h"
#include "interactions.h"
#include "latticemap.h"
#include "configuration.h"

// -----------------------------------------------------------------------------
//
Matcher::Matcher()
{
    // NOTHING HERE YET
}


// -----------------------------------------------------------------------------
//
void Matcher::calculateMatching(Interactions & interactions,
                                const std::vector<int> & indices,
                                const LatticeMap & lattice_map,
                                const Configuration & configuration) const
{
    // Loop through all provided indices.
    for(size_t i = 0; i < indices.size(); ++i)
    {
        // This is the index in the configuration which should
        // be compared (together with its neighbourhood) with
        // all possible processes stored in the interactions object.
        const int index = indices[i];

        // Match against all processes.
        for (int j = 0; j < interactions.processes().size(); ++j)
        {
            Process & process = interactions.processes()[j];
            calculateMatching(process, index, lattice_map, configuration);
        }
    }
}


// -----------------------------------------------------------------------------
//
void Matcher::calculateMatching(Process & process,
                                const int index,
                                const LatticeMap & lattice_map,
                                const Configuration & configuration) const
{
    // Check in the indices list of this process to see if we have
    // a previous match.
    const bool in_list = process.isListed(index);

    // Get the neighbrourhood out.
    const std::vector<int> neighbourhood = lattice_map.neighbourIndices(index);

    // Match the neighbrourhood with the process.
    const bool is_match = isMatch(index, neighbourhood, process, lattice_map, configuration);

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
bool Matcher::isMatch(const int index,
                      const std::vector<int> & neighbourhood,
                      const Process & process,
                      const LatticeMap & lattice_map,
                      const Configuration & configuration) const
{
    // Return false if not enough sites in the neighbourhood.
    if (neighbourhood.size() < process.sites().size())
    {
        return false;
    }

    // Get the types and distances match lists.
    const std::vector<MatchListEntry> & process_match_list = process.matchList();
    const std::vector<MatchListEntry> index_match_list = configuration.matchList(index, neighbourhood, lattice_map);

    // Check the match lists and return.
    return isMatch(process_match_list,
                   index_match_list);
}


// -----------------------------------------------------------------------------
//
bool Matcher::isMatch(const std::vector<MatchListEntry> & process_match_list,
                      const std::vector<MatchListEntry> & index_match_list) const
{
    // Iterators to the match list entries.
    std::vector<MatchListEntry>::const_iterator it1 = process_match_list.begin();
    std::vector<MatchListEntry>::const_iterator it2 = index_match_list.begin();

    it1 = process_match_list.begin();
    it2 = index_match_list.begin();
    // Loop over the process match list and compare.
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
