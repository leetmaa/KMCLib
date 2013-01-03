/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  simulationtimer.cpp
 *  \brief File for the implementation code of the SimulationTimer class.
 */

#include "simulationtimer.h"
#include "random.h"
#include <cmath>

// -----------------------------------------------------------------------------
//
SimulationTimer::SimulationTimer() :
    simulation_time_(0.0)
{
    // NOTHING HERE
}


// -----------------------------------------------------------------------------
//
void SimulationTimer::propagateTime(const double total_rate)
{
    // Propagate the time of the system.
    const double rnd = randomDouble01();
    const double dt  = -std::log(rnd)/total_rate;
    simulation_time_ += dt;
}
