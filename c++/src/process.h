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
#include "matchlistentry.h"

/*! \brief Class for defining a possible process int the system.
 */
class Process {

public:

    /*! \brief Constructor.
     */
    Process();

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

    const std::vector<MatchListEntry> & matchList() const { return match_list_; }

protected:

private:

    /// The available sites for this process.
    std::vector<int> sites_;

    /// The match list for comparing against local configurations.
    std::vector<MatchListEntry> match_list_;

};


#endif // __PROCESS__

