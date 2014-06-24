/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  typebucket.cpp
 *  \brief File for the implementation code of the TypeBucket class.
 */

#include "typebucket.h"

// -----------------------------------------------------------------------------
//
TypeBucket::TypeBucket(const int size) :
    data_(size, 0)
{
    // NOTHING HERE
}


// -----------------------------------------------------------------------------
//
TypeBucket::TypeBucket(const TypeBucket & other) :
    data_(other.data_)
{
    // NOTHING HERE
}


