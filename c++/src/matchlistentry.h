/*
  Copyright (c)  2012-2013  Mikael Leetmaa

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

    /// Flag for indicating if we have a move coordinate.
    bool has_move_coordinate;

    /// The match type integer.
    int match_type;

    /// The update type integer.
    int update_type;

    /// The index in the global structure.
    int index;

    /// The move vector cell component in the a direction.
    int move_cell_i;

    /// The move vector cell component in the b direction.
    int move_cell_j;

    /// The move vector cell component in the c direction.
    int move_cell_k;

    /// The basis component of the move vector.
    int move_basis;

    /// The distance.
    double distance;

    /// The coordinate.
    Coordinate coordinate;

    /// The move coordinate.
    Coordinate move_coordinate;

};


/*! \brief The epsilon value for comparing lattice positions.
 */
static double epsi__ = 1.0e-5;


/*! \brief 'equal' for comparing points. NOTE: This operator does not
 *         compare match types.
 */
inline
bool operator==(const MinimalMatchListEntry & m1,
                const MinimalMatchListEntry & m2)
{
    // Check the distance.
    if (std::fabs(m2.distance - m1.distance) > epsi__)
    {
        return false;
    }
    // Check the coordinate.
    else if (std::fabs(m2.coordinate.x() - m1.coordinate.x()) > epsi__)
    {
        return false;
    }
    else if (std::fabs(m2.coordinate.y() - m1.coordinate.y()) > epsi__)
    {
        return false;
    }
    else if (std::fabs(m2.coordinate.z() - m1.coordinate.z()) > epsi__)
    {
        return false;
    }
    return true;
}



/*! \brief 'not equal' for comparing entries.
 */
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
    else if (m2.match_type != m1.match_type)
    {
        return true;
    }
    else
    {
        return !(m1 == m2);
    }
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

