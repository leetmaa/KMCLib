/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  interactions.cpp
 *  \brief File for the implementation code of the Interactions class.
 */

#include <algorithm>

#include "interactions.h"
#include "random.h"
#include "configuration.h"
#include "latticemap.h"
#include "ratecalculator.h"
#include "matchlist.h"


// -----------------------------------------------------------------------------
// Function for comparing two entries in the probability list.
bool pairComp(const std::pair<double,int> & p1,
              const std::pair<double, int> & p2)
{
    // This skips all processes with no available sites.
    if (p2.second == 0)
    {
        return false;
    }
    else
    {
        return (p1.first < p2.first);
    }
}


// -----------------------------------------------------------------------------
//
Interactions::Interactions(const std::vector<Process> & processes,
                           const bool implicit_wildcards) :
    processes_(processes),
    custom_rate_processes_(0),
    process_pointers_(processes.size(), NULL),
    probability_table_(processes.size(), std::pair<double,int>(0.0,0)),
    implicit_wildcards_(implicit_wildcards),
    use_custom_rates_(false),
    rate_calculator_placeholder_(RateCalculator()),
    rate_calculator_(rate_calculator_placeholder_)
{
    // Point the process pointers to the right places.
    for (size_t i = 0; i < processes_.size(); ++i)
    {
        process_pointers_[i] = &processes_[i];
    }

    // DONE
}


// -----------------------------------------------------------------------------
//
Interactions::Interactions(const std::vector<CustomRateProcess> & processes,
                           const bool implicit_wildcards,
                           const RateCalculator & rate_calculator) :
    processes_(0),
    custom_rate_processes_(processes),
    process_pointers_(processes.size(), NULL),
    probability_table_(processes.size(), std::pair<double,int>(0.0,0)),
    implicit_wildcards_(implicit_wildcards),
    use_custom_rates_(true),
    rate_calculator_(rate_calculator)
{
    // Point the process pointers to the right places.
    for (size_t i = 0; i < custom_rate_processes_.size(); ++i)
    {
        process_pointers_[i] = &custom_rate_processes_[i];
    }

    // DONE
}


// -----------------------------------------------------------------------------
//
int Interactions::maxRange() const
{
    // Loop through all processes and find the largest range in each of them.
    int max_range = 1;

    std::vector<Process*>::const_iterator it1 = process_pointers_.begin();
    for ( ; it1 != process_pointers_.end(); ++it1 )
    {
        max_range = std::max( max_range, (**it1).range() );
    }

    // Return.
    return max_range;
}


// -----------------------------------------------------------------------------
//
void Interactions::updateProcessMatchLists(const Configuration & configuration,
                                           const LatticeMap & lattice_map)
{
    // Skip if we are not using implicit wildcards.
    if (!implicit_wildcards_)
    {
        return;
    }

    // Loop through each process.
    for (size_t i = 0; i < process_pointers_.size(); ++i)
    {
        Process & p = (*process_pointers_[i]);

        // Skip this process unless the size of basis sites is one.
        if ( p.basisSites().size() != 1 )
        {
            continue;
        }

        // Get the match list for this process.
        ProcessBucketMatchList & process_matchlist = p.processMatchList();

        // Take out the basis position for the process.
        const int  basis_position = p.basisSites()[0];

        // Get the configuration match list for this basis position in the
        // most central cell.
        const int ii    = lattice_map.repetitionsA() / 2;
        const int jj    = lattice_map.repetitionsB() / 2;
        const int kk    = lattice_map.repetitionsC() / 2;
        const int index = lattice_map.indicesFromCell(ii, jj, kk)[basis_position];
        const ConfigBucketMatchList config_matchlist = configuration.configMatchList(index);

        // Perform the match where we add wildcards to fill the vacancies in the
        // process match list.
        ProcessBucketMatchList::iterator it1 = process_matchlist.begin();
        ConfigBucketMatchList::const_iterator it2 = config_matchlist.begin();

        // Insert the wildcards and update the indexing.
        int old_index = 0;
        int new_index = 0;
        std::vector<int> index_mapping(config_matchlist.size());

        for ( ; it1 != process_matchlist.end() && it2 != config_matchlist.end(); ++it1, ++it2 )
        {
            // Check if there is a match in lattice point.
            if( ! samePoint(*it1, *it2) )
            {
                // If not matching, add a wildcard entry to it1.
                ProcessBucketMatchListEntry wildcard_entry;// = (*it2);
                wildcard_entry.initWildcard(*it2);

                it1 = process_matchlist.insert(it1, wildcard_entry);
                // it1 now points to the newly inserted position.

                ++new_index;
            }
            else
            {
                // Add the mapping.
                index_mapping[old_index] = new_index;

                ++old_index;
                ++new_index;
           }
        }

        // With this mapping information we can update the process id moves.
        index_mapping.resize(old_index);
        std::vector<std::pair<int,int> > & id_moves = p.idMoves();
        for (size_t j = 0; j < id_moves.size(); ++j)
        {
            const int old_index_first  = id_moves[j].first;
            const int old_index_second = id_moves[j].second;

            id_moves[j].first  = index_mapping[old_index_first];
            id_moves[j].second = index_mapping[old_index_second];
        }
    }
}


// -----------------------------------------------------------------------------
//
int Interactions::totalAvailableSites() const
{
    // Loop through and sum all available sites on all processes.
    size_t sum = 0;
    for (size_t i = 0; i < process_pointers_.size(); ++i)
    {
        sum += process_pointers_[i]->nSites();
    }
    return static_cast<int>(sum);
}


// -----------------------------------------------------------------------------
//
void Interactions::updateProbabilityTable()
{
    // Loop over all processes.
    std::vector<Process*>::const_iterator it1 = process_pointers_.begin();
    std::vector<std::pair<double,int> >::iterator it2 = probability_table_.begin();
    const std::vector<Process*>::const_iterator end = process_pointers_.end();

    double previous_rate = 0.0;
    for ( ; it1 != end; ++it1, ++it2 )
    {
        // Find out its total probability.
        const int n_sites = (**it1).nSites();
        const double total_rate = (**it1).totalRate();
        // Store the probability with the process number in a table.
        (*it2).first = total_rate + previous_rate;
        previous_rate += total_rate;
        // Store the number of available processes to filter out zeroes later.
        (*it2).second = n_sites;
    }
}


// -----------------------------------------------------------------------------
//
int Interactions::pickProcessIndex() const
{
    // PERFORMME:
    // This implements the O(N) SSA algorithm.
    // One could consider implementing the O(logN) SSA-GB algorithm,
    // or the O(1) algorithm described in J.Chem.Phys. 128, 205101, (2008)
    // but it is typically the re-matching after an event that sets the
    // limit in terms of scaling with the number of processes.

    // Get a random number between 0.0 and the total imcremented rate.
    const double rnd = randomDouble01() * totalRate();
    const std::pair<double,int> rnd_pair(rnd,1);

    // Find the lower bound - corresponding to the first element for which
    // the accumulated rate is not smaller than rnd, and the number
    // of available processes is larger than zero.
    const std::vector<std::pair<double, int> >::const_iterator begin = probability_table_.begin();
    const std::vector<std::pair<double, int> >::const_iterator end   = probability_table_.end();
    const std::vector<std::pair<double, int> >::const_iterator it1   = std::lower_bound( begin,
                                                                                         end,
                                                                                         rnd_pair,
                                                                                         pairComp );

    // Find the index in the process list.
    return it1-begin;
}


// -----------------------------------------------------------------------------
//
Process* Interactions::pickProcess()
{
    const int index = pickProcessIndex();

    // Update the process internal probablility table if needed.
    process_pointers_[index]->updateRateTable();

    return process_pointers_[index];
}


// -----------------------------------------------------------------------------
//
void Interactions::clearMatching()
{
    for (size_t i = 0; i < process_pointers_.size(); ++i)
    {
        process_pointers_[i]->clearSites();
    }
}


