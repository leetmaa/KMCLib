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
#include "mpicommons.h"
#include "mpiroutines.h"
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
        int time_seed = 0;

        // Make sure all processes gets the same seed value.
        if ( MPICommons::isMaster() )
        {
            static time_t seconds;
            time(&seconds);
            time_seed = static_cast<int>(seconds);
        }

        // Distribute over all processes.
        distributeToAll(time_seed);

        // Set the seed, now equal on all processes.
        seed += time_seed;
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

