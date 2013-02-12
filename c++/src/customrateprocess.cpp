/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  customrateprocess.cpp
 *  \brief File for the implementation code of the CustomRateProcess class.
 */

#include <algorithm>
#include <cstdio>

#include "customrateprocess.h"
#include "random.h"


// -----------------------------------------------------------------------------
//
CustomRateProcess::CustomRateProcess(const Configuration & first,
                                     const Configuration & second,
                                     const double rate,
                                     const std::vector<int> & basis_sites,
                                     const double cutoff) :
    Process(first, second, rate, basis_sites)
{
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
double CustomRateProcess::totalRate() const
{
    // Sum all individual rates.
    double sum = 0.0;
    for (size_t i = 0; i < site_rates_.size(); ++i)
    {
        sum += site_rates_[i];
    }

    return sum;
}

// -----------------------------------------------------------------------------
//
void CustomRateProcess::addSite(const int index, const double rate)
{
    sites_.push_back(index);
    site_rates_.push_back(rate);
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
    site_rates_.pop_back();
}

// -----------------------------------------------------------------------------
//
int CustomRateProcess::pickSite() const
{
    // Get the total rate.
    const double total_rate = incremental_rate_table_.back();

    // Get a random number between 0.0 and the total rate.
    const double rnd = randomDouble01() * total_rate;

    // Pick the site.
    const std::vector<double>::const_iterator begin = incremental_rate_table_.begin();
    const std::vector<double>::const_iterator end   = incremental_rate_table_.end();
    const std::vector<double>::const_iterator it1   = std::lower_bound( begin, end, rnd);

    // Get the site index.
    const int site_index = it1-begin;

    // Return the site.
    return sites_[site_index];
}

// -----------------------------------------------------------------------------
//
void CustomRateProcess::updateRateTable()
{
    // Resize and update the incremental rate table.
    incremental_rate_table_.resize(site_rates_.size());
    double previous = 0.0;
    for (size_t i = 0; i < site_rates_.size(); ++i)
    {
        incremental_rate_table_[i] = previous + site_rates_[i];
        previous = incremental_rate_table_[i];
    }

    // DONE
}

