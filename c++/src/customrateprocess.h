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
               to the process are local configurations and no periodic boundaries
               will be taken into consideration.
     *  \param first         : The first configuration, to match against the local
                               coordinates around an active site.
     *  \param second        : The second configuration, to update the local
                               configuration with if the process is selected.
     *  \param rate          : The rate in Hz associated with the process.
     *  \param basis_sites   : The basis sites where this process is applicable.
     */
    CustomRateProcess(const Configuration & first,
                      const Configuration & second,
                      const double rate,
                      const std::vector<int> & basis_sites);

    /*! \brief Virtual destructor needed for inheritance.
     */
    virtual ~CustomRateProcess() {}

    // ML: dummy
    virtual void printMyType() const;

    /*! \brief Query for the total rate.
     *  \return : The total rate of the process.
     */
    virtual double totalRate() const;

    /*! \brief Add the index to the list of available sites.
     *  \param index : The index to add.
     *  \param rate  : Dummy argument needed for common interface.
     */
    virtual void addSite(const int index, const double rate);

    /*! \brief Remove the index from the list of available sites.
     *  \param index : The index to remove.
     */
    virtual void removeSite(const int index);

    /*! \brief Pick a random available process.
     *  \return : A random available process.
     */
    virtual int pickSite() const;

protected:

private:

    /// The list of individual site rates.
    std::vector<double> site_rates_;

};


#endif // __CUSTOMRATEPROCESS__

