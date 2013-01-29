/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  ratecalculator.h
 *  \brief File for the Process class definition.
 */

#ifndef __RATECALCULATOR__
#define __RATECALCULATOR__


#include <string>

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

    /*! \brief NEEDS IMPLEMENTATION
     */
    virtual double rate() const;

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


/// Dummy/test function for calling the RateCalculator from C++.
double getRate(const RateCalculator & rc);


#endif // __RATECALCULATOR__

