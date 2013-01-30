/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  process.cpp
 *  \brief File for the implementation code of the Process class.
 */

#include <algorithm>
#include <cstdio>

#include "process.h"
#include "random.h"
#include "configuration.h"

// -----------------------------------------------------------------------------
//
Process::Process(const Configuration & first,
                 const Configuration & second,
                 const double rate,
                 const std::vector<int> & basis_sites) :
    rate_(rate),
    sites_(0),
    affected_indices_(0),
    basis_sites_(basis_sites)
{
    // The check that the first and second configurartions match well enough
    // should be carried out in python so there is no need to do it again here.

    // Get a handle to the coordinates and elements.
    const std::vector<Coordinate> & coords  = first.coordinates();

    // Get the first coordinate out to calculate the distance against.
    const Coordinate origin = coords[0];

    // Transform the configurations into match lists.
    for (size_t i = 0; i < first.elements().size(); ++i)
    {
        // Get the types as integers.
        const int first_type  = first.types()[i];
        const int second_type = second.types()[i];

        // Calculate the distance.
        const Coordinate coordinate = coords[i];
        const double distance = coordinate.distance(origin);

        // Set up the match list.
        MinimalMatchListEntry m;
        m.match_type  = first_type;
        m.update_type = second_type;
        m.distance    = distance;
        m.coordinate  = coordinate;
        m.index       = -1;
        minimal_match_list_.push_back(m);

        // If the first and second type differ increase the length of the
        // affected_sites list accordingly.
        if (first_type != second_type)
        {
            affected_indices_.push_back(0);
        }
    }

    // Sort the match list.
    std::sort(minimal_match_list_.begin(), minimal_match_list_.end());
}

// -----------------------------------------------------------------------------
//
bool Process::isListed(const int index) const
{
    // Search in the list to find out if it is added.
    return std::find(sites_.begin(), sites_.end(), index) != sites_.end();
}

// -----------------------------------------------------------------------------
//
void Process::addSite(const int index)
{
    sites_.push_back(index);
}

// -----------------------------------------------------------------------------
//
void Process::removeSite(const int index)
{
    // Find the index to remove.
    std::vector<int>::iterator it1 = std::find(sites_.begin(),
                                               sites_.end(),
                                               index);
    // Swap the index to remove with the last index.
    std::vector<int>::iterator last = sites_.end()-1;
    std::swap((*it1), (*last));
    // Remove the last index from the list.
    sites_.pop_back();
}


// -----------------------------------------------------------------------------
//
int Process::pickSite() const
{
    // Draw an integer between 0 and sites_.size() - 1
    const int rnd = static_cast<int>(randomDouble01() * sites_.size());
    return sites_[rnd];
}


