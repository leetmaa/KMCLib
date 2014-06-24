/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  typebucket.h
 *  \brief File for the TypeBucket class definition.
 */

#ifndef __TYPE_BUCKET__
#define __TYPE_BUCKET__


#include <vector>

// Forward declarations if any.


/*! \brief Class for defining the type bucket data structure.
 */
class TypeBucket {

public:

    /*! \brief Default constructor needed for use in std::vector SWIG wrapping.
     */
    TypeBucket() {}

    /*! \brief Constructor for the type bucket. Initialized with zeros.
     *  \param size : The size of the bucket.
     */
    TypeBucket(const int size);

    /*! \brief Copy constructor for the type bucket. Initialize with the
     *         parameters of the other bucket.
     *  \param other : The other bucket to copy from.
     */
    TypeBucket(const TypeBucket & other);

protected:

private:

    /// The bucket data field.
    std::vector<int> data_;

};


#endif // __TYPE_BUCKET__

