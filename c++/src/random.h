/*
  Copyright (c)  2012-2015  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  random.h
 *  \brief File for the random number generator interface.
 */

#ifndef __RANDOM__
#define __RANDOM__

/// The supported random number generator types.
enum RNG_TYPE {MT, MINSTD, RANLUX24, RANLUX48, DEVICE};


/*! \brief Set the type of random number generator to use.
 *  \param rng_type : The type of random number generator to use.
 */
bool setRngType(const RNG_TYPE rng_type);


/*! \brief Set the seed to the random number generator.
 *  \param time_seed : If true the random number generator will be seeded with the
 *                     given seed value plus the present time.
 *  \param seed : An integer to use as seed.
 */
void seedRandom(const bool time_seed, int seed);


/*! \brief Get a pseudo random number between 0.0 and 1.0 using the
 *         std::mt19937 random number generator.
 *  \return : A pseudo random number on the interval (0.0,1.0)
 */
double randomDouble01();


#endif // __RANDOM__

