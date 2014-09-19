/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  latticemodel.h
 *  \brief File for the LatticeModel class definition.
 */



#ifndef __LATTICEMODEL__
#define __LATTICEMODEL__


#include "latticemap.h"
#include "interactions.h"
#include "matcher.h"

// Forward declarations.
class Configuration;
class SimulationTimer;
class Process;

/// Class for defining and running a lattice KMC model.
class LatticeModel {

public:

    /*! \brief Constructor for setting up the model.
     *  \param configuration    : The configuration to run the simulation on.
     *  \param simulation_timer : The timer for the simulation.
     *  \param lattice_map      : A lattice map object describing the lattice.
     *  \param interactions     : An interactions object describing all interactions
     *                         and possible processes in the system.
     */
    LatticeModel(Configuration & configuration,
                 SimulationTimer & simulation_timer,
                 const LatticeMap & lattice_map,
                 const Interactions & interactions);

    /*! \brief Function for taking one time step in the KMC lattice model.
     */
    void singleStep();

    /*! \brief Function for updating the time for the single step.
     */
    void propagateTime();

    /*! \brief Query for the interactions.
     *  \return : A handle to the interactions stored on the class.
     */
    const Interactions & interactions() const { return interactions_; }

    /*! \brief Query for the configuration.
     *  \return : A handle to the configuration stored on the class.
     */
    const Configuration & configuration() const { return configuration_; }

    /*! \brief Query for the lattice map.
     *  \return : A handle to the lattice map stored on the class.
     */
    const LatticeMap & latticeMap() const { return lattice_map_; }

protected:

private:

    /*! \brief Private helper function to initiate matching of all
     *         processes with all indices in the configuration.
     */
    void calculateInitialMatching();

    /// A reference to the configuration given at construction.
    Configuration & configuration_;

    /// A reference to the timer given at construction.
    SimulationTimer & simulation_timer_;

    /// A description of the lattice.
    LatticeMap lattice_map_;

    /// The description of all interactions in the system.
    Interactions interactions_;

    /// The Matcher to use for calculating matches and update the process lists.
    Matcher matcher_;
};


#endif // __LATTICEMODEL__

