/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  matchlistentry.h
 *  \brief File for the MatchListEntry class definition.
 */

#ifndef __MATCHLISTENTRY__
#define __MATCHLISTENTRY__

#include "coordinate.h"
#include <cmath>

/// A minimal struct to replace the MatchListEntry class.
struct MinimalMatchListEntry {

    /// The match type integer.
    int match_type;

    /// The update type integer.
    int update_type;

    /// The index in the global structure.
    int index;

    /// The distance.
    double distance;

    /// The coordinate.
    Coordinate coordinate;

};


/*! \brief 'not equal' for comparing entries.
 */
static double epsi__ = 1.0e-5;

inline
bool operator!=(const MinimalMatchListEntry & m1,
                const MinimalMatchListEntry & m2)
{
    // Handle the wildcard case.
    if (m1.match_type == 0)
    {
        return false;
    }
    // Check the type.
    else if (m2.match_type != m1.match_type && m1.match_type)
    {
        return true;
    }
    // Check the distance.
    else if (std::fabs(m2.distance - m1.distance) > epsi__)
    {
        return true;
    }
    // Check the coordinate.
    else if (std::fabs(m2.coordinate.x() - m1.coordinate.x()) > epsi__)
    {
        return true;
    }
    else if (std::fabs(m2.coordinate.y() - m1.coordinate.y()) > epsi__)
    {
        return true;
    }
    else if (std::fabs(m2.coordinate.z() - m1.coordinate.z()) > epsi__)
    {
        return true;
    }
    return false;
}


/*! \brief 'less than' for sorting matchlists.
 */
inline
bool operator<(const MinimalMatchListEntry & m1,
               const MinimalMatchListEntry & m2)
{

    // Sort along distance, type and coordinate.
    if (std::fabs(m2.distance - m1.distance) < epsi__)
    {
        // If the distances are practically the same,
        // check the coordinate.
        return (m1.coordinate < m2.coordinate);
    }
    else
    {
        // Sort wrt. distance.
        return (m1.distance < m2.distance);
    }
}

#endif // __MATCHLISTENTRY__

