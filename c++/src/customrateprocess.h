/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  customrateprocess.h
 *  \brief File for the CustomRateProcess class definition.
 */

#ifndef __CUSTOMRATEPROCESS__
#define __CUSTOMRATEPROCESS__

#include "process.h"

/*! \brief Class for defining a possible process int the system.
 */
class CustomRateProcess : public Process {

public:

    /*! \brief Default constructor needed for use in std::vector SWIG wrapping.
     */
    CustomRateProcess() {}


    /*! \brief Constructor for the process. Note that the configurations given
     *         to the process are local configurations and no periodic boundaries
     *         will be taken into consideration.
     *  \param first         : The first configuration, to match against the local
     *                         coordinates around an active site.
     *  \param second        : The second configuration, to update the local
     *                         configuration with if the process is selected.
     *  \param rate          : The rate in Hz associated with the process.
     *  \param basis_sites   : The basis sites where this process is applicable.
     *  \param cutoff        : The cutoff distance in primitive cell fractional units.
     *  \param move_origins  : The vector of indices in the local configurations
     *                         that the move vectors originate from. This vector
     *                         can be empty if no move vectors are used.
     *  \param move_vectors  : The vector of coordinates for each moved atom.
     *                         This vector can be empty if no move vectors are used,
     *                         i.e., if only elements are moved on the lattice and no
     *                         atom id moves are considered.
     *  \param process_number: The id number of the process.
     */
    CustomRateProcess(const Configuration & first,
                      const Configuration & second,
                      const double rate,
                      const std::vector<int> & basis_sites,
                      const double cutoff,
                      const std::vector<int> & move_origins=std::vector<int>(0),
                      const std::vector<Coordinate> & move_vectors=std::vector<Coordinate>(0),
                      const int process_number=-1,
                      const bool cache_rate=false);

    /*! \brief Virtual destructor needed for inheritance.
     */
    virtual ~CustomRateProcess() {}

    /*! \brief Add the index to the list of available sites.
     *  \param index        : The index to add.
     *  \param rate         : The rate of the process at the added site.
     *  \param multiplicity : The multiplicity of the site, to be multiplied
                              with the rate for determining the total rate.
     */
    virtual void addSite(const int index,
                         const double rate,
                         const double multiplicity);

    /*! \brief Remove the index from the list of available sites.
     *  \param index : The index to remove.
     */
    virtual void removeSite(const int index);


    /*! \brief Update the rate table prior to drawing a rate.
     */
    virtual void updateRateTable();

protected:

private:


};


#endif // __CUSTOMRATEPROCESS__

