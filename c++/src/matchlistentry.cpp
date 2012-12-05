/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  matchlistentry.cpp
 *  \brief File for the implementation code of the MatchListEntry class.
 */

#include <cmath>
#include "matchlistentry.h"
#include "coordinate.h"


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
    // NOTE: The eps should be set from knowledge of system size in cartesina
    //       coordinates.
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

    // Sort wrt. distance.
    return (distance_ < other.distance_);
}

