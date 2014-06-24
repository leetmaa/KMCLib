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
#include <stdexcept>

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

    /*! \brief Get the number of slots in the bucket.
     *  \return : The size of the data vector.
     */
    int size() const { return static_cast<int>(data_.size()); }

    /*! \brief Access operator. Const version.
     *  \param i : The index to access at.
     *  \return : A copy of the element to access.
     */
    int operator[](const int i) const { return data_[i]; }

    /*! \brief Access operator, non-const version.
     *  \param i : The index to access at.
     *  \return : A copy of the element to access.
     */
    int & operator[](const int i) { return data_[i]; }

    /*! \brief Check if the buckets are identical.
     * \return : True if identical.
     */
    bool identical(const TypeBucket & other) const { return data_ == other.data_; }

    /*! \brief Check if this bucket is greater or equal to the other bucket.
     * \return : True if this is greater equal to the other, otherwise false.
     */
    inline
    bool greaterOrEqual(const TypeBucket & other) const;


protected:

private:

    /// The bucket data field.
    std::vector<int> data_;

};



// -----------------------------------------------------------------------------
// INLINE FUNCTOION IMPLEMENTATIONS FOLLOW
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//
bool TypeBucket::greaterOrEqual(const TypeBucket & other) const
{
    if (data_.size() != other.data_.size())
    {
        throw std::runtime_error("Fatal backend error. Size must match in bucket comparisons.");
    }

    return data_ >= other.data_;
}


#endif // __TYPE_BUCKET__

