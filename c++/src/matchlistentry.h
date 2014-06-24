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
    std::vector<int> match_types;

    /// The update types used in bucket mode.
    std::vector<int> update_types;

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

    // FIXME: Only for testing. Replace with special function.
    virtual
    bool operator!=(const MatchListEntry & m2) const
    {
        for (size_t i = 0; i < match_types.size(); ++i)
        {
            if (m2.match_types[i] != match_types[i])
            {
                return true;
            }
        }

        return !((*this) == m2);
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
        // ML: Rework this thing.

        // Handle the wildcard case.
        if (match_types[0] == 1)
        {
            return false;
        }

        // ML: Handle cases where we want an exact match.
        //     Or a match with an upper bound also.

        // Check the type matching.
        else
        {
            // ML: FIXME: PERFORMME
            for (size_t i = 0; i < match_types.size(); ++i)
            {
                if (m2.match_types[i] < match_types[i])
                {
                    return true;
                }
            }

            return !((*this) == m2);
        }
    }

    // FIXME: Dummy, needed temporarily, while we have two different classes.
    void initWildcard(const ConfigBucketMatchListEntry & c)
    {
        has_move_coordinate = c.has_move_coordinate;
        index = -1;
        distance = c.distance;
        coordinate = c.coordinate;
        match_types = std::vector<int>(c.match_types.size(), 0);
        match_types[0] = 1; // Indicating wildcard.
        update_types.resize(match_types.size());
    }

};


#endif // __MATCHLISTENTRY__

