/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  process.h
 *  \brief File for the Process class definition.
 */

#ifndef __PROCESS__
#define __PROCESS__

#include <vector>
#include <map>
#include <string>
#include "matchlistentry.h"

class Configuration;

/*! \brief Class for defining a possible process int the system.
 */
class Process {

public:

    /*! \brief Constructor for the process.
     *  \param first         : The first configuration, to match against the local
                               coordinates around an active site.
     *  \param second        : The second configuration, to update the local
                               configuration with if the process is selected.
     *  \param barrier       : The energy barrier in eV for performing the process.
     *  \param possible_types: A global mapping from types to vector indices
                               needed to set up the match lists properly.
     */
    Process(const Configuration & first,
            const Configuration & second,
            const double barrier,
            const std::map<std::string,int> & possible_types);

    /*! \brief Query for the number of listed possible sites for this process.
     *  \return : The number of listed indices.
     */
    size_t nSites() const {return sites_.size(); }

    /*! \brief Determine if an index is listed as available site for this process.
     *  \param index : The index to check.
     *  \return : True if match.
     */
    bool isListed(const int index) const;

    /*! \brief Add the index to the list of available sites.
     *  \param index : The index to add.
     */
    void addSite(const int index);

    /*! \brief Remove the index from the list of available sites.
     *  \param index : The index to remove.
     */
    void removeSite(const int index);

    /*! \brief Query for the available sites for this process.
     *  \return : The available sites.
     */
    const std::vector<int> & sites() const { return sites_; }

    /*! \brief Query for the configuration as a vector of match list entries.
     *  \return : The stored match list.
     */
    const std::vector<MatchListEntry> & matchList() const { return match_list_; }

protected:

private:

    /// The available sites for this process.
    std::vector<int> sites_;

    /// The match list for comparing against local configurations.
    std::vector<MatchListEntry> match_list_;



};


#endif // __PROCESS__

