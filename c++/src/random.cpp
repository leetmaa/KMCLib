/*
  Copyright (c)  2012-2015  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  random.cpp
 *  \brief File for the implementation code of the random number generator
 *         interfaces.
 */

#include "random.h"
#include "mpicommons.h"
#include "mpiroutines.h"
#include <ctime>
#include <stdexcept>

// c++11
#include <random>

// On systems where std::random_device isn't implemented in the
// standard <random> header this definition must be commented out,
// or else the construction of rng_minstd__ will throw an exeption.
#define __DEVICE__


// -----------------------------------------------------------------------------
// The random number generators.

// c++11
static std::mt19937       rng_mt__;
static std::minstd_rand   rng_minstd__;
static std::ranlux24      rng_ranlux24__;
static std::ranlux48      rng_ranlux48__;

#ifdef __DEVICE__
static std::random_device rng_device__;
#endif // __DEVICE__

static RNG_TYPE rng_type__ = MT;

// -----------------------------------------------------------------------------
//
bool setRngType(const RNG_TYPE rng_type)
{
    rng_type__ = rng_type;

    if (rng_type == DEVICE)
    {

        // Make sure that false is returned in case the rng_device__ is eiher
        // implemented using a PRNG (entropy == 0.0), or there is no support
        // at all for using rng_device__ (__DEVICE__ is not defined).

#ifdef __DEVICE__
        if (std::abs(rng_device__.entropy()) < 1.0e-8)
        {
            // Zero entropy means no random device is present.
            return false;
        }
#else
        return false;
#endif // __DEVICE__
    }


    return true;
}


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
    switch (rng_type__)
    {

    case MT:
        rng_mt__.seed(seed);
        break;

    case MINSTD:
        rng_minstd__.seed(seed);
        break;

    case RANLUX24:
        rng_ranlux24__.seed(seed);
        break;

    case RANLUX48:
        rng_ranlux48__.seed(seed);
        break;

    case DEVICE:
        // No seeding functionality for DEVICE.
        break;

    default:
        // This can never happen from previous checks.
        throw std::runtime_error("Invalid random number generator.");
    }
}



// -----------------------------------------------------------------------------
//
double randomDouble01()
{
    switch (rng_type__)
    {
    case MT:
        return std::generate_canonical<double, 32>(rng_mt__);

    case MINSTD:
        return std::generate_canonical<double, 32>(rng_minstd__);

    case RANLUX24:
        return std::generate_canonical<double, 32>(rng_ranlux24__);

    case RANLUX48:
        return std::generate_canonical<double, 32>(rng_ranlux48__);

#ifdef __DEVICE__
    case DEVICE:
        return std::generate_canonical<double, 32>(rng_device__);
#endif // __DEVICE__

    default:
        // This can never happen from previous checks.
        throw std::runtime_error("Invalid random number generator.");
    }
}

