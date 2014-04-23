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
RateTable::RateTable() :
    n_tables_(8),
    max_size_(1024),
    current_table_(0),
    tables_(std::vector< std::unordered_map<ratekey, double> >(n_tables_))
{
    // NOTHING HERE
}


// -----------------------------------------------------------------------------
//
int RateTable::stored(const ratekey key)
{
    // Check all tables and see if we find the key.
    for (size_t i = 0; i < tables_.size(); ++i)
    {
        if (tables_[i].find(key) != tables_[i].end())
        {
            return i;
        }
    }
    return -1;
}


// -----------------------------------------------------------------------------
//
void RateTable::store(const ratekey key, const double value)
{
    // PERFORMME:
    // This is slow but necessary to prevent errors.
    // if (stored(key) != -1)
    // {
    //     throw std::runtime_error("Key clash error in RateTable.");
    // }

    // Add the value to the current table.
    tables_[current_table_][key] = value;

    // Check if it is time to switch table.
    if (tables_[current_table_].size() == max_size_)
    {
        // Take the next current table.
        current_table_ = (current_table_ + 1) % n_tables_;

        // Clear its content.
        tables_[current_table_].clear();
    }
}


// -----------------------------------------------------------------------------
//
double RateTable::retrieve(const ratekey key)
{
    // Find which table this key is stored in.
    const int nt = stored(key);
    return tables_.at(nt).at(key);
}

