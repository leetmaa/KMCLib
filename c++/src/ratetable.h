/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  ratetable.h
 *  \brief File for the RateTaböe class definition.
 */

#ifndef __RATETABLE__
#define __RATETABLE__

#include <unordered_map>

// NOTE: This implementation is based on the c++11 std::unordered_map.


// Define the ratekey type.
typedef unsigned long ratekey;


/*! \brief Class for storing and retrieving calculated rates.
 */
class RateTable {

public:

    /*! \brief Constructor.
     */
    RateTable();

    /*! \brief Check if a key has a stored value.
     *  \param key : The key to check for.
     *  \returns : True or false if the key has a stored value or not.
     */
    bool stored(const ratekey key);

    /*! \brief Store a key value pair.
     *  \param key   : The key to store for.
     *  \param value : The value to store.
     */
    void store(const ratekey key, const double value);

    /*! \brief Retrieve a stored value.
     *  \param key   : The key.
     *  \returns : The stored value.
     */
    double retrieve(const ratekey key);

protected:

private:

    /// The map to store key-value pairs in.
    std::unordered_map<ratekey, double> table_;

    /// Count how often a key is retrieved.
    std::unordered_map<ratekey, int> counters_;

};


#endif // __RATETABLE__

