/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_simulationtimer.h"

// Include the files to test.
#include "simulationtimer.h"

// Other inclusions.
#include "random.h"

#include <cmath>

// -------------------------------------------------------------------------- //
//
void Test_SimulationTimer::testConstruct()
{
    // Only default construction possible.
    SimulationTimer timer;
}


// -------------------------------------------------------------------------- //
//
void Test_SimulationTimer::testPropagateAndQuery()
{
    SimulationTimer timer;

    // Total rate.
    const double rate = 0.234;

    // Make sure our global random number generator is seeded.
    seedRandom(false, 2017);

    // Setup a reference distribution.
    std::vector<int> ref_distribution(200,0);

    // The resulting delta t should be  dt = [-ln(rnd)/rate]
    for (int i = 0; i < 1000000; ++i)
    {
        const double rnd = randomDouble01();
        const double dt  = -std::log(rnd)/rate;
        const int index  = static_cast<int>(dt);
        if (index < 200)
        {
            ++ref_distribution[index];
        }
    }

    // Setup the timer distribution.
    std::vector<int> timer_distribution(200,0);

    // Propagate time and store the distribution of delta t.
    for (int i = 0; i < 1000000; ++i)
    {
        const double prevoius_t = timer.simulationTime();
        timer.propagateTime(rate);
        const double dt = timer.simulationTime() - prevoius_t;
        const int index = static_cast<int>(dt);
        if (index < 200)
        {
            ++timer_distribution[index];
        }
    }

    // Check that the distribution is simillar for all binds.
    for (size_t i = 0; i < ref_distribution.size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref_distribution[i]/1000000.0,
                                      timer_distribution[i]/1000000.0,
                                      1.0e-3);
    }

    // DONE
}

