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


// Forward declarations.
class Configuration;
class Coordinate;

/// A minimal struct for representing three integers as a cell index.
struct CellIndex {

    /// The index in the a direction.
    int i;
    /// The index in the b direction.
    int j;
    /// The index in the c direction.
    int k;
};

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
     *         including all indices in nearby cells. The number
     *         of cell shells to include depends on the map_size
     *         parameter given at construction.
     * \param index : The index to query for.
     * \returns: The list of indices.
     */
    std::vector<int> neighbourIndices(const int index) const;

    /*! \brief Get the indices from a given cell.
     * \param i : The cell index in the a direction.
     * \param j : The cell index in the b direction.
     * \param k : The cell index in the c direction.
     * \returns: The list of indices.
     */
    const std::vector<int> & indicesFromCell(const int i,
                                             const int j,
                                             const int k) const;


    /*! \brief Query for the periodicity.
     * \returns: The periodicity in the direction.
     */
    const bool periodicA() const { return periodic_[0]; }

    /*! \brief Query for the periodicity.
     * \returns: The periodicity in the direction.
     */
    const bool periodicB() const { return periodic_[1]; }

    /*! \brief Query for the periodicity.
     * \returns: The periodicity in the direction.
     */
    const bool periodicC() const { return periodic_[2]; }

    /*! \brief Wrap the coordinate according to periodic boundaries.
     * \param c (in/out): The coordinate to wrap.
     */
    void wrap(Coordinate & c) const;

protected:

private:

    /// The number of basis points in the elemntary unitcell.
    int n_basis_;
    /// The number of repetitions along the a, b and c directions.
    std::vector<int> repetitions_;
    /// The periodicity in the a, b and c directions.
    std::vector<bool> periodic_;
    /// The mapping from cell to a list of lattice indices.
    std::map<CellIndex,std::vector<int> > cell_to_index_;
    /// The mapping from index to cell.
    std::map<int,CellIndex> index_to_cell_;

};


#endif // __LATTICEMAP__

