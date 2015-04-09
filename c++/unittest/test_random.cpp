/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_random.h"

// Include the files to test.
#include "random.h"

#include <cmath>
#include <unistd.h>

// -------------------------------------------------------------------------- //
//
void Test_Random::testSeedAndCall()
{
    // Seed the random number generator withouth using the time.
    seedRandom(false, 13);

    // Call the 0-1 double distribution and check against hardcoded values.
    double rnd0 = randomDouble01();
    double rnd1 = randomDouble01();
    double rnd2 = randomDouble01();
    double rnd3 = randomDouble01();
    double rnd4 = randomDouble01();

    // Here are the references.
    const double ref_rnd0 = 0.777702410239726;
    const double ref_rnd1 = 0.607341330498457;
    const double ref_rnd2 = 0.237541215959936;
    const double ref_rnd3 = 0.859434012323618;
    const double ref_rnd4 = 0.824278527405113;

    // Check.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd0, ref_rnd0, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd1, ref_rnd1, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd2, ref_rnd2, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd3, ref_rnd3, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd4, ref_rnd4, 1.0e-10);

    // Sleep for two seconds and run agin. This should not
    // affect the results.
    sleep(2);
    // Seed again.
    seedRandom(false, 13);
    rnd0 = randomDouble01();
    rnd1 = randomDouble01();
    rnd2 = randomDouble01();
    rnd3 = randomDouble01();
    rnd4 = randomDouble01();
    // Check.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd0, ref_rnd0, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd1, ref_rnd1, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd2, ref_rnd2, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd3, ref_rnd3, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd4, ref_rnd4, 1.0e-10);

    // Set the random number generator explicitly to MT.
    setRngType(MT);
    seedRandom(false, 13);

    // Results should now be identical.
    rnd0 = randomDouble01();
    rnd1 = randomDouble01();
    rnd2 = randomDouble01();
    rnd3 = randomDouble01();
    rnd4 = randomDouble01();
    // Check.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd0, ref_rnd0, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd1, ref_rnd1, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd2, ref_rnd2, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd3, ref_rnd3, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd4, ref_rnd4, 1.0e-10);

    // Run with time seeding.
    seedRandom(true, 13);
    rnd0 = randomDouble01();
    rnd1 = randomDouble01();
    rnd2 = randomDouble01();
    rnd3 = randomDouble01();
    rnd4 = randomDouble01();

    // Check that we get a different series.

    // This could eventually fail by chanse, but that would be very unlikely.
    CPPUNIT_ASSERT( std::fabs(rnd0-ref_rnd0) > 1.0e-10);
    CPPUNIT_ASSERT( std::fabs(rnd1-ref_rnd1) > 1.0e-10);
    CPPUNIT_ASSERT( std::fabs(rnd2-ref_rnd2) > 1.0e-10);
    CPPUNIT_ASSERT( std::fabs(rnd3-ref_rnd3) > 1.0e-10);
    CPPUNIT_ASSERT( std::fabs(rnd4-ref_rnd4) > 1.0e-10);

    // Sleep, seed, call and check again.
    sleep(2);
    seedRandom(true, 13);
    const double rnd01 = randomDouble01();

    // This should give another number.
    CPPUNIT_ASSERT( std::fabs(rnd01-rnd0) > 1.0e-10);

    // DONE

}


// -------------------------------------------------------------------------- //
//
void Test_Random::testCallMT()
{
    // Seed the random number generator withouth using the time.
    setRngType(MT);
    seedRandom(false, 13);

    // Call the 0-1 double distribution and check against hardcoded values.
    double rnd0 = randomDouble01();
    double rnd1 = randomDouble01();
    double rnd2 = randomDouble01();
    double rnd3 = randomDouble01();
    double rnd4 = randomDouble01();

    // Here are the references.
    const double ref_rnd0 = 0.777702410239726;
    const double ref_rnd1 = 0.607341330498457;
    const double ref_rnd2 = 0.237541215959936;
    const double ref_rnd3 = 0.859434012323618;
    const double ref_rnd4 = 0.824278527405113;

    // Check.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd0, ref_rnd0, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd1, ref_rnd1, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd2, ref_rnd2, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd3, ref_rnd3, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd4, ref_rnd4, 1.0e-10);

}


// -------------------------------------------------------------------------- //
//
void Test_Random::testCallRANLUX24()
{
    // Seed the random number generator withouth using the time.
    setRngType(RANLUX24);
    seedRandom(false, 13);

    // Call the 0-1 double distribution and check against hardcoded values.
    double rnd0 = randomDouble01();
    double rnd1 = randomDouble01();
    double rnd2 = randomDouble01();
    double rnd3 = randomDouble01();
    double rnd4 = randomDouble01();

    // Reset.
    setRngType(MT);

    // Here are the references.
    const double ref_rnd0 = 0.898736171384922;
    const double ref_rnd1 = 0.254935983524078;
    const double ref_rnd2 = 0.813379226273522;
    const double ref_rnd3 = 0.630122149801068;
    const double ref_rnd4 = 0.703361323161307;

    // Check.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd0, ref_rnd0, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd1, ref_rnd1, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd2, ref_rnd2, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd3, ref_rnd3, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd4, ref_rnd4, 1.0e-10);

}


// -------------------------------------------------------------------------- //
//
void Test_Random::testCallRANLUX48()
{
    // Seed the random number generator withouth using the time.
    setRngType(RANLUX48);
    seedRandom(false, 13);

    // Call the 0-1 double distribution and check against hardcoded values.
    double rnd0 = randomDouble01();
    double rnd1 = randomDouble01();
    double rnd2 = randomDouble01();
    double rnd3 = randomDouble01();
    double rnd4 = randomDouble01();

    // Reset.
    setRngType(MT);

    // Here are the references.
    const double ref_rnd0 = 0.076451949735752;
    const double ref_rnd1 = 0.263616565570590;
    const double ref_rnd2 = 0.225085912293423;
    const double ref_rnd3 = 0.311277891015823;
    const double ref_rnd4 = 0.060484161658469;

    // Check.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd0, ref_rnd0, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd1, ref_rnd1, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd2, ref_rnd2, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd3, ref_rnd3, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd4, ref_rnd4, 1.0e-10);

}


// -------------------------------------------------------------------------- //
//
void Test_Random::testCallMINSTD()
{
    // Seed the random number generator withouth using the time.
    setRngType(MINSTD);
    seedRandom(false, 13);

    // Call the 0-1 double distribution and check against hardcoded values.
    double rnd0 = randomDouble01();
    double rnd1 = randomDouble01();
    double rnd2 = randomDouble01();
    double rnd3 = randomDouble01();
    double rnd4 = randomDouble01();

    // Reset.
    setRngType(MT);

    // Here are the references.
    const double ref_rnd0 = 0.105421838448912;
    const double ref_rnd1 = 0.590946602169181;
    const double ref_rnd2 = 0.465967033764049;
    const double ref_rnd3 = 0.174109046367418;
    const double ref_rnd4 = 0.665661869919442;

    // Check.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd0, ref_rnd0, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd1, ref_rnd1, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd2, ref_rnd2, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd3, ref_rnd3, 1.0e-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(rnd4, ref_rnd4, 1.0e-10);

}
