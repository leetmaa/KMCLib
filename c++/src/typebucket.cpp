/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  typebucket.cpp
 *  \brief File for the implementation code of the TypeBucket class.
 */

#include <iostream>
#include <cstring>
#include "typebucket.h"


// -----------------------------------------------------------------------------
//
TypeBucket::TypeBucket() :
    size_(0),
    raw_data_(NULL)
{
    // NOTHING HERE
}


// -----------------------------------------------------------------------------
//
TypeBucket::TypeBucket(const int size) :
    size_(size)
{
    raw_data_ = (int*)malloc(sizeof(int)*size_);

    memset(raw_data_, 0U, sizeof(int)*size_);

}


// -----------------------------------------------------------------------------
//
TypeBucket::TypeBucket(const TypeBucket & other) :
    size_(other.size_)
{
    raw_data_ = (int*)malloc(sizeof(int)*size_);
    memcpy(raw_data_, other.raw_data_, sizeof(int)*size_);
}


// -----------------------------------------------------------------------------
//
void TypeBucket::operator=(const TypeBucket & other)
{
    size_ = other.size_;
    raw_data_ = (int*)realloc(raw_data_, sizeof(int)*size_);
    memcpy(raw_data_, other.raw_data_, sizeof(int)*size_);
}

// -----------------------------------------------------------------------------
//
TypeBucket::~TypeBucket()
{
    free(raw_data_);
}


// -----------------------------------------------------------------------------
// NON-MEMBER FUNCTION DEFINITIONS FOLLOW.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// NEEDED FOR CPPUNIT
std::ostream & operator<<(std::ostream & os, const TypeBucket & tb)
{
    os.operator<<("[");
    for (int i = 0; i < tb.size(); ++i)
    {
        os.operator<<(" ");
        os.operator<<(tb[i]);
        os.operator<<(" ");
    }
    os.operator<<("]");

  return os;
}

