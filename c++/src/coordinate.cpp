/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  coordinate.cpp
 *  \brief File for the implementation code of the Coordinate class.
 */

#include <cmath>
#include <cstdio>

#include "coordinate.h"

// -----------------------------------------------------------------------------
//
Coordinate::Coordinate() :
    x_(0.0),
    y_(0.0),
    z_(0.0)
{
    // NOTHING HERE
}

// -----------------------------------------------------------------------------
//
Coordinate::Coordinate(const double x,
                       const double y,
                       const double z) :
    x_(x),
    y_(y),
    z_(z)
{
    // NOTHING HERE
}

// -----------------------------------------------------------------------------
//
void Coordinate::print() const
{
    printf("(%20.10e %20.10e %20.10e)\n", x_, y_, z_);
}

