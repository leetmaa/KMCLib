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
               const double rate_constant,
               const int process_number,
               const double global_x,
               const double global_y,
               const double global_z)
{
    // Translate to row major contiguous memoery layout.
    std::vector<double> py_geo(geometry.size()*3);

    // Fill with values.
    for (size_t i = 0; i < geometry.size(); ++i)
    {
        py_geo[3*i]   = geometry[i].x();
        py_geo[3*i+1] = geometry[i].y();
        py_geo[3*i+2] = geometry[i].z();
    }

   const double rate = rc.backendRateCallback(py_geo,
                                              geometry.size(),
                                              types_before,
                                              types_after,
                                              rate_constant,
                                              process_number,
                                              global_x,
                                              global_y,
                                              global_z);
   return rate;

}

// -----------------------------------------------------------------------------
