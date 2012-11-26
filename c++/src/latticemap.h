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
     *  \param n_basis    : The number of basis points.
     *  \param rep_a      : The number of repetitions along the a axis.
     *  \param rep_b      : The number of repetitions along the b axis.
     *  \param rep_c      : The number of repetitions along the c axis.
     *  \param periodic_a : Indicating periodicity along the a axis.
     *  \param periodic_b : Indicating periodicity along the b axis.
     *  \param periodic_c : Indicating periodicity along the c axis.
     */
    LatticeMap(const int n_basis,
               const int rep_a,
               const int rep_b,
               const int rep_c,
               const bool periodic_a,
               const bool periodic_b,
               const bool periodic_c);

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

protected:

private:

    /// The number of basis points in the elemntary unitcell.
    int n_basis_;
    /// The number of repetitions along the a direction.
    int rep_a_;
    /// The number of repetitions along the b direction.
    int rep_b_;
    /// The number of repetitions along the c direction.
    int rep_c_;
    /// The periodicity in the a direction.
    bool periodic_a_;
    /// The periodicity in the b direction.
    bool periodic_b_;
    /// The periodicity in the c direction.
    bool periodic_c_;

    /// The mapping from cell to a list of lattice indices.
    std::map<CellIndex,std::vector<int> > cell_to_index_;

    /// The mapping from index to cell.
    std::map<int,CellIndex> index_to_cell_;

};


#endif // __LATTICEMAP__

