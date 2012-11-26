/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  latticemap.cpp
 *  \brief File for the implementation code of the LatticeMap class.
 */


#include "latticemap.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>


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
                       const int rep_a,
                       const int rep_b,
                       const int rep_c,
                       const bool periodic_a,
                       const bool periodic_b,
                       const bool periodic_c) :
    n_basis_(n_basis),
    rep_a_(rep_a),
    rep_b_(rep_b),
    rep_c_(rep_c),
    periodic_a_(periodic_a),
    periodic_b_(periodic_b),
    periodic_c_(periodic_c)
{
    // Setup the maps.
    int index = 0;
    for (int i = 0; i < rep_a_; ++i)
    {
        for (int j = 0; j < rep_b_; ++j)
        {
            for (int k = 0; k < rep_c_; ++k)
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
        if (periodic_a_)
        {
            if (ii < 0)
            {
                ii += rep_a_;
            }
            else if (ii >= rep_a_)
            {
                ii -= rep_a_;
            }
        }
        // Go on only if i is within bounds.
        if (ii >= 0 && ii < rep_a_)
        {
            for (int j = cell.j - 1; j <= cell.j + 1; ++j)
            {
                int jj = j;
                // Handle periodicity.
                if (periodic_b_)
                {
                    if (jj < 0)
                    {
                        jj += rep_b_;
                    }
                    else if (jj >= rep_b_)
                    {
                        jj -= rep_b_;
                    }
                }
                // Go on only if j is within bounds.
                if (jj >= 0 && jj < rep_b_)
                {
                    for (int k = cell.k - 1; k <= cell.k + 1; ++k)
                    {
                        int kk = k;
                        // Check that k is within bounds.
                        if (periodic_c_)
                        {
                            if (kk < 0)
                            {
                                kk += rep_c_;
                            }
                            else if (kk >= rep_c_)
                            {
                                kk -= rep_c_;
                            }
                        }

                        // Go on only if k is within bounds.
                        if (0 <= kk && kk < rep_c_)
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


