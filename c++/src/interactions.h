/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  interactions.h
 *  \brief File for the Interactions class definition.
 */

#ifndef __INTERACTIONS__
#define __INTERACTIONS__


#include <vector>

#include "process.h"
#include "customrateprocess.h"
#include "ratecalculator.h"


// Forward declarations.
class Configuration;
class LatticeMap;

/*! \brief Class for holding information about all interactions and possible
 *         processes in the system.
 */
class Interactions {

public:

    /*! \brief Construct the interactions object from a list of processes.
     *  \param processes: The list of proceeses.
     *  \param implicit_wildcards: A flag indicating if implicit wildcards should be added
     *                             to the process matchlists.
     */
    Interactions(const std::vector<Process> & processes, const bool implicit_wildcards);

    /*! \brief Construct the interactions object from a list of processes.
     *  \param processes: The list of proceeses.
     *  \param implicit_wildcards: A flag indicating if implicit wildcards should be added
     *                             to the process matchlists.
     *  \param rate_calculator: The custom rate calculator to use for updating the rates.
     */
    Interactions(const std::vector<CustomRateProcess> & processes,
                 const bool implicit_wildcards,
                 const RateCalculator & rate_calculator);

    /*! \brief Get the max range of all processes.
     *  \return : The max range in shells.
     */
    int maxRange() const;

    /*! \brief Query for the custom rates flag.
     *  \return : The custom rates flag, (true) if we use custom rates.
     */
    bool useCustomRates() const { return use_custom_rates_; }

    /*! \brief Update the process matchlists with implicit wildcards if needed.
     *  \param configuration : The configuration needed to determine wildcard positions.
     *  \param lattice_map   : The lattice map to determine wildcard positions.
     */
    void updateProcessMatchLists( const Configuration & configuration,
                                  const LatticeMap & lattice_map);

    /*! \brief Query for the processes.
     *  \return : The processes of the system.
     */
    std::vector<Process*> & processes() { return process_pointers_; }

    /*! \brief Const query for the processes.
     *  \return : A handle to the processes of the system.
     */
    const std::vector<Process*> & processes() const { return process_pointers_; }

    /*! \brief Const query for the rate calculator reference.
     *  \return : A handle to the rate calculator in use.
     */
    const RateCalculator & rateCalculator() const { return rate_calculator_; }

    /*! \brief Const query for the number of available sites in the whole system.
     *  \return : The number of available sites in the whole system.
     */
    int totalAvailableSites() const;

    /*! \brief Const query for the probability table.
     *  \return : A handle to the present probability table.
     */
    const std::vector<std::pair<double,int> > & probabilityTable() const { return probability_table_; }

    /*! \brief Recalculate the table of process probabilities based on the
     *         number of available sites for each process and their rates.
     */
    void updateProbabilityTable();

    /*! \brief Query for the total rate of the system.
     *  \return : The total rate.
     */
    double totalRate() const { return probability_table_.back().first; }

    /*! \brief Pick an availabe process according to its probability.
     *  \return : The index of a possible available process picked according
     *            to its probability.
     */
    int pickProcessIndex() const;

    /*! \brief Pick an availabe process according to its probability and return
     *         a reference to that process.
     *  \return : A reference to a possible available process picked according
     *            to its probability.
     */
    Process* pickProcess();

    /*! \brief Erase any matching information from the processe.
     *         Used at initialization.
     */
    void clearMatching();

protected:

private:

    /// The processes.
    std::vector<Process> processes_;

    /// The process vector of processes with individual rates.
    std::vector<CustomRateProcess> custom_rate_processes_;

    /// Pointers to the processes we use.
    std::vector<Process*> process_pointers_;

    /// The probability table.
    std::vector<std::pair<double,int> > probability_table_;

    /// The flag indicating if implicit wildcards should  be used.
    bool implicit_wildcards_;

    /// The flag indicating if custom rates should be used.
    bool use_custom_rates_;

    /// A rate calculator placeholder if non is given on construction.
    RateCalculator rate_calculator_placeholder_;

    /// A reference to the rate calculator to use.
    const RateCalculator & rate_calculator_;

};


#endif // __INTERACTIONS__

