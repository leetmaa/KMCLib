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

    /*! \brief Function for calculating the initial matching.
     */
    void calculateInitialMatching();

    /*! \brief Function for calculating / updating the matching for a given set
     *        of indices.
     *  \param indices: The indices to calculate for.
     */
    void calculateMatching(const std::vector<int> & indices);

    /*! \brief Match a process against an index, and update the process' table
     *        of available sites when necessary.
     *  \param process : The process to consider.
     *  \param index   : The index to check.
     */
    void match(Process & process,
               const int index) const;

    /*! \brief Helper function to determine if a given neighbourhood
     *         matches a certain process.
     *  \param process       : The process to check.
     *  \param neighbourhood : The neighbourhood to check.
     *  \return : True if match.
     */
    bool isMatch(const Process & process,
                 std::vector<int> & neighbourhood) const;

    /// A reference to the configuration given at construction.
    Configuration & configuration_;

    /// A description of the lattice.
    LatticeMap lattice_map_;

    /// The description of all interactions in the system.
    Interactions interactions_;

};


#endif // __LATTICEMODEL__

