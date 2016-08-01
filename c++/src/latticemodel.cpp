/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  latticemodel.cpp
 *  \brief File for the implementation code of the LatticeModel class.
 */


#include "latticemodel.h"
#include "configuration.h"
#include "simulationtimer.h"
#include "random.h"

#include <cstdio>

// -----------------------------------------------------------------------------
//
LatticeModel::LatticeModel(Configuration & configuration,
                           SimulationTimer & simulation_timer,
                           const LatticeMap & lattice_map,
                           const Interactions & interactions) :
    configuration_(configuration),
    simulation_timer_(simulation_timer),
    lattice_map_(lattice_map),
    interactions_(interactions),
    matcher_(configuration.coordinates().size(), interactions.processes().size())
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
    // Calculate the match lists.
    configuration_.initMatchLists(lattice_map_, interactions_.maxRange());

    // Update the interactions matchlists.
    interactions_.clearMatching();
    interactions_.updateProcessMatchLists(configuration_, lattice_map_);

   // Match all centeres.
    std::vector<int> indices;

    for(size_t i = 0; i < configuration_.elements().size(); ++i)
    {
        indices.push_back(i);
    }

    // Match.
    matcher_.calculateMatching(interactions_,
                               configuration_,
                               lattice_map_,
                               indices);
}

// -----------------------------------------------------------------------------
//
void LatticeModel::propagateTime()
{
    // Propagate the time.
    simulation_timer_.propagateTime(interactions_.totalRate());
}

// -----------------------------------------------------------------------------
//
void LatticeModel::singleStep()
{
    // Select a process.
    Process & process = (*interactions_.pickProcess());

    // Select a site.
    const int site_index = process.pickSite();

    // Perform the operation.
    configuration_.performBucketProcess(process, site_index, lattice_map_);

    // Run the re-matching of the affected sites and their neighbours.
    const std::vector<int> & indices = \
        lattice_map_.supersetNeighbourIndices(process.affectedIndices(), interactions_.maxRange());

    matcher_.calculateMatching(interactions_,
                               configuration_,
                               lattice_map_,
                               indices);

    // Update the interactions' probability table.
    interactions_.updateProbabilityTable();
}

