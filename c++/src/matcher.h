/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  matcher.h
 *  \brief File for the Matcher class definition.
 */

#ifndef __MATCHER__
#define __MATCHER__

#include <vector>

#include "matchlistentry.h"

// Forward declarations.
class Interactions;
class LatticeMap;
class Configuration;
class Process;

/*! \brief Class for matching local geometries.
 */
class Matcher {

public:

    /*! \brief Default constructor.
     */
    Matcher();

    /*! \brief Calculate/update the matching of provided indices with
     *         all possible processes.
     *  \param interactions  : The interactions object holding info on possible processes.
     *  \param indices       : The configuration indices for which the neighbourhood should
     *                         be matched against all possible processes.
     *  \param lattice_map   : The lattice map describing the lattice.
     *  \param configuration : The configuration which the list of indices refers to.
     */
    void calculateMatching(Interactions & interactions,
                           const std::vector<int> & indices,
                           const LatticeMap & lattice_map,
                           const Configuration & configuration) const;

    /*! \brief Calculate/update the matching of a provided index and process.
     *  \param process       : The process to check against and update if needed.
     *  \param index         : The configuration index for which the neighbourhood should
     *                         be matched against the process.
     *  \param lattice_map   : The lattice map describing the lattice.
     *  \param configuration : The configuration which the index refers to.
     */
    void calculateMatching(Process & process,
                           const int index,
                           const LatticeMap & lattice_map,
                           const Configuration & configuration) const;

    /*! \brief Check the matching between a local neighbourhood index list and a process.
     *  \param index         : The configuration index for which the
                               neighbourhood should be matched.
     *  \param process       : The process to check against.
     *  \param lattice_map   : The lattice map describing the lattice.
     *  \param configuration : The configuration which the neighbrourhood indices refers to.
     *  \return : True if match.
     */
    bool isMatch(const int index,
                 const std::vector<int> & neighbourhood,
                 const Process & process,
                 const LatticeMap & lattice_map,
                 const Configuration & configuration) const;

    /*! \brief Check the matching between two match lists, where the process match list
     *         must be shorter or equal in length to the index match list.
     *  \param process_match_list : The process match list to compare against.
     *  \param index_match_list   : The index match list to compare.
     *  \return : True if match.
     */
    bool isMatch(const std::vector<MinimalMatchListEntry> & process_match_list,
                 const std::vector<MinimalMatchListEntry> & index_match_list) const;


protected:

private:

};


#endif // __MATCHER__

