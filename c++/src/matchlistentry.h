/*
  Copyright (c)  2012-2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  matchlistentry.h
 *  \brief File for the MatchListEntry class definition.
 */

#ifndef __MATCHLISTENTRY__
#define __MATCHLISTENTRY__

#include "coordinate.h"
#include "typebucket.h"
#include <cstdio>
#include <cmath>


/*! \brief The epsilon value for comparing lattice positions.
 */
static double epsi__ = 1.0e-5;


/// The base class for the match list entries.
class MatchListEntry {

public:

    /// Flag for indicating if we have a move coordinate.
    bool has_move_coordinate;

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

    /// The match types used in bucket mode.
    TypeBucket match_types;

    /// The update types used in bucket mode.
    TypeBucket update_types;

    /*! \brief 'equal' for comparing points. NOTE: This operator does not
     *         compare match types.
     */
    bool samePoint(const MatchListEntry & m2) const
    {
        // Check the distance.
        if (std::fabs(m2.distance - distance) > epsi__)
        {
            return false;
        }
        // Check the coordinate.
        else if (std::fabs(m2.coordinate.x() - coordinate.x()) > epsi__)
        {
            return false;
        }
        else if (std::fabs(m2.coordinate.y() - coordinate.y()) > epsi__)
        {
            return false;
        }
        else if (std::fabs(m2.coordinate.z() - coordinate.z()) > epsi__)
        {
            return false;
        }
        return true;
    }

    /*! \brief 'less than' for sorting matchlists.
     */
    bool operator<(const MatchListEntry & m2) const
    {
        // Sort along distance, type and coordinate.
        if (std::fabs(m2.distance - distance) < epsi__)
        {
            // If the distances are practically the same,
            // check the coordinate.
            return (coordinate < m2.coordinate);
        }
        else
        {
            // Sort wrt. distance.
            return (distance < m2.distance);
        }
    }

protected:

    /*! \brief Explicitly protected default constructor.
     */
    MatchListEntry() {};
};


/// Match list entry for the configuration bucket match list.
class ConfigBucketMatchListEntry : public MatchListEntry {

public:

};


/// Match list entry for the process bucket match list.
class ProcessBucketMatchListEntry : public MatchListEntry {

public:

    bool match(const MatchListEntry & m2) const
    {
        // Handle the wildcard case.
        if (match_types[0] == 1)
        {
            return true;
        }

        // Check the type matching.
        else
        {
            if (!match_types.match(m2.match_types))
            {
                return false;
            }
            return ((*this).samePoint(m2));
        }
    }

    // FIXME: Dummy, needed temporarily, while we have two different classes.
    void initWildcard(const ConfigBucketMatchListEntry & c)
    {
        has_move_coordinate = c.has_move_coordinate;
        index = -1;
        distance = c.distance;
        coordinate = c.coordinate;
        match_types = TypeBucket(c.match_types.size());
        match_types[0] = 1;           // Indicating wildcard.
        update_types = match_types;   // Indicating wildcard.
    }

};


#endif // __MATCHLISTENTRY__

