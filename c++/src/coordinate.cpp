/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  coordinate.cpp
 *  \brief File for the implementation code of the Coordinate class.
 */


#include "coordinate.h"

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
bool Coordinate::operator<(const Coordinate & other) const
{
    if (x_ == other.x_)
    {
        if (y_ == other.y_)
        {
            return (z_ < other.z_);
        }
        else
        {
            return (y_ < other.y_);
        }
    }

    return (x_ < other.x_);
}

