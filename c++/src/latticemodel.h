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
class Process;

/// Class for defining and running a lattice KMC model.
class LatticeModel {

public:

    /*! \brief Constructor for setting up the model.
     *  \param configuration : The configuration to run the simulation on.
     *  \param lattice_map   : A lattice map object describing the lattice.
     *  \param interactions  : An interactions object describing all interactions
     *                         and possible processes in the system.
     */
    LatticeModel(Configuration & configuration,
                 const LatticeMap & lattice_map,
                 const Interactions & interactions);

    /*! \brief Function for taking one time step in the KMC lattice model.
     */
    void singleStep();

protected:

private:

    /*! \brief Private helper function to initiate matching of all
     *         processes with all indices in the configuration.
     */
    void calculateInitialMatching();

    /// A reference to the configuration given at construction.
    Configuration & configuration_;

    /// A description of the lattice.
    LatticeMap lattice_map_;

    /// The description of all interactions in the system.
    Interactions interactions_;

    /// The Matcher to use for calculating matches and update the process lists.
    Matcher matcher_;
};


#endif // __LATTICEMODEL__

