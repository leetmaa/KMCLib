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

/*! \brief Class for defining an entry in the match list, corresponding to
 *         a type a distance and coordinates.
 */
class MatchListEntry {

public:

    /*! \brief Constructor.
     *  \param index      : The index of the entry.
     *  \param type       : The type of the entry.
     *  \param distance   : The distance from center.
     *  \param coordinate : The coordinate for which the distance was calculated.
     */
    MatchListEntry(const int index,
                   const int type,
                   const double distance,
                   const Coordinate & coordinate);

    /*! \brief 'not equal' operator for comparing matchlist entries.
     */
    bool operator!=(const MatchListEntry & other) const;

    /*! \brief 'less than' for sorting matchlists.
     */
    bool operator<(const MatchListEntry & other) const;

protected:

private:

    /// The index.
    int index_;

    /// The type integer.
    int type_;

    /// The distance.
    double distance_;

    /// The coordinate.
    Coordinate coordinate_;

    /// The epsilon to determine if two distances are equal or not.
    double eps_;

};


#endif // __MATCHLISTENTRY__

