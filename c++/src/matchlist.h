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
class Configuration;


/// Define the match lists.
typedef std::vector<ProcessBucketMatchListEntry> ProcessBucketMatchList;
typedef std::vector<ConfigBucketMatchListEntry> ConfigBucketMatchList;


/*! \brief Set up a process matchlist from two local configurations.
 *  \param first            : (in) The first (before) configuration.
 *  \param second           : (in) The second (after) configuration.
 *  \param range            : (in/out) The rang of the process.
 *  \param cutoff           : (in/out) The cutoff of the process.
 *  \param match_list       : (in/out) The matchlist to construct.
 *  \param affected_indices : (in/out) The affected indices to set the size of.
 */
void configurationsToMatchList(const Configuration & first,
                               const Configuration & second,
                               int & range,
                               double & cutoff,
                               ProcessBucketMatchList & match_list,
                               std::vector<int> & affected_indices);


/*! \brief Calculate the multiplicity of a set of matching matchlists.
 *  \param process_match_list : The process matchlist to compare.
 *  \param config_match_list  : The configuration matchlist to compare.
 *  \return : The multiplicity of the match.
 */
double multiplicity(const ProcessBucketMatchList & process_match_list,
                    const ConfigBucketMatchList & config_match_list);


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
        if (!m1[i].match(m2[i]))
        {
            return false;
        }
    }

    return true;
}



#endif // __MATCHLIST__

