/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  latticemap.cpp
 *  \brief File for the implementation code of the LatticeMap class.
 */


#include "latticemap.h"
#include "coordinate.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>

// -----------------------------------------------------------------------------
/*! \brief 'Less than' operator for comparing two cell index structs needed for
 *         the use in std::map.
 *  \param index1 : The first cell index.
 *  \param index2 : The second cell index.
 *  \return (index1 < index2)
 */
bool operator<(const CellIndex & index1, const CellIndex & index2)
{
    // If the first index is equal, compare the second, etc.
    if (index1.i == index2.i)
    {
        if (index1.j == index2.j)
        {
            return (index1.k < index2.k);
        }
        else
        {
            return (index1.j < index2.j);
        }
    }
    else
    {
        return (index1.i < index2.i);
    }
}


// -----------------------------------------------------------------------------
//
LatticeMap::LatticeMap(const int n_basis,
                       const std::vector<int> repetitions,
                       const std::vector<bool> periodic) :
    n_basis_(n_basis),
    repetitions_(repetitions),
    periodic_(periodic)
{
    // Setup the maps.
    int index = 0;
    for (int i = 0; i < repetitions_[0]; ++i)
    {
        for (int j = 0; j < repetitions_[1]; ++j)
        {
            for (int k = 0; k < repetitions_[2]; ++k)
            {
                for (int b = 0; b < n_basis_; ++b)
                {
                    // Map the cell to the index.
                    CellIndex cell;
                    cell.i = i;
                    cell.j = j;
                    cell.k = k;
                    cell_to_index_[cell].push_back(index);

                    // Map the index to the cell.
                    index_to_cell_[index] = cell;

                    // Increment the index.
                    index++;
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
//
std::vector<int> LatticeMap::neighbourIndices(const int index) const
{
    // Get the cell index.
    const CellIndex & cell = index_to_cell_.find(index)->second;

    // Setup the return data structure.
    std::vector<int> neighbours(n_basis_*27);

    // Get a pointer to the neighbours data for direct write access.
    int* neighbours_ptr = &neighbours[0];

    // A counter to know how much we have added.
    int counter = 0;

    for (int i = cell.i - 1; i <= cell.i + 1; ++i)
    {
        int ii = i;
        // Handle periodicity.
        if (periodic_[0])
        {
            if (ii < 0)
            {
                ii += repetitions_[0];
            }
            else if (ii >= repetitions_[0])
            {
                ii -= repetitions_[0];
            }
        }
        // Go on only if i is within bounds.
        if (ii >= 0 && ii < repetitions_[0])
        {
            for (int j = cell.j - 1; j <= cell.j + 1; ++j)
            {
                int jj = j;
                // Handle periodicity.
                if (periodic_[1])
                {
                    if (jj < 0)
                    {
                        jj += repetitions_[1];
                    }
                    else if (jj >= repetitions_[1])
                    {
                        jj -= repetitions_[1];
                    }
                }
                // Go on only if j is within bounds.
                if (jj >= 0 && jj < repetitions_[1])
                {
                    for (int k = cell.k - 1; k <= cell.k + 1; ++k)
                    {
                        int kk = k;
                        // Check that k is within bounds.
                        if (periodic_[2])
                        {
                            if (kk < 0)
                            {
                                kk += repetitions_[2];
                            }
                            else if (kk >= repetitions_[2])
                            {
                                kk -= repetitions_[2];
                            }
                        }

                        // Go on only if k is within bounds.
                        if (0 <= kk && kk < repetitions_[2])
                        {
                            // Take a reference to the mapped data.
                            const std::vector<int> & indices = indicesFromCell(ii,jj,kk);
                            // Copy data over from the neighbour cell.
                            size_t size = n_basis_ * sizeof(int);
                            std::memcpy(neighbours_ptr, &indices[0], size);

                            // Increment the pointer.
                            neighbours_ptr += n_basis_;

                            // Increment the counter.
                            counter += n_basis_;
                        }
                    }
                }
            }
        }
    }

    // Resize and return.
    neighbours.resize(counter);
    return neighbours;
}


// -----------------------------------------------------------------------------
//
std::vector<int> LatticeMap::supersetNeighbourIndices(const std::vector<int> & indices)
{
    // PERFORMME:
    // We can use several different stategies here and this might
    // be performance critical, thus we need to time it. So, for now,
    // use the simplest possible naive implementation.

    std::vector<int> superset;

    for (size_t i = 0; i < indices.size(); ++i)
    {
        // Get the index.
        const int index = indices[i];

        // And its neighbours.
        const std::vector<int> neighbours = neighbourIndices(index);

        // Add its neighbourlist to the superset.
        for (size_t j = 0; j < neighbours.size(); ++j)
        {
            superset.push_back(neighbours[j]);
        }
    }

    // Sort the superset.
    std::sort(superset.begin(), superset.end());

    // Get the unique elements out.
    superset.resize(std::unique(superset.begin(), superset.end())-superset.begin());
    return superset;
}


// -----------------------------------------------------------------------------
//
const std::vector<int> & LatticeMap::indicesFromCell(const int i,
                                                     const int j,
                                                     const int k) const
{
    // Take care of periodic boundary conditions here.
    CellIndex cell;
    cell.i = i;
    cell.j = j;
    cell.k = k;

    // Get the list out and return.
    return cell_to_index_.find(cell)->second;
}


// -----------------------------------------------------------------------------
//
void LatticeMap::wrap(Coordinate & c) const
{
    // Loop over directions.
    for (int i = 0; i < 3; ++i)
    {
        // Wrap if periodic.
        if (periodic_[i])
        {
            const double half_cell = 1.0 * repetitions_[i] / 2.0;
            if (c[i] >= half_cell)
            {
                c[i] -= repetitions_[i];
            }
            else if (c[i] < -half_cell)
            {
                c[i] += repetitions_[i];
            }
        }
    }
}


