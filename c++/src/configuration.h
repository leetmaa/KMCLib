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
#include <map>
#include "matchlistentry.h"
#include "coordinate.h"

// Forward declarations.
class LatticeMap;
class Process;

/*! \brief Class for defining the configuration used in a KMC simulation to
 *         use for communicating elements and positions to and from python.
 */
class Configuration {

public:

    /*! \brief Constructor for setting up the configuration.
     *  \param coordinates   : The coordinates of the configuration.
     *  \param elements      : The elements of the configuration.
     *  \param possible_types: A global mapping from type string to number.
     */
    Configuration(std::vector< std::vector<double> > const & coordinates,
                  std::vector<std::string> const & elements,
                  const std::map<std::string,int> & possible_types);

    /*! \brief Initiate the calculation of the match lists.
     *  \param lattice_map : The lattice map needed to get coordinates wrapped.
     */
    void initMatchLists(const LatticeMap & lattice_map);

    /*! \brief Const query for the coordinates.
     *  \return : The coordinates of the configuration.
     */
    const std::vector<Coordinate> & coordinates() const { return coordinates_; }

    /*! \brief Const query for the elements.
     *  \return : The elements of the configuration.
     */
    const std::vector<std::string> & elements() const { return elements_; }

    /*! \brief Const query for the types.
     *  \return : The types of the configuration.
     */
    const std::vector<int> & types() const { return types_; }

    /*! \brief Construct and return the match list for the given list of
     *         indices.
     *  \param origin_index : The index to treat as the origin.
     *  \param indices      : The indices to get the match list for.
     *  \param lattice_map  : The lattice map needed for calculating distances
     *                        using correct boundaries.
     *  \return : The match list.
     */
    const std::vector<MinimalMatchListEntry> & minimalMatchList(const int origin_index,
                                                                const std::vector<int> & indices,
                                                                const LatticeMap & lattice_map) const;

    /*! \brief Update and return the cached match list for the given central index.
     *  \param index : The index to get the match list for.
     *  \return : The match list.
     */
    const std::vector<MinimalMatchListEntry> & minimalMatchList(const int index);

    /*! \brief Perform the given process.
     *  \param process : The process to perform, which will be updated with the affected
     *                   indices.
     *  \param site_index : The index of the site where the process should be performed.
     */
    void performProcess(Process & process,
                        const int site_index);

protected:

private:

    /// The coordinates.
    std::vector<Coordinate> coordinates_;

    /// The elements.
    std::vector<std::string> elements_;

    /// The elements.
    std::vector<int> types_;

    /// The mapping from type integers to names.
    std::vector<std::string> type_names_;

    /// The match lists for all indices.
    std::vector< std::vector<MinimalMatchListEntry> > match_lists_;
};


#endif // __CONFIGURATION__

