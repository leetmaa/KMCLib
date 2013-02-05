/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  matcher.cpp
 *  \brief File for the implementation code of the Matcher class.
 */

#include <cstdio>
#include <algorithm>

#include "matcher.h"
#include "process.h"
#include "interactions.h"
#include "latticemap.h"
#include "configuration.h"
#include "latticemap.h"


// -----------------------------------------------------------------------------
//
Matcher::Matcher()
{
    // NOTHING HERE YET
}


// -----------------------------------------------------------------------------
//
void Matcher::calculateMatching(Interactions & interactions,
                                Configuration & configuration,
                                const LatticeMap & lattice_map,
                                const std::vector<int> & indices) const
{
    // PERFORMME:
    // Any changes here should be timed using realistic test systems.

    if (! interactions.useCustomRates())
    {
        // Loop through all provided indices.
        for(size_t i = 0; i < indices.size(); ++i)
        {
            // This is the index in the configuration which should
            // be compared against all possible processes stored in the
            // interactions object.
            const int index = indices[i];

            // Get the basis site.
            const int basis_site = lattice_map.basisSiteFromIndex(index);

            // Match against all processes.
            for (size_t j = 0; j < interactions.processes().size(); ++j)
            {
                Process & process = (*interactions.processes()[j]);

                // Check if the basis site is listed.
                const std::vector<int> & process_basis_sites = process.basisSites();
                if ( std::find(process_basis_sites.begin(), process_basis_sites.end(), basis_site) != process_basis_sites.end() )
                {
                    calculateMatching(process, configuration, index);
                }
            }
        }
    }
    else
    {
        // Custom rates with call to the RateCalculator for getting the
        // updated rates.

        // NEEDS IMPLEMENTATION - This is a prototype.

        // -------------------------------------------------
        // Build the list of indices and processes to match.
        // -------------------------------------------------

        // Filter out index - process combination that are certain never
        // to match.

        std::vector<std::pair<int,int> > index_process_to_match;
        for(size_t i = 0; i < indices.size(); ++i)
        {
            // Get the index.
            const int index = indices[i];
            const int basis_site = lattice_map.basisSiteFromIndex(index);

            // For each process, check if we should try to match.
            for (size_t j = 0; j < interactions.processes().size(); ++j)
            {
                // Check if the basis site is listed.
                const std::vector<int> & process_basis_sites = (*interactions.processes()[j]).basisSites();
                if ( std::find(process_basis_sites.begin(), process_basis_sites.end(), basis_site) != process_basis_sites.end() )
                {
                    // This is a potential match.
                    index_process_to_match.push_back(std::pair<int,int>(i,j));
                }
            }
        }

        // ----------------------------
        // Generate the lists of tasks.
        // ----------------------------

        // IN PARALLEL:
        // Split the index_process_to_match vector up on each process.

        std::vector<RemoveTask> remove_tasks;
        std::vector<UpdateTask> update_tasks;
        std::vector<AddTask>    add_tasks;

        matchIndicesWithProcesses(index_process_to_match,
                                  interactions,
                                  configuration,
                                  remove_tasks,
                                  update_tasks,
                                  add_tasks);

        // IN PARALLEL:
        // Broadcast the lists of tasks.


        // ------------------------
        // Calculate the new rates.
        // ------------------------

        // IN PARALLEL:
        // Split the update and add tasks amongst processes.

        // Calculate the rates.
//        updateRates(add_tasks,    interactions, configuration);
//        updateRates(update_tasks, interactions, configuration);

        // IN PARALLEL:
        // Broadcast the add and update tasks.


        // ---------------------
        // Update the processes.
        // ---------------------

        updateProcesses(remove_tasks,
                        update_tasks,
                        add_tasks,
                        interactions);

        // DONE

        // const RateCalculator & rc = interactions.rateCalculator();
        // const double rate = rc.backendRateCallback();
        // printf("This is the rate we got from python %f\n", rate);
        // exit(8);
    }
}

// -----------------------------------------------------------------------------
//
void Matcher::matchIndicesWithProcesses(const std::vector<std::pair<int,int> > & index_process_to_match,
                                        Interactions  & interactions,
                                        Configuration & configuration,
                                        std::vector<RemoveTask> & remove_tasks,
                                        std::vector<UpdateTask> & update_tasks,
                                        std::vector<AddTask>    & add_tasks) const
{
    // Loop over pairs to match.
    for (size_t i = 0; i < index_process_to_match.size(); ++i)
    {
        // Get the process and index to match.
        const int index = index_process_to_match[i].first;
        const int p_idx = index_process_to_match[i].second;
        Process & process = (*interactions.processes()[p_idx]);

        // Perform the matching.
        const bool in_list = process.isListed(index);

        const std::vector<MinimalMatchListEntry> & process_match_list = process.minimalMatchList();
        const std::vector<MinimalMatchListEntry> & index_match_list   = configuration.minimalMatchList(index);

        const bool is_match = isMatch(process_match_list,
                                      index_match_list);

        // -------------------------------------------------------------
        // Determine what to do with this pair of processes and indices.
        // -------------------------------------------------------------

        // If no match and previous match - remove.
        if (!is_match && in_list)
        {
            RemoveTask t;
            t.index   = index;
            t.process = p_idx;
            remove_tasks.push_back(t);
        }

        // If match and previous match - update the rate.
        /* // ML
        else if (is_match && in_list)
        {
            UpdateTask t;
            t.index   = index;
            t.process = p_idx;
            t.rate    = process.rateConstant();
            update_tasks.push_back(t);
        }
        */

        // If match and not previous match - add.
        else if (is_match && !in_list)
        {
            AddTask t;
            t.index   = index;
            t.process = p_idx;
            t.rate    = process.rateConstant();
            add_tasks.push_back(t);
        }
    }

    // DONE
}

// -----------------------------------------------------------------------------
//
void Matcher::calculateMatching(Process & process,
                                Configuration & configuration,
                                const int index) const
{
    // Check in the indices list of this process to see if we have
    // a previous match.
    const bool in_list = process.isListed(index);

    // Get the match lists out.
    const std::vector<MinimalMatchListEntry> & process_match_list = process.minimalMatchList();

    // This gets the updated match list.
    const std::vector<MinimalMatchListEntry> & index_match_list   = configuration.minimalMatchList(index);

    // Check the match.
    const bool is_match = isMatch(process_match_list,
                                  index_match_list);

    // If match and not previous match - add to the list.
    if (is_match && !in_list)
    {
        process.addSite(index);
    }
    // If no match and previous match - remove from the list.
    else if (!is_match && in_list)
    {
        process.removeSite(index);
    }
}


// -----------------------------------------------------------------------------
//
bool Matcher::isMatch(const std::vector<MinimalMatchListEntry> & process_match_list,
                      const std::vector<MinimalMatchListEntry> & index_match_list) const
{
    if (index_match_list.size() < process_match_list.size())
    {
        return false;
    }

    // Iterators to the match list entries.
    std::vector<MinimalMatchListEntry>::const_iterator it1 = process_match_list.begin();
    std::vector<MinimalMatchListEntry>::const_iterator it2 = index_match_list.begin();

    // Loop over the process match list and compare.
    for( ; it1 != process_match_list.end(); ++it1, ++it2)
    {
        if ((*it1) != (*it2))
        {
            return false;
        }
    }

    // All match, return true.
    return true;
}


// -----------------------------------------------------------------------------
//
void Matcher::updateProcesses(const std::vector<RemoveTask> & remove_tasks,
                              const std::vector<UpdateTask> & update_tasks,
                              const std::vector<AddTask>    & add_tasks,
                              Interactions & interactions) const
{
    // Remove.
    for (size_t i = 0; i < remove_tasks.size(); ++i)
    {
        const int index = remove_tasks[i].index;
        const int p_idx = remove_tasks[i].process;
        interactions.processes()[p_idx]->removeSite(index);
    }

    // Update.
    for (size_t i = 0; i < update_tasks.size(); ++i)
    {
        const int index   = update_tasks[i].index;
        const int p_idx   = update_tasks[i].process;
        const double rate = update_tasks[i].rate;
        interactions.processes()[p_idx]->removeSite(index);
        interactions.processes()[p_idx]->addSite(index, rate);
    }

    // Add.
    for (size_t i = 0; i < add_tasks.size(); ++i)
    {
        const int index   = add_tasks[i].index;
        const int p_idx   = add_tasks[i].process;
        const double rate = add_tasks[i].rate;
        interactions.processes()[p_idx]->addSite(index, rate);
    }
}
