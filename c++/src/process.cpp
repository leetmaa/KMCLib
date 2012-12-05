/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  process.cpp
 *  \brief File for the implementation code of the Process class.
 */

#include <algorithm>

#include "process.h"

#include "configuration.h"

// -----------------------------------------------------------------------------
//
Process::Process(const Configuration & first,
                 const Configuration & second,
                 const double barrier,
                 const std::map<std::string,int> & possible_types) :
    sites_(0)
{
    // The check that the first and second configurartions match well enough
    // should be carried out in python so there is no need to do it again here.

    // Get a handle to the coordinates and elements.
    const std::vector<std::vector<double> > & coords  = first.coordinates();

    // Get the first coordinate out to calculate the distance against.
    const Coordinate origin(coords[0][0], coords[0][1], coords[0][2]);

    // Transform the configurations into match lists.
    for (size_t i = 0; i < first.elements().size(); ++i)
    {
        // Get the types as integers.
        const std::string & first_key  = first.elements()[i];
        const std::string & second_key = second.elements()[i];
        const int first_type  = possible_types.find(first_key)->second;
        const int second_type = possible_types.find(second_key)->second;

        // Calculate the distance.
        const Coordinate coordinate(coords[i][0], coords[i][1], coords[i][2]);
        const double distance = coordinate.distance(origin);

        // Setup and add the match list entry.
        const MatchListEntry entry(first_type, second_type, distance, coordinate);
        match_list_.push_back(entry);
    }

    // Sort the match list.
    std::sort(match_list_.begin(), match_list_.end());
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
    // Swap the index with the last and remove.
    std::vector<int>::iterator it1 = std::find(sites_.begin(),
                                               sites_.end(),
                                               index);
    std::vector<int>::iterator last = sites_.end()-1;
    std::swap((*it1), (*last));
    sites_.pop_back();
}



