/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  configuration.cpp
 *  \brief File for the implementation code of the Configuration class.
 */


#include <cstdio>

#include "configuration.h"
#include "latticemap.h"

// -----------------------------------------------------------------------------
//
Configuration::Configuration(std::vector<std::vector<double> > const & coordinates,
                             std::vector<std::string> const & elements) :
    coordinates_(coordinates),
    elements_(elements)
{
    // NOTHING HERE YET
}


// -----------------------------------------------------------------------------
//
std::vector<MatchListEntry> Configuration::matchList(const std::vector<int> & indices,
                                                     const LatticeMap & lattice_map) const
{
    // NEEDS IMPLEMENTATION
    return std::vector<MatchListEntry>(0,MatchListEntry(0,0,0.0,Coordinate(0.0,0.0,0.0)));
}
