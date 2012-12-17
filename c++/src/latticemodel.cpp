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

    // Initialize the interactions table here.
    interactions_.updateProbabilityTable();
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
    matcher_.calculateMatching(interactions_,
                               indices,
                               lattice_map_,
                               configuration_);
}


// -----------------------------------------------------------------------------
//
void LatticeModel::singleStep()
{
    printf("Taking a step.\n");
    // NOTHING HERE YET
}

