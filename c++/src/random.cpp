/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  random.cpp
 *  \brief File for the implementation code of the random number generator
 *         interfaces.
 */

//#define __CPP11__

#include "random.h"
#include <ctime>

// c++11
#ifdef __CPP11__
#include <random>
#else
#include "externals/cslarsenmt.h"
#endif

// -----------------------------------------------------------------------------
// The random number generator and distributions.

// c++11
#ifdef __CPP11__
static std::mt19937 rng;
#endif


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
#ifdef __CPP11__
    // c++11
    rng.seed(seed);
#else
    mt::srand(seed);
#endif
}


// -----------------------------------------------------------------------------
//
double randomDouble01()
{
#ifdef __CPP11__
    // c++11
    return std::generate_canonical<double, 32>(rng);
#else
    return  mt::randf_co();
#endif
}

