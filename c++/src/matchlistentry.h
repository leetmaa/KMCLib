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


// Forward declarations.
class ProcessBucketMatchListEntry;
class MinimalMatchListEntry;


/*! \brief 'equal' for comparing points. NOTE: This operator does not
 *         compare match types.
 */
//inline
bool samePoint(const MinimalMatchListEntry & m1,
               const MinimalMatchListEntry & m2);

/*! \brief 'equal' for comparing points. NOTE: This operator does not
 *         compare match types.
 */
//inline
bool samePoint(const ProcessBucketMatchListEntry & m1,
               const MinimalMatchListEntry & m2);


/*! \brief 'less than' for sorting matchlists.
 */
//inline
bool operator<(const MinimalMatchListEntry & m1,
               const MinimalMatchListEntry & m2);


bool operator<(const ProcessBucketMatchListEntry & m1,
               const MinimalMatchListEntry & m2);

bool operator<(const ProcessBucketMatchListEntry & m1,
               const ProcessBucketMatchListEntry & m2);


/// The base class for the match list entries.
class MinimalMatchListEntry {

public:
    /// The index in the global structure.
    int index;

    /// The distance.
    double distance;

    /// The coordinate.
    double x;
    double y;
    double z;

    /// The match types used in bucket mode.
    TypeBucket match_types;
};



/// Match list entry for the configuration bucket match list.
class ConfigBucketMatchListEntry : public MinimalMatchListEntry {

public:

};


/// Match list entry for the process bucket match list.
class ProcessBucketMatchListEntry {

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


    bool match(const MinimalMatchListEntry & m2) const
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
            return samePoint((*this), m2);
        }
    }

    /*! \brief Create a wildcard entry from a config bucket matchlist entry.
     *  \param c : The match list entry to use as template.
     */
    void initWildcard(const ConfigBucketMatchListEntry & c)
    {
        index = -1;
        distance = c.distance;
        coordinate = Coordinate(c.x, c.y, c.z);
        match_types  = TypeBucket(c.match_types.size());
        update_types = TypeBucket(c.match_types.size());
        match_types[0] = 1;           // Indicating wildcard.
    }
};



#endif // __MATCHLISTENTRY__

