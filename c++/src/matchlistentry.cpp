/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  matchlistentry.cpp
 *  \brief File for the implementation code of the match list entries.
 */

#include "matchlistentry.h"


/*! \brief The epsilon value for comparing lattice positions.
 */
static double epsi__ = 1.0e-5;


// -----------------------------------------------------------------------------
//
bool samePoint(const MinimalMatchListEntry & m1,
               const MinimalMatchListEntry & m2)
{
    // Check the distance.
    if (std::fabs(m2.distance - m1.distance) > epsi__)
    {
        return false;
    }
    // Check the coordinate.
    else if (std::fabs(m2.x - m1.x) > epsi__)
    {
        return false;
    }
    else if (std::fabs(m2.y - m1.y) > epsi__)
    {
        return false;
    }
    else if (std::fabs(m2.z - m1.z) > epsi__)
    {
        return false;
    }
    return true;
}


// -----------------------------------------------------------------------------
//
bool samePoint(const ProcessBucketMatchListEntry & m1,
               const MinimalMatchListEntry & m2)
{
    // Check the distance.
    if (std::fabs(m2.distance - m1.distance) > epsi__)
    {
        return false;
    }
    // Check the coordinate.
    else if (std::fabs(m2.x - m1.coordinate.x()) > epsi__)
    {
        return false;
    }
    else if (std::fabs(m2.y - m1.coordinate.y()) > epsi__)
    {
        return false;
    }
    else if (std::fabs(m2.z - m1.coordinate.z()) > epsi__)
    {
        return false;
    }
    return true;
}


// -----------------------------------------------------------------------------
//
bool operator<(const MinimalMatchListEntry & m1,
               const MinimalMatchListEntry & m2)
{
    // Sort along distance, type and coordinate.
    if (std::fabs(m2.distance - m1.distance) < epsi__)
    {
        // If the distances are practically the same,
        // check the coordinate.
        if ( m1.x < m2.x)
        {
            return true;
        }
        else if (m1.x > m2.x)
        {
            return false;
            }
        else
        {
            if ( m1.y < m2.y )
            {
                return true;
            }
                else if ( m1.y > m2.y )
                {
                    return false;
                }
                else
                {
                    return ( m1.z < m2.z );
                }
            }
    }
    else
    {
            // Sort wrt. distance.
        return (m1.distance < m2.distance);
    }
}


// -----------------------------------------------------------------------------
//
bool operator<(const ProcessBucketMatchListEntry & m1,
               const MinimalMatchListEntry & m2)
{
    // Sort along distance, type and coordinate.
    if (std::fabs(m2.distance - m1.distance) < epsi__)
    {
        // If the distances are practically the same,
        // check the coordinate.
        if ( m1.coordinate.x() < m2.x)
        {
            return true;
        }
        else if (m1.coordinate.x() > m2.x)
        {
            return false;
            }
        else
        {
            if ( m1.coordinate.y() < m2.y )
            {
                return true;
            }
            else if ( m1.coordinate.y() > m2.y )
                {
                    return false;
                }
                else
                {
                    return ( m1.coordinate.z() < m2.z );
                }
            }
    }
    else
    {
            // Sort wrt. distance.
        return (m1.distance < m2.distance);
    }
}

// -----------------------------------------------------------------------------
//
bool operator<(const ProcessBucketMatchListEntry & m1,
               const ProcessBucketMatchListEntry & m2)
{
    // Sort along distance, type and coordinate.
    if (std::fabs(m2.distance - m1.distance) < epsi__)
    {
        // If the distances are practically the same,
        // check the coordinate.
        if ( m1.coordinate.x() < m2.coordinate.x())
        {
            return true;
        }
        else if (m1.coordinate.x() > m2.coordinate.x())
        {
            return false;
            }
        else
        {
            if ( m1.coordinate.y() < m2.coordinate.y() )
            {
                return true;
            }
            else if ( m1.coordinate.y() > m2.coordinate.y() )
                {
                    return false;
                }
                else
                {
                    return ( m1.coordinate.z() < m2.coordinate.z() );
                }
            }
    }
    else
    {
            // Sort wrt. distance.
        return (m1.distance < m2.distance);
    }
}
