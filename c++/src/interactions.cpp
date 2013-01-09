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
Interactions::Interactions(const std::vector<Process> & processes) :
    processes_(processes),
    probability_table_(processes.size(), std::pair<double,int>(0.0,0))
{
    // NOTHING HERE YET
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

    // ML:
    size_t sum = 0;
    for (size_t i = 0; i < processes_.size(); ++i)
    {
        sum += processes_[i].sites().size();
    }
    if (sum == 0)
    {
        // Should be a graceful callback.
        printf("ERROR - no more available processes.\n");
        exit(8);
    }

    // Get a random number between 0.0 and the total imcremented rate.
    const double rnd = randomDouble01() * totalRate();
    const std::pair<double,int> rnd_pair(rnd,1);

    // Find the lower bound - corresponding to the first element for which
    // the accumulated rate is not smaller than rnd, and the number
    // of available processes is larger than zero.
    const std::vector<std::pair<double, int> >::const_iterator begin = probability_table_.begin();
    const std::vector<std::pair<double, int> >::const_iterator end   = probability_table_.end();
    const std::vector<std::pair<double, int> >::const_iterator it1   =  std::lower_bound( begin,
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

