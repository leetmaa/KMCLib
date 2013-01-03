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

    /*! \brief Const query for the processes.
     *  \return : A handle to the processes of the system.
     */
    const std::vector<Process> & processes() const { return processes_; }

    /*! \brief Const query for the probability table.
     *  \return : A handle to the present probability table.
     */
    const std::vector<std::pair<double,int> > & probabilityTable() const { return probability_table_; }

    /*! \brief Recalculate the table of process probabilities based on the
     *         number of available sites for each process and their barriers.
     */
    void updateProbabilityTable();

    /*! \brief Query for the total rate of the system.
     *  \return : The total rate.
     */
    double totalRate() const { return probability_table_.back().first; }

    /*! \brief Pick an availabe process according to its probability.
     *  \return : The index of a possible available process picked according
     *            to its probability.
     */
    int pickProcessIndex() const;

    /*! \brief Pick an availabe process according to its probability and return
     *         a reference to that process.
     *  \return : A reference to a possible available process picked according
     *            to its probability.
     */
    Process & pickProcess();

protected:

private:

    /// The processes.
    std::vector<Process> processes_;

    /// The probability table.
    std::vector<std::pair<double,int> > probability_table_;
};


#endif // __INTERACTIONS__

