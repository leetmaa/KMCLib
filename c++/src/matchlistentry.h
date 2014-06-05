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
#include <cmath>


/*! \brief The epsilon value for comparing lattice positions.
 */
static double epsi__ = 1.0e-5;


/// The base class for the match list entries.
class MatchListEntry {

public:

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

    /// The match types used in bucket mode.
    std::vector<int> match_types;

    /// The minimum number of particles of matching type for bucket match.
    int match_min_quantity;

    /*! \brief 'equal' for comparing points. NOTE: This operator does not
     *         compare match types.
     */
    bool operator==(const MatchListEntry & m2) const
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

    /*! \brief Pure virtual 'not equal' operator.
     */
    virtual
    bool operator!=(const MatchListEntry & m2) const=0;


protected:

    /*! \brief Explicitly protected default constructor.
     */
    MatchListEntry() {};
};


/// Match list entry for the configuration bucket match list.
class ConfigBucketMatchListEntry : public MatchListEntry {

public:

    virtual
    bool operator!=(const MatchListEntry & m2) const
    { // Throw an error.
        exit(0);
    }
};


/// Match list entry for the process bucket match list.
class ProcessBucketMatchListEntry : public MatchListEntry {

public:

    /*! \brief 'not equal' for comparing entries.
     */
    virtual
    bool operator!=(const MatchListEntry & m2) const
    {
        // Handle the wildcard case.
        if (match_type == -1)
        {
            return false;
        }

        // Check the quantity at the specific match type.
        else if (m2.match_types[match_type] < match_min_quantity)
        {
            return true;
        }
        else
        {
            return !((*this) == m2);
        }
    }
};


/// A minimal match list entry class.
class MinimalMatchListEntry : public MatchListEntry {

public:

    /*! \brief 'not equal' for comparing entries.
     */
    virtual
    bool operator!=(const MatchListEntry & m2) const
    {
        // Handle the wildcard case.
        if (match_type == 0)
        {
            return false;
        }
        // Check the type.
        else if (m2.match_type != match_type)
        {
            return true;
        }
        else
        {
            return !((*this) == m2);
        }
    }
};


#endif // __MATCHLISTENTRY__

