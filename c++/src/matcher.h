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

#include "matchlist.h"
#include "ratetable.h"

// Forward declarations.
class Interactions;
class Configuration;
class Process;
class LatticeMap;
class RateCalculator;

/// A minimal struct for representing a task with a rate.
struct RateTask
{
    int index;
    int process;
    double rate;
    double multiplicity;
};


/// A minimal struct for representing a remove task.
struct RemoveTask
{
    int index;
    int process;
};


/*! \brief Class for matching local geometries.
 */
class Matcher {

public:

    /*! \brief Constructor for the mather.
     *  \param sites : The number of sites in the system.
     *  \param processes : The number of processes in the system.
     */
    Matcher(const size_t & sites, const size_t & processes);

    /*! \brief Calculate/update the matching of provided indices with
     *         all possible processes.
     *  \param interactions  : The interactions object holding info on possible processes.
     *  \param configuration : The configuration which the list of indices refers to.
     *  \param lattice_map   : The lattice map describing the configuration.
     *  \param indices       : The configuration indices for which the neighbourhood should
     *                         be matched against all possible processes.
     */
    void calculateMatching(Interactions & interactions,
                           Configuration & configuration,
                           const LatticeMap & lattice_map,
                           const std::vector<int> & indices);

    /*! \brief Calculate the matching for a list of match tasks (pairs of indices and processes).
     *  \param index_process_to_match : The list of indices and process numbers to match.
     *  \param interactions           : The interactions to get the processes from.
     *  \param configuration          : The configuration which the index refers to.
     *  \param remove_tasks (out)     : A vector that will be filled with tasks for removal after match.
     *  \param update_tasks (out)     : A vector that will be filled with tasks for update after match.
     *  \param add_tasks    (out)     : A vector that will be filled with tasks for adding after match.
     */
    void matchIndicesWithProcesses(const std::vector<std::pair<int,int> > & index_process_to_match,
                                   const Interactions  & interactions,
                                   const Configuration & configuration,
                                   std::vector<RemoveTask> & remove_tasks,
                                   std::vector<RateTask>   & update_tasks,
                                   std::vector<RateTask>   & add_tasks) const;

    /*! \brief Update the rates of the rate tasks by calling the
     *         backend call-back function of the RateCalculator stored
     *         on the interactions object.
     *  \param new_rates(out): The vector to place the updated rates in.
     *  \param tasks         : A vector with tasks to update.
     *  \param interactions  : The interactions to get the rate calculator from.
     *  \param configuration : The configuration to use.
     */
    void updateRates(std::vector<double>         & new_rates,
                     const std::vector<RateTask> & tasks,
                     const Interactions          & interactions,
                     const Configuration         & configuration);

    /*! \brief Update the processes with the given tasks.
     *  \param remove_tasks  : A vector with remove tasks for updating the processes.
     *  \param update_tasks  : A vector with update tasks for updating the processes.
     *  \param add_tasks     : A vector with add tasks for updating the processes.
     *  \param interactions  : The interactions to get the processes from.
     */
    void updateProcesses(const std::vector<RemoveTask> & to_remove,
                         const std::vector<RateTask>   & to_update,
                         const std::vector<RateTask>   & to_add,
                         Interactions & interactions);

    /*! \brief Calculate the rate for a single process using the rate calculator.
     *  \param index           : The index to perform the process at.
     *  \param process         : The process to perform.
     *  \param configuration   : The configuration the index is referring to.
     *  \param rate_calculator : The rate calculator to use.
     *  \returns : The calculated rate for the process at the given index.
     */
    double updateSingleRate(const int index,
                            const Process        & process,
                            const Configuration  & configuration,
                            const RateCalculator & rate_calculator) const;

    /*! \brief Calculate/update the matching of a provided index and process.
     *  \param process       : The process to check against and update if needed.
     *  \param configuration : The configuration which the index refers to.
     *  \param index         : The configuration index for which the neighbourhood should
     *                         be matched against the process.
     */
    void calculateMatching(Process & process,
                           Configuration & configuration,
                           const int index) const;

    /*! \brief Check the matching between two match lists, where the process match list
     *         must be shorter or equal in length to the index match list.
     *  \param process_match_list : The process match list to compare against.
     *  \param index_match_list   : The index match list to compare.
     *  \return : True if match.
     */
    bool isMatch(const ProcessBucketMatchList & process_match_list,
                 const ConfigBucketMatchList & index_match_list) const;

    /*! \brief Print the match lists to std out. Useful for debugging.
     *  \param process_match_list : The process match list to compare against.
     *  \param index_match_list   : The index match list to compare.
     */
    void printMatchLists(const ProcessBucketMatchList & process_match_list,
                         const ConfigBucketMatchList & index_match_list) const;

    void printMatchLists(const ConfigBucketMatchList & process_match_list,
                         const ConfigBucketMatchList & index_match_list) const;

    void printMatchList(const ConfigBucketMatchList & process_match_list) const;



protected:

private:

    /// The rate table for storing calculated custom rates.
    RateTable rate_table_;

    /// The inverse matching information table.
    std::vector<std::vector<bool> > inverse_table_;

};


#endif // __MATCHER__

