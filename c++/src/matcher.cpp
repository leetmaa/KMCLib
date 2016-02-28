/*
  Copyright (c)  2012-2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  matcher.cpp
 *  \brief File for the implementation code of the Matcher class.
 */

#include <cstdio>
#include <algorithm>
#include <cstdlib>

#include "matcher.h"
#include "matchlist.h"
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
Matcher::Matcher(const size_t & sites, const size_t & processes) :
    rate_table_(),
    inverse_table_(sites, std::vector<bool>(processes, false))
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
    // PERFORMME: What happens in this function is
    //            highly performance critical.

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
        std::vector<RateTask> global_tasks;
        std::vector<ratekey>  global_keys;
        std::vector<int>      global_process_numbers;

        // Find out which tasks are allready calculated and stored.
        std::vector<int> add_task_indices;
        for (size_t i = 0; i < add_tasks.size(); ++i)
        {
            // Calculate the key.
            const Process & process = (*interactions.processes()[add_tasks[i].process]);
            const int index   = add_tasks[i].index;
            const ratekey key = hashCustomRateInput(index, process, configuration);

            // Check if the key has a stored value.
            if (rate_table_.stored(key) != -1)
            {
                add_tasks[i].rate = rate_table_.retrieve(key);
            }
            else
            {
                global_tasks.push_back(add_tasks[i]);
                global_keys.push_back(key);
                global_process_numbers.push_back(process.processNumber());
                add_task_indices.push_back(i);
            }
        }

        // The same procedure for the update tasks.
        std::vector<int> update_task_indices;
        for (size_t i = 0; i < update_tasks.size(); ++i)
        {
            // Calculate the key.
            const Process & process = (*interactions.processes()[update_tasks[i].process]);
            const int index   = update_tasks[i].index;
            const ratekey key = hashCustomRateInput(index, process, configuration);

            // Check if the key has a stored value.
            if (rate_table_.stored(key) != -1)
            {
                update_tasks[i].rate = rate_table_.retrieve(key);
            }
            else
            {
                global_tasks.push_back(update_tasks[i]);
                global_keys.push_back(key);
                global_process_numbers.push_back(process.processNumber());
                update_task_indices.push_back(i);
            }
        }

        // ------------------------------------------------------------------------
        // Here comes the MPI parallelism
        // ------------------------------------------------------------------------
        // Split up the tasks.
        std::vector<RateTask> local_tasks = splitOverProcesses(global_tasks);
        std::vector<double> local_tasks_rates(local_tasks.size(), 0.0);

        // Update in parallel.
        updateRates(local_tasks_rates, local_tasks, interactions, configuration);

        // Join the results.
        const std::vector<double> global_tasks_rates = joinOverProcesses(local_tasks_rates);
        // ------------------------------------------------------------------------

        // Copy the results over to the tasks vectors.
        for (size_t i = 0; i < add_task_indices.size(); ++i)
        {
            const int index = add_task_indices[i];
            add_tasks[index].rate = global_tasks_rates[i];
        }

        const size_t offset = add_task_indices.size();
        for (size_t i = 0; i < update_task_indices.size(); ++i)
        {
            const int index = update_task_indices[i];
            update_tasks[index].rate = global_tasks_rates[offset + i];
        }

        // Store the calculated key value pairs.
        for (size_t i = 0; i < global_tasks_rates.size(); ++i)
        {
            // But only if the procees can safely be cached.
            const int process_number = global_process_numbers[i];
            if ((*interactions.processes()[process_number]).cacheRate())
            {
                const ratekey key = global_keys[i];
                const double rate = global_tasks_rates[i];
                rate_table_.store(key, rate);
            }
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
        const bool in_list = inverse_table_[index][p_idx];

        // ML:
        const bool is_match = whateverMatch(process.processMatchList(),
                                            configuration.configMatchList(index));

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

        else if (task_types[i] == 2 || task_types[i] == 3)
        {
            // Get the multiplicity.
            const double m = multiplicity(process.processMatchList(),
                                          configuration.configMatchList(index));

            RateTask t;
            t.index        = index;
            t.process      = p_idx;
            t.rate         = process.rateConstant();
            t.multiplicity = m;

            // If match and previous match - update the rate.
            if (task_types[i] == 2)
            {
                update_tasks.push_back(t);
            }

            // If match and not previous match - add.
            else if (task_types[i] == 3)
            {
                add_tasks.push_back(t);
            }
        }
    }

    // DONE
}

// -----------------------------------------------------------------------------
//
bool Matcher::isMatch(const ProcessBucketMatchList & process_match_list,
                      const ConfigBucketMatchList & index_match_list) const
{
    return whateverMatch(process_match_list, index_match_list);

    /*
    if (index_match_list.size() < process_match_list.size())
    {
        return false;
    }

    // Iterators to the match list entries.
    ProcessBucketMatchList::const_iterator it1 = process_match_list.begin();
    ConfigBucketMatchList::const_iterator it2 = index_match_list.begin();

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
    */
}


// -----------------------------------------------------------------------------
//
void Matcher::printMatchLists(const ProcessBucketMatchList & process_match_list,
                              const ConfigBucketMatchList & index_match_list) const
{
    printf("\n\n       PROCESS match list           INDEX match list\n");
    printf("size   %16lu %16lu\n", process_match_list.size(), index_match_list.size());


    // Iterators to the match list entries.
    ProcessBucketMatchList::const_iterator it1 = process_match_list.begin();
    ConfigBucketMatchList::const_iterator it2 = index_match_list.begin();

    // Loop over the process match list and compare.
    int step = 0;
    for( ; it1 != process_match_list.end(); ++it1, ++it2)
    {
        printf("%5i:", step);
        printf("  distance:");
        printf("%13f",   (*it1).distance);
        printf("%13f\n", (*it2).distance);

        printf("              [ ");
        for (int i = 0; i < (*it1).match_types.size(); ++i)
        {
            printf("%i ", (*it1).match_types[i]);
        }
        printf(" ]");


        printf("    [ ");
        for (int i = 0; i < (*it2).match_types.size(); ++i)
        {
            printf("%i ", (*it2).match_types[i]);
        }
        printf(" ]\n");


        ++step;

        if (!(*it1).match(*it2))
        {
            printf("break at step %i\n", step);
            break;
        }
    }

}


// -----------------------------------------------------------------------------
//
void Matcher::printMatchLists(const ConfigBucketMatchList & process_match_list,
                              const ConfigBucketMatchList & index_match_list) const
{
    printf("\n\n       PROCESS match list           INDEX match list\n");
    printf("size   %16lu %16lu\n", process_match_list.size(), index_match_list.size());


    // Iterators to the match list entries.
    ConfigBucketMatchList::const_iterator it1 = process_match_list.begin();
    ConfigBucketMatchList::const_iterator it2 = index_match_list.begin();

    // Loop over the process match list and compare.
    int step = 0;
    for( ; it1 != process_match_list.end(); ++it1, ++it2)
    {
        printf("%5i:", step);
        printf("  distance:");
        printf("%13f",   (*it1).distance);
        printf("%13f\n", (*it2).distance);

        printf("              [ ");
        for (int i = 0; i < (*it1).match_types.size(); ++i)
        {
            printf("%i ", (*it1).match_types[i]);
        }
        printf(" ]");


        printf("    [ ");
        for (int i = 0; i < (*it2).match_types.size(); ++i)
        {
            printf("%i ", (*it2).match_types[i]);
        }
        printf(" ]\n");


        ++step;
    }

}


// -----------------------------------------------------------------------------
//
void Matcher::printMatchList(const ConfigBucketMatchList & index_match_list) const
{
    printf("\n\n      INDEX match list\n");
    printf("size   %16lu\n", index_match_list.size());


    // Iterators to the match list entries.
    ConfigBucketMatchList::const_iterator it2 = index_match_list.begin();

    // Loop over the process match list and compare.
    int step = 0;
    for( ; it2 != index_match_list.end(); ++it2)
    {
        printf("%5i:", step);
        printf("  distance:");
        printf("%13f\n", (*it2).distance);

        printf("    [ ");
        for (int i = 0; i < (*it2).match_types.size(); ++i)
        {
            printf("%i ", (*it2).match_types[i]);
        }
        printf(" ]\n");


        ++step;

    }

}


// -----------------------------------------------------------------------------
//
void Matcher::updateProcesses(const std::vector<RemoveTask> & remove_tasks,
                              const std::vector<RateTask>   & update_tasks,
                              const std::vector<RateTask>   & add_tasks,
                              Interactions & interactions)
{
    // This could perhaps be OpenMP parallelized.

    // Remove.
    for (size_t i = 0; i < remove_tasks.size(); ++i)
    {
        const int index = remove_tasks[i].index;
        const int p_idx = remove_tasks[i].process;
        interactions.processes()[p_idx]->removeSite(index);
        inverse_table_[index][p_idx] = false;
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
        inverse_table_[index][p_idx] = true;
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

        // Calculate the new rate.
        new_rates[i] = updateSingleRate(index, process, configuration, rate_calculator);
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
    const ProcessBucketMatchList & process_match_list = process.processMatchList();
    const ConfigBucketMatchList & config_match_list   = configuration.configMatchList(index);

    // We will also need the elements.
    const std::vector<std::vector<std::string> > & elements = configuration.elements();
    const std::vector<TypeBucket> & types = configuration.types();

    // Get cutoff distance from the process.
    const double cutoff = process.cutoff();
    ConfigBucketMatchList::const_iterator it1 = config_match_list.begin();
    int len = 0;
    while ( (*it1).distance <= cutoff && it1 != config_match_list.end())
    {
        ++it1;
        ++len;
    }

    const size_t distance = it1 - config_match_list.begin();

    // Allocate memory for the numpy geometry and copy the data over.
    std::vector<double> numpy_geo(len*3);
    std::vector<std::string> types_before(distance);
    std::vector<TypeBucket> occupations(distance);

    for (size_t i = 0; i < distance; ++i)
    {
        const Coordinate coord(config_match_list[i].x,
                               config_match_list[i].y,
                               config_match_list[i].z);

        numpy_geo[3*i]   = coord.x();
        numpy_geo[3*i+1] = coord.y();
        numpy_geo[3*i+2] = coord.z();

        const int idx   = config_match_list[i].index;
        types_before[i] = elements[idx][0];
        occupations[i]  = types[idx];
    }

    // Types after the process.
    std::vector<std::string> types_after = types_before;
    std::vector<TypeBucket> update(len, TypeBucket(occupations[0].size()));

    // Loop over the process match list and update the types_after vector.
    for (size_t i = 0; i < process_match_list.size(); ++i)
    {
        const TypeBucket & update_types = process_match_list[i].update_types;

        // Save the update types in the full update vector.
        update[i] = update_types;

        // PERFORMME: This should be handeled more efficiently.

        // Check that the update type is not 'zero'.
        int sum = 0;
        for (int j = 0; j < update_types.size(); ++j)
        {
            sum += std::abs(update_types[j]);
        }

        if ( sum != 0  && !(update_types == 0) )
        {

            // For non-buckets only.
            for (int j = 0; j < update_types.size(); ++j)
            {
                // Taking the first only.
                if (update_types[j] == 1)
                {
                    types_after[i] = configuration.typeName(j);
                    break;
                }
            }

        }
    }

    // Calculate the rate using the provided rate calculator.
    const double rate_constant = process.rateConstant();
    const int process_number   = process.processNumber();
    const double global_x = configuration.coordinates()[index].x();
    const double global_y = configuration.coordinates()[index].y();
    const double global_z = configuration.coordinates()[index].z();

    // Determine if we should use the buckets interface or not.
    if (!process.bucketProcess())
    {
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
    else
    {
        return rate_calculator.backendRateCallbackBuckets(numpy_geo,
                                                          len,
                                                          occupations,
                                                          update,
                                                          configuration.typeNames(),
                                                          rate_constant,
                                                          process_number,
                                                          global_x,
                                                          global_y,
                                                          global_z);
    }
}
