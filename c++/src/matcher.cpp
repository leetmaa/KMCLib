/*
  Copyright (c)  2012-2013  Mikael Leetmaa

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
#include "hash.h"

#include "mpicommons.h"
#include "mpiroutines.h"

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
                                const std::vector<int> & indices)
{
    // Build the list of indices and processes to match.

    std::vector<std::pair<int,int> > index_process_to_match;
    for(size_t i = 0; i < indices.size(); ++i)
    {
        // Get the index.
        const int index = indices[i];
        bool use_index = false;

        // Get the basis site.
        const int basis_site = lattice_map.basisSiteFromIndex(index);

        // For each process, check if we should try to match.
        for (size_t j = 0; j < interactions.processes().size(); ++j)
        {
            // Check if the basis site is listed.
            const std::vector<int> & process_basis_sites = (*interactions.processes()[j]).basisSites();
            if ( std::find(process_basis_sites.begin(), process_basis_sites.end(), basis_site) != process_basis_sites.end() )
            {
                // This is a potential match.
                index_process_to_match.push_back(std::pair<int,int>(index,j));
                use_index = true;
            }
        }

        // Update the configuration match list for this index if it will be used.
        if (use_index)
        {
            configuration.updateMatchList(index);
        }
    }

    // Generate the lists of tasks.

    std::vector<RemoveTask> remove_tasks;
    std::vector<RateTask>   update_tasks;
    std::vector<RateTask>   add_tasks;

    matchIndicesWithProcesses(index_process_to_match,
                              interactions,
                              configuration,
                              remove_tasks,
                              update_tasks,
                              add_tasks);

    // Calculate the new rates if needed.

    if (interactions.useCustomRates())
    {
        // Create a common task list for getting a good load balance.
        std::vector<RateTask> global_tasks(add_tasks.size()+update_tasks.size());
        std::copy( update_tasks.begin(), update_tasks.end(),
                   std::copy( add_tasks.begin(), add_tasks.end(), global_tasks.begin()) );

        // ML: FIXME:
        // This is where we have to screen for allready calculated rates for the parallelism to work well.

        // Split up the tasks.
        std::vector<RateTask> local_tasks = splitOverProcesses(global_tasks);
        std::vector<double> local_tasks_rates(local_tasks.size(), 0.0);

        // Update.
        updateRates(local_tasks_rates, local_tasks, interactions, configuration);

        // Join the results.
        const std::vector<double> global_tasks_rates = joinOverProcesses(local_tasks_rates);

        // Copy the results over.
        for (size_t i = 0; i < add_tasks.size(); ++i)
        {
            add_tasks[i].rate = global_tasks_rates[i];
        }

        const size_t offset = add_tasks.size();
        for (size_t i = 0; i < update_tasks.size(); ++i)
        {
            update_tasks[i].rate = global_tasks_rates[offset + i];
        }
    }

    // Update the processes.

    updateProcesses(remove_tasks,
                    update_tasks,
                    add_tasks,
                    interactions);

    // DONE
}

// -----------------------------------------------------------------------------
//
void Matcher::matchIndicesWithProcesses(const std::vector<std::pair<int,int> > & index_process_to_match,
                                        const Interactions  & interactions,
                                        const Configuration & configuration,
                                        std::vector<RemoveTask> & remove_tasks,
                                        std::vector<RateTask>   & update_tasks,
                                        std::vector<RateTask>   & add_tasks) const
{
    // Setup local variables for running in parallel.
    std::vector< std::pair<int,int> > local_index_process_to_match = \
        splitOverProcesses(index_process_to_match);

    // These are the local task types to fill with matching restults.
    const int n_local_tasks = local_index_process_to_match.size();
    std::vector<int> local_task_types(n_local_tasks, 0);

    // Loop over pairs to match.
    for (size_t i = 0; i < local_index_process_to_match.size(); ++i)
    {
        // Get the process and index to match.
        const int index = local_index_process_to_match[i].first;
        const int p_idx = local_index_process_to_match[i].second;
        Process & process = (*interactions.processes()[p_idx]);

        // Perform the matching.
        const bool in_list = process.isListed(index);

        const std::vector<MinimalMatchListEntry> & process_match_list = process.minimalMatchList();
        const std::vector<MinimalMatchListEntry> & index_match_list   = configuration.minimalMatchList(index);

        const bool is_match = isMatch(process_match_list,
                                      index_match_list);

        // Determine what to do with this pair of processes and indices.
        if (!is_match && in_list)
        {
            // If no match and previous match - remove.
            local_task_types[i] = 1;
        }
        else if (is_match && in_list)
        {
            // If match and previous match - update the rate.
            local_task_types[i] = 2;
        }
        else if (is_match && !in_list)
        {
            // If match and not previous match - add.
            local_task_types[i] = 3;
        }
    }

    // Join the result - parallel.
    const std::vector<int> task_types = joinOverProcesses(local_task_types);

    // Loop again (not in parallel) and add the tasks to the taks vectors.
    const size_t n_tasks = index_process_to_match.size();
    for (size_t i = 0; i < n_tasks; ++i)
    {
        const int index = index_process_to_match[i].first;
        const int p_idx = index_process_to_match[i].second;
        const Process & process = (*interactions.processes()[p_idx]);

        // If no match and previous match - remove.
        if (task_types[i] == 1)
        {
            RemoveTask t;
            t.index   = index;
            t.process = p_idx;
            remove_tasks.push_back(t);
        }

        // If match and previous match - update the rate.
        else if (task_types[i] == 2)
        {
            RateTask t;
            t.index   = index;
            t.process = p_idx;
            t.rate    = process.rateConstant();
            update_tasks.push_back(t);
        }

        // If match and not previous match - add.
        else if (task_types[i] == 3)
        {
            RateTask t;
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
                              const std::vector<RateTask>   & update_tasks,
                              const std::vector<RateTask>   & add_tasks,
                              Interactions & interactions) const
{
    // This could perhaps be OpenMP parallelized.

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


// -----------------------------------------------------------------------------
//
void Matcher::updateRates(std::vector<double>         & new_rates,
                          const std::vector<RateTask> & tasks,
                          const Interactions          & interactions,
                          const Configuration         & configuration)
{
    // Use the backendCallBack function on the RateCalculator stored on the
    // interactions object, to get an updated rate for each process.
    const RateCalculator & rate_calculator = interactions.rateCalculator();

    for (size_t i = 0; i < tasks.size(); ++i)
    {
        // Get the rate process to use.
        const Process & process = (*interactions.processes()[tasks[i].process]);

        // Get the coordinate index.
        const int index = tasks[i].index;

        // Try to retrieve the key form earlier calculation.
        const ratekey key = hashCustomRateInput(index, process, configuration);
        if (rate_table_.stored(key) != -1)
        {
            new_rates[i] = rate_table_.retrieve(key);
        }
        else
        {
            // Calculate the new rate.
            new_rates[i] = updateSingleRate(index, process, configuration, rate_calculator);

            // Store the rate for future usage.
            rate_table_.store(key, new_rates[i]);
        }
    }
}


// -----------------------------------------------------------------------------
//
double Matcher::updateSingleRate(const int index,
                                 const Process        & process,
                                 const Configuration  & configuration,
                                 const RateCalculator & rate_calculator) const
{
    // Get the match lists.
    const std::vector<MinimalMatchListEntry> & process_match_list = process.minimalMatchList();
    const std::vector<MinimalMatchListEntry> & config_match_list  = configuration.minimalMatchList(index);

    // We will also need the elements.
    const std::vector<std::string> & elements = configuration.elements();

    // Get cutoff distance from the process.
    const double cutoff = process.cutoff();
    std::vector<MinimalMatchListEntry>::const_iterator it1 = config_match_list.begin();
    int len = 0;
    while ( (*it1).distance <= cutoff && it1 != config_match_list.end())
    {
        ++it1;
        ++len;
    }
    const std::vector<MinimalMatchListEntry>::const_iterator new_end = it1;

    // Allocate memory for the numpy geometry and copy the data over.
    std::vector<double> numpy_geo(len*3);
    std::vector<double>::iterator it_geo = numpy_geo.begin();
    std::vector<std::string> types_before;

    it1 = config_match_list.begin();
    for ( ; it1 != new_end; ++it1 )
    {
        const Coordinate & coord   = (*it1).coordinate;

        (*it_geo) = coord.x();
        ++it_geo;

        (*it_geo) = coord.y();
        ++it_geo;

        (*it_geo) = coord.z();
        ++it_geo;

        const int idx = (*it1).index;
        types_before.push_back(elements[idx]);
    }

    // Types after the process.
    std::vector<std::string> types_after = types_before;

    // Rewind the config match list iterator.
    it1 = config_match_list.begin();

    // Get the iterators to the process match list and types after.
    std::vector<MinimalMatchListEntry>::const_iterator it2 = process_match_list.begin();
    std::vector<std::string>::iterator it3 = types_after.begin();
    const std::vector<MinimalMatchListEntry>::const_iterator end = process_match_list.end();

    // Loop over the process match list and update the types_after vector.
    for ( ; it2 != end; ++it1, ++it2, ++it3 )
    {
        const int update_type = (*it2).update_type;
        const int match_type  = (*it1).match_type;

        // Set the type after process. NOTE: The > 0 is needed for handling wildcards.
        if ( match_type != update_type  && update_type > 0)
        {
            (*it3) = configuration.typeName(update_type);
        }
    }

    // Calculate the rate using the provided rate calculator.
    const double rate_constant = process.rateConstant();
    const int process_number   = process.processNumber();
    const double global_x = configuration.coordinates()[index].x();
    const double global_y = configuration.coordinates()[index].y();
    const double global_z = configuration.coordinates()[index].z();

    return rate_calculator.backendRateCallback(numpy_geo,
                                               len,
                                               types_before,
                                               types_after,
                                               rate_constant,
                                               process_number,
                                               global_x,
                                               global_y,
                                               global_z);
}
