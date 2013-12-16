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

// A minimal struct for representing three integers as a cell index.
struct CellIndex {

    // The index in the a direction.
    int i;
    // The index in the b direction.
    int j;
    // The index in the c direction.
    int k;
};


// Temporary storage for the indices form cell.
static std::vector<int> tmp_cell_indices__;


// -----------------------------------------------------------------------------
//
LatticeMap::LatticeMap(const int n_basis,
                       const std::vector<int> repetitions,
                       const std::vector<bool> periodic) :
    n_basis_(n_basis),
    repetitions_(repetitions),
    periodic_(periodic)
{
    // Resize the global data.
    tmp_cell_indices__.resize(n_basis_);
}


// -----------------------------------------------------------------------------
//
std::vector<int> LatticeMap::neighbourIndices(const int index,
                                              const int shells) const
{
    // PERFORMME

    // Get the cell index.
    CellIndex c;
    indexToCell(index, c.i, c.j, c.k);

    const CellIndex & cell = c;

    // Setup the return data structure.
    const int n_neighbours = static_cast<int>(std::pow((2.0*shells + 1), 3) * n_basis_);
    std::vector<int> neighbours(n_neighbours);

    // Get a pointer to the neighbours data for direct write access.
    int* neighbours_ptr = &neighbours[0];

    // A counter to know how much we have added.
    int counter = 0;

    for (int i = cell.i - shells; i <= cell.i + shells; ++i)
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
            for (int j = cell.j - shells; j <= cell.j + shells; ++j)
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
                    for (int k = cell.k - shells; k <= cell.k + shells; ++k)
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
std::vector<int> LatticeMap::supersetNeighbourIndices(const std::vector<int> & indices,
                                                      const int shells) const
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
        const std::vector<int> neighbours = neighbourIndices(index, shells);

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
    // Get the indices that are in cell i,j,k.
    const int tmp1 = i * repetitions_[1] + j;
    const int tmp2 = tmp1 * repetitions_[2] + k;
    const int tmp3 = tmp2 * n_basis_;

    for (int l = 0; l < n_basis_; ++l)
    {
        tmp_cell_indices__[l] = tmp3 + l;
    }

    return tmp_cell_indices__;
}


// -----------------------------------------------------------------------------
//
int LatticeMap::indexFromMoveInfo(const int index,
                                  const int i,
                                  const int j,
                                  const int k,
                                  const int basis) const
{
    // PERFORMME

    // Find out which cell the index is in.
    int cell_i, cell_j, cell_k;
    indexToCell(index, cell_i, cell_j, cell_k);

    // Get the new cell indices.
    cell_i += i;
    cell_j += j;
    cell_k += k;

    // Wrap the cell indices.
    if (periodic_[0])
    {
        if (cell_i < 0)
        {
            cell_i += repetitions_[0];
        }
        else if (cell_i >= repetitions_[0])
        {
            cell_i -= repetitions_[0];
        }
    }

    if (periodic_[1])
    {
        if (cell_j < 0)
        {
            cell_j += repetitions_[1];
        }
        else if (cell_j >= repetitions_[1])
        {
            cell_j -= repetitions_[1];
        }
    }

    if (periodic_[2])
    {
        if (cell_k < 0)
        {
            cell_k += repetitions_[2];
        }
        else if (cell_k >= repetitions_[2])
        {
            cell_k -= repetitions_[2];
        }
    }

    // Get the indices in the wrapped cell and return the
    // index at the given relative basis position.
    const int basis_index = basis + basisSiteFromIndex(index);
    return indicesFromCell(cell_i, cell_j, cell_k)[basis_index];
}


// -----------------------------------------------------------------------------
//
void LatticeMap::indexToCell(const int index,
                             int & cell_i,
                             int & cell_j,
                             int & cell_k) const
{
    // Given an index, calculate the cell i,j,k.
    const double eps   = 1.0e-9;
    const int idx      = index / n_basis_ + 1;
    const int factor_i = repetitions_[1] * repetitions_[2];
    const int ii       = static_cast<int>((idx - eps) / factor_i);

    const int ci       = ii * factor_i;
    const int factor_j = repetitions_[2];
    const int idx_j    = idx - ci;
    const int jj       = static_cast<int>((idx_j - eps) / factor_j);

    const int cij      = ci + jj * factor_j;
    const int idx_k    = idx - cij;
    const int kk       = static_cast<int>(idx_k - eps);

    // Set the result.
    cell_i = ii;
    cell_j = jj;
    cell_k = kk;

    // DONE
}

