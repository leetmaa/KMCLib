/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  configuration.h
 *  \brief File for the Configuration class definition.
 */

#ifndef __CONFIGURATION__
#define __CONFIGURATION__


#include <vector>
#include <string>
#include "matchlistentry.h"

// Forward declarations.
class LatticeMap;


/*! \brief Class for defining the configuration used in a KMC simulation to
 *         use for communicating elements and positions to and from python.
 */
class Configuration {

public:

    /*! \brief Constructor for setting up the configuration.
     *  \param coordinates : The coordinates of the configuration.
     *  \param elements    : The elements of the configuration.
     */
    Configuration(std::vector< std::vector<double> > const & coordinates,
                  std::vector<std::string> const & elements);

    /*! \brief Const query for the coordinates.
     *  \return : The coordinates of the configuration.
     */
    const std::vector<std::vector<double> > & coordinates() const { return coordinates_; }

    /*! \brief Const query for the elements.
     *  \return : The elements of the configuration.
     */
    const std::vector<std::string> & elements() const { return elements_; }

    /*! \brief Construct and return the match list for the given list of
     *         indices.
     *  \param indices     : The indices to get the match list for.
     *  \param lattice_map : The lattice map needed for calculating distances
     *                       using correct boundaries.
     *  \return : The match list.
     */
    std::vector<MatchListEntry> matchList(const std::vector<int> & indices,
                                          const LatticeMap & lattice_map) const;

protected:

private:

    /// The coordinates.
    std::vector<std::vector<double> > coordinates_;

    /// The elements.
    std::vector<std::string> elements_;

};


#endif // __CONFIGURATION__

