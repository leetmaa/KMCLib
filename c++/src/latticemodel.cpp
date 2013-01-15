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
    interactions_(interactions)
{
    // ML: Temporarily init the random number generator here.
    seedRandom(false, 2013);

    printf(" KMCLib: Calculating initial matching.\n");
    // Setup the mapping between coordinates and processes.
    calculateInitialMatching();

    printf(" KMCLib: Update the probability table.\n");

    // Initialize the interactions table here.
    interactions_.updateProbabilityTable();

    printf(" KMCLib: Setup done.\n");
}


// -----------------------------------------------------------------------------
//
void LatticeModel::calculateInitialMatching()
{
    // Calculate the match lists.
    configuration_.initMatchLists(lattice_map_);

    // Update the interactions matchlists.
    interactions_.updateProcessMatchLists(lattice_map_);

   // Match all centeres.
    std::vector<int> indices;

    for(size_t i = 0; i < configuration_.elements().size(); ++i)
    {
        indices.push_back(i);
    }
    matcher_.calculateMatching(interactions_,
                               configuration_,
                               indices);
}


// -----------------------------------------------------------------------------
//
void LatticeModel::singleStep()
{
    // Select a process.
    Process & process = interactions_.pickProcess();

    // Select a site.
    const int site_index = process.pickSite();

    // Perform the operation.
    configuration_.performProcess(process, site_index);

    // Propagate the time.
    simulation_timer_.propagateTime(interactions_.totalRate());

    // Run the re-matching of the affected sites and their neighbours.
    const std::vector<int> & indices = \
        lattice_map_.supersetNeighbourIndices(process.affectedIndices());

    matcher_.calculateMatching(interactions_,
                               configuration_,
                               indices);

    // Update the interactions' probability table.
    interactions_.updateProbabilityTable();
}

