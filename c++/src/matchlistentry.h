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

    /*! \brief Default constructor needed for SWIG wrapping in std::vector.
     */
    MatchListEntry() {}

    /*! \brief Constructor.
     *  \param match_type : The matching type of the entrty.
     *  \param update_type: The update type of the entrty.
     *  \param distance   : The distance from center.
     *  \param coordinate : The coordinate for which the distance was calculated.
     *  \param index      : The index of the entry, defaults to -1.
     */
    MatchListEntry(const int match_type_,
                   const int update_type_,
                   const double distance,
                   const Coordinate & coordinate,
                   const int index=-1);

    /*! \brief 'not equal' operator for comparing matchlist entries.
     */
    bool operator!=(const MatchListEntry & other) const;

    /*! \brief 'less than' for sorting matchlists.
     */
    bool operator<(const MatchListEntry & other) const;

    /*! \brief Query for the match type.
     */
    const int & matchType() const { return match_type_; }

    /*! \brief Query for the update type.
     */
    const int & updateType() const { return update_type_; }

    /*! \brief Query for the index.
     */
    const int & index() const { return index_; }

    /*! \brief Query for the distance.
     */
    const double & distance() const { return distance_; }

    /*! \brief Query for the coordinate.
     */
    const Coordinate & coordinate() const { return coordinate_; }

protected:

private:

    /// The match type integer.
    int match_type_;

    /// The update type integer.
    int update_type_;

    /// The index in the global structure.
    int index_;

    /// The distance.
    double distance_;

    /// The coordinate.
    Coordinate coordinate_;

    /*! \brief The epsilon to determine if two distances are equal or not.
     *         It is in units of primitive cell fractional coordinates.
     */
    double eps_;

};


#endif // __MATCHLISTENTRY__

