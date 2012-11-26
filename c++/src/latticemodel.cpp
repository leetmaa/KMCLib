/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  latticemodel.cpp
 *  \brief File for the implementation code of the LatticeModel class.
 */


#include "latticemodel.h"
#include "configuration.h"

#include <cstdio>

// -----------------------------------------------------------------------------
//
LatticeModel::LatticeModel(Configuration & configuration,
                           const LatticeMap & lattice_map) :
    configuration_(configuration),
    lattice_map_(lattice_map)
{
    // NOTHING HERE YET
}


// -----------------------------------------------------------------------------
//
void LatticeModel::singleStep()
{
    printf("Taking a step.\n");
    // NOTHING HERE YET
}

