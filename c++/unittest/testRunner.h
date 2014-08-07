/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// -------------------------------------------------------------------------- //
// Include the tests.
#include "test_latticemodel.h"
#include "test_configuration.h"
#include "test_latticemap.h"
#include "test_process.h"
#include "test_customrateprocess.h"
#include "test_interactions.h"
#include "test_coordinate.h"
#include "test_matchlist.h"
#include "test_matchlistentry.h"
#include "test_matcher.h"
#include "test_random.h"
#include "test_simulationtimer.h"
#include "test_ratecalculator.h"
#include "test_mpicommons.h"
#include "test_mpiroutines.h"
#include "test_ontheflymsd.h"
#include "test_blocker.h"
#include "test_hash.h"
#include "test_ratetable.h"
#include "test_typebucket.h"

// -------------------------------------------------------------------------- //
// Add tests.
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Blocker );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Configuration );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Coordinate );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_CustomRateProcess );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Hash );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Interactions );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_LatticeMap );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_LatticeModel );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_MPICommons );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_MPIRoutines );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_MatchList );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_MatchListEntry );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Matcher );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_OnTheFlyMSD );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Process );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Random );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_RateCalculator );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_RateTable );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_SimulationTimer );
CPPUNIT_TEST_SUITE_REGISTRATION( Test_TypeBucket );
