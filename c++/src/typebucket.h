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
#include <cstring>

// Forward declarations if any.


/*! \brief Class for defining the type bucket data structure.
 */
class TypeBucket {

public:

    /*! \brief Default constructor needed for use in std::vector SWIG wrapping.
     */
    TypeBucket();

    // ML:
    ~TypeBucket();

    /*! \brief Constructor for the type bucket. Initialized with zeros.
     *  \param size : The size of the bucket.
     */
    TypeBucket(const int size);

    /*! \brief Copy constructor for the type bucket. Initialize with the
     *         parameters of the other bucket.
     *  \param other : The other bucket to copy from.
     */
    TypeBucket(const TypeBucket & other);

    // ML
    void operator=(const TypeBucket & other);

    /*! \brief Get the number of slots in the bucket.
     *  \return : The size of the data vector.
     */
    int size() const { return size_; }

    /*! \brief Access operator. Const version.
     *  \param i : The index to access at.
     *  \return : A copy of the element to access.
     */
    int operator[](const int i) const { return static_cast<int>(raw_data_[i]); }

    /*! \brief Access operator, non-const version.
     *  \param i : The index to access at.
     *  \return : A copy of the element to access.
     */
    int & operator[](const int i) { return raw_data_[i]; }

    /*! \brief Check if the buckets are identical.
     *  \return : True if identical.
     */
    inline
    bool identical(const TypeBucket & other) const;

    /*! \brief Check if the bucket has only one slot filled
               with one atom at slot i.
     *  \param i : The slot to check.
     *  \return  : True if slot i is filled with one atom and all other
                   slots are empty.
     */
    inline
    bool operator==(const int i) const;

    /*! \brief Check if the buckets are identical.
     *  \return  : True if identical, otherwise false.
     */
    bool operator==(const TypeBucket & other) const { return identical(other); }

    /*! \brief Check if the buckets are not identical.
     *  \return  : True if not identical, otherwise false.
     */
    bool operator!=(const TypeBucket & other) const { return !identical(other); }

    /*! \brief Check if this bucket is greater or equal to the other bucket.
     * \return : True if this is greater equal to the other, otherwise false.
     */
    inline
    bool greaterOrEqual(const TypeBucket & other) const;

    /*! \brief Check if this bucket is less than the other bucket.
     * \return : True if this is less than the other, otherwise false.
     */
    inline
    bool lessThan(const TypeBucket & other) const;

    /*! \brief Check if this bucket match the other. Meaning that each element
     *         in the other is greater than or equal to the corresponding
     *         element in this.
     * \return : True this (process) matchers the other (config).
     */
    inline
    bool match(const TypeBucket & other) const;

    /*! \brief Element wise addition.
     *  \param other : The other bucket to add.
     *  \return : The element wise addition of this and the other.
     */
    inline
    TypeBucket add(const TypeBucket & other) const;

protected:

private:

    /// The size of the bucket.
    int size_;

    /// The bucket data field.
    //std::vector<int> data_;

    /// The bucket raw data field.
    int * raw_data_;

};


// -----------------------------------------------------------------------------
// INLINE FUNCTOION IMPLEMENTATIONS FOLLOW
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//
bool TypeBucket::identical(const TypeBucket & other) const
{
    if (size_ != other.size_)
    {
        return false;
    }

    for (int i = 0; i < size_; ++i)
    {
        if (raw_data_[i] != other.raw_data_[i])
        {
            return false;
        }
    }

    return true;

}


// -----------------------------------------------------------------------------
//
bool TypeBucket::operator==(const int i) const
{
    if (size_ <= i)
    {
        throw std::runtime_error("Fatal backend error. Out of bound in bucket comparison.");
    }

    // ML: This is slow, but will only be used untill we are fully
    //     rid of the old style type implementation.

    for (int j = 0; j < size_; ++j)
    {
        if (i != j && raw_data_[j] != 0)
        {
            return false;
        }
        else if (i == j && raw_data_[j] != 1)
        {
            return false;
        }
    }

    return true;
}


// -----------------------------------------------------------------------------
//
bool TypeBucket::greaterOrEqual(const TypeBucket & other) const
{
    if (size_ != other.size_)
    {
        throw std::runtime_error("Fatal backend error. Size must match in bucket comparisons.");
    }

    for (int i = 0; i < size_; ++i)
    {
        if (raw_data_[i] < other.raw_data_[i])
        {
            return false;
        }
        else if (raw_data_[i] > other.raw_data_[i])
        {
            return true;
        }
    }

    return true;
}


// -----------------------------------------------------------------------------
//
bool TypeBucket::match(const TypeBucket & other) const
{
    if (size_ != other.size_)
    {
        throw std::runtime_error("Fatal backend error. Size must match in bucket comparisons.");
    }

    for (int i = 0; i < size_; ++i)
    {
        if (raw_data_[i] > other.raw_data_[i])
        {
            return false;
        }
    }

    return true;

}


// -----------------------------------------------------------------------------
//
TypeBucket TypeBucket::add(const TypeBucket & other) const
{
    if (size_ != other.size_)
    {
        throw std::runtime_error("Fatal backend error. Size must match in bucket additions.");
    }

    TypeBucket t(size_);

    for (int i = 0; i < size_; ++i)
    {
        t.raw_data_[i] = raw_data_[i] + other.raw_data_[i];
    }

    return t;
}


// -----------------------------------------------------------------------------
//
bool TypeBucket::lessThan(const TypeBucket & other) const
{
    if (size_ != other.size_)
    {
        throw std::runtime_error("Fatal backend error. Size must match in bucket comparisons.");
    }

    return !greaterOrEqual(other);
}


// -----------------------------------------------------------------------------
// NON-MEMBER FUNCTION DECLARATIONS FOLLOW.
// -----------------------------------------------------------------------------

/*! \brief Dummy << operator needed for CPPUNIT.
 * \return : A handle to the modified ostream.
 */
std::ostream & operator<<(std::ostream & os, const TypeBucket & tb);


#endif // __TYPE_BUCKET__



