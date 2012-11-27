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
            const Process & process = interactions_.processes()[j];
            match(process, index);
        }
    }
}


// -----------------------------------------------------------------------------
//
void LatticeModel::match(const Process & process,
                         const int index)
{
    // NEEDS IMPLEMENTATION

    // Check in the indices list of this process to see if we have
    // a previous match.

    // Go through the list of indices.

    // Get the neighbrourhood out.

    // Match the neighbrourhood with the process.

    // If match and previous match - do nothing.

    // If no match and not previous match - do nothing.

    // If match and not previous match - add to the list.

    // If no match and previous match - remove from the list.

}


// -----------------------------------------------------------------------------
//
void LatticeModel::singleStep()
{
    printf("Taking a step.\n");
    // NOTHING HERE YET
}

