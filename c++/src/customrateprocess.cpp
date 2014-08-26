/*
  Copyright (c)  2013-2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  customrateprocess.cpp
 *  \brief File for the implementation code of the CustomRateProcess class.
 */

#include <algorithm>
#include <cstdio>
#include <numeric>

#include "customrateprocess.h"
#include "random.h"


// -----------------------------------------------------------------------------
//
CustomRateProcess::CustomRateProcess(const Configuration & first,
                                     const Configuration & second,
                                     const double rate,
                                     const std::vector<int> & basis_sites,
                                     const double cutoff,
                                     const std::vector<int> & move_origins,
                                     const std::vector<Coordinate> & move_vectors,
                                     const int process_number,
                                     const bool cache_rate) :
    Process(first, second, rate, basis_sites, move_origins, move_vectors, process_number)
{
    // Set the cache rate flag.
    cache_rate_ = cache_rate;

    // Save the cutoff if it is larger than what we have allready.
    if (cutoff > cutoff_)
    {
        cutoff_ = cutoff;
    }

    // Update the range such that all distances withing the cutoff are
    // guarranteed to be included.
    range_ = static_cast<int>(cutoff_);
}


// -----------------------------------------------------------------------------
//
void CustomRateProcess::addSite(const int index,
                                const double rate,
                                const double multiplicity)
{
    sites_.push_back(index);
    site_multiplicity_.push_back(multiplicity);
    site_rates_.push_back(rate);
    total_rate_ += multiplicity * rate;
}


// -----------------------------------------------------------------------------
//
void CustomRateProcess::removeSite(const int index)
{
    // Find the index to remove.
    std::vector<int>::iterator it1 = std::find(sites_.begin(),
                                               sites_.end(),
                                               index);

    // Swap the index to remove with the last index.
    std::vector<int>::iterator last = sites_.end()-1;
    std::swap((*it1), (*last));

    // Remove the last index from the list.
    sites_.pop_back();

    // Calculate the positions in the site_rates_ vector.
    std::vector<double>::iterator it2 = site_rates_.begin() + (it1-sites_.begin());
    std::vector<double>::iterator last_rate = site_rates_.end()-1;

    // Swap and remove.
    std::swap((*it2), (*last_rate));

    // Store the value temporarily.
    const double site_rate = site_rates_.back();
    site_rates_.pop_back();

    // Calculate the position in the site_multiplicity_ vector.
    std::vector<double>::iterator it3 = site_multiplicity_.begin() + (it1-sites_.begin());
    std::vector<double>::iterator last_multiplicity = site_multiplicity_.end()-1;

    // Swap and remove.
    std::swap((*it3), (*last_multiplicity));

    // Update the total rate.
    total_rate_ -= site_rate * site_multiplicity_.back();

    site_multiplicity_.pop_back();

}


// -----------------------------------------------------------------------------
//
void CustomRateProcess::updateRateTable()
{
    // Resize and update the incremental rate table.
    incremental_rate_table_.resize(site_multiplicity_.size());
    double previous = 0.0;
    for (size_t i = 0; i < site_multiplicity_.size(); ++i)
    {
        incremental_rate_table_[i] = previous + (site_rates_[i] * site_multiplicity_[i]);
        previous = incremental_rate_table_[i];
    }

    // DONE
}

