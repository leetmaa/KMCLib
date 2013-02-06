/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  ratecalculator.cpp
 *  \brief File for the implementation code of the PateCalculator class.
 */

#include <cstdio>

#include "ratecalculator.h"


// -----------------------------------------------------------------------------
//
RateCalculator::RateCalculator()
{
}


// -----------------------------------------------------------------------------
//
RateCalculator::~RateCalculator()
{
}


// -----------------------------------------------------------------------------
//
double RateCalculator::backendRateCallback() const
{
    // NEEDS IMPLEMENTATION
    return -1.0;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// PROTOTYPE AND TEST CODE FOLLOW
// -----------------------------------------------------------------------------

/// Dummy/test function for calling the objects whoAmI function from C++.
std::string callWhoAmI(const SimpleDummyBaseClass & obj) { return obj.whoAmI(); }

/// Dummy/test function for calling the RateCalculator from C++.
double getRate(const RateCalculator & rc) { return rc.backendRateCallback(); }

// -----------------------------------------------------------------------------
