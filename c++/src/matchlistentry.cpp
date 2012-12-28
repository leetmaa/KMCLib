/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  matchlistentry.cpp
 *  \brief File for the implementation code of the MatchListEntry class.
 */

#include <cmath>
#include <cstdio>
#include "matchlistentry.h"
#include "coordinate.h"

//static double eps__ = 1.0e-5;

// -----------------------------------------------------------------------------
//
/*
bool operator!=(const MinimalMatchListEntry & m1,
                const MinimalMatchListEntry & m2)
{
    // Check the type.
    if (m2.match_type != m1.match_type)
    {
        return true;
    }
    // Check the distance.
    else if (std::fabs(m2.distance - m1.distance) > eps__)
    {
        return true;
    }
    // Check the coordinate.
    else if (std::fabs(m2.coordinate.x() - m1.coordinate.x()) > eps__)
    {
        return true;
    }
    else if (std::fabs(m2.coordinate.y() - m1.coordinate.y()) > eps__)
    {
        return true;
    }
    else if (std::fabs(m2.coordinate.z() - m1.coordinate.z()) > eps__)
    {
        return true;
    }
    return false;
}
*/

// -----------------------------------------------------------------------------
//
 /*
bool operator<(const MinimalMatchListEntry & m1,
               const MinimalMatchListEntry & m2)
{
    // Sort along distance, type and coordinate.
    if (std::fabs(m2.distance - m1.distance) < eps__)
    {
        // If the distances are practically the same,
        // check the types.
        if (m1.match_type == m2.match_type)
        {
            // If the types are identical, check the coordinate.
            return (m1.coordinate < m2.coordinate);
        }
        else
        {
            return (m1.match_type < m2.match_type);
        }
    }
    else
    {
        // Sort wrt. distance.
        return (m1.distance < m2.distance);
    }
}
 */
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//
MatchListEntry::MatchListEntry(const int match_type,
                               const int update_type,
                               const double distance,
                               const Coordinate & coordinate,
                               const int index) :
    match_type_(match_type),
    update_type_(update_type),
    index_(index),
    distance_(distance),
    coordinate_(coordinate),
    eps_(1.0e-5)
{
}


// -----------------------------------------------------------------------------
//
bool MatchListEntry::operator!=(const MatchListEntry & other) const
{
    // Check the type.
    if (other.match_type_ != match_type_)
    {
        return true;
    }
    // Check the distance.
    else if (std::fabs(other.distance_ - distance_) > eps_)
    {
        return true;
    }
    // Check the coordinate.
    else if (std::fabs(other.coordinate_.x() - coordinate_.x()) > eps_)
    {
        return true;
    }
    else if (std::fabs(other.coordinate_.y() - coordinate_.y()) > eps_)
    {
        return true;
    }
    else if (std::fabs(other.coordinate_.z() - coordinate_.z()) > eps_)
    {
        return true;
    }

    return false;
}


// -----------------------------------------------------------------------------
//
bool MatchListEntry::operator<(const MatchListEntry & other) const
{
    // Sort along distance, type and coordinate.
    if (std::fabs(other.distance_ - distance_) < eps_)
    {
        // If the distances are practically the same,
        // check the types.
        if (match_type_ == other.match_type_)
        {
            // If the types are identical, check the coordinate.
            return (coordinate_ < other.coordinate_);
        }
        else
        {
            return (match_type_ < other.match_type_);
        }
    }
    else
    {
        // Sort wrt. distance.
        return (distance_ < other.distance_);
    }
}

