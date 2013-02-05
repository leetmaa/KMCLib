/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  process.h
 *  \brief File for the Process class definition.
 */

#ifndef __PROCESS__
#define __PROCESS__

#include <vector>
#include <map>
#include <string>
#include "matchlistentry.h"

class Configuration;

/*! \brief Class for defining a possible process int the system.
 */
class Process {

public:

    /*! \brief Default constructor needed for use in std::vector SWIG wrapping.
     */
    Process() {}

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
    Process(const Configuration & first,
            const Configuration & second,
            const double rate,
            const std::vector<int> & basis_sites);

    /*! \brief Virtual destructor needed for use as base class.
     */
    virtual ~Process() {}

    // ML: dummy
    virtual void printMyType() const;

    /*! \brief Query for the total rate.
     *  \return : The total rate of the process.
     */
    virtual double totalRate() const { return rate_ * sites_.size(); }

    /*! \brief Add the index to the list of available sites.
     *  \param index : The index to add.
     *  \param rate  : Dummy argument needed for common interface.
     */
    virtual void addSite(const int index, const double rate=0.0);

    /*! \brief Remove the index from the list of available sites.
     *  \param index : The index to remove.
     */
    virtual void removeSite(const int index);

    /*! \brief Pick a random available process.
     *  \return : A random available process.
     */
    virtual int pickSite() const;

    /*! \brief Interface function for inherited classes.
     *         This function does nothing if not overloaded.
     */
    virtual void updateRateTable() {}

    /*! \brief Query for the rate constant associated with the process.
     *  \return : The rate constant part of the of rate for the process.
     */
    double rateConstant() const { return rate_; }

    /*! \brief Query for the number of listed possible sites for this process.
     *  \return : The number of listed indices.
     */
    size_t nSites() const {return sites_.size(); }

    /*! \brief Determine if an index is listed as available site for this process.
     *  \param index : The index to check.
     *  \return : True if match.
     */
    bool isListed(const int index) const;

    // ML: when is this used?
    /*! \brief Query for the available sites for this process.
     *  \return : The available sites.
     */
    const std::vector<int> & sites() const { return sites_; }

    /*! \brief Query for the configuration as a vector of match list entries.
     *  \return : The stored match list.
     */
    const std::vector<MinimalMatchListEntry> & minimalMatchList() const { return minimal_match_list_; }

    /*! \brief Query for the configuration as a vector of match list entries.
     *  \return : A reference to the stored match list.
     */
    std::vector<MinimalMatchListEntry> & minimalMatchList() { return minimal_match_list_; }

    /*! \brief Query for the latest affected indices.
     *  \return : The affected indices from the last time the process was
     *            performed on a calculation.
     */
    const std::vector<int> & affectedIndices() const { return affected_indices_; }

    /*! \brief Query for the latest affected indices.
     *  \return : The affected indices from the last time the process was
     *            performed on a calculation.
     */
    std::vector<int> & affectedIndices() { return affected_indices_; }

    /*! \brief Query for the basis sites.
     *  \return : The basis sites at which this process is applicable.
     */
    const std::vector<int> & basisSites() const { return basis_sites_; }

protected:

    /// The rate in Hz.
    double rate_;

    /// The available sites for this process.
    std::vector<int> sites_;

    /// The match list for comparing against local configurations.
    std::vector<MinimalMatchListEntry> minimal_match_list_;

    /*! \brief: The configuration indices that were affected last time
     *          the process was used to update a configuration.
     */
    std::vector<int> affected_indices_;

    /// The basis sites to which this process can be applied.
    std::vector<int> basis_sites_;

private:

};


#endif // __PROCESS__

