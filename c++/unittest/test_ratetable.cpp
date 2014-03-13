/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_ratetable.h"

// Include the files to test.
#include "ratetable.h"


// -------------------------------------------------------------------------- //
//
void Test_RateTable::testConstruction()
{
    // Construct with default constructor.
    CPPUNIT_ASSERT_NO_THROW( RateTable rt );
}
