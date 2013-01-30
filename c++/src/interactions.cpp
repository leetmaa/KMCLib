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
#include "ratecalculator.h"

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
    probability_table_(processes.size(), std::pair<double,int>(0.0,0)),
    implicit_wildcards_(implicit_wildcards),
    custom_rates_(false),
    rate_calculator_placeholder_(RateCalculator()),
    rate_calculator_(rate_calculator_placeholder_)
{
    // NOTHING HERE YET
}


// -----------------------------------------------------------------------------
//
Interactions::Interactions(const std::vector<Process> & processes,
                           const bool implicit_wildcards,
                           const RateCalculator & rate_calculator) :
    processes_(processes),
    probability_table_(processes.size(), std::pair<double,int>(0.0,0)),
    implicit_wildcards_(implicit_wildcards),
    custom_rates_(true),
    rate_calculator_(rate_calculator)
{
    // NOTHING HERE YET
}


// -----------------------------------------------------------------------------
//
int Interactions::maxRange() const
{
    // Loop through all processes and find the largest range in each of them.
    int max_range = 1;

    std::vector<Process>::const_iterator it1 = processes_.begin();
    for ( ; it1 != processes_.end(); ++it1 )
    {
        const std::vector<MinimalMatchListEntry> & list = (*it1).minimalMatchList();
        std::vector<MinimalMatchListEntry>::const_iterator it2 = list.begin();

        // Get the largest coordinate out.
        for ( ; it2 != list.end(); ++it2 )
        {
            // Check the distance in x y and z. If the distance is say -1.5 this means
            // we need to go out to 2 shells to get the position included. Therfore the
            // logics for the negative values.

            const double x = (*it2).coordinate.x();
            const int cmp_x = static_cast<int>( ( x < 0.0 ) ? (-1.0*x)+0.99999 : x );
            max_range = std::max(cmp_x, max_range);

            const double y = (*it2).coordinate.y();
            const int cmp_y = static_cast<int>( ( y < 0.0 ) ? (-1.0*y)+0.99999 : y );
            max_range = std::max(cmp_y, max_range);

            const double z = (*it2).coordinate.z();
            const int cmp_z = static_cast<int>( ( z < 0.0 ) ? (-1.0*z)+0.99999 : z );
            max_range = std::max(cmp_z, max_range);
        }
    }

    // Return.
    return max_range;
}


// -----------------------------------------------------------------------------
//
void Interactions::updateProcessMatchLists(const Configuration & configuration)
{
    // Skip if we are not using implicit wildcards.
    if (!implicit_wildcards_)
    {
        return;
    }

    // Loop through each process.
    for (size_t i = 0; i < processes_.size(); ++i)
    {
        Process & p = processes_[i];

       // Skip this process unless the size of basis sites is one.
        if ( p.basisSites().size() != 1 )
        {
            continue;
        }

        // Get the match list for this process.
        std::vector<MinimalMatchListEntry> & process_matchlist = p.minimalMatchList();

        // Take out the basis position for the process.
        const int  basis_position = p.basisSites()[0];

        // Get the configuration match list for this basis position in the
        // first cell.
        const std::vector<MinimalMatchListEntry> config_matchlist = configuration.minimalMatchList(basis_position);

        // Perform the match where we add wildcards to fill the vacancies in the
        // process match list.

        std::vector<MinimalMatchListEntry>::iterator it1 = process_matchlist.begin();
        std::vector<MinimalMatchListEntry>::const_iterator it2 = config_matchlist.begin();

        // Loop over the process match list.
        for ( ; it1 != process_matchlist.end() && it2 != config_matchlist.end(); ++it1, ++it2 )
        {
            // Check if there is a match in lattice point.
            if( !((*it1) == (*it2)) )
            {
                // If not matching, add a wildcard entry to it1.
                MinimalMatchListEntry wildcard_entry = (*it2);
                wildcard_entry.match_type = 0;

                it1 = process_matchlist.insert(it1, wildcard_entry);
                // it1 now points to the newly inserted position.
            }
        }
    }
}

// -----------------------------------------------------------------------------
//
int Interactions::totalAvailableSites() const
{
    // Loop through and sum all available sites on all processes.
    size_t sum = 0;
    for (size_t i = 0; i < processes_.size(); ++i)
    {
        sum += processes_[i].sites().size();
    }
    return static_cast<int>(sum);
}


// -----------------------------------------------------------------------------
//
void Interactions::updateProbabilityTable()
{
    // Loop over all processes.
    std::vector<Process>::const_iterator it1 = processes_.begin();
    std::vector<std::pair<double,int> >::iterator it2 = probability_table_.begin();
    const std::vector<Process>::const_iterator end = processes_.end();

    double previous_rate = 0.0;
    for ( ; it1 != end; ++it1, ++it2 )
    {
        // Find out its total probability.
        const int n_sites = (*it1).nSites();
        const double total_rate = n_sites * (*it1).rateConstant();
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
Process & Interactions::pickProcess()
{
    const int index = pickProcessIndex();
    return processes_[index];
}

