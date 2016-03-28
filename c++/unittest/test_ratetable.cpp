/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_ratetable.h"

// Include the files to test.
#include "ratetable.h"
#include <stdexcept>

// -------------------------------------------------------------------------- //
//
void Test_RateTable::testConstruction()
{
    // Construct with default constructor.
    CPPUNIT_ASSERT_NO_THROW( RateTable rt );
}


// -------------------------------------------------------------------------- //
//
void Test_RateTable::testStored()
{
    // Test the function that determines if a values is stored for a given key.
    RateTable rt;

    ratekey key = 8765434567643;
    CPPUNIT_ASSERT_EQUAL( rt.stored(key), -1 );
}


// -------------------------------------------------------------------------- //
//
void Test_RateTable::testStoreAndRetrieve()
{
    // Test adding a key value pair to the table.
    RateTable rt;
    const ratekey key  = 8765434567643;
    const double value = 1.23456;
    rt.store(key, value);

    // Check that the key was added.
    CPPUNIT_ASSERT( rt.stored(key) != -1 );

    // Get the value.
    const double ret_value = rt.retrieve(key);

    // Check that the value is identical to the value we stored.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( ret_value, value, 1.0e-10 );
}


// -------------------------------------------------------------------------- //
//
void Test_RateTable::testRetrieveFail()
{
    // Test that retrieve on an empty key fails.
    RateTable rt;
    const ratekey key  = 8765434567643;
    const double value = 1.23456;
    rt.store(key, value);

    const ratekey new_key = 222222223;
    CPPUNIT_ASSERT_THROW( rt.retrieve(new_key), std::out_of_range );
}


// -------------------------------------------------------------------------- //
//
void Test_RateTable::testStoreFail()
{
    // Test that storing on an existing key fails.
    RateTable rt;
    const ratekey key  = 8765434567643;
    const double value = 1.23456;
    rt.store(key, value);

    // This should throw an error.
    // CPPUNIT_ASSERT_THROW( rt.store(key, value), std::runtime_error );

    // FIXME
}


