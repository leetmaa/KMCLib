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
// -----------------------------------------------------------------------------
// PROTOTYPE AND TEST CODE FOLLOW
// -----------------------------------------------------------------------------

/// Dummy/test function for calling the objects whoAmI function from Python via C++.
std::string callWhoAmI(const SimpleDummyBaseClass & obj) { return obj.whoAmI(); }

/// Dummy/test function for calling the RateCalculator's backend callback from Python via C++.
double getRate(const RateCalculator & rc,
               const std::vector<Coordinate> & geometry,
               const std::vector<std::string> & types_before,
               const std::vector<std::string> & types_after,
               const double rate_constant) { return rc.backendRateCallback(geometry,
                                                                           types_before,
                                                                           types_after,
                                                                           rate_constant); }

// -----------------------------------------------------------------------------
