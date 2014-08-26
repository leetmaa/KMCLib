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
#include "matchlist.h"

class Configuration;

/*! \brief Class for defining a possible process int the system.
 */
class Process {

public:

    /*! \brief Default constructor needed for use in std::vector SWIG wrapping.
     */
    Process() {}

    /*! \brief Constructor for the process. Note that the configurations given
     *         to the process are local configurations and no periodic boundaries
     *         will be taken into consideration.
     *  \param first         : The first configuration, to match against the local
     *                         coordinates around an active site.
     *  \param second        : The second configuration, to update the local
     *                         configuration with if the process is selected.
     *                         It is assumed that the first and second configuration
     *                         has identical coordinates.
     *  \param rate          : The rate in Hz associated with the process.
     *  \param basis_sites   : The basis sites where this process is applicable.
     *  \param move_origins  : The vector of indices in the local configurations
     *                         that the move vectors originate from. This vector
     *                         can be empty if no move vectors are used.
     *  \param move_vectors  : The vector of coordinates for each moved atom.
     *                         This vector can be empty if no move vectors are used,
     *                         i.e., if only elements are moved on the lattice and no
     *                         atom id moves are considered.
     *  \param process_number: The id number of the process.
     */
    Process(const Configuration & first,
            const Configuration & second,
            const double rate,
            const std::vector<int> & basis_sites,
            const std::vector<int> & move_origins=std::vector<int>(0),
            const std::vector<Coordinate> & move_vectors=std::vector<Coordinate>(0),
            const int process_number=-1);

    /*! \brief Virtual destructor needed for use as base class.
     */
    virtual ~Process() {}

    /*! \brief Query for the total rate.
     *  \return : The total rate of the process.
     */
    double totalRate() const { return total_rate_; }

    /*! \brief Add the index to the list of available sites.
     *  \param index        : The index to add.
     *  \param rate         : Dummy argument needed for common interface.
     *  \param multiplicity : The multiplicity of the site, to be multiplied
                              with the rate for determining the total rate.
     */
    virtual void addSite(const int index,
                         const double rate=0.0,
                         const double multiplicity=1.0);

    /*! \brief Remove the index from the list of available sites.
     *  \param index : The index to remove.
     */
    virtual void removeSite(const int index);

    /*! \brief Remove all indices from the list of available sites.
     */
    virtual void clearSites();

    /*! \brief Pick a site weighted by its individual total rate (multiplicity).
     *  \return : An available process.
     */
    virtual int pickSite() const;

    /*! \brief Update the rate table prior to drawing a rate.
     */
    virtual void updateRateTable();

    /*! \brief Returns true if the process rates can be cached.
     *  \return : True if rates can be cached. False if no caching is allowed.
     */
    bool cacheRate() const { return cache_rate_; }

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

    /*! \brief Query for the available sites for this process.
     *         Convenient when testing other functionality of the class.
     *  \return : The available sites.
     */
    const std::vector<int> & sites() const { return sites_; }

    /*! \brief Query for the configuration as a vector of match list entries.
     *  \return : The stored match list.
     */
    const ProcessBucketMatchList & processMatchList() const { return match_list_; }

    /*! \brief Query for the configuration as a vector of match list entries.
     *  \return : A reference to the stored match list.
     */
    ProcessBucketMatchList & processMatchList() { return match_list_; }

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

    /*! \brief Query for the atom id moves.
     *  \return : The pairs of match list indices corresponding to atom id moves.
     */
    const std::vector< std::pair<int,int> > & idMoves() const { return id_moves_; }

    /*! \brief Non-const query for the atom id moves.
     *  \return : The pairs of match list indices corresponding to atom id moves.
     */
    std::vector< std::pair<int,int> > & idMoves() { return id_moves_; }

    /*! \brief Query for the cutoff distance.
     *  \return : The cutoff radius for the process.
     */
    double cutoff() const { return cutoff_; }

    /*! \brief Query for the range.
     *  \return : The range for the process in number of cells.
     */
    int range() const { return range_; }

    /*! \brief Query for the process number.
     *  \return : The number of the process.
     */
    int processNumber() const { return process_number_; }

    /*! \brief Query for the flag indicating if this is a bucket process.
     *  \return : The bucket process flag.
     */
    bool bucketProcess() const { return bucket_process_; }

protected:

    // If the process rate can be cached.
    bool cache_rate_;

    /// The process number.
    int process_number_;

    /// The range in primitive cells.
    int range_;

    /// The rate in Hz.
    double rate_;

    /// The cutoff radius primitive unit-cell fractional units.
    double cutoff_;

    /// The available sites for this process.
    std::vector<int> sites_;

    /// The multiplicity for the available sites for this process.
    std::vector<double> site_multiplicity_;

    /// The list of individual site rates.
    std::vector<double> site_rates_;

    /// The incremental rates.
    std::vector<double> incremental_rate_table_;

    /// The match list for comparing against local configurations.
    ProcessBucketMatchList match_list_;

    /*! \brief: The configuration indices that were affected last time
     *          the process was used to update a configuration.
     */
    std::vector<int> affected_indices_;

    /// The basis sites to which this process can be applied.
    std::vector<int> basis_sites_;

    /// The atom id moves.
    std::vector< std::pair<int,int> > id_moves_;

    /// Flag indicating if this is a bucket process or not.
    bool bucket_process_;

    /// The total available rate for this process.
    double total_rate_;

private:

};


#endif // __PROCESS__

