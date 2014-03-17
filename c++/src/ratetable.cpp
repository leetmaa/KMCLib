/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  ratetable.cpp
 *  \brief File for the implementation code of the RateTable class.
 */

#include "ratetable.h"
#include <stdexcept>


// -----------------------------------------------------------------------------
//
RateTable::RateTable()
{
    // NOTHING HERE
}


// -----------------------------------------------------------------------------
//
bool RateTable::stored(const ratekey key)
{
    return table_.find(key) != table_.end();
}


// -----------------------------------------------------------------------------
//
void RateTable::store(const ratekey key, const double value)
{
    // ML: This is where we can disable storing when we need to.

    // PERFORMME:
    // This is slow but necessary to prevent errors.

    if (stored(key))
    {
        throw std::runtime_error("Key clash error in RateTable.");
    }

    table_[key] = value;
}


// -----------------------------------------------------------------------------
//
double RateTable::retrieve(const ratekey key)
{
    // Add one to the counter of this key.
    counters_[key] += 1;
    return table_.at(key);
}

