/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  bucketstestcalculator.h
 *  \brief File for the definition of the BucketsTestCalculator class.
 */


#ifndef __BUCKETSTESTCALCULATOR__
#define __BUCKETSTESTCALCULATOR__


#include "ratecalculator.h"
#include "configuration.h"

/*! \brief Class for providing a custom rate calculator in C++
 *         for a simple 2D buckets test system. This calculator is
 *         used in the test KMCLib/python/functests/Buckets/BucketsTest.py
 */
class BucketsTestCalculator : public RateCalculator {

public:

    /*! \brief Constructor for the calculator.
     *  \param config : A reference to the configuration in the system.
     */
    BucketsTestCalculator(const Configuration & config);

    /*! \brief Destructor.
     */
    virtual ~BucketsTestCalculator();

    /*! \brief The backend callback function called from the matcher to obtain
     *         the updated rate.
     * \param geometry       : The geometry, with x,y,z coordinates for each atom in contiguous memory.
     * \param len            : The number of atoms, must be geometry.size()/3 used for reshping
     *                         geometry data in Python.
     * \param occupation     : The occupation before the process.
     * \param update         : The occupation update to perform.
     * \param type_map       : Mapping between type strings and integers.
     * \param rate_constant  : The rate constant associated with the process.
     * \param process_number : The id number of the process.
     * \param global_x       : The global coordinate in the x direction for the central site.
     * \param global_y       : The global coordinate in the y direction for the central site.
     * \param global_z       : The global coordinate in the z direction for the central site.
     * \return : The updated rate.
     */
    virtual double backendRateCallbackBuckets(const std::vector<double> geometry,
                                              const int len,
                                              const std::vector<TypeBucket> & occupation,
                                              const std::vector<TypeBucket> & update,
                                              const std::vector<std::string> & type_map,
                                              const double rate_constant,
                                              const int process_number,
                                              const double global_x,
                                              const double global_y,
                                              const double global_z) const;

    /*! \brief Function for getting the cutoff for the calculator.
     *  \return : The cutoff.
     */
    double cutoff() const;

    /*! \brief Function for indicating if caching should be used or not.
     *  \return : true if caching should be used, othewise false.
     */
    bool cacheRates() const;

    /*! \brief Function for indicating which process numbers should be
     *         excluded from caching.
     *  \return : A vector containing the process numbers to exclude.
     */
    std::vector<int> excludeFromCaching() const;

protected:

private:

    // A reference to the configuration.
    const Configuration & config_;

};

#endif // __BUCKETSTESTCALCULATOR__
