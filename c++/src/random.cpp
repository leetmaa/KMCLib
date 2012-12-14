/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  random.cpp
 *  \brief File for the implementation code of the random number generator
 *         interfaces.
 */

#include <random>
#include <ctime>

#include "random.h"


// -----------------------------------------------------------------------------
// The random number generator and distributions.

static std::mt19937 rng;
static std::uniform_real_distribution<double> distribution01(0.0,1.0);


// -----------------------------------------------------------------------------
//
void seedRandom(const bool time_seed, int seed)
{
    // Seed with time.
    if (time_seed)
    {
        static time_t seconds;
        time(&seconds);
        seed += seconds;
    }

    // Seed.
    rng.seed(seed);

}


// -----------------------------------------------------------------------------
//
double randomDouble01()
{
    return distribution01(rng);
}

