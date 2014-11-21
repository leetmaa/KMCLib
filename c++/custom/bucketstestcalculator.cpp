/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  bucketstestcalculator.cpp
 *  \brief File for the implementation code of the
 *         BucketsTestCalculator class.
 */

#include <cstdio>

#include "bucketstestcalculator.h"


// -----------------------------------------------------------------------------
//
BucketsTestCalculator::BucketsTestCalculator(const Configuration & config) :
    config_(config)
{
    // NOTHING HERE.
}


// -----------------------------------------------------------------------------
//
BucketsTestCalculator::~BucketsTestCalculator()
{
    // NOTHING HERE.
}


// -----------------------------------------------------------------------------
//
double BucketsTestCalculator::backendRateCallbackBuckets(const std::vector<double> geometry,
                                                         const int len,
                                                         const std::vector<TypeBucket> & occupation,
                                                         const std::vector<TypeBucket> & update,
                                                         const std::vector<std::string> & type_map,
                                                         const double rate_constant,
                                                         const int process_number,
                                                         const double global_x,
                                                         const double global_y,
                                                         const double global_z) const
{
    // Creation process.
    if (process_number == 0)
    {
        return 0.1;
    }

    // Right.
    else if (process_number == 1)
    {
        if (occupation[4][2] != 0)
        {
            return 0.1;
        }
        else
        {
            return 1.0;
        }
    }

    // Left.
    else if (process_number == 2)
    {
        if (occupation[1][2] != 0)
        {
            return 0.1;
        }
        else
        {
            return 1.0;
        }
    }

    // Up.
    else if (process_number == 3)
    {
        if (occupation[3][2] != 0)
        {
            return 0.1;
        }
        else
        {
            return 1.0;
        }
    }

    // Down.
    else if (process_number == 4)
    {
        if (occupation[2][2] != 0)
        {
            return 0.1;
        }
        else
        {
            return 1.0;
        }
    }

    return 1.0;
}


// -----------------------------------------------------------------------------
//
double BucketsTestCalculator::cutoff() const
{
    return 1.0;
}


// -----------------------------------------------------------------------------
//
bool BucketsTestCalculator::cacheRates() const
{
    return false;
}


// -----------------------------------------------------------------------------
//
std::vector<int> BucketsTestCalculator::excludeFromCaching() const
{
    return std::vector<int>(0);
}
