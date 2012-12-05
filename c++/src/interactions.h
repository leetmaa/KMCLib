/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  interactions.h
 *  \brief File for the Interactions class definition.
 */

#ifndef __INTERACTIONS__
#define __INTERACTIONS__


#include <vector>

#include "process.h"

/*! \brief Class for holding information about all interactions and possible
 *         processes in the system.
 */
class Interactions {

public:

    /*! \brief Construct the interactions object from a list of processes.
     *  \param processes: The list of proceeses.
     */
    Interactions(const std::vector<Process> & processes);

    /*! \brief Query for the processes.
     *  \return : The processes of the system.
     */
    std::vector<Process> & processes() { return processes_; }

protected:

private:

    /// The processes.
    std::vector<Process> processes_;

};


#endif // __INTERACTIONS__

