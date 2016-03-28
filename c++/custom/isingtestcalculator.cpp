/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  isingtestcalculator.cpp
 *  \brief File for the implementation code of the
 *         IsingTestCalculator class.
 */

#include <cstdio>

#include "isingtestcalculator.h"


// -----------------------------------------------------------------------------
//
IsingTestCalculator::IsingTestCalculator(const Configuration & config) :
    config_(config)
{
    // NOTHING HERE.
}


// -----------------------------------------------------------------------------
//
IsingTestCalculator::~IsingTestCalculator()
{
    // NOTHING HERE.
}


// -----------------------------------------------------------------------------
//
double IsingTestCalculator::backendRateCallback(const std::vector<double> geometry,
                                                const int len,
                                                const std::vector<std::string> & types_before,
                                                const std::vector<std::string> & types_after,
                                                const double rate_constant,
                                                const int process_number,
                                                const double global_x,
                                                const double global_y,
                                                const double global_z) const
{
    // Get the number of U and D neighbours.
    int n_up  = (types_before[1] == "U") ? 1 : 0;
    n_up     += (types_before[2] == "U") ? 1 : 0;
    n_up     += (types_before[3] == "U") ? 1 : 0;
    n_up     += (types_before[4] == "U") ? 1 : 0;
    const int n_down = 4.0 - n_up;

    // Set the diff depending on the flipping direction.
    const int neg_diff = (process_number == 0) ? (n_down - n_up) : (n_up - n_down);
    return std::min(1.0, std::exp(neg_diff));
}



// -----------------------------------------------------------------------------
//
double IsingTestCalculator::cutoff() const
{
    return 1.0;
}


// -----------------------------------------------------------------------------
//
bool IsingTestCalculator::cacheRates() const
{
    return true;
}


// -----------------------------------------------------------------------------
//
std::vector<int> IsingTestCalculator::excludeFromCaching() const
{
    return std::vector<int>(0);
}
