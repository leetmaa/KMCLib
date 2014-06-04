/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  matchlist.h
 *  \brief File for the MatchList definition and utility functions.
 */

#ifndef __MATCHLIST__
#define __MATCHLIST__

#include <vector>

#include "matchlistentry.h"


// Forward declarations, if any.


/// Define the match lists.
typedef std::vector<MatchListEntry> MatchList;
typedef std::vector<MinimalMatchListEntry> MinimalMatchList;
typedef std::vector<ProcessBucketMatchListEntry> ProcessBucketMatchList;
typedef std::vector<ConfigBucketMatchListEntry> ConfigBucketMatchList;


/*! \brief Determines if matchlists m1 and m2 match.
 *  \param m1: The first match list to compare.
 *  \param m2: The second match list to compare.
 */
template <class T1, class T2>
bool whateverMatch(const T1 & m1, const T2 & m2);


// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
//
// TEMPLATE IMPLEMENTATION CODE FOLLOW
//
// -------------------------------------------------------------------------- //
//
template <class T1, class T2>
bool whateverMatch(const T1 & m1, const T2 & m2)
{
    // For the shortest one, (allways the first), loop and match.
    const size_t size = m1.size();

    if (m2.size() < size)
    {
        return false;
    }

    for (size_t i = 0; i < size; ++i)
    {
        if (!specificMatch(m1[i], m2[i]))
        {
            return false;
        }
    }

    return true;
}



#endif // __MATCHLIST__

