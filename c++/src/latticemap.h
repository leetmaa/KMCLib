/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  latticemap.h
 *  \brief File for the LatticeMap class definition.
 */



#ifndef __LATTICEMAP__
#define __LATTICEMAP__

#include <vector>
#include <map>
#include "coordinate.h"

// Forward declarations.
class Configuration;
//class Coordinate;

/// Class for handling lattice indeces and neighbours.
class LatticeMap {

public:

    /*! \brief Constructor for the map.
     *  \param n_basis      : The number of basis points.
     *  \param repetitions  : The number of repetitions along the a, b and c axes.
     *  \param periodic     : Indicating periodicity along the a, b and c axes.
     */
    LatticeMap(const int n_basis,
               const std::vector<int> repetitions,
               const std::vector<bool> periodic);

    /*! \brief Get the neighbouring indices of a given index,
     *         including all indices in nearby cells.
     * \param index : The index to query for.
     * \return: The list of indices.
     */
    std::vector<int> neighbourIndices(const int index) const;

    /*! \brief Get the unique neighbouring indices of a set of given
     *         indices.
     * \param indices : The vector of indices to get the neighbours for.
     * \return: The list of indices.
     */
    std::vector<int> supersetNeighbourIndices(const std::vector<int> & indices) const;

    /*! \brief Get the indices from a given cell.
     * \param i : The cell index in the a direction.
     * \param j : The cell index in the b direction.
     * \param k : The cell index in the c direction.
     * \return: The list of indices.
     */
    const std::vector<int> & indicesFromCell(const int i,
                                             const int j,
                                             const int k) const;

    /*! \brief Get the cell indices given a global index.
     * \param index (int) : The index to the the cell for.
     * \param i (out): The cell index in the a direction.
     * \param j (out): The cell index in the b direction.
     * \param k (out): The cell index in the c direction.
     */
    void indexToCell(const int index,
                     int & cell_i,
                     int & cell_j,
                     int & cell_k) const;

    /*! \brief Query for the periodicity.
     * \return: The periodicity in the direction.
     */
    bool periodicA() const { return periodic_[0]; }

    /*! \brief Query for the periodicity.
     * \return: The periodicity in the direction.
     */
    bool periodicB() const { return periodic_[1]; }

    /*! \brief Query for the periodicity.
     * \return: The periodicity in the direction.
     */
    bool periodicC() const { return periodic_[2]; }

    /*! \brief Query for the repetition.
     * \return: The repetitions in the direction.
     */
    int repetitionsA() const { return repetitions_[0]; }

    /*! \brief Query for the repetition.
     * \return: The repetitions in the direction.
     */
    int repetitionsB() const { return repetitions_[1]; }

    /*! \brief Query for the repetition.
     * \return: The repetitions in the direction.
     */
    int repetitionsC() const { return repetitions_[2]; }

    /*! \brief Wrap the coordinate according to periodic boundaries.
     * \param c (in/out): The coordinate to wrap.
     */
    inline
    void wrap(Coordinate & c) const;

protected:

private:

    /// The number of basis points in the elemntary unitcell.
    int n_basis_;
    /// The number of repetitions along the a, b and c directions.
    std::vector<int> repetitions_;
    /// The periodicity in the a, b and c directions.
    std::vector<bool> periodic_;
};

// -----------------------------------------------------------------------------
// INLINE FUNCTION DEFINITIONS FOLLOW

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


#endif // __LATTICEMAP__

