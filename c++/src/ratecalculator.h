/*
  Copyright (c)  2013 - 2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  ratecalculator.h
 *  \brief File for the Process class definition.
 */

#ifndef __RATECALCULATOR__
#define __RATECALCULATOR__

#include <vector>
#include <string>

#include "coordinate.h"
#include "typebucket.h"

/*! \brief Class for defining the interface for making a custom Python
 *         rate calculator function called from within the inner C++ loop.
 */
class RateCalculator {

public:

    /*! \brief Constructor for the rate calculator.
     */
    RateCalculator();

    /*! \brief Destructor for the rate calculator.
     */
    virtual ~RateCalculator();

    /*! \brief The backend callback function for sending process information
     *         to objects inheriting from this class. The KMCRateCalculatorPlugin
     *         class in python overloads this function.
     * \param geometry       : The geometry, with x,y,z coordinates for each atom in contiguous memory.
     * \param len            : The number of atoms, must be geometry.size()/3 used for reshping
     *                         geometry data in Python.
     * \param types_before   : The types before the process.
     * \param types_after    : The types after the process.
     * \param rate_constant  : The rate constant associated with the process.
     * \param process_number : The id number of the process.
     * \param global_x       : The global coordinate in the x direction for the central site.
     * \param global_y       : The global coordinate in the y direction for the central site.
     * \param global_z       : The global coordinate in the z direction for the central site.
     * \return : The base class implementation returns the rate constant unmodified.
     */
    virtual
    double backendRateCallback(const std::vector<double> geometry,
                               const int len,
                               const std::vector<std::string> & types_before,
                               const std::vector<std::string> & types_after,
                               const double rate_constant,
                               const int process_number,
                               const double global_x,
                               const double global_y,
                               const double global_z) const {
        return rate_constant; }

    /*! \brief The backend callback function for sending bucket process information
     *         to objects inheriting from this class. The KMCRateCalculatorPlugin
     *         class in python overloads this function.
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
     * \return : The base class implementation returns the rate constant unmodified.
     */
    virtual
    double backendRateCallbackBuckets(const std::vector<double> geometry,
                                      const int len,
                                      const std::vector<TypeBucket> & occupation,
                                      const std::vector<TypeBucket> & update,
                                      const std::vector<std::string> & type_map,
                                      const double rate_constant,
                                      const int process_number,
                                      const double global_x,
                                      const double global_y,
                                      const double global_z) const {
                return rate_constant; }


protected:

private:

};


// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
// PROTOTYPE AND TEST CODE FOLLOW
// -------------------------------------------------------------------------- //

/// A simple dummy class for prototyping.
class SimpleDummyBaseClass {

public:
    /*! \brief Constructor.
     */
    SimpleDummyBaseClass() {}

    /*! \brief Destructor, virtual since we want to inherit.
     */
    virtual ~SimpleDummyBaseClass() {}

    /*! \brief A function printing a message to screen indicating
     *         what type of object we have.
     */
    virtual std::string whoAmI() const { return "I am a C++ object"; }

protected:

private:

};


/// Dummy/test function for calling the objects whoAmI function from C++.
std::string callWhoAmI(const SimpleDummyBaseClass & obj);


/// Function for calling the RateCalculator from C++ used for testing.
double getRate(const RateCalculator & rc,
               const std::vector<Coordinate> & geometry,
               const std::vector<std::string> & types_before,
               const std::vector<std::string> & types_after,
               const double rate_constant,
               const int process_number,
               const double global_x,
               const double global_y,
               const double global_z);


#endif // __RATECALCULATOR__

